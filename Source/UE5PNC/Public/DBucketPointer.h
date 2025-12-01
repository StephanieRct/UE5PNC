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
        /// Maximum number of Nodes this Chunk can grow to.
        /// </summary>
        NodeCapacityT<Size_t> NodeCapacity;

    protected:
        template<typename TProps>
        DBucketPointer(const DPropsTag& tag, const TProps& props)
            : Base_t(tag, props)
            , NodeCapacity(props.GetNodeCapacity())
        {
        }

        /// <summary>
        /// Create a VoidNull Chunk.
        /// </summary>
        DBucketPointer()
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
        DBucketPointer(const StructurePtr<ChunkStructure_t>& chunkStructure, const NodeCapacityT<Size_t> nodeCapacity, const NodeCountT<Size_t> nodeCount = 0)
            : Base_t(chunkStructure, nodeCount)
            , NodeCapacity(nodeCapacity)
        {
        }

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
        /// <returns>The capacity of the chunk</returns>
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
        //Size_t RemoveNode(const Size_t count) // Remove from the end of the chunk
        void Clear()
        {
            auto& chunk = GetInternalChunk(*this);
            if (chunk.IsNull()) return;
            Node_t::DestructAllNodeComponentsUnsafe(GetChunk(), 0, chunk.NodeCount);
            chunk.NodeCount = 0;
        }
    protected:

        void SetNodeCapacity(const Size_t value) { NodeCapacity = value; }


        /// <summary>
        /// Notes:
        ///     Called by derived structs
        ///     Does not set the NodeCapacity nor NodeCount on chunk. It only allocate, construct and set componentData
        /// </summary>
        template<typename TChunk, typename TProps>
        static void AllocateConstruct(TChunk& chunk, const TProps& props)
        {
            const auto nodeCapacity = props.GetNodeCapacity();
            const auto nodeCount = props.GetNodeCount();
            Node_t::AllocateConstructAllComponentsUnsafe(chunk, 0, 0, nodeCount, 1, nodeCapacity, 1);
        }

        /// <summary>
        /// Notes:
        ///     Called by derived structs
        ///     Does not set the NodeCapacity nor NodeCount on chunk. It only allocate, construct and set componentData
        ///     chunk and chunkFrom cannot be the same chunk
        /// </summary>
        template<typename TChunk>
        static void AllocateCopy(TChunk& chunk, const TChunk& chunkFrom, const NodeCapacityT<Size_t> nodeCapacity, const ChunkCapacityT<Size_t> chunkCapacity)
        {
            pnc_assert(!IsSameData(chunk, chunkFrom));
            const auto nodeCount = chunkFrom.GetNodeCount();
            const auto chunkCount = chunkFrom.GetChunkCount();
            Node_t::AllocateCopyConstructAllComponentsForwardUnsafe(chunk, 0, 0, chunkFrom, 0, 0, nodeCount, chunkCount, nodeCapacity, chunkCapacity);

        }
        /// <summary>
        /// Notes:
        ///     Called by derived structs
        ///     Does not set the NodeCapacity nor NodeCount on chunk. It only allocate, construct and set componentData
        /// </summary>
        template<typename TChunk>
        static void AllocateCopy(TChunk& chunk, const TChunk& chunkFrom)
        {
            AllocateCopy(chunk, chunkFrom, chunkFrom.GetNodeCapacity(), chunkFrom.GetChunkCapacity());
        }

        /// <summary>
        /// Notes:
        ///     Called by derived structs
        ///     Will destruct and free data from chunkTo.
        ///     Does not set NodeCapacity nor NodeCount on chunkTo.
        ///     chunkTo cannot be the same as chunkFrom
        ///     newNodeCapacity must be greater or equal to chunkFrom.GetNodeCapacity()
        /// </summary>
        template<typename TChunk>
        static void ReallocateCopy(TChunk& chunkToReallocate, const TChunk& chunkFrom, const NodeCapacityT<Size_t> newNodeCapacity, const ChunkCapacityT<Size_t> newChunkCapacity)
        {
            pnc_assert(newNodeCapacity >= chunkFrom.GetNodeCapacity());
            pnc_assert(newChunkCapacity >= chunkFrom.GetChunkCapacity());
            pnc_assert(!IsSameData(chunkToReallocate, chunkFrom));
            const auto nodeCount = chunkFrom.GetNodeCount();
            const auto chunkCount = chunkFrom.GetChunkCount();
            Node_t::ReallocateCopyConstructAllComponentsForwardUnsafe(chunkToReallocate, 0, 0,
                chunkFrom, 0, 0,
                nodeCount, chunkCount,
                newNodeCapacity, newChunkCapacity);
        }
        template<typename TChunk>
        static void ReallocateCopy(TChunk& chunkToReallocate, const TChunk& chunkFrom)
        {
            return ReallocateCopy(chunkToReallocate, chunkFrom, chunkFrom.GetNodeCapacity(), chunkFrom.GetChunkCapacity());
        }

        /// <summary>
        /// Notes:
        ///     Called by derived structs
        ///     Will destruct and free (or reuse) data from chunkToReallocate.
        ///     Does not set NodeCapacity nor NodeCount on chunkToReallocate.
        ///     chunkToReallocate and chunkFrom can be the same
        ///     newNodeCapacity must be greater or equal to chunkFrom.GetNodeCapacity()
        /// </summary>
        template<typename TChunk>
        static void ReallocateMove(TChunk& chunkToReallocate, TChunk& chunkFrom, const NodeCapacityT<Size_t> newNodeCapacity, const ChunkCapacityT<Size_t> newChunkCapacity)
        {
            pnc_assert(!chunkToReallocate.IsNull());
            pnc_assert(!chunkFrom.IsNull());
            pnc_assert(IsSameStructure(chunkToReallocate, chunkFrom));
            pnc_assert(newNodeCapacity >= chunkFrom.GetNodeCapacity());

            const ChunkStructure_t& structure = chunkToReallocate.GetStructure();
            const auto componentCount = structure.GetComponentCount();
            const auto nodeCountFrom = chunkFrom.GetNodeCount();
            const auto nodeCountTo = chunkToReallocate.GetNodeCount();
            const auto nodeCapacityTo = chunkToReallocate.GetNodeCapacity();

            typename TChunk::ChunkPointerInternal_t& internalChunk = TChunk::GetInternalChunk(chunkToReallocate);
            void** const componentDataArrayTo = internalChunk.ComponentData;
            for (Size_t i = 0; i < componentCount; ++i)
            {
                const ComponentType_t& componentType = *structure.Components[i];
                void* const dataFrom = chunkFrom.GetComponentData(i);
                switch (componentType.GetOwner())
                {
                case ComponentOwner_Chunk:
                    if (componentDataArrayTo[i] != dataFrom)
                    {
                        //componentType.DestructDataUnsafe(componentDataArrayTo[i], 0, 1);
                        componentType.MoveAssignDataForwardUnsafe(
                            componentDataArrayTo[i], 0,
                            dataFrom, 0, 1);
                    }
                    break;
                case ComponentOwner_Node:
                {
                    void* const dataNew = (void*)pnc_alloc(componentType.GetSize(newNodeCapacity), componentType.GetAlignment());
                    componentType.MoveConstructDataForwardUnsafe(
                        dataNew, 0,
                        dataFrom, 0, nodeCountFrom);

                    componentType.DestructDataUnsafe(componentDataArrayTo[i], 0, nodeCountTo);
                    pnc_free_dirty(componentDataArrayTo[i], componentType.GetSize(nodeCapacityTo), componentType.GetAlignment());

                    componentDataArrayTo[i] = dataNew;
                    break;
                }
                    pnc_assert_switch_default_no_entry();
                }
            }
        }
        template<typename TChunk>
        static void ReallocateMove(TChunk& chunkToReallocate, TChunk& chunkFrom)
        {
            return ReallocateMove(chunkToReallocate, chunkFrom, chunkFrom.GetNodeCapacity(), chunkFrom.GetChunkCapacity());
        }

        /// <summary>
        /// Notes:
        ///     Called by derived structs
        /// </summary>
        template<typename TChunk>
        static void FreeDestruct(TChunk& chunk)
        {
            const auto nodeCount = chunk.GetNodeCount();
            const auto nodeCapacity = chunk.GetNodeCapacity();
            const auto chunkCount = chunk.GetChunkCount();
            const auto chunkCapacity = chunk.GetChunkCapacity();
            void** const componentDataArrayTo = TChunk::GetInternalChunk(chunk).ComponentData;
            const ChunkStructure_t& structure = chunk.GetStructure();
            auto componentCount = structure.GetComponentCount();
            for (Size_t i = 0; i < componentCount; ++i)
            {
                const ComponentType_t& componentType = structure.GetComponentType(i);
                componentType.DestructComponentUnsafe(componentDataArrayTo[i], 0, nodeCount, 0, chunkCount);
                pnc_free_clean(componentDataArrayTo[i], componentType.GetSize(nodeCapacity, chunkCapacity), componentType.GetAlignment());
            }
        }
    };



}
