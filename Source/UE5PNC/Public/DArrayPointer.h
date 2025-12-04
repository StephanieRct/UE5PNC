// MIT License
// Copyright (c) 2025 Stephanie Rancourt

#pragma once
#include "common.h"
#include "DArrayPointerInternal.h"

namespace PNC
{
    template<typename TArrayExtension, typename TBase>
    struct DArrayPointer : public TBase
    {
    public:
        using Base_t = TBase;
        using Self_t = DArrayPointer<TArrayExtension, TBase>;
        using ArrayExtension_t = TArrayExtension;
        using typename Base_t::Size_t;
        using typename Base_t::ComponentType_t;
        using typename Base_t::ChunkStructure_t;
        using typename Base_t::Node_t;

        
        using ChunkPointerElement_t = ArrayExtension_t::ChunkPointerElement_t;
        using ChunkPointerElementInternal_t = typename ChunkPointerElement_t::ChunkPointerInternal_t;

        // ChunkPointer_t memory layout must be a ChunkPointer_t followed by a ArrayExtension_t
        using ChunkPointer_t = DArrayPointerProtectedT<ArrayExtension_t, typename Base_t::ChunkPointer_t>;

        // ChunkPointerInternal_t must be the same memory layout as ChunkPointer_t
        using ChunkPointerInternal_t = DArrayPointerInternalT<ArrayExtension_t, typename Base_t::ChunkPointerInternal_t>;

    protected:
        ArrayExtension_t Array;

    public:
        DArrayPointer() = default;
        DArrayPointer(const Self_t& o) = default;
        Self_t& operator=(const Self_t& o) = default;
        DArrayPointer(Self_t&& o) = default;
        Self_t& operator=(Self_t&& o) = default;

        /// <summary>
        /// Number of constructed chunk elements in the array
        /// </summary>
        ChunkCountT<Size_t> GetChunkCount()const { return Array.GetChunkCount(); }

        /// <summary>
        /// Maximum number of chunk elements the array can hold.
        /// For an Array, it is the same as the ChunkCount.
        /// </summary>
        ChunkCapacityT<Size_t> GetChunkCapacity()const { return PropCountToCapacity(GetChunkCount()); }

        const ChunkPointer_t& GetChunk()const { return reinterpret_cast<const ChunkPointer_t&>(Base_t::GetChunk()); }
              ChunkPointer_t& GetChunk()      { return reinterpret_cast<      ChunkPointer_t&>(Base_t::GetChunk()); }

        const ChunkPointerElement_t& GetChunk(const Size_t index)const { return Array.GetChunk(GetInternalChunk(*this), index); }
              ChunkPointerElement_t& GetChunk(const Size_t index)      { return Array.GetChunk(GetInternalChunk(*this), index); }

        const ChunkPointerElement_t& operator[](const Size_t index)const { return Array.GetChunk(GetInternalChunk(*this), index); }
              ChunkPointerElement_t& operator[](const Size_t index)      { return Array.GetChunk(GetInternalChunk(*this), index); }

        // TODO operator -> and * should return first chunk element

        static const ChunkPointerInternal_t& GetInternalChunk(const Self_t& container) { return reinterpret_cast<const ChunkPointerInternal_t&>(Base_t::GetInternalChunk(container)); }
        static       ChunkPointerInternal_t& GetInternalChunk(      Self_t& container) { return reinterpret_cast<      ChunkPointerInternal_t&>(Base_t::GetInternalChunk(container)); }
        static const ChunkPointerElementInternal_t& GetInternalChunkElement(const Self_t& container, const Size_t index) { return reinterpret_cast<const ChunkPointerElementInternal_t&>(container.Array.GetInternalChunk(GetInternalChunk(container), index)); }
        static       ChunkPointerElementInternal_t& GetInternalChunkElement(      Self_t& container, const Size_t index) { return reinterpret_cast<      ChunkPointerElementInternal_t&>(container.Array.GetInternalChunk(GetInternalChunk(container), index)); }

    protected:
        /// <summary>
        /// Construct from a Props type
        /// </summary>
        template<typename TProps>
        DArrayPointer(const DPropsTag& tag, const TProps& props)
            : Base_t(tag, props)
            , Array(tag, props)
        {
        }

        const ArrayExtension_t& GetArrayExtension()const { return Array; }
              ArrayExtension_t& GetArrayExtension()      { return Array; }

