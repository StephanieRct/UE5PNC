// MIT License
// Copyright (c) 2025 Stephanie Rancourt

#pragma once
#include "common.h"
#include "ChunkArrayPointer.h"

namespace PNC
{
    template<typename TBase>
    struct BucketChunkArrayPointerT : public TBase
    {
    public:
        using Base_t = TBase;
        using Self_t = BucketChunkArrayPointerT<TBase>;
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
        BucketChunkArrayPointerT()
            : NodeCapacityPerChunk(0)
            , ChunkCapacity(0)
        {
        }

        BucketChunkArrayPointerT(Self_t&& o) = default;

    protected:
        BucketChunkArrayPointerT(const ChunkStructure_t* const chunkStructure, const Size_t nodeCapacityPerChunk, const Size_t chunkCapacity, const Size_t chunkCount, const Size_t nodeCountPerChunk)
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
        using Base_t::IsSameStructure;
        
        Size_t AvailableChunks()const { return ChunkCapacity - GetChunkCount(); }
        
        void NewChunk() 
        {

        }
        // TODO
        void Clear() {}

        /// <summary>
        /// The total maximum number of Nodes the Array can grow to.
        /// </summary>
        /// <returns></returns>
        Size_t GetNodeCapacity()const { return NodeCapacityPerChunk * ChunkCapacity; }

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

        using Base_t::GetInternalChunk;
        using Base_t::GetInternalChunkElement;

    protected:

        void ConstructChunkElementAndNodes(const Size_t chunkIndex, const Size_t nodeCount)
        {
            pnc_assert(chunkIndex >= 0);
            pnc_assert(chunkIndex < ChunkCapacity);
            pnc_assert(chunkIndex < GetChunkCount());

            pnc_assert(nodeCount >= 0);
            pnc_assert(nodeCount <= NodeCapacityPerChunk);

            ChunkPointerInternal_t& internalChunkArray = GetInternalChunk(*this);
            pnc_assert(!internalChunkArray.IsNull());

            new(&internalChunkArray.Array.Chunks[chunkIndex]) ChunkPointerElement_t(internalChunkArray.Structure, nodeCount, GetComponentDataArrayForChunk(chunkIndex));
            ChunkPointerElementInternal_t& internalChunkElement = GetInternalChunkElement(*this, chunkIndex);

            const Size_t componentCount = internalChunkArray.Structure->Components.GetSize();
            for (Size_t i = 0; i < componentCount; ++i)
            {
                const ComponentType_t& componentType = *internalChunkArray.Structure->Components[i];
                internalChunkElement.ComponentData[i] = componentType.Forward(internalChunkArray.ComponentData[i], chunkIndex * NodeCapacityPerChunk, chunkIndex);
            }
            
            if(nodeCount > 0)
                Node_t::ConstructAllComponentsUnsafe(internalChunkElement, 0, nodeCount);
            else
                Node_t::ConstructAllChunkComponentsUnsafe(internalChunkElement, 0);

        }

        void DestructChunkElementAndNodes(const Size_t chunkIndex)
        {
            pnc_assert(chunkIndex >= 0);
            pnc_assert(chunkIndex < ChunkCapacity);
            pnc_assert(chunkIndex < GetChunkCount());

            ChunkPointerInternal_t& internalChunkArray = GetInternalChunk(*this);
            pnc_assert(!internalChunkArray.IsNull());

            ChunkPointerElementInternal_t& internalChunkElement = GetInternalChunkElement(*this, chunkIndex);
            if (internalChunkElement.NodeCount > 0)
                Node_t::DestructAllComponentsUnsafe(internalChunkElement, 0, internalChunkElement.NodeCount);
            else
                Node_t::DestructAllChunkComponentsUnsafe(internalChunkElement, 0);
            
            internalChunkElement.~ChunkPointerElementInternal_t();
        }

