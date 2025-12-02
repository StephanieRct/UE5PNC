// MIT License
// Copyright (c) 2025 Stephanie Rancourt

#pragma once
#include "common.h"

namespace PNC
{
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
        DBucketPointer(const StructurePtr<ChunkStructure_t>& chunkStructure)
            : Base_t(chunkStructure)
            , NodeCapacity(0)
        {
        }

        /// <summary>
        /// Create a StructData from a ComponentDataArray with a NodeCount equal to its NodeCapacity
        /// </summary>
        DBucketPointer(const StructurePtr<ChunkStructure_t>& chunkStructure, const NodeCountT<Size_t> nodeCount, const PropComponentDataArray& componentDataArray)
            : Base_t(chunkStructure, nodeCount, componentDataArray)
            , NodeCapacity(PropCountToCapacity(nodeCount))
        {
        }

    protected:
        template<typename TProps>
        DBucketPointer(const DPropsTag& tag, const TProps& props)
            : Base_t(tag, props)
            , NodeCapacity(props.GetNodeCapacity())
        {
        }


        /// <summary>
        /// Create a StructData Chunk
        /// The Components' memory can fit as many instances of each Components as the Chunk's capacity.
        /// Any computation performed on this Chunk must only process nodes within the container's NodeCount and not it's capacity.
        /// </summary>
        /// <param name="chunkStructure">Structure of the Chunk's component data.</param>
        /// <param name="nodeCapacity">Maximum number of Nodes this Chunk can grow to.</param>
        /// <param name="nodeCount"></param>
        DBucketPointer(const StructurePtr<ChunkStructure_t>& chunkStructure, const NodeCapacityT<Size_t> nodeCapacity, const NodeCountT<Size_t> nodeCount = 0)
            : Base_t(chunkStructure, nodeCount)
            , NodeCapacity(nodeCapacity)
        {
        }
    public:
        DBucketPointer(Self_t&& o)
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

        DBucketPointer(const Self_t& o) = default;
        Self_t& operator=(const Self_t& o) = default;


    public:
        PNC_USING_CHUNKPOINTER_INTERFACE();
        using Base_t::GetChunk;

        /// <summary>
        /// Get the maximum number of Nodes the Chunk can grow to.
        /// </summary>
        /// <returns>The capacity of the container</returns>
        NodeCapacityT<Size_t> GetNodeCapacity()const { return NodeCapacity; }


        NodeCountT<Size_t> AvailableNodes()const
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
        
        void RemoveNodeKeepOrder(const Size_t firstNodexIndex, const NodeCountT<Size_t> nodeCount = 1)
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
        
        void RemoveNode(const Size_t firstNodeIndex, const NodeCountT<Size_t> nodeCount = 1)
        {
            pnc_assert(firstNodeIndex >= 0);
            pnc_assert(nodeCount >= 0);
            pnc_assert(firstNodeIndex < GetNodeCount());
            pnc_assert(firstNodeIndex + nodeCount <= GetNodeCount());

            auto& internalChunk = GetInternalChunk(*this);

            Node_t::DestructAllNodeComponentsUnsafe(internalChunk, firstNodeIndex, nodeCount);
            const Size_t lastNodexIndex = firstNodeIndex + nodeCount;
            const Size_t movingFirstNodeIndex = std::max(Size_t(lastNodexIndex), Size_t(internalChunk.NodeCount - nodeCount));
            const Size_t movingNodeCount = internalChunk.NodeCount - movingFirstNodeIndex;
            if(movingNodeCount > 0)
            {
                Node_t::MoveAllNodeComponentsForwardUnsafe(internalChunk, firstNodeIndex,
                                                           internalChunk, movingFirstNodeIndex,
                                                           movingNodeCount);
            }
            internalChunk.NodeCount -= nodeCount;
        }
        //Size_t RemoveNode(const Size_t count) // Remove from the end of the container
        void Clear()
        {
            auto& container = GetInternalChunk(*this);
            if (container.IsNull()) return;
            Node_t::DestructAllNodeComponentsUnsafe(GetChunk(), 0, container.NodeCount);
            container.NodeCount = 0;
        }
    protected:

        void SetNodeCapacity(const Size_t value) { NodeCapacity = value; }


        /// <summary>
        /// Notes:
        ///     Called by derived structs
        ///     Does not set the NodeCapacity nor NodeCount on container. It only allocate, construct and set componentData
        /// </summary>
        template<typename TContainer, typename TProps>
        static void AllocateConstruct(TContainer& container, const TProps& props)
        {
            const auto nodeCapacity = props.GetNodeCapacity();
            const auto nodeCount = props.GetNodeCount();
            Node_t::AllocateConstructAllComponentsUnsafe(container, 0, 0, nodeCount, 1, nodeCapacity, 1);
        }