        /// <summary>
        /// Set a container's ComponentDataArray to an allocated array of void* large enough to fit one per component per chunk (ChunkCapacity) in the container.
        /// This array is used to store the nodes and chunks component data pointer as void*.
        /// </summary>
        template<typename TContainer>
        static void AllocateComponentDataArray(TContainer& container)
        {
            pnc_assert(container.IsStruct());
            typename TContainer::ChunkPointerInternal_t& internalChunk = TContainer::GetInternalChunk(container);
            const ChunkCapacityT<Size_t> chunkCapacity = container.GetChunkCapacity();
            internalChunk.ComponentDataArray = (void**)pnc_alloc(chunkCapacity * internalChunk.Structure->Components.GetSize() * sizeof(void*), alignof(void*));
        }

        /// <summary>
        /// Free a container's ComponentDataArray
        /// </summary>
        template<typename TContainer>
        static void FreeComponentDataArray(TContainer& container)
        {
            pnc_assert(container.IsStruct());
            typename TContainer::ChunkPointerInternal_t& internalChunk = TContainer::GetInternalChunk(container);
            ChunkCapacityT<Size_t> chunkCapacity = container.GetChunkCapacity();
            pnc_free_clean(internalChunk.ComponentDataArray, chunkCapacity * internalChunk.Structure->Components.GetSize() * sizeof(void*), alignof(void*));
        }

        /// <summary>
        /// Allocate all components in the array for props.GetNodeCapacity() nodes and props.GetChunkCapacity() chunks.
        /// Contruct all components in the array for props.GetNodeCount() nodes and props.GetChunkCount() chunks.
        /// Notes:
        ///     container must be StructData.
        ///     Does not set the NodeCapacity, NodeCount, ChunkCapacity nor ChunkCount in the container. 
        ///     Will Set the chunk's ComponentDataArray values to the newly allocated component data.
        /// </summary>
        template<typename TContainer, typename TProps>
        static void AllocateConstruct(TContainer& container, const TProps& props)
        {
            const auto nodeCapacity  = props.GetNodeCapacity();
            const auto nodeCount     = props.GetNodeCount();
            const auto chunkCapacity = props.GetChunkCapacity();
            const auto chunkCount    = props.GetChunkCount();
            Node_t::AllocateConstructAllComponentsUnsafe(
                        container, 0/*:firstNodeIndex*/, 0/*:firstChunkIndex*/, 
                                   nodeCount,            chunkCount, 
                                   nodeCapacity,         chunkCapacity);
            container.Array.Allocate(container, chunkCapacity, nodeCapacity);
            if (chunkCount > 0)
            {
                const NodeCountPerChunkT<Size_t> nodeCountPerChunk = props.GetNodeCountPerChunk();
                for (Size_t i = 0; i < chunkCount; ++i)
                    container.Array.ConstructElement(container, 
                                        /*elementIndex:*/    i,
                                        /*firstNodeInArray:*/i * nodeCountPerChunk,
                                        /*nodeCount:*/       ChunkCountT<Size_t>(1) * nodeCountPerChunk);
            }
        }

        /// <summary>
        /// Destruct all components and free their memory.
        /// </summary>
        template<typename TContainer>
        static void FreeDestruct(TContainer& container)
        {
            container.Array.Destruct(container);
            container.Array.Deallocate(container);
            const auto nodeCapacity  = container.GetNodeCapacity();
            const auto nodeCount     = container.GetNodeCount();
            const auto chunkCapacity = container.GetChunkCapacity();
            const auto chunkCount    = container.GetChunkCount();
            Base_t::FreeDestruct(container, nodeCapacity, nodeCount, chunkCapacity, chunkCount);
        }

        /// <summary>
        /// Allocate all components in the array and copy components' data from a different array container with the same structure.
        /// Notes:
        ///     containerTo and containerFrom must be StructData of the same structure.
        ///     containerTo and containerFrom CANNOT be the same container
        ///     Does not set the NodeCapacity, NodeCount, ChunkCapacity, ChunkCount on container.
        ///     Will Set the chunk's ComponentDataArray values to the newly allocated component data.
        /// </summary>
        template<typename TContainer>
        static void AllocateCopy(TContainer& containerTo, const TContainer& containerFrom)
        {
            pnc_assert(!IsSameData(containerTo, containerFrom));
            const auto nodeCapacity  = containerFrom.GetNodeCapacity();
            const auto chunkCapacity = containerFrom.GetChunkCapacity();
            Base_t::AllocateCopy           (containerTo, containerFrom,                      nodeCapacity, chunkCapacity);
            containerTo.Array.Allocate     (containerTo,                                     nodeCapacity, chunkCapacity);
            containerTo.Array.ConstructCopy(containerTo, containerFrom, containerFrom.Array, nodeCapacity, chunkCapacity);
        }


