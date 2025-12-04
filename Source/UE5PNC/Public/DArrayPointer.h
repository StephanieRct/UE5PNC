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

        using ChunkPointer_t = Self_t;
        using ChunkPointerElement_t = ArrayExtension_t::ChunkPointerElement_t;
        using ChunkPointerElementInternal_t = typename ChunkPointerElement_t::ChunkPointerInternal_t;

        // ChunkPointerInternal_t must be the same memory layout as this struct
        using ChunkPointerInternal_t = DArrayPointerInternalT<ArrayExtension_t, typename Base_t::ChunkPointerInternal_t>;

    protected:
        /// <summary>
        /// Contains pointer to the array of Chunks
        /// </summary>
        ArrayExtension_t Array;

    public:
        DArrayPointer() = default;

    protected:
        template<typename TProps>
        DArrayPointer(const DPropsTag& tag, const TProps& props)
            : Base_t(tag, props)
            , Array(tag, props)
        {
        }

        DArrayPointer(const Self_t& o) = default;
        Self_t& operator=(const Self_t& o) = default;
        DArrayPointer(Self_t&& o) = default;
        Self_t& operator=(Self_t&& o) = default;

        ///// <summary>
        ///// Contructs from its member data fields.
        ///// </summary>
        ///// <param name="chunkStructure">Structure of the Chunk's Component data.</param>
        ///// <param name="componentData">Points to an array of Component data pointers created according to the ChunkStructure.</param>
        ///// <param name="chunks">Points to an array of ChunkPointerElement_t the size of chunkCount or more.</param>
        ///// <param name="chunkCount">Number of Chunks in the Array.</param>
        ///// <param name="arrayNodeCount">The total number of Nodes used by all Chunks in the Array.</param>
        //DArrayPointerT(const StructurePtr<ChunkStructure_t>& chunkStructure, void**const componentData, ChunkPointerElement_t*const chunks, const ChunkCountT<Size_t> chunkCount, const ArrayNodeCountT<Size_t> arrayNodeCount)
        //    : Base_t(chunkStructure, arrayNodeCount, componentData)
        //    , Array(chunks, chunkCount)
        //{

        //}

    protected:
        //DArrayPointerT(const StructurePtr<ChunkStructure_t>& chunkStructure, const ChunkCountT<Size_t> chunkCount, const NodeCountPerChunkT<Size_t> nodeCountPerChunk)
        //    : Base_t(chunkStructure, chunkCount * nodeCountPerChunk)
        //    , Array(chunkCount)
        //{
        //}

    public:
        /// <summary>
        /// Number of elements in the array
        /// </summary>
        /// <returns></returns>
        ChunkCountT   <Size_t> GetChunkCount   ()const { return Array.GetChunkCount(); }
        ChunkCapacityT<Size_t> GetChunkCapacity()const { return PropCountToCapacity(GetChunkCount()); }


        const ChunkPointer_t& GetChunk()const { return *this; }
              ChunkPointer_t& GetChunk()      { return *this; }

        const ChunkPointerElement_t& GetChunk(const Size_t index)const { return Array.GetChunk(GetInternalChunk(*this), index); }
              ChunkPointerElement_t& GetChunk(const Size_t index)      { return Array.GetChunk(GetInternalChunk(*this), index); }

        const ChunkPointerElement_t& operator[](const Size_t index)const { return Array.GetChunk(GetInternalChunk(*this), index); }
              ChunkPointerElement_t& operator[](const Size_t index)      { return Array.GetChunk(GetInternalChunk(*this), index); }
        // TODO operator -> and * should return first chunk

        static const ChunkPointerInternal_t& GetInternalChunk(const Self_t& container) { return reinterpret_cast<const ChunkPointerInternal_t&>(Base_t::GetInternalChunk(container)); }
        static       ChunkPointerInternal_t& GetInternalChunk(Self_t& container) { return reinterpret_cast<ChunkPointerInternal_t&>(Base_t::GetInternalChunk(container)); }
        static const ChunkPointerElementInternal_t& GetInternalChunkElement(const Self_t& container, const Size_t index) { return reinterpret_cast<const ChunkPointerElementInternal_t&>(container.Array.GetInternalChunk(GetInternalChunk(container), index)); }
        static       ChunkPointerElementInternal_t& GetInternalChunkElement(Self_t& container, const Size_t index) { return reinterpret_cast<ChunkPointerElementInternal_t&>(container.Array.GetInternalChunk(GetInternalChunk(container), index)); }

    protected:
        const ArrayExtension_t& GetArrayExtension()const { return Array; }
        ArrayExtension_t& GetArrayExtension() { return Array; }

        /// <summary>
        /// Set a container's ComponentDataArray to an allocated array of void* large enough to fit one per component per chunk (ChunkCapacity) in the container.
        /// This array is used to store the nodes and chunks component data pointer as void*.
        /// </summary>
        template<typename TChunk>
        static void AllocateComponentDataArray(TChunk& chunk)
        {
            pnc_assert(chunk.IsStruct());
            typename TChunk::ChunkPointerInternal_t& internalChunk = TChunk::GetInternalChunk(chunk);
            const ChunkCapacityT<Size_t> chunkCapacity = chunk.GetChunkCapacity();
            internalChunk.ComponentDataArray = (void**)pnc_alloc(chunkCapacity * internalChunk.Structure->Components.GetSize() * sizeof(void*), alignof(void*));
        }

        /// <summary>
        /// Free a container's ComponentDataArray
        /// </summary>
        template<typename TChunk>
        static void FreeComponentDataArray(TChunk& chunk)
        {
            pnc_assert(chunk.IsStruct());
            typename TChunk::ChunkPointerInternal_t& internalChunk = TChunk::GetInternalChunk(chunk);
            ChunkCapacityT<Size_t> chunkCapacity = chunk.GetChunkCapacity();
            pnc_free_clean(internalChunk.ComponentDataArray, chunkCapacity * internalChunk.Structure->Components.GetSize() * sizeof(void*), alignof(void*));
        }


        /// <summary>
        /// Notes:
        ///     Called by derived structs
        /// </summary>
        template<typename TChunk, typename TProps>
        static void AllocateConstruct(TChunk& chunk, const TProps& props)
        {
            const auto nodeCapacity = props.GetNodeCapacity();
            const auto nodeCount = props.GetNodeCount();
            const auto chunkCapacity = props.GetChunkCapacity();
            const auto chunkCount = props.GetChunkCount();
            Node_t::AllocateConstructAllComponentsUnsafe(chunk, 0, 0, nodeCount, chunkCount, nodeCapacity, chunkCapacity);
            chunk.Array.Allocate (chunk, chunkCapacity, nodeCapacity);
            if (chunkCount > 0)
            {
                const NodeCountPerChunkT<Size_t> nodeCountPerChunk = props.GetNodeCountPerChunk();
                for (Size_t i = 0; i < chunkCount; ++i)
                    chunk.Array.ConstructElement(chunk, 
                        /*elementIndex:*/i,
                        /*firstNodeInArray:*/i * nodeCountPerChunk,
                        /*nodeCount:*/ChunkCountT<Size_t>(1) * nodeCountPerChunk);
            }

        }

        /// <summary>
        /// Notes:
        ///     Called by derived structs
        /// </summary>
        template<typename TChunk>
        static void FreeDestruct(TChunk& chunk)
        {
            chunk.Array.Destruct(chunk);
            chunk.Array.Deallocate(chunk);
            const auto nodeCapacity = chunk.GetNodeCapacity();
            const auto nodeCount = chunk.GetNodeCount();
            const auto chunkCapacity = chunk.GetChunkCapacity();
            const auto chunkCount = chunk.GetChunkCount();
            Base_t::FreeDestruct(chunk, nodeCapacity, nodeCount, chunkCapacity, chunkCount);
        }

        /// <summary>
        /// Notes:
        ///     Called by derived structs
        ///     chunk and chunkFrom CANNOT be the same
        /// </summary>
        template<typename TChunk>
        static void AllocateCopy(TChunk& chunk, const TChunk& chunkFrom, const NodeCapacityT<Size_t> newNodeCapacity, const ChunkCapacityT<Size_t> newChunkCapacity)
        {
            pnc_assert(!IsSameData(chunk, chunkFrom));
            Base_t::AllocateCopy(chunk, chunkFrom,                  newNodeCapacity, newChunkCapacity);
            chunk.Array.Allocate      (chunk,                             newNodeCapacity, newChunkCapacity);
            chunk.Array.ConstructCopy (chunk, chunkFrom, chunkFrom.Array, newNodeCapacity, newChunkCapacity);
        }
        template<typename TChunk>
        static void AllocateCopy(TChunk& chunk, const TChunk& chunkFrom)
            { AllocateCopy(chunk, chunkFrom, chunkFrom.GetNodeCapacity(), chunkFrom.GetChunkCapacity()); }


        /// <summary>
        /// Notes:
        ///     Called by derived structs
        ///     chunkToReallocate and chunkFrom CANNOT be the same
        /// </summary>
        template<typename TChunk>
        static void ReallocateCopy(TChunk& chunkToReallocate, const TChunk& chunkFrom, const NodeCapacityT<Size_t> newNodeCapacity, const ChunkCapacityT<Size_t> newChunkCapacity)
        {
            pnc_assert(!IsSameData(chunkToReallocate, chunkFrom));
            Base_t::ReallocateCopy(chunkToReallocate, chunkFrom, newNodeCapacity, newChunkCapacity);
            chunkToReallocate.Array.ReallocateCopy(chunkToReallocate, chunkFrom, chunkFrom.Array, newNodeCapacity, newChunkCapacity);
        }
        template<typename TChunk>
        static void ReallocateCopy(TChunk& chunkToReallocate, const TChunk& chunkFrom)
            { return ReallocateCopy(chunkToReallocate, chunkFrom, chunkFrom.GetNodeCapacity(), chunkFrom.GetChunkCapacity()); }


        /// <summary>
        /// Notes:
        ///     Called by derived structs
        ///     chunkToReallocate and chunkFrom CAN be the same
        /// </summary>
        template<typename TChunk>
        static void ReallocateMove(TChunk& chunkToReallocate, TChunk& chunkFrom, const NodeCapacityT<Size_t> newNodeCapacity, const ChunkCapacityT<Size_t> newChunkCapacity)
        {
            Base_t::ReallocateMove(chunkToReallocate, chunkFrom, newNodeCapacity, newChunkCapacity);
            chunkToReallocate.Array.ReallocateMove(chunkToReallocate, chunkFrom, chunkFrom.Array, newNodeCapacity, newChunkCapacity);
        }
        template<typename TChunk>
        static void ReallocateMove(TChunk& chunkToReallocate, TChunk& chunkFrom)
            { return ReallocateMove(chunkToReallocate, chunkFrom, chunkFrom.GetNodeCapacity(), chunkFrom.GetChunkCapacity()); }


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

        // Nodes must fit in target chunk.
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

                // Copy chunk pointer element using copy asignment, will point to the data in the chunkArrayFrom
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
