// MIT License
// Copyright (c) 2025 Stephanie Rancourt

#pragma once
#include "common.h"
#include "ChunkArrayPointer.h"

namespace PNC
{
    template<typename TBase>
    struct BucketEvenChunkArrayT : public TBase
    {
    public:
        using Base_t = TBase;
        using Self_t = BucketEvenChunkArrayT<TBase>;
        using typename Base_t::ChunkStructure_t;
        using typename Base_t::Size_t;
        using typename Base_t::ChunkPointerElement_t;
        using typename Base_t::ChunkPointerInternal_t;
        using typename Base_t::ChunkPointerElementInternal_t;
        using typename Base_t::Node_t;
        using ComponentType_t = typename ChunkStructure_t::ComponentType_t;

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
        BucketEvenChunkArrayT()
            : NodeCapacityPerChunk(0)
            , ChunkCapacity(0)
        {
        }

        BucketEvenChunkArrayT(Self_t&& o) = default;

    protected:
        BucketEvenChunkArrayT(const ChunkStructure_t* const chunkStructure, const Size_t nodeCapacityPerChunk, const Size_t chunkCapacity, const Size_t chunkCount, const Size_t nodeCountPerChunk)
            : Base_t(chunkStructure, chunkCapacity* nodeCapacityPerChunk, chunkCount)
            , NodeCapacityPerChunk(nodeCapacityPerChunk)
            , ChunkCapacity(chunkCapacity)
        {
        }


    public:
        using Base_t::operator*;
        using Base_t::operator->;
        using Base_t::operator[];
        using Base_t::GetChunkCount;
        using Base_t::IsSameChunkStructure;
        
        Size_t AvailableChunks()const { return ChunkCapacity - GetChunkCount(); }
        
        void AddChunk() 
        {

        }
        // TODO
        void Clear() {}

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
        using Base_t::GetInternalChunk;
        using Base_t::GetInternalChunkElement;
        
        void ConstructChunkElementAndNodes(const Size_t chunkIndex, const Size_t nodeCount)
        {
            assert_pnc(chunkIndex >= 0);
            assert_pnc(chunkIndex < ChunkCapacity);
            assert_pnc(chunkIndex < GetChunkCount());

            assert_pnc(nodeCount >= 0);
            assert_pnc(nodeCount <= NodeCapacityPerChunk);

            ChunkPointerInternal_t& internalChunkArray = GetInternalChunk();
            assert_pnc(!internalChunkArray.IsNull());

            *new(&internalChunkArray.Array.Chunks[chunkIndex])ChunkPointerElement_t(internalChunkArray.Structure, nodeCount, GetComponentDataArrayForChunk(chunkIndex));
            ChunkPointerElementInternal_t& internalChunkElement = GetInternalChunkElement(chunkIndex);

            const Size_t componentCount = internalChunkArray.Structure->Components.GetSize();
            for (Size_t i = 0; i < componentCount; ++i)
            {
                const ComponentType_t& componentType = *internalChunkArray.Structure->Components[i];
                internalChunkElement.ComponentData[i] = componentType.Forward(internalChunkArray.ComponentData[i], chunkIndex * NodeCapacityPerChunk, chunkIndex);
            }
            
            if(nodeCount > 0)
                Node_t::ConstructNode(internalChunkElement, 0, nodeCount);

        }

        void DestructChunkElementAndNodes(const Size_t chunkIndex)
        {
            assert_pnc(chunkIndex >= 0);
            assert_pnc(chunkIndex < ChunkCapacity);
            assert_pnc(chunkIndex < GetChunkCount());

            ChunkPointerInternal_t& internalChunkArray = GetInternalChunk();
            assert_pnc(!internalChunkArray.IsNull());

            ChunkPointerElementInternal_t& internalChunkElement = GetInternalChunkElement(chunkIndex);
            if (internalChunkElement.NodeCount > 0)
                Node_t::DestructNode(internalChunkElement, 0, internalChunkElement.NodeCount);
            
            internalChunkElement.NodeCount = 0;
            internalChunkElement.~ChunkPointerElementInternal_t();
        }

        void CopyChunkElementAndNodes(const Size_t chunkIndexTo, const Size_t chunkIndexFrom, const Size_t chunkCount, const Self_t& chunkArrayFrom)
        {
            assert_pnc(chunkIndexTo >= 0);
            assert_pnc(chunkIndexTo < ChunkCapacity);
            assert_pnc(chunkIndexTo + chunkCount < ChunkCapacity);
            assert_pnc(chunkIndexTo < GetChunkCount());
            assert_pnc(chunkIndexTo + chunkCount <= GetChunkCount());
            assert_pnc(chunkIndexFrom >= 0);
            assert_pnc(chunkIndexFrom < chunkArrayFrom.GetChunkCount());
            assert_pnc(chunkIndexFrom + chunkCount <= chunkArrayFrom.GetChunkCount());
            assert_pnc(chunkCount >= 0);
            assert_pnc(!chunkArrayFrom.IsNull());
            assert_pnc(IsSameChunkStructure(*this, chunkArrayFrom));
            
            ChunkPointerElementInternal_t& internalChunkElementFrom = chunkArrayFrom.GetInternalChunkElement(chunkIndexFrom);
            const Size_t nodeCountToCopy = internalChunkElementFrom.NodeCount;
            assert_pnc(nodeCountToCopy < NodeCapacityPerChunk);

            ChunkPointerInternal_t& internalChunkArrayTo = GetInternalChunk();
            assert_pnc(!internalChunkArrayTo.IsNull());

            new(&internalChunkArrayTo.Array.Chunks[chunkIndexTo])ChunkPointerElement_t(internalChunkArrayTo.Structure, nodeCountToCopy, GetComponentDataArrayForChunk(chunkIndexTo));

            if (nodeCountToCopy > 0)
            {
                ChunkPointerElementInternal_t& internalChunkElementTo = GetInternalChunkElement(chunkIndexTo);
                Node_t::CopyNode(internalChunkElementTo, internalChunkElementFrom, 0, 0, nodeCountToCopy);
            }
        }
        //void ConstructElementNode(const Size_t chunkIndex, const Size_t nodeCount)
        //{
        //    assert_pnc(chunkIndex < ChunkCapacity);
        //    assert_pnc(chunkIndex < GetChunkCount());// Element be constructed before the ChunkCount includes it.
        //    assert_pnc(chunkIndex * NodeCapacityPerChunk + nodeCount < GetNodeCapacityTotal());

        //    ChunkPointerInternal_t& internal = GetInternalChunk();
        //    if (nodeCount > 0)
        //        Node_t::ConstructNode(internal.Array.Chunks[chunkIndex], 0, nodeCount);
        //}

        //void ConstructChunkArray(Size_t nodeCountPerChunk = 0)
        //{
        //    auto& chunk = GetInternalChunk();
        //    if (chunk.IsNull())
        //        for (int i = 0; i < ChunkCapacity; ++i)
        //            new(&chunk.Array.Chunks[i])ChunkPointerElement_t();
        //    else
        //        for (int i = 0; i < ChunkCapacity; ++i)
        //            new(&chunk.Array.Chunks[i])ChunkPointerElement_t(chunk.Structure, nodeCountPerChunk, GetComponentDataForChunk(i));
        //}

        void** GetComponentDataArrayForChunk(Size_t chunkIndex)
        {
            auto& chunk = GetInternalChunk();
            return &chunk.ComponentData[chunkIndex * chunk.Structure->Components.GetSize()];
        }

    };
}