        /// <summary>
        /// Notes:
        ///     Called by derived structs
        ///     containerToReallocate and containerFrom CANNOT be the same
        /// </summary>
        template<typename TContainer>
        static void ReallocateCopy(TContainer& containerToReallocate, const TContainer& containerFrom, const NodeCapacityT<Size_t> newNodeCapacity, const ChunkCapacityT<Size_t> newChunkCapacity)
        {
            pnc_assert(!IsSameData(containerToReallocate, containerFrom));
            Base_t::ReallocateCopy(containerToReallocate, containerFrom, newNodeCapacity, newChunkCapacity);
            containerToReallocate.Array.ReallocateCopy(containerToReallocate, containerFrom, containerFrom.Array, newNodeCapacity, newChunkCapacity);
        }
        template<typename TContainer>
        static void ReallocateCopy(TContainer& containerToReallocate, const TContainer& containerFrom)
            { return ReallocateCopy(containerToReallocate, containerFrom, containerFrom.GetNodeCapacity(), containerFrom.GetChunkCapacity()); }


        /// <summary>
        /// Notes:
        ///     Called by derived structs
        ///     containerToReallocate and containerFrom CAN be the same
        /// </summary>
        template<typename TContainer>
        static void ReallocateMove(TContainer& containerToReallocate, TContainer& containerFrom, const NodeCapacityT<Size_t> newNodeCapacity, const ChunkCapacityT<Size_t> newChunkCapacity)
        {
            Base_t::ReallocateMove(containerToReallocate, containerFrom, newNodeCapacity, newChunkCapacity);
            containerToReallocate.Array.ReallocateMove(containerToReallocate, containerFrom, containerFrom.Array, newNodeCapacity, newChunkCapacity);
        }
        template<typename TContainer>
        static void ReallocateMove(TContainer& containerToReallocate, TContainer& containerFrom)
            { return ReallocateMove(containerToReallocate, containerFrom, containerFrom.GetNodeCapacity(), containerFrom.GetChunkCapacity()); }


    public:
        static void ConstructChunkElementAndNodes(Self_t& chunkArray, const Size_t chunkIndex, const Size_t nodeFirstIndex, const NodeCountT<Size_t> nodeCount, void**const componentDataArray)
        {
            pnc_assert(chunkIndex >= 0);
            pnc_assert(chunkIndex < chunkArray.Array.GetChunkCount());
            pnc_assert(nodeFirstIndex >= 0);
            pnc_assert(nodeCount >= 0);
            pnc_assert(nodeFirstIndex < chunkArray.GetNodeCount());
            pnc_assert(nodeFirstIndex + nodeCount <= chunkArray.GetNodeCount());

            ChunkPointerInternal_t& internalChunkArray = GetInternalChunk(chunkArray);
            pnc_assert(!internalChunkArray.IsNull());

            new(&internalChunkArray.Array.Chunks[chunkIndex]) ChunkPointerElement_t(internalChunkArray.Structure, nodeCount, componentDataArray);
            ChunkPointerElementInternal_t& internalChunkElement = chunkArray.GetInternalChunkElement(chunkArray, chunkIndex);

            const Size_t componentCount = internalChunkArray.Structure->Components.GetSize();
            for (Size_t i = 0; i < componentCount; ++i)
            {
                const ComponentType_t& componentType = *internalChunkArray.Structure->Components[i];
                internalChunkElement.ComponentDataArray[i] = componentType.Forward(internalChunkArray.ComponentDataArray[i], nodeFirstIndex, chunkIndex);
            }
            
            if(nodeCount > 0)
                Node_t::ConstructAllComponentsUnsafe(internalChunkElement, 0, nodeCount);
            else
                Node_t::ConstructAllChunkComponentsUnsafe(internalChunkElement, 0);

        }

        static void DestructChunkElementAndNodes(Self_t& chunkArray, const Size_t chunkIndex)
        {
            pnc_assert(chunkIndex >= 0);
            pnc_assert(chunkIndex < chunkArray.Array.GetChunkCount());

            ChunkPointerInternal_t& internalChunkArray = GetInternalChunk(chunkArray);
            pnc_assert(!internalChunkArray.IsNull());

            auto internalChunkElement = GetInternalChunkElement(chunkArray, chunkIndex);
            if (internalChunkElement.NodeCount > 0)
                Node_t::DestructAllComponentsUnsafe(internalChunkElement, 0, 0, internalChunkElement.NodeCount, 1);
            else
                Node_t::DestructAllChunkComponentsUnsafe(internalChunkElement, 0);
            
            internalChunkElement.~ChunkPointerElementInternal_t();
        }

