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
        using ChunkType_t = typename TBase::ChunkType_t;
        using Size_t = typename TBase::Size_t;
        using ChunkPointerElement_t = typename TBase::ChunkPointerElement_t;

    protected:
        using Internal_t = ChunkArrayPointerInternalT<ChunkType_t, ChunkPointerElement_t>;

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

        /// <summary>
        /// Allocate a Chunk Array with a maximum number of Chunks and Nodes per Chunks.
        /// </summary>
        /// <param name="chunkType">Structure of the Chunk's Component data.</param>
        /// <param name="nodeCapacityPerChunk">Maximum number of Nodes each Chunks in the Array can grow to.</param>
        /// <param name="chunkCapacity">Maximum number of Chunks this Array can grow to.</param>
        /// <param name="chunkCount">Number of valid Chunks in the Array.</param>
        /// <param name="nodeCountPerChunk">Number of valid Nodes in each Chunks in the Array.</param>
        ChunkArrayAllocationT(const ChunkType_t* chunkType, Size_t nodeCapacityPerChunk, Size_t chunkCapacity, Size_t chunkCount = 0, Size_t nodeCountPerChunk = 0)
            : Base_t(chunkType, chunkCapacity * nodeCapacityPerChunk, chunkCount)
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
