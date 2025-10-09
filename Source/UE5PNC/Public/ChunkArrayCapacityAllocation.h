// MIT License
// Copyright (c) 2025 Stephanie Rancourt

#pragma once
#include "common.h"
#include "ChunkArrayPointer.h"

namespace PNC
{
    template<typename TBase>
    struct ChunkArrayCapacityAllocationT : public TBase
    {
    public:
        using Base_t = TBase;
        using Self_t = ChunkArrayCapacityAllocationT<TBase>;
        using typename Base_t::ChunkStructure_t;
        using typename Base_t::Size_t;
        using typename Base_t::ChunkPointerElement_t;
        using typename Base_t::ChunkPointerInternal_t;
        using ComponentType_t = typename ChunkStructure_t::ComponentType_t;
        
    public:
        /// <summary>
        /// Create a Null Chunk
        /// </summary>
        ChunkArrayCapacityAllocationT() = default;

        //ChunkArrayCapacityAllocationT(const ChunkStructure_t* const chunkStructure, const Size_t chunkCapacity, const Size_t totalNodeCapacity)
        //    : Base_t(chunkStructure, nodeCapacityPerChunk, chunkCapacity, chunkCount, nodeCountPerChunk)
        //{
        //    AllocateComponentDataArray();
        //    AllocateChunkArray();
        //    ConstructChunkArray(nodeCountPerChunk);
        //    AllocateComponentData();
        //    ConstructComponentData(chunkCount, nodeCountPerChunk);
        //}
        ChunkArrayCapacityAllocationT(const ChunkStructure_t* const chunkStructure, const Size_t nodeCapacityPerChunk, const Size_t chunkCapacity, const Size_t chunkCount, const Size_t nodeCountPerChunk)
            : Base_t(chunkStructure, nodeCapacityPerChunk, chunkCapacity, chunkCount, nodeCountPerChunk)
        {
            Allocate();
            ConstructChunkAndNode(0, chunkCount, nodeCountPerChunk);
        }

        ChunkArrayCapacityAllocationT(const ChunkArrayCapacityAllocationT& o)
            : Base_t(o)
        {
            if (o.IsNull())
                return;
            Allocate();
            CopyChunkAndNodes(o, o.GetChunkCount());
        }

        ChunkArrayCapacityAllocationT& operator=(const ChunkArrayCapacityAllocationT& o)
        {
            if (this == &o)
                return *this;
            DestructChunkAndNode(0, GetChunkCount());
            Deallocate();
            Base_t::operator=(o);
            if (o.IsNull())
                return;
            Allocate();
            CopyChunkAndNodes(o, o.GetChunkCount());
            return *this;
        }

        ~ChunkArrayCapacityAllocationT()
        {
            if (IsNull())
                return;
            DestructChunkAndNode(0, GetChunkCount());
            Deallocate();
        }

    public:
        using Base_t::IsNull;
        using Base_t::GetChunk;
        using Base_t::GetStructure;
        using Base_t::GetComponentData;
        using Base_t::GetNodeCapacityTotal;
        using Base_t::GetNodeCapacityPerChunk;
        using Base_t::GetChunkCapacity;
        using Base_t::GetChunkCount;
        using Base_t::operator*;
        using Base_t::operator->;
        using Base_t::operator[];


    protected:
        using Base_t::GetInternalChunk;
        using Base_t::ConstructChunkElementAndNodes;
        using Base_t::DestructChunkElementAndNodes;
        using Base_t::CopyChunkElementAndNodes;

        void** GetComponentDataArrayForChunk(const Size_t chunkIndex)
        {
            ChunkPointerInternal_t& internalChunk = GetInternalChunk();
            assert_pnc(!internalChunk.IsNull());
            return &internalChunk.ComponentData[chunkIndex * internalChunk.Structure->GetComponentCount()];
        }

#pragma region Construction/Destruction/Copy
        void ConstructChunkAndNode(const Size_t chunkFirst, const Size_t chunkCount, const Size_t nodeCountPerChunk)
        {
            for (Size_t i = 0; i < chunkCount; ++i)
                ConstructChunkElementAndNodes(chunkFirst + i, nodeCountPerChunk);
        }