        // Nodes must fit in target container.
        static void CopyElementAndNodesForward(Self_t& chunkArrayTo, const Size_t elementFirstIndexTo, const Self_t& chunkArrayFrom, const Size_t elementFirstIndexFrom, const Size_t elementCount)
        {
            pnc_assert(!chunkArrayTo.IsNull());
            pnc_assert(!chunkArrayFrom.IsNull());
            pnc_assert(IsSameStructure(chunkArrayTo, chunkArrayFrom));
            pnc_assert(elementCount >= 0);
            pnc_assert(elementFirstIndexTo >= 0);
            pnc_assert(elementFirstIndexTo < chunkArrayTo.GetChunkCount());
            pnc_assert(elementFirstIndexTo + elementCount <= chunkArrayTo.GetChunkCount());
            pnc_assert(elementFirstIndexFrom >= 0);
            pnc_assert(elementFirstIndexFrom < chunkArrayFrom.GetChunkCount());
            pnc_assert(elementFirstIndexFrom + elementCount <= chunkArrayFrom.GetChunkCount());

            for (Size_t i = 0; i < elementCount; ++i)
            {
                auto elementIndexTo = elementFirstIndexTo + i;
                auto elementIndexFrom = elementFirstIndexFrom + i;
                ChunkPointerElement_t& elementTo = this->GetChunkElement(elementIndexTo);
                ChunkPointerElement_t& elementFrom = chunkArrayFrom->GetChunkElement(elementIndexFrom);
                ChunkPointerElementInternal_t& internalElementTo = ChunkPointerElement_t::GetInternalChunk(elementTo);
                ChunkPointerElementInternal_t& internalElementFrom = ChunkPointerElement_t::GetInternalChunk(elementFrom);
                const Size_t nodeCountFrom = elementFrom.GetNodeCount();

                // Copy container pointer element using copy asignment, will point to the data in the chunkArrayFrom
                elementTo = elementFrom;
                // point back to data owned by chunkArrayTo 
                internalElementTo.ComponentDataArray = GetComponentDataArrayForChunk(elementIndexTo);

                // Copy Nodes
                if (nodeCountFrom > 0)
                    Node_t::CopyConstructAllComponentsForward(internalElementTo, internalElementFrom, 0, 0, nodeCountFrom);
            }
        }
        static void MoveElementAndNodesForward(Self_t& chunkArrayTo, const Size_t chunkIndexTo, Self_t& chunkArrayFrom, const Size_t chunkIndexFrom, const ChunkCountT<Size_t> chunkCount)
        {
            pnc_todo;
        //    pnc_assert(chunkIndexTo >= 0);
        //    pnc_assert(chunkIndexTo < ChunkCapacity);
        //    pnc_assert(chunkIndexTo + chunkCount < ChunkCapacity);
        //    pnc_assert(chunkIndexTo < GetChunkCount());
        //    pnc_assert(chunkIndexTo + chunkCount <= GetChunkCount());
        //    pnc_assert(chunkIndexFrom >= 0);
        //    pnc_assert(chunkIndexFrom < chunkArrayFrom.GetChunkCount());
        //    pnc_assert(chunkIndexFrom + chunkCount <= chunkArrayFrom.GetChunkCount());
        //    pnc_assert(chunkCount >= 0);
        //    pnc_assert(!chunkArrayFrom.IsNull());
        //    pnc_assert(IsSameStructure(*this, chunkArrayFrom));

        //    ChunkPointerElement_t& chunkElementTo = this->GetChunkElement(chunkIndexTo);
        //    ChunkPointerElement_t& chunkElementFrom = chunkArrayFrom->GetChunkElement(chunkIndexFrom);
        //    ChunkPointerElementInternal_t& internalChunkElementTo = ChunkPointerElement_t::GetInternalChunk(chunkElementTo);
        //    ChunkPointerElementInternal_t& internalChunkElementFrom = ChunkPointerElement_t::GetInternalChunk(chunkElementFrom);
        //    const Size_t nodeCountFrom = chunkElementFrom.GetNodeCount();
        //    pnc_assert(NodeCapacityPerChunk >= nodeCountFrom);

        //    // Move Chunk
        //    chunkElementTo = std::move(chunkElementFrom);
        //    internalChunkElementTo.ComponentDataArray = GetComponentDataArrayForChunk(chunkIndexTo);

        //    // Move Nodes
        //    if (nodeCountFrom > 0)
        //        Node_t::MoveNode(internalChunkElementTo, internalChunkElementFrom, 0, 0, nodeCountFrom);
        //    
        }

    };
}
