// MIT License
// Copyright (c) 2025 Stephanie Rancourt

#pragma once
#include "common.h"

namespace PNC
{
    template<typename TBase>
    struct ChunkCapacityAllocationT : public TBase
    {
    public:
        using Base_t = TBase;
        using Self_t = ChunkCapacityAllocationT<TBase>;
        using typename TBase::ChunkStructure_t;
        using typename TBase::Size_t;
        using typename Base_t::Node_t;
        using ComponentType_t = typename ChunkStructure_t::ComponentType_t;

    protected:
        using ChunkPointerInternal_t = ChunkPointerT<ChunkStructure_t>::ChunkPointerInternal_t;

    public:
        /// <summary>
        /// Create a Null Chunk without ChunkStructure.
        /// IsNull() will evaluate to true.
        /// </summary>
        ChunkCapacityAllocationT() = default;

        /// <summary>
        /// Create a Chunk of a given ChunkStructure and allocate the Component's memory
        /// The Components' memory can fit as many instances of each Components as the Chunk's capacity.
        /// Any computation performed on this Chunk will only process node within the chunk's Node count and not it's capacity.
        /// </summary>
        /// <param name="chunkStructure">Structure of the Chunk's component data.</param>
        /// <param name="nodeCapacity">Maximum number of Nodes this Chunk can grow to.</param>
        /// <param name="nodeCount"></param>
        ChunkCapacityAllocationT(const ChunkStructure_t* chunkStructure, Size_t nodeCapacity, Size_t nodeCount = 0)
            : Base_t(chunkStructure, nodeCapacity, nodeCount)
        {
            AllocateComponentDataArray();
            AllocateAndConstructData();
        }

        /// <summary>
        /// Copy a Chunk and its Component data.
        /// The result chunk will have the same node count and capacity as the original.
        /// </summary>
        /// <param name="o"></param>
        ChunkCapacityAllocationT(const Self_t& o)
            : Base_t(o)
        {
            if (o.IsNull())
                return;
            AllocateComponentDataArray();
            AllocateDataCopy(o);
        }

        ChunkCapacityAllocationT(Self_t&& o) = default;

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
            DestructAndDeallocateData();
            DeallocateComponentDataArray();
            Base_t::operator=(o);
            if (o.IsNull())
                return *this;
            AllocateComponentDataArray();
            AllocateDataCopy(o);
            return *this;
        }

        /// <summary>
        /// Deallocate data if not a Null Chunk
        /// </summary>
        ~ChunkCapacityAllocationT()
        {
            DestructAndDeallocateData();
            DeallocateComponentDataArray();
        }

    public:
        using Base_t::operator*;
        using Base_t::operator->;
        using TBase::GetNodeCapacity;

    protected:
        using TBase::GetInternalChunk;

        void AllocateAndConstructData()
        {
            auto& chunk = GetInternalChunk();
            assert_pnc(!chunk.IsNull());
            auto componentCount = chunk.Structure->Components.GetSize();
            Size_t nodeCapacity = GetNodeCapacity();
            if (chunk.NodeCount == 0)
                for (Size_t i = 0; i < componentCount; ++i)
                {
                    const ComponentType_t& componentType = *chunk.Structure->Components[i];
                    chunk.ComponentData[i] = (void*)pnc_alloc(componentType.GetSize(nodeCapacity), componentType.GetAlignment());
                }
            else
                for (Size_t i = 0; i < componentCount; ++i)
                {
                    const ComponentType_t& componentType = *chunk.Structure->Components[i];
                    chunk.ComponentData[i] = (void*)pnc_alloc(componentType.GetSize(nodeCapacity), componentType.GetAlignment());
                    Node_t::ConstructComponentUnsafe(componentType, chunk.ComponentData[i], 0, chunk.NodeCount);
                }
        }

        void AllocateDataCopy(const Self_t& o)
        {
            auto& chunk = GetInternalChunk();
            assert_pnc(!chunk.IsNull());
            assert_pnc(chunk.IsSameChunkStructure(chunk, o.GetChunk()));
            auto componentCount = chunk.Structure->Components.GetSize();
            Size_t nodeCapacity = o.GetNodeCapacity();
            Size_t nodeCount = o.GetNodeCount();
            for (Size_t i = 0; i < componentCount; ++i)
            {
                const ComponentType_t& componentType = *chunk.Structure->Components[i];
                chunk.ComponentData[i] = (void*)pnc_alloc(componentType.GetSize(nodeCapacity), componentType.GetAlignment());
                Node_t::CopyComponentUnsafe(componentType, chunk.ComponentData[i], o.ComponentData[i], 0, nodeCount);
            }
        }

        void DestructAndDeallocateData()
        {
            auto& chunk = GetInternalChunk();
            if (chunk.Structure == nullptr)
                return;
            auto componentCount = chunk.Structure->Components.GetSize();
            for (Size_t i = 0; i < componentCount; ++i)
            {
                const ComponentType_t& componentType = *chunk.Structure->Components[i];
                Node_t::DestructComponentUnsafe(componentType, chunk.ComponentData[i], 0, chunk.NodeCount);
                pnc_free(chunk.ComponentData[i], componentType.GetSize(nodeCapacity), componentType.GetAlignment());
                chunk.ComponentData[i] = nullptr;
            }
        }

        void AllocateComponentDataArray()
        {
            auto& chunk = GetInternalChunk();
            chunk.ComponentData = (void**)pnc_alloc(chunk.Structure->Components.GetSize() * sizeof(void*), alignof(void*));
        }

        void DeallocateComponentDataArray()
        {
            auto& chunk = GetInternalChunk();
            if (chunk.Structure == nullptr)
                return;
            pnc_free(chunk.ComponentData, chunk.Structure->Components.GetSize() * sizeof(void*), alignof(void*));
        }

        void CopyComponentDataArray(void** componentData)const
        {
            auto& chunk = GetInternalChunk();
            auto componentCount = chunk.Structure->Components.GetSize();
            for (Size_t i = 0; i < componentCount; ++i)
            {
                chunk.ComponentData[i] = componentData[i];
            }
        }
    };
}