        void DestructChunkAndNode(const Size_t chunkFirst, const Size_t chunkCount)
        {
            for (Size_t i = 0; i < chunkCount; ++i)
                DestructChunkElementAndNodes(chunkFirst + i);
        }

        void CopyChunkAndNodes(const Self_t& o, const Size_t chunkCount)
        {
            for (Size_t i = 0; i < chunkCount; ++i)
                CopyChunkElementAndNodes(o, i);
        }
#pragma endregion 

#pragma region Allocate/Deallocate
        void Allocate()
        {
            AllocateComponentDataArray();
            AllocateComponentData();
            AllocateChunkArray();
        }
        void Deallocate()
        {
            DeallocateChunkArray();
            DeallocateComponentData();
            DeallocateComponentDataArray();
        }

        void AllocateComponentDataArray()
        {
            ChunkPointerInternal_t& internalChunk = GetInternalChunk();
            assert_pnc(!internalChunk.IsNull());
            const Size_t chunkCapacity = GetChunkCapacity();
            internalChunk.ComponentData = (void**)pnc_alloc(chunkCapacity * internalChunk.Structure->Components.GetSize() * sizeof(void*), alignof(void*));
        }

        void AllocateComponentData()
        {
            ChunkPointerInternal_t& internalChunk = GetInternalChunk();
            assert_pnc(!internalChunk.IsNull());
            const Size_t componentCount = internalChunk.Structure->Components.GetSize();
            const Size_t nodeCapacityTotal = GetNodeCapacityTotal();
            const Size_t chunkCapacity = GetChunkCapacity();
            for (size_t i = 0; i < componentCount; ++i)
            {
                const ComponentType_t& componentType = *internalChunk.Structure->Components[i];
                internalChunk.ComponentData[i] = (void*)pnc_alloc(componentType.GetSize(nodeCapacityTotal, chunkCapacity), componentType.GetAlignment());
            }
        }

        void AllocateChunkArray()
        {
            ChunkPointerInternal_t& internalChunk = GetInternalChunk();
            const Size_t chunkCapacity = GetChunkCapacity();
            internalChunk.Array.Chunks = (ChunkPointerElement_t*)pnc_alloc(chunkCapacity * sizeof(ChunkPointerElement_t), alignof(ChunkPointerElement_t));
        }


        void DeallocateChunkArray()
        {
            ChunkPointerInternal_t& internalChunk = GetInternalChunk();
            Size_t chunkCapacity = GetChunkCapacity();
            pnc_free(internalChunk.Array.Chunks, chunkCapacity * sizeof(ChunkPointerElement_t), alignof(ChunkPointerElement_t));
            internalChunk.Array.Chunks = nullptr;
        }

        void DeallocateComponentData()
        {
            ChunkPointerInternal_t& internalChunk = GetInternalChunk();
            assert_pnc(!internalChunk.IsNull());
            Size_t componentCount = internalChunk.Structure->Components.GetSize();
            Size_t nodeCapacityTotal = GetNodeCapacityTotal();
            Size_t chunkCapacity = GetChunkCapacity();
            for (Size_t i = 0; i < componentCount; ++i)
            {
                const ComponentType_t& componentType = *internalChunk.Structure->Components[i];
                pnc_free(internalChunk.ComponentData[i], componentType.GetSize(nodeCapacityTotal, chunkCapacity), componentType.GetAlignment());
            }
        }

        void DeallocateComponentDataArray()
        {
            ChunkPointerInternal_t& internalChunk = GetInternalChunk();
            assert_pnc(!internalChunk.IsNull());
            Size_t chunkCapacity = GetChunkCapacity();
            pnc_free(internalChunk.ComponentData, chunkCapacity * internalChunk.Structure->Components.GetSize() * sizeof(void*), alignof(void*));
        }

#pragma endregion 

    };
}
