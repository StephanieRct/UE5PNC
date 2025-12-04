// MIT License
// Copyright (c) 2025 Stephanie Rancourt

#pragma once
#include "common.h"

namespace PNC
{
    template<typename TBase>
    struct DBarrelPointer : public TBase
    {
    public:
        using Base_t = TBase;
        using Self_t = DBarrelPointer<TBase>;
        using typename Base_t::ChunkStructure_t;
        using typename Base_t::Size_t;
        using typename Base_t::ChunkPointerElement_t;
        using typename Base_t::ChunkPointerInternal_t;
        using typename Base_t::ChunkPointerElementInternal_t;
        using typename Base_t::Node_t;
        using ComponentType_t = typename ChunkStructure_t::ComponentType_t;

    protected:

        /// <summary>
        /// Maximum number of Chunks this Array can grow to.
        /// </summary>
        ChunkCapacityT<Size_t> ChunkCapacity;

        /// <summary>
        /// Maximum number of Nodes each Chunk can grow to.
        /// </summary>
        NodeCapacityPerChunkT<Size_t> NodeCapacityPerChunk;

    public:
        template<typename TProps>
        DBarrelPointer(const DPropsTag& tag, const TProps& props)
            : Base_t(tag, props),
              ChunkCapacity(props.GetChunkCapacity()),
              NodeCapacityPerChunk(props.GetNodeCapacityPerChunk())
        {
        }

        /// <summary>
        /// Create a Null Chunk
        /// </summary>
        DBarrelPointer()
            : ChunkCapacity(0),
              NodeCapacityPerChunk(0)
        {
        }

        DBarrelPointer(Self_t&& o) = default;

    protected:
        DBarrelPointer(const StructurePtr<ChunkStructure_t>& chunkStructure, const ChunkCapacityT<Size_t> chunkCapacity, const ChunkCountT<Size_t> chunkCount, const NodeCapacityPerChunkT<Size_t> nodeCapacityPerChunk, const NodeCountPerChunkT<Size_t> nodeCountPerChunk)
            : Base_t(chunkStructure, chunkCount, chunkCapacity * nodeCapacityPerChunk),
              ChunkCapacity(chunkCapacity),
              NodeCapacityPerChunk(nodeCapacityPerChunk)
        {
        }


    public:
        using Base_t::operator*;
        using Base_t::operator->;
        using Base_t::operator[];
        using Base_t::GetChunkCount;
        using Base_t::IsSameStructure;
        
        Size_t AvailableChunks()const { return ChunkCapacity - GetChunkCount(); }

        // TODO 
        void AddChunks() {}
        // TODO
        void RemoveChunks() {}
        // TODO
        void ClearChunks() {}

        /// <summary>
        /// The total maximum number of Nodes the Array can grow to.
        /// </summary>
        /// <returns></returns>
        NodeCapacityT<Size_t> GetNodeCapacity()const { return ChunkCapacity * NodeCapacityPerChunk; }

        /// <summary>
        /// The maximum number of Nodes each Chunks in the Array can grow to.
        /// </summary>
        /// <returns></returns>
        NodeCapacityPerChunkT<Size_t> GetNodeCapacityPerChunk()const { return NodeCapacityPerChunk; }

        /// <summary>
        /// The macimum number of Chunks the Array can grow to.
        /// </summary>
        /// <returns></returns>
        ChunkCapacityT<Size_t> GetChunkCapacity()const { return ChunkCapacity; }

        using Base_t::GetInternalChunk;
        using Base_t::GetInternalChunkElement;

    protected:

        static void ConstructChunkElementAndNodes(Self_t& chunkArray, const Size_t chunkIndex, const Size_t nodeFirstIndex, const NodeCountT<Size_t> nodeCount, void** const componentDataArray)
        {
            pnc_assert(chunkIndex < chunkArray.ChunkCapacity);
            pnc_assert(nodeCount <= chunkArray.NodeCapacityPerChunk);
            Base_t::ConstructChunkElementAndNodes(chunkArray, chunkIndex, nodeFirstIndex, nodeCount, componentDataArray);
        }

        static void DestructChunkElementAndNodes(Self_t& chunkArray, const Size_t chunkIndex)
        {
            pnc_assert(chunkIndex < chunkArray.ChunkCapacity);
            Base_t::DestructChunkElementAndNodes(chunkArray, chunkIndex);
        }

        // Nodes must fit in target chunk.
        static void CopyElementAndNodesForward(const Self_t& chunkArrayTo, const Size_t elementFirstIndexTo, const Self_t& chunkArrayFrom, const Size_t elementFirstIndexFrom, const Size_t elementCount)
        {
            pnc_assert(chunkArrayTo.NodeCapacityPerChunk >= chunkArrayFrom.NodeCapacityPerChunk);
            pnc_assert(elementFirstIndexTo < chunkArrayTo.GetChunkCapacity());
            pnc_assert(elementFirstIndexTo + elementCount <= chunkArrayTo.GetChunkCapacity());
            pnc_assert(elementFirstIndexFrom < chunkArrayFrom.GetChunkCapacity());
            pnc_assert(elementFirstIndexFrom + elementCount <= chunkArrayFrom.GetChunkCapacity());

            Base_t::DestructChunkElementAndNodes(chunkArrayTo, elementFirstIndexTo, chunkArrayFrom, elementFirstIndexFrom, elementCount);
        }

        static void MoveElementAndNodesForward(Self_t& chunkArrayTo, const Size_t chunkIndexTo, Self_t& chunkArrayFrom, const Size_t chunkIndexFrom, const ChunkCountT<Size_t> chunkCount)
        {
            pnc_todo;
            Base_t::MoveElementAndNodesForward(chunkArrayTo, chunkIndexTo, chunkArrayFrom, chunkIndexFrom, chunkCount);
        }
        //void** GetComponentDataArrayForChunk(Size_t chunkIndex)
        //{
        //    auto& chunk = GetInternalChunk(*this);
        //    return &chunk.ComponentDataArray[chunkIndex * chunk.Structure->Components.GetSize()];
        //}

    };
}