        // Nodes must fit in target chunk.
        static void CopyElementAndNodesForward(const Self_t& chunkArrayTo, const Size_t elementFirstIndexTo, const Self_t& chunkArrayFrom, const Size_t elementFirstIndexFrom, const Size_t elementCount)
        {
            pnc_assert(!chunkArrayTo.IsNull());
            pnc_assert(!chunkArrayFrom.IsNull());
            pnc_assert(IsSameStructure(chunkArrayTo, chunkArrayFrom));
            pnc_assert(elementCount >= 0);
            pnc_assert(elementFirstIndexTo >= 0);
            pnc_assert(elementFirstIndexTo < chunkArrayTo.GetChunkCapacity());
            pnc_assert(elementFirstIndexTo < chunkArrayTo.GetChunkCount());
            pnc_assert(elementFirstIndexTo + elementCount <= chunkArrayTo.GetChunkCapacity());
            pnc_assert(elementFirstIndexTo + elementCount <= chunkArrayTo.GetChunkCount());
            pnc_assert(elementFirstIndexFrom >= 0);
            pnc_assert(elementFirstIndexFrom < chunkArrayFrom.GetChunkCapacity());
            pnc_assert(elementFirstIndexFrom < chunkArrayFrom.GetChunkCount());
            pnc_assert(elementFirstIndexFrom + elementCount <= chunkArrayFrom.GetChunkCapacity());
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
                pnc_assert(chunkArrayTo.NodeCapacityPerChunk >= nodeCountFrom);

                // Copy chunk pointer element using copy asignment, will point to the data in the chunkArrayFrom
                elementTo = elementFrom;
                // point back to data owned by chunkArrayTo 
                internalElementTo.ComponentData = GetComponentDataArrayForChunk(elementIndexTo);

                // Copy Nodes
                if (nodeCountFrom > 0)
                    Node_t::CopyConstructAllComponentsForward(internalElementTo, internalElementFrom, 0, 0, nodeCountFrom);
            }
        }
        void MoveElementAndNodesForward(const Size_t chunkIndexTo, const Size_t chunkIndexFrom, const Size_t chunkCount, const Self_t& chunkArrayFrom)
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
        //    internalChunkElementTo.ComponentData = GetComponentDataArrayForChunk(chunkIndexTo);

        //    // Move Nodes
        //    if (nodeCountFrom > 0)
        //        Node_t::MoveNode(internalChunkElementTo, internalChunkElementFrom, 0, 0, nodeCountFrom);
        //    
        }
        void SwapElementAndNodes(const Size_t chunkIndexTo, const Size_t chunkIndexFrom, const Size_t chunkCount, const Self_t& chunkArrayFrom)
        {
            pnc_todo;
        //    
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
        //    const Size_t nodeCountTo = chunkElementTo.GetNodeCount();
        //    const Size_t nodeCountFrom = chunkElementFrom.GetNodeCount();
        //    pnc_assert(NodeCapacityPerChunk >= nodeCountFrom);
        //    pnc_assert(chunkArrayFrom.NodeCapacityPerChunk >= nodeCountFrom);

        //    pnc_assert(false);
        //    //// Swap Chunk
        //    //chunkElementTo.Swap(chunkElementFrom);
        //    //
        //    //// Swap ComponentData pointers back, we will be swaping the node data
        //    //std::swap(internalChunkElementTo.ComponentData = internalChunkElementFrom.ComponentData);
        //    //
        //    //// Swap Nodes
        //    //if (nodeCountToMove > 0)
        //    //    Node_t::MoveNode(internalChunkElementTo, internalChunkElementFrom, 0, 0, nodeCountToMove);
        }

        //void ConstructElementNode(const Size_t chunkIndex, const Size_t nodeCount)
        //{
        //    pnc_assert(chunkIndex < ChunkCapacity);
        //    pnc_assert(chunkIndex < GetChunkCount());// Element be constructed before the ChunkCount includes it.
        //    pnc_assert(chunkIndex * NodeCapacityPerChunk + nodeCount < GetNodeCapacityTotal());

        //    ChunkPointerInternal_t& internal = GetInternalChunk(*this);
        //    if (nodeCount > 0)
        //        Node_t::ConstructNode(internal.Array.Chunks[chunkIndex], 0, nodeCount);
        //}

        //void ConstructChunkArray(Size_t nodeCountPerChunk = 0)
        //{
        //    auto& chunk = GetInternalChunk(*this);
        //    if (chunk.IsNull())
        //        for (int i = 0; i < ChunkCapacity; ++i)
        //            new(&chunk.Array.Chunks[i])ChunkPointerElement_t();
        //    else
        //        for (int i = 0; i < ChunkCapacity; ++i)
        //            new(&chunk.Array.Chunks[i])ChunkPointerElement_t(chunk.Structure, nodeCountPerChunk, GetComponentDataForChunk(i));
        //}

        void** GetComponentDataArrayForChunk(Size_t chunkIndex)
        {
            auto& chunk = GetInternalChunk(*this);
            return &chunk.ComponentData[chunkIndex * chunk.Structure->Components.GetSize()];
        }

    };
}
