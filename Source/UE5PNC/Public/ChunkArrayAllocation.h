// MIT License
// Copyright (c) 2025 Stephanie Rancourt

#pragma once
#include "common.h"

namespace PNC
{
    /// <summary>
    /// Decorator struct that allocates an Array of Chunks with the same capacity of Nodes per Chunks.
    /// </summary>
    /// <typeparam name="TBase">A ChunkPointer. TODO: ChunkArrayPointer are not yet implemented</typeparam>
    template<typename TBase>
    struct ChunkArrayAllocationT : public TBase
    {
    public:
        using Base_t = TBase;
        using Self_t = ChunkArrayAllocationT<TBase>;
        using ChunkStructure_t = typename TBase::ChunkStructure_t;
        using Size_t = typename TBase::Size_t;
        using ChunkPointerElement_t = typename TBase::ChunkPointerElement_t;

    protected:
        /// <summary>
        /// Maximum number of Nodes each Chunk can grow to.
        /// </summary>
        Size_t NodeCapacityPerChunk;

        /// <summary>
        /// Maximum number of Chunks this Array can grow to.
        /// </summary>
        Size_t ChunkCapacity;

    public:
        /// <summary>
        /// Create a Null Chunk
        /// </summary>
        ChunkArrayAllocationT()
            : NodeCapacityPerChunk(0)
            , ChunkCapacity(0)
        {
        }

        ChunkArrayAllocationT(const ChunkStructure_t* chunkStructure, Size_t nodeCapacityPerChunk, Size_t chunkCapacity, Size_t chunkCount, Size_t nodeCountPerChunk)
            : Base_t(chunkStructure, chunkCapacity * nodeCapacityPerChunk, chunkCount)
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

        ChunkArrayAllocationT& operator=(const ChunkArrayAllocationT& o)
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

    public:
        using Base_t::operator[];

        /// <summary>
        /// The total maximum number of Nodes the Array can grow to.
        /// </summary>
        /// <returns></returns>
        Size_t GetNodeCapacityTotal()const { return NodeCapacityPerChunk * ChunkCapacity; }

        /// <summary>
        /// The maximum number of Nodes each Chunks in the Array can grow to.
        /// </summary>
        /// <returns></returns>
        Size_t GetNodeCapacityPerChunk()const { return NodeCapacityPerChunk; }

        /// <summary>
        /// The macimum number of Chunks the Array can grow to.
        /// </summary>
        /// <returns></returns>
        Size_t GetChunkCapacity()const { return ChunkCapacity; }

        using Base_t::GetInternalChunk;

    protected:
        
        void CopyChunkArray(const Self_t& o)
        {
            auto& chunk = GetInternalChunk(*this);
            for (int i = 0; i < ChunkCapacity; ++i)
                chunk.Array.Chunks[i] = ChunkPointerElement_t(chunk.Structure, o[i].GetNodeCount(), GetComponentDataForChunk(i));
        }

        void InitChunkArray(Size_t nodeCountPerChunk = 0)
        {
            auto& chunk = GetInternalChunk(*this);
            if (chunk.IsNull())
                for (int i = 0; i < ChunkCapacity; ++i)
                    new(&chunk.Array.Chunks[i]) ChunkPointerElement_t();
            else
                for (int i = 0; i < ChunkCapacity; ++i)
                    new(&chunk.Array.Chunks[i]) ChunkPointerElement_t(chunk.Structure, nodeCountPerChunk, GetComponentDataForChunk(i));
        }

        void** GetComponentDataForChunk(Size_t chunkIndex)
        {
            auto& chunk = GetInternalChunk(*this);
            return &chunk.ComponentData[chunkIndex * chunk.Structure->Components.GetSize()];
        }

        void AllocateChunkArray()
        {
            auto& chunk = GetInternalChunk(*this);
            chunk.Array.Chunks = (ChunkPointerElement_t*)FMemory::Malloc(ChunkCapacity * sizeof(ChunkPointerElement_t), alignof(ChunkPointerElement_t));
        }

        void DeallocateChunkArray()
        {
            FMemory::Free(GetInternalChunk(*this).Array.Chunks);
        }

        void AllocateComponentDataArray()
        {
            auto& chunk = GetInternalChunk(*this);
            chunk.ComponentData = (void**)FMemory::Malloc(ChunkCapacity * chunk.Structure->Components.GetSize() * sizeof(void*), alignof(void*));
        }

        void DeallocateComponentDataArray()
        {
            auto& chunk = GetInternalChunk();
            FMemory::Free(chunk.ComponentData);
        }

        void AllocateData()
        {
            auto& chunk = GetInternalChunk(*this);
            assert_pnc(!chunk.IsNull());
            auto componentCount = chunk.Structure->Components.GetSize();
            auto nodeCapacityTotal = GetNodeCapacityTotal();
            for (size_t i = 0; i < componentCount; ++i)
            {
                auto componentTypeInfo = chunk.Structure->Components[i];
                chunk.ComponentData[i] = componentTypeInfo->Allocate(nodeCapacityTotal, ChunkCapacity);
                for (size_t k = 1; k < ChunkCapacity; ++k)
                {
                    chunk.ComponentData[k * componentCount + i] = componentTypeInfo->Forward(chunk.ComponentData[i], NodeCapacityPerChunk);
                }
            }
        }

        void AllocateDataCopy(const Self_t& o)
        {
            auto& chunk = GetInternalChunk(*this);
            assert_pnc(!chunk.IsNull());
            assert_pnc(chunk.IsSameStructure(*this, o));
            auto componentCount = chunk.Structure->Components.GetSize();
            auto nodeCapacityTotal = o.GetNodeCapacityTotal();
            for (size_t i = 0; i < componentCount; ++i)
            {
                auto componentTypeInfo = chunk.Structure->Components[i];
                chunk.ComponentData[i] = componentTypeInfo->AllocateCopy(o.ComponentData[i], nodeCapacityTotal, o.GetChunkCount() * o.GetNodeCapacityPerChunk(), ChunkCapacity);
                for (size_t k = 1; k < ChunkCapacity; ++k)
                {
                    chunk.ComponentData[k * componentCount + i] = componentTypeInfo->Forward(chunk.ComponentData[i], NodeCapacityPerChunk);
                }
            }
        }

        void DeallocateData()
        {
            auto& chunk = GetInternalChunk(*this);
            if (&chunk.Structure == nullptr)
                return;
            auto componentCount = chunk.Structure->Components.GetSize();
            auto nodeCapacityTotal = GetNodeCapacityTotal();
            for (size_t i = 0; i < componentCount; ++i)
            {
                auto componentTypeInfo = chunk.Structure->Components[i];
                componentTypeInfo->Deallocate(chunk.ComponentData[i], nodeCapacityTotal, ChunkCapacity);
            }
        }
    };
}
