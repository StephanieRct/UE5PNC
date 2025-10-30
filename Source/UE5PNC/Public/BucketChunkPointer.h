// MIT License
// Copyright (c) 2025 Stephanie Rancourt

#pragma once
#include "common.h"

namespace PNC
{
    template<typename TBase>
    struct BucketChunkPointerT : public TBase
    {
    public:
        using Base_t = TBase;
        using Self_t = BucketChunkPointerT<TBase>;
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
        /// Create a VoidNull Chunk.
        /// </summary>
        BucketChunkPointerT()
            : Base_t()
            , NodeCapacity(0)
        {
        }

        /// <summary>
        /// Create a StructData Chunk
        /// The Components' memory can fit as many instances of each Components as the Chunk's capacity.
        /// Any computation performed on this Chunk must only process nodes within the chunk's NodeCount and not it's capacity.
        /// </summary>
        /// <param name="chunkStructure">Structure of the Chunk's component data.</param>
        /// <param name="nodeCapacity">Maximum number of Nodes this Chunk can grow to.</param>
        /// <param name="nodeCount"></param>
        BucketChunkPointerT(const ChunkStructure_t* chunkStructure, Size_t nodeCapacity, Size_t nodeCount = 0)
            : Base_t(chunkStructure, nodeCount)
            , NodeCapacity(nodeCapacity)
        {
        }

        BucketChunkPointerT(Self_t&& o)
            : Base_t(std::forward<Self_t>(o))
            , NodeCapacity(o.NodeCapacity)
        {
            o.NodeCapacity = 0;
        }

        Self_t& operator=(Self_t&& o)
        {
            Base_t::operator=(std::move(o));
            auto tmpNodeCapacity = o.NodeCapacity;
            o.NodeCapacity = 0;
            NodeCapacity = tmpNodeCapacity;
            return *this;
        }

        BucketChunkPointerT(const Self_t& o) = default;
        Self_t& operator=(const Self_t& o) = default;


    public:
        using TBase::GetChunk;
        using TBase::GetNodeCount;
        using Base_t::operator*;
        using Base_t::operator->;
        using TBase::GetInternalChunk;

        /// <summary>
        /// Get the maximum number of Nodes the Chunk can grow to.
        /// </summary>
        /// <returns>The capacity of the chunk</returns>
        Size_t GetNodeCapacity()const { return NodeCapacity; }


        Size_t AvailableNodes()const
        {
            return NodeCapacity - GetNodeCount();
        }
        Size_t AddNode() { return AddNodes(1); }
        Size_t AddNodes(const Size_t count)
        {
            auto& internalChunk = GetInternalChunk(*this);
            pnc_assert(!internalChunk.IsNull());
            Size_t firstIndex = internalChunk.NodeCount;
            if (firstIndex + count <= NodeCapacity)
            {
                Node_t::ConstructAllNodeComponentsUnsafe(internalChunk, firstIndex, count);
                internalChunk.NodeCount += count;
                return firstIndex;
            }
            return -1;
        }
        
        void RemoveNodeKeepOrder(const Size_t firstNodexIndex, const Size_t nodeCount = 1)
        {
            pnc_assert(firstNodexIndex >= 0);
            pnc_assert(nodeCount >= 0);
            pnc_assert(firstNodexIndex < GetNodeCount());
            pnc_assert(firstNodexIndex + nodeCount <= GetNodeCount());

            auto& internalChunk = GetInternalChunk(*this);

            Node_t::DestructAllNodeComponentsUnsafe(internalChunk, firstNodexIndex, nodeCount);

            const Size_t firstMovingNodeIndex = internalChunk.NodeCount - nodeCount;
            const Size_t firstFollowingNodeIndex = firstNodexIndex + nodeCount;
            if(firstFollowingNodeIndex < internalChunk.NodeCount)
            {
                const Size_t followingNodeCount = internalChunk.NodeCount - firstFollowingNodeIndex;
                Node_t::MoveAllNodeComponentsForwardUnsafe(internalChunk, firstNodexIndex,
                                                           internalChunk, firstFollowingNodeIndex,
                                                           followingNodeCount);
                internalChunk.NodeCount = firstNodexIndex + followingNodeCount;
            }
            else
                internalChunk.NodeCount = firstNodexIndex;
        }
        
        void RemoveNode(const Size_t firstNodeIndex, const Size_t nodeCount = 1)
        {
            pnc_assert(firstNodeIndex >= 0);
            pnc_assert(nodeCount >= 0);
            pnc_assert(firstNodeIndex < GetNodeCount());
            pnc_assert(firstNodeIndex + nodeCount <= GetNodeCount());

            auto& internalChunk = GetInternalChunk(*this);

            Node_t::DestructAllNodeComponentsUnsafe(internalChunk, firstNodeIndex, nodeCount);
            const Size_t lastNodexIndex = firstNodeIndex + nodeCount;
            const Size_t movingFirstNodeIndex = std::max(lastNodexIndex, internalChunk.NodeCount - nodeCount);
            const Size_t movingNodeCount = internalChunk.NodeCount - movingFirstNodeIndex;
            if(movingNodeCount > 0)
            {
                Node_t::MoveAllNodeComponentsForwardUnsafe(internalChunk, firstNodeIndex,
                                                           internalChunk, movingFirstNodeIndex,
                                                           movingNodeCount);
            }
            internalChunk.NodeCount -= nodeCount;
        }
        //Size_t RemoveNode(const Size_t count) // Remove from the end of the chunk
        void Clear()
        {
            auto& chunk = GetInternalChunk(*this);
            if (chunk.IsNull()) return;
            Node_t::DestructAllNodeComponentsUnsafe(GetChunk(), 0, chunk.NodeCount);
            chunk.NodeCount = 0;
        }

    };
}
