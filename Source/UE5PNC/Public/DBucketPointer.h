// MIT License
// Copyright (c) 2025 Stephanie Rancourt

#pragma once
#include "common.h"

namespace NiT
{
    /// <summary>
    /// Add a NodeCapacity and methods to add and remove nodes up to the NodeCapacity.
    /// </summary>
    /// <typeparam name="TBase"></typeparam>
    template<typename TBase>
    struct DBucketPointer : public TBase
    {
    public:
        using Base_t = TBase;
        using Self_t = DBucketPointer<TBase>;
        using typename Base_t::ChunkStructure_t;
        using typename Base_t::Size_t;
        using typename Base_t::Node_t;
        using typename Base_t::ChunkPointerInternal_t;
        using typename Base_t::ChunkPointer_t;
        using ComponentType_t = typename ChunkStructure_t::ComponentType_t;

    protected:
        /// <summary>
        /// Maximum number of Nodes this Container can grow to.
        /// </summary>
        NodeCapacityT<Size_t> NodeCapacity;
    public:

        /// <summary>
        /// Create a VoidNull Container.
        /// </summary>
        DBucketPointer()
            : Base_t()
            , NodeCapacity(0)
        {
        }

        /// <summary>
        /// Create a StructNull Container
        /// </summary>
        DBucketPointer(const ChunkStructure_t* const chunkStructure)
            : Base_t(chunkStructure)
            , NodeCapacity(0)
        {
        }

        /// <summary>
        /// Create a StructData from a ComponentDataArray with a NodeCount equal to its NodeCapacity
        /// </summary>
        DBucketPointer(const ChunkStructure_t* const chunkStructure, 
                       const NodeCountT<Size_t> nodeCount, 
                       const PropComponentDataArray& componentDataArray)
            : Base_t(chunkStructure, nodeCount, componentDataArray)
            , NodeCapacity(PropCountToCapacity(nodeCount))
        {
        }

        DBucketPointer(Self_t&& o)
            : Base_t(std::move(o))
            , NodeCapacity(o.NodeCapacity)
        {
            o.NodeCapacity = NodeCapacityT<Size_t>::V_0();
        }

        Self_t& operator=(Self_t&& o)
        {
            Base_t::operator=(std::move(o));
            auto tmpNodeCapacity = o.NodeCapacity;
            o.NodeCapacity = NodeCapacityT<Size_t>::V_0();
            NodeCapacity = tmpNodeCapacity;
            return *this;
        }

        DBucketPointer(const Self_t& o) = default;
        Self_t& operator=(const Self_t& o) = default;

        using Base_t::GetChunk;
        using Base_t::GetNodeCount;
        using Base_t::GetInternalChunk;
        using Base_t::IsSameData;
        using Base_t::IsSameStructure;

        /// <summary>
        /// Get the maximum number of Nodes the Chunk can grow to.
        /// </summary>
        /// <returns>The capacity of the container</returns>
        NodeCapacityT<Size_t> GetNodeCapacity()const { return NodeCapacity; }

        NodeCountT<Size_t> AvailableNodes()const
        {
            return NodeCapacity - GetNodeCount();
        }
        /// <summary>
        /// Add a single node at index NodeCount and return the index.
        /// If NodeCount >= NodeCapacity, return -1 without adding a new node.
        /// </summary>
        Size_t AddNode() 
        { 
            return AddNodes(1); 
        }

        /// <summary>
        /// Add a multiple sequential nodes at index NodeCount and return the index of the first node added.
        /// If NodeCount + count >= NodeCapacity, return -1 without adding any new nodes.
        /// </summary>
        Size_t AddNodes(const NodeCountT<Size_t> count)
        {
            auto& internalChunk = GetInternalChunk(*this);
            ni_assert(!internalChunk.IsNull());
            Size_t firstIndex = internalChunk.NodeCount;
            if (firstIndex + count <= NodeCapacity)
            {
                Node_t::ConstructAllNodeComponentsUnsafe(internalChunk, firstIndex, count);
                internalChunk.NodeCount += count;
                return firstIndex;
            }
            return -1;
        }
#ifndef PNC_PROPS_STRICT
        Size_t AddNodes(const Size_t count) 
        { 
            return AddNodes(PropNodeCountT<Size_t>(count)); 
        }
#endif
        