        /// <summary>
        /// Notes:
        ///     Called by derived structs
        ///     Does not set the NodeCapacity nor NodeCount on container. It only allocate, construct and set componentData
        ///     container and containerFrom cannot be the same container
        /// </summary>
        template<typename TContainer>
        static void AllocateCopy(TContainer& container, const TContainer& containerFrom, const NodeCapacityT<Size_t> nodeCapacity, const ChunkCapacityT<Size_t> chunkCapacity)
        {
            pnc_assert(!IsSameData(container, containerFrom));
            const auto nodeCount = containerFrom.GetNodeCount();
            const auto chunkCount = containerFrom.GetChunkCount();
            Node_t::AllocateCopyConstructAllComponentsForwardUnsafe(container, 0, 0, containerFrom, 0, 0, nodeCount, chunkCount, nodeCapacity, chunkCapacity);

        }
        /// <summary>
        /// Notes:
        ///     Called by derived structs
        ///     Does not set the NodeCapacity nor NodeCount on container. It only allocate, construct and set componentData
        /// </summary>
        template<typename TContainer>
        static void AllocateCopy(TContainer& container, const TContainer& containerFrom)
        {
            AllocateCopy(container, containerFrom, containerFrom.GetNodeCapacity(), containerFrom.GetChunkCapacity());
        }

        /// <summary>
        /// Notes:
        ///     Called by derived structs
        ///     Will destruct and free data from containerTo.
        ///     Does not set NodeCapacity nor NodeCount on containerTo.
        ///     containerTo cannot be the same as containerFrom
        ///     newNodeCapacity must be greater or equal to containerFrom.GetNodeCapacity()
        /// </summary>
        template<typename TContainer>
        static void ReallocateCopy(TContainer& containerToReallocate, const TContainer& containerFrom, const NodeCapacityT<Size_t> newNodeCapacity, const ChunkCapacityT<Size_t> newChunkCapacity)
        {
            pnc_assert(newNodeCapacity >= containerFrom.GetNodeCapacity());
            pnc_assert(newChunkCapacity >= containerFrom.GetChunkCapacity());
            pnc_assert(!IsSameData(containerToReallocate, containerFrom));
            const auto nodeCountFrom = containerFrom.GetNodeCount();
            const auto chunkCountFrom = containerFrom.GetChunkCount();
            Node_t::ReallocateCopyAllComponentsForwardUnsafe(containerToReallocate, 0/*:firstNodeIndexTo*/,   0/*:firstChunkIndexTo*/,
                                                             containerFrom,         0/*:firstNodeIndexFrom*/, 0/*:firstChunkIndexFrom*/,
                                                                                    nodeCountFrom,            chunkCountFrom,
                                                                                    newNodeCapacity,          newChunkCapacity);
        }
        template<typename TContainer>
        static void ReallocateCopy(TContainer& containerToReallocate, const TContainer& containerFrom)
        {
            return ReallocateCopy(containerToReallocate, containerFrom, containerFrom.GetNodeCapacity(), containerFrom.GetChunkCapacity());
        }

        /// <summary>
        /// Notes:
        ///     Called by derived structs
        ///     Will destruct and free (or reuse) data from containerToReallocate.
        ///     Does not set NodeCapacity nor NodeCount on containerToReallocate.
        ///     containerToReallocate and containerFrom can be the same
        ///     newNodeCapacity must be greater or equal to containerFrom.GetNodeCapacity()
        /// </summary>
        template<typename TContainer>
        static void ReallocateMove(TContainer& containerToReallocate, TContainer& containerFrom, const NodeCapacityT<Size_t> newNodeCapacity, const ChunkCapacityT<Size_t> newChunkCapacity)
        {
            pnc_assert(!containerToReallocate.IsNull());
            pnc_assert(!containerFrom.IsNull());
            pnc_assert(IsSameStructure(containerToReallocate, containerFrom));
            pnc_assert(newNodeCapacity >= containerFrom.GetNodeCapacity());
            const auto nodeCountFrom = containerFrom.GetNodeCount();
            const auto chunkCountFrom = containerFrom.GetChunkCount();
            
            Node_t::ReallocateMoveAllComponentsForwardUnsafe(containerToReallocate, 0/*:firstNodeIndexTo*/,   0/*:firstChunkIndexTo*/,
                                                             containerFrom,         0/*:firstNodeIndexFrom*/, 0/*:firstChunkIndexFrom*/,
                                                                                    nodeCountFrom,            chunkCountFrom,
                                                                                    newNodeCapacity,          newChunkCapacity);
        }
        template<typename TContainer>
        static void ReallocateMove(TContainer& chunkToReallocate, TContainer& containerFrom)
        {
            return ReallocateMove(chunkToReallocate, containerFrom, containerFrom.GetNodeCapacity(), containerFrom.GetChunkCapacity());
        }

        /// <summary>
        /// Notes:
        ///     Called by derived structs
        /// </summary>
        template<typename TContainer>
        static void FreeDestruct(TContainer& container)
        {
            const auto nodeCount = container.GetNodeCount();
            const auto nodeCapacity = container.GetNodeCapacity();
            const auto chunkCount = container.GetChunkCount();
            const auto chunkCapacity = container.GetChunkCapacity();
            void** const componentDataArrayTo = TContainer::GetInternalChunk(container).ComponentData;
            const ChunkStructure_t& structure = container.GetStructure();
            auto componentCount = structure.GetComponentCount();
            for (Size_t i = 0; i < componentCount; ++i)
            {
                const ComponentType_t& componentType = structure.GetComponentType(i);
                componentType.DestructComponentUnsafe(componentDataArrayTo[i], 0, 0, nodeCount, chunkCount);
                pnc_free_clean(componentDataArrayTo[i], componentType.GetSize(nodeCapacity, chunkCapacity), componentType.GetAlignment());
            }
        }
    };
}
