// MIT License
// Copyright (c) 2025 Stephanie Rancourt

#pragma once
#include "common.h"

namespace PNC
{
    /// <summary>
    /// Decorator struct that adds allocation of chunk's component data
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
        Size_t NodeCapacity;

    public:
        /// <summary>
        /// Get the capacity of the chunk.
        /// The capacity is the max size the chunk can be without reallocating the component's memory.
        /// Algorithms will only process nodes up to the chunk's size.
        /// </summary>
        /// <returns>The capacity of the chunk</returns>
        Size_t GetNodeCapacity()const { return NodeCapacity; }

        /// <summary>
        /// Create a null chunk without chunk type nor component data.
        /// </summary>
        /// <returns></returns>
        static Self_t Null() { return Self_t(); }

    public:
        /// <summary>
        /// Create a null chunk without chunk type.
        /// IsNull() will evaluate to true.
        /// </summary>
        ChunkAllocationT()
            : Base_t()
            , NodeCapacity(0)
        {
        }

        /// <summary>
        /// Deep copy a chunk and its component data.
        /// The result chunk will have the same size and capacity as the original.
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
        /// Deep copy a chunk and its component data.
        /// It will deallocate any previous chunk data.
        /// The result chunk will have the same size and capacity as the original.
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
        /// Create a chunk of a given chunk type and allocate the component's memory
        /// The components' memory can fit as many instances of each component as the chunk's capacity.
        /// Any computation perform in this chunk will only process node within the chunk's size and not it's capacity.
        /// </summary>
        /// <param name="chunkType"></param>
        /// <param name="nodeCapacity"></param>
        /// <param name="nodeCount"></param>
        ChunkAllocationT(const ChunkType_t* chunkType, Size_t nodeCapacity, Size_t nodeCount = 0)
            : Base_t(chunkType, nodeCount)
            , NodeCapacity(nodeCapacity)
        {
            AllocateComponentDataArray();
            AllocateData();
        }

        /// <summary>
        /// Deallocate data if not a null chunk
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
