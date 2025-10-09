// MIT License
// Copyright (c) 2025 Stephanie Rancourt

#pragma once
#include "common.h"

namespace PNC
{
    template<typename TBase>
    struct BucketChunkT : public TBase
    {
    public:
        using Base_t = TBase;
        using Self_t = BucketChunkT<TBase>;
        using typename Base_t::ChunkStructure_t;
        using typename Base_t::Size_t;
        using typename Base_t::ChunkPointer_t;
        using typename Base_t::Node_t;

    protected:
        using ChunkPointerInternal_t = ChunkPointerT<ChunkStructure_t>::ChunkPointerInternal_t;

    protected:
        /// <summary>
        /// Maximum number of Nodes this Chunk can grow to.
        /// </summary>
        Size_t NodeCapacity;

    protected:
        /// <summary>
        /// Create a Null Chunk without ChunkStructure.
        /// IsNull() will evaluate to true.
        /// </summary>
        BucketChunkT()
            : Base_t()
            , NodeCapacity(0)
        {
        }

        BucketChunkT(const Self_t& o) = default;
        BucketChunkT(Self_t&& o) = default;
        Self_t& operator=(const Self_t& o) = default;

        /// <summary>
        /// Create a Chunk of a given ChunkStructure and allocate the Component's memory
        /// The Components' memory can fit as many instances of each Components as the Chunk's capacity.
        /// Any computation performed on this Chunk will only process node within the chunk's Node count and not it's capacity.
        /// </summary>
        /// <param name="chunkStructure">Structure of the Chunk's component data.</param>
        /// <param name="nodeCapacity">Maximum number of Nodes this Chunk can grow to.</param>
        /// <param name="nodeCount"></param>
        BucketChunkT(const ChunkStructure_t* chunkStructure, Size_t nodeCapacity, Size_t nodeCount = 0)
            : Base_t(chunkStructure, nodeCount)
            , NodeCapacity(nodeCapacity)
        {
        }

    public:
        using TBase::GetChunk;
        using Base_t::operator*;
        using Base_t::operator->;

        /// <summary>
        /// Get the maximum number of Nodes the Chunk can grow to.
        /// </summary>
        /// <returns>The capacity of the chunk</returns>
        Size_t GetNodeCapacity()const { return NodeCapacity; }

        /// <summary>
        /// Create a Null Chunk without ChunkStructure nor Component data.
        /// </summary>
        /// <returns></returns>
        static Self_t Null() { return Self_t(); }

        Size_t AvailableNodes()const
        {
            auto& chunk = GetChunk();
            return NodeCapacity - chunk.GetNodeCount();
        }
        Size_t AddNode() { return AddNodes(1); }
        Size_t AddNodes(const Size_t count)
        {
            auto& chunk = GetInternalChunk();
            assert_pnc(!chunk.IsNull());
            Size_t firstIndex = chunk.NodeCount;
            if (firstIndex + count <= NodeCapacity)
            {
                if (firstIndex == 0)
                {
                    // construct chunk components
                    Node_t::ConstructChunkComponentsUnsafe(GetChunk());
                }
                Node_t::ConstructNodeComponentsUnsafe(GetChunk(), firstIndex, count);
                chunk.NodeCount += count;
                return firstIndex;
            }
            return -1;
        }

        //Size_t RemoveNode(const Size_t count) // Remove from the end of the chunk
        void Clear()
        {
            auto& chunk = GetInternalChunk();
            if (chunk.IsNull()) return;
            Node_t::DestructNode(GetChunk(), 0, chunk.NodeCount);
            chunk.NodeCount = 0;
        }
    protected:
        using TBase::GetInternalChunk;
    };
}