        /// <summary>
        /// Remove a range of nodes and close the gap by moving the higher nodes after 
        /// the lower nodes, preserving the order of nodes.
        /// </summary>
        void RemoveNodeKeepOrder(const Size_t firstNodexIndex, const NodeCountT<Size_t> nodeCount = NodeCountT<Size_t>::V_1())
        {
            ni_assert(firstNodexIndex >= 0);
            ni_assert(nodeCount >= 0);
            ni_assert(firstNodexIndex < GetNodeCount());
            ni_assert(firstNodexIndex + nodeCount <= GetNodeCount());

            auto& internalChunk = GetInternalChunk(*this);

            Node_t::DestructAllNodeComponentsUnsafe(internalChunk, firstNodexIndex, nodeCount);

            const auto firstMovingNodeIndex = internalChunk.NodeCount - nodeCount;
            const auto firstFollowingNodeIndex = firstNodexIndex + nodeCount;
            if(firstFollowingNodeIndex < internalChunk.NodeCount)
            {
                const auto followingNodeCount = internalChunk.NodeCount - firstFollowingNodeIndex;
                Node_t::MoveAllNodeComponentsForwardUnsafe(internalChunk, firstNodexIndex,
                                                           internalChunk, firstFollowingNodeIndex,
                                                                          followingNodeCount);
                internalChunk.NodeCount = PropNodeCountT<Size_t>(firstNodexIndex + followingNodeCount);
            }
            else
                internalChunk.NodeCount = PropNodeCountT<Size_t>(firstNodexIndex);
        }
#ifndef PNC_PROPS_STRICT
        void RemoveNodeKeepOrder(const Size_t firstNodexIndex, const Size_t nodeCount = (Size_t)0)
        {
            RemoveNodeKeepOrder(firstNodexIndex, PropNodeCountT<Size_t>(nodeCount));
        }
#endif

        /// <summary>
        /// Remove a range of nodes and close the gap by moving the trailing nodes in 
        /// the gap, which will break the previous ordering of nodes.
        /// </summary>
        void RemoveNode(const Size_t firstNodeIndex, const NodeCountT<Size_t> nodeCount = NodeCountT<Size_t>::V_1())
        {
            ni_assert(firstNodeIndex >= 0);
            ni_assert(nodeCount >= 0);
            ni_assert(firstNodeIndex < GetNodeCount());
            ni_assert(firstNodeIndex + nodeCount <= GetNodeCount());

            auto& internalChunk = GetInternalChunk(*this);

            Node_t::DestructAllNodeComponentsUnsafe(internalChunk, firstNodeIndex, nodeCount);
            const auto lastNodexIndex = firstNodeIndex + nodeCount;
            const auto movingFirstNodeIndex = std::max<Size_t>(lastNodexIndex, internalChunk.NodeCount - nodeCount);
            const auto movingNodeCount = internalChunk.NodeCount - movingFirstNodeIndex;
            if(movingNodeCount > 0)
            {
                Node_t::MoveAllNodeComponentsForwardUnsafe(internalChunk, firstNodeIndex,
                                                           internalChunk, movingFirstNodeIndex,
                                                                          movingNodeCount);
            }
            internalChunk.NodeCount -= nodeCount;
        }
#ifndef PNC_PROPS_STRICT
        void RemoveNode(const Size_t firstNodexIndex, const Size_t nodeCount = (Size_t)1)
        {
            RemoveNode(firstNodexIndex, PropNodeCountT<Size_t>(nodeCount));
        }
#endif

        //TODO Size_t RemoveNode(const Size_t count) // Remove from the end of the container

        /// <summary>
        /// Destruct all nodes and set NodeCount to 0.
        /// ChunkComponents are NOT destructed.
        /// </summary>
        void Clear()
        {
            auto& container = GetInternalChunk(*this);
            if (container.IsNull()) return;
            Node_t::DestructAllNodeComponentsUnsafe(GetChunk(), 0, container.NodeCount);
            container.NodeCount = NodeCountT<Size_t>::V_0();
        }

    protected:
        /// <summary>
        /// Construct from Props
        /// </summary>
        template<typename TProps>
        DBucketPointer(const DPropsTag& tag, const TProps& props)
            : Base_t(tag, props)
            , NodeCapacity(props.GetNodeCapacity())
        {
        }

        void SetNodeCapacity(const NodeCapacityT<Size_t> nodeCapacity) { NodeCapacity = nodeCapacity; }
#ifndef PNC_PROPS_STRICT
        void SetNodeCapacity(const Size_t nodeCapacity) { NodeCapacity = PropNodeCapacityT<Size_t>(nodeCapacity); }
#endif

        /// <summary>
        /// Allocate component memory for props.GetNodeCapacity() nodes and contruct all components for props.GetNodeCount() nodes.
        /// Notes:
        ///     container must be StructData.
        ///     Does not set the NodeCapacity nor NodeCount on container. 
        ///     Will Set the container's ComponentDataArray void* values to the newly allocated component data.
        /// </summary>
        template<typename TContainer, typename TProps>
        static void AllocateConstruct(TContainer& container, const TProps& props)
        {
            const auto nodeCapacity = props.GetNodeCapacity();
            const auto nodeCount = props.GetNodeCount();
            Node_t::AllocateConstructAllComponentsUnsafe(
                        container, 0/*:firstNodeIndex*/, 0/*:firstChunkIndex*/, 
                                   nodeCapacity,         ChunkCapacityT<Size_t>::V_1(), 
                                   nodeCount,            ChunkCountT<   Size_t>::V_1());
        }
    };
}
