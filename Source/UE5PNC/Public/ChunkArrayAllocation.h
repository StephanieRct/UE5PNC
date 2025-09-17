// MIT License
// Copyright (c) 2025 Stephanie Rancourt

#pragma once
#include "common.h"

namespace PNC
{
    template<typename TBase>
    struct ChunkArrayAllocationT : public TBase
    {
        // TODO specialization when TChunkPointer is a ChunkArrayPointerT
    public:
        using Base_t = TBase;
        using ChunkType_t = typename TBase::ChunkType_t;
        using Size_t = typename TBase::Size_t;

        using ChunkPointerElement_t = typename TBase::ChunkPointerElement_t;
        using Self_t = ChunkArrayAllocationT<TBase>;
        //using ChunkArray_t = ChunkArrayT<TChunkPointerElement>;
        using Internal_t = ChunkArrayPointerInternalT<ChunkType_t, ChunkPointerElement_t>;
    protected:
        Size_t NodeCapacityPerChunk;
        Size_t ChunkCapacity;

    public:
        Size_t GetNodeCapacityTotal()const { return NodeCapacityPerChunk * ChunkCapacity; }
        Size_t GetNodeCapacityPerChunk()const { return NodeCapacityPerChunk; }
        Size_t GetChunkCapacity()const { return ChunkCapacity; }
        const ChunkPointerElement_t& operator[](Size_t index)const { return Base_t::operator[](index); }
        ChunkPointerElement_t& operator[](Size_t index) { return Base_t::operator[](index); }

        ChunkArrayAllocationT()
            : NodeCapacityPerChunk(0)
            , ChunkCapacity(0)
        {
        }
        ChunkArrayAllocationT(const ChunkType_t* chunkType, Size_t nodeCapacityPerChunk, Size_t chunkCapacity, Size_t chunkCount = 0, Size_t nodeCountPerChunk = 0)
            : Base_t(chunkType, nodeCapacityPerChunk, chunkCount)
            , NodeCapacityPerChunk(nodeCapacityPerChunk)
            , ChunkCapacity(chunkCapacity)
        {
            AllocateComponentDataArray();
            AllocateData();
            AllocateChunkArray();
            InitChunkArray(nodeCountPerChunk);
        }

        ChunkArrayAllocationT(const ChunkArrayAllocationT& o)
            : Base_t(o)
            , NodeCapacityPerChunk(o.NodeCapacityPerChunk)
            , ChunkCapacity(o.ChunkCapacity)
        {
            if (o.IsNull())
                return;
            AllocateComponentDataArray();
            AllocateDataCopy(o);
            AllocateChunkArray();
            CopyChunkArray(o);
        }

        Self_t& operator=(const Self_t& o)
        {
            if (this == &o)
                return *this;
            DeallocateData();
            DeallocateComponentDataArray();
            Base_t::operator=(o);
            NodeCapacityPerChunk = o.NodeCapacityPerChunk;
            if (o.IsNull())
                return;
            AllocateComponentDataArray();
            AllocateDataCopy(o);
            AllocateChunkArray();
            CopyChunkArray(o);
            return *this;
        }

        ~ChunkArrayAllocationT()
        {
            DeallocateChunkArray();
            DeallocateData();
            DeallocateComponentDataArray();
        }

    protected:
        Internal_t& GetInternalChunk() { return (Internal_t&)this->GetChunk(); }
        void CopyChunkArray(const Self_t& o)
        {
            auto& chunk = GetInternalChunk();
            for (int i = 0; i < ChunkCapacity; ++i)
                chunk.Array.Chunks[i] = ChunkPointerElement_t(chunk.Type, o[i].GetNodeCount(), GetComponentDataForChunk(i));
        }
        void InitChunkArray(Size_t nodeCountPerChunk = 0)
        {
            auto& chunk = GetInternalChunk();
            if (chunk.IsNull())
                for (int i = 0; i < ChunkCapacity; ++i)
                    chunk.Array.Chunks[i] = ChunkPointerElement_t::Null();
            else
                for (int i = 0; i < ChunkCapacity; ++i)
                    chunk.Array.Chunks[i] = ChunkPointerElement_t(chunk.Type, nodeCountPerChunk, GetComponentDataForChunk(i));
        }

        void** GetComponentDataForChunk(Size_t chunkIndex)
        {
            auto& chunk = GetInternalChunk();
            return &chunk.ComponentData[chunkIndex * chunk.Type->Components.GetSize()];
        }

        void AllocateChunkArray()
        {
            auto& chunk = GetInternalChunk();
            chunk.Array.Chunks = (ChunkPointerElement_t*)FMemory::Malloc(ChunkCapacity * sizeof(ChunkPointerElement_t), alignof(ChunkPointerElement_t));
        }
        void DeallocateChunkArray()
        {
            FMemory::Free(GetInternalChunk().Array.Chunks);
        }
        void AllocateComponentDataArray()
        {
            auto& chunk = GetInternalChunk();
            chunk.ComponentData = (void**)FMemory::Malloc(ChunkCapacity * chunk.Type->Components.GetSize() * sizeof(void*), alignof(void*));
        }

        void DeallocateComponentDataArray()
        {
            auto& chunk = GetInternalChunk();
            FMemory::Free(chunk.ComponentData);
        }

        void AllocateData()
        {
            auto& chunk = GetInternalChunk();
            assert_pnc(!chunk.IsNull());
            auto componentCount = chunk.Type->Components.GetSize();
            auto nodeCapacityTotal = GetNodeCapacityTotal();
            for (size_t i = 0; i < componentCount; ++i)
            {
                auto componentTypeInfo = chunk.Type->Components[i];
                chunk.ComponentData[i] = componentTypeInfo->Allocate(nodeCapacityTotal, ChunkCapacity);
                for (size_t k = 1; k < ChunkCapacity; ++k)
                {
                    chunk.ComponentData[k * componentCount + i] = componentTypeInfo->Forward(chunk.ComponentData[i], NodeCapacityPerChunk);
                }
            }
        }

        void AllocateDataCopy(const Self_t& o)
        {
            auto& chunk = (Internal_t&)GetInternalChunk();
            assert_pnc(!chunk.IsNull());
            assert_pnc(chunk.IsSameChunkType(*this, o));
            auto componentCount = chunk.Type->Components.GetSize();
            auto nodeCapacityTotal = o.GetNodeCapacityTotal();
            for (size_t i = 0; i < componentCount; ++i)
            {
                auto componentTypeInfo = chunk.Type->Components[i];
                chunk.ComponentData[i] = componentTypeInfo->AllocateCopy(o.ComponentData[i], nodeCapacityTotal, o.GetChunkCount() * o.GetNodeCapacityPerChunk(), ChunkCapacity);
                for (size_t k = 1; k < ChunkCapacity; ++k)
                {
                    chunk.ComponentData[k * componentCount + i] = componentTypeInfo->Forward(chunk.ComponentData[i], NodeCapacityPerChunk);
                }
            }
        }
        void DeallocateData()
        {
            auto& chunk = GetInternalChunk();
            if (&chunk.Type == nullptr)
                return;
            auto componentCount = chunk.Type->Components.GetSize();
            auto nodeCapacityTotal = GetNodeCapacityTotal();
            for (size_t i = 0; i < componentCount; ++i)
            {
                auto componentTypeInfo = chunk.Type->Components[i];
                componentTypeInfo->Deallocate(chunk.ComponentData[i], nodeCapacityTotal, ChunkCapacity);
            }
        }

    };
}
