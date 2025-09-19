// MIT License
// Copyright (c) 2025 Stephanie Rancourt

#pragma once
#include "common.h"

namespace PNC
{
    /// <summary>
    /// Decorator struct that adds allocation of Chunk's Component data
    /// </summary>
    /// <typeparam name="TBase"></typeparam>
    template<typename TBase>
    struct ChunkAllocationT : public TBase
    {
    public:
        using Base_t = TBase;
        using ChunkType_t = typename TBase::ChunkType_t;
        using Size_t = typename TBase::Size_t;
        using Self_t = ChunkAllocationT<TBase>;
        using Internal_t = ChunkPointerInternalT<ChunkType_t>;

    private:
        /// <summary>
        /// Maximum number of Nodes this Chunk can grow to.
        /// </summary>
        Size_t NodeCapacity;

    public:
        /// <summary>
        /// Get the maximum number of Nodes the Chunk can grow to.
        /// </summary>
        /// <returns>The capacity of the chunk</returns>
        Size_t GetNodeCapacity()const { return NodeCapacity; }

        /// <summary>
        /// Create a Null Chunk without ChunkType nor Component data.
        /// </summary>
        /// <returns></returns>
        static Self_t Null() { return Self_t(); }

    public:
        /// <summary>
        /// Create a Null Chunk without ChunkType.
        /// IsNull() will evaluate to true.
        /// </summary>
        ChunkAllocationT()
            : Base_t()
            , NodeCapacity(0)
        {
        }

        /// <summary>
        /// Copy a Chunk and its Component data.
        /// The result chunk will have the same node count and capacity as the original.
        /// </summary>
        /// <param name="o"></param>
        ChunkAllocationT(const Self_t& o)
            : Base_t(o)
            , NodeCapacity(o.NodeCapacity)
        {
            if (o.IsNull())
                return;
            AllocateComponentDataArray();
            AllocateDataCopy(o);
        }

        /// <summary>
        /// Copy a Chunk and its Component data.
        /// It will deallocate any previous data.
        /// The result Chunk will have the same node count and capacity as the one being copied.
        /// </summary>
        /// <param name="o"></param>
        Self_t& operator=(const Self_t& o)
        {
            if (this == &o)
                return *this;
            DeallocateData();
            DeallocateComponentDataArray();
            Base_t::operator=(o);
            NodeCapacity = o.NodeCapacity;
            if (o.IsNull())
                return *this;
            AllocateComponentDataArray();
            AllocateDataCopy(o);
            return *this;
        }

        /// <summary>
        /// Create a Chunk of a given ChunkType and allocate the Component's memory
        /// The Components' memory can fit as many instances of each Components as the Chunk's capacity.
        /// Any computation performed on this Chunk will only process node within the chunk's Node count and not it's capacity.
        /// </summary>
        /// <param name="chunkType">Structure of the Chunk's component data.</param>
        /// <param name="nodeCapacity">Maximum number of Nodes this Chunk can grow to.</param>
        /// <param name="nodeCount"></param>
        ChunkAllocationT(const ChunkType_t* chunkType, Size_t nodeCapacity, Size_t nodeCount = 0)
            : Base_t(chunkType, nodeCount)
            , NodeCapacity(nodeCapacity)
        {
            AllocateComponentDataArray();
            AllocateData();
        }

        /// <summary>
        /// Deallocate data if not a Null Chunk
        /// </summary>
        ~ChunkAllocationT()
        {
            DeallocateData();
            DeallocateComponentDataArray();
        }

    protected:
        Internal_t& GetInternalChunk() { return (Internal_t&)this->GetChunk(); }

        void AllocateData()
        {
            auto& chunk = GetInternalChunk();
            assert_pnc(!chunk.IsNull());
            auto componentCount = chunk.Type->Components.GetSize();
            for (Size_t i = 0; i < componentCount; ++i)
            {
                auto componentTypeInfo = chunk.Type->Components[i];
                chunk.ComponentData[i] = componentTypeInfo->Allocate(NodeCapacity);
            }
        }

        void AllocateDataCopy(const Self_t& o)
        {
            auto& chunk = GetInternalChunk();
            assert_pnc(!chunk.IsNull());
            assert_pnc(chunk.IsSameChunkType(chunk, o.GetChunk()));
            auto componentCount = chunk.Type->Components.GetSize();
            for (Size_t i = 0; i < componentCount; ++i)
            {
                auto componentTypeInfo = chunk.Type->Components[i];
                chunk.ComponentData[i] = componentTypeInfo->AllocateCopy(o.ComponentData[i], o.NodeCapacity, o.NodeCount);
            }
        }

        void DeallocateData()
        {
            auto& chunk = GetInternalChunk();
            if (chunk.Type == nullptr)
                return;
            auto componentCount = chunk.Type->Components.GetSize();
            for (Size_t i = 0; i < componentCount; ++i)
            {
                auto componentTypeInfo = chunk.Type->Components[i];
                componentTypeInfo->Deallocate(chunk.ComponentData[i], NodeCapacity);
            }
        }

        void AllocateComponentDataArray()
        {
            auto& chunk = GetInternalChunk();
            chunk.ComponentData = (void**)FMemory::Malloc(chunk.Type->Components.GetSize() * sizeof(void*), alignof(void*));
        }

        void DeallocateComponentDataArray()
        {
            auto& chunk = GetInternalChunk();
            if (chunk.Type == nullptr)
                return;
            FMemory::Free(chunk.ComponentData);
        }

        void CopyComponentDataArray(void** componentData)const
        {
            auto& chunk = GetInternalChunk();
            auto componentCount = chunk.Type->Components.GetSize();
            for (Size_t i = 0; i < componentCount; ++i)
            {
                chunk.ComponentData[i] = componentData[i];
            }
        }
    };
}
