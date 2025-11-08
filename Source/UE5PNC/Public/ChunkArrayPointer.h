// MIT License
// Copyright (c) 2025 Stephanie Rancourt

#pragma once
#include "common.h"
#include "ChunkArrayPointerInternal.h"

namespace PNC
{
    /// <summary>
    /// A ChunkArrayPointer points to an Array of ChunkPointers whose Component data is adjacent in memory.
    /// A ChunkArrayPointer is itself a ChunkPointer pointing to the beginning of the Component data, 
    /// effectively pointing to the first Chunk in the Array.
    /// </summary>
    /// <typeparam name="TChunkStructure">Structure of the Chunk's Component data.</typeparam>
    /// <typeparam name="TChunkPointerElement">Structure of the Chunk pointer in the Array. 
    ///     Must be a ChunkPointer (does not copy nodes when copied) rather than a Chunk (copy nodes when copied)
    /// </typeparam>
    template<typename TChunkStructure, typename TChunkPointerElement = ChunkPointerT<TChunkStructure>>
    struct ChunkArrayPointerT : public ChunkPointerT<TChunkStructure>
    {
    public:
        using Base_t = ChunkPointerT<TChunkStructure>;
        using Self_t = ChunkArrayPointerT<TChunkStructure, TChunkPointerElement>;
        using Base_t::Size_t;
        using Base_t::ComponentType_t;
        using Base_t::ChunkStructure_t;
        using Base_t::Node_t;
        using ChunkPointerElement_t = TChunkPointerElement;


        /// <summary>
        /// Chunk_t is the type a ChunkPointer points to
        /// </summary>
        using Chunk_t = Self_t;

        /// <summary>
        /// Use the same ChunkArrayExtension_t as ChunkArrayPointerInternalT so both keep the same memory layout.
        /// </summary>
        using ChunkArrayExtension_t = typename ChunkArrayPointerInternalT<ChunkStructure_t, TChunkPointerElement>::ChunkArrayExtension_t;

        /// <summary>
        /// Reinterprete_cast this object to this type to get read-write access to it's private data fields
        /// Use with caution.
        /// </summary>
        using ChunkPointerInternal_t = ChunkArrayPointerInternalT<ChunkStructure_t, TChunkPointerElement>;

        using ChunkPointerElementInternal_t = typename ChunkPointerElement_t::ChunkPointerInternal_t;

    protected:
        /// <summary>
        /// Contains pointer to the array of Chunks
        /// </summary>
        ChunkArrayExtension_t Array;

    public:
        ChunkArrayPointerT() = default;
        ChunkArrayPointerT(const Self_t& o) = default;
        Self_t& operator=(const Self_t& o) = default;
        ChunkArrayPointerT(Self_t&& o) = default;
        Self_t& operator=(Self_t&& o) = default;

        /// <summary>
        /// Contructs from its member data fields.
        /// </summary>
        /// <param name="chunkStructure">Structure of the Chunk's Component data.</param>
        /// <param name="componentData">Points to an array of Component data pointers created according to the ChunkStructure.</param>
        /// <param name="chunks">Points to an array of ChunkPointerElement_t the size of chunkCount or more.</param>
        /// <param name="chunkCount">Number of Chunks in the Array.</param>
        /// <param name="totalNodeCount">The total number of Nodes used by all Chunks in the Array.</param>
        ChunkArrayPointerT(const ChunkStructure_t* chunkStructure, void** componentData, ChunkPointerElement_t* chunks, Size_t chunkCount, Size_t totalNodeCount)
            : Base_t(chunkStructure, totalNodeCount, componentData)
            , Array(chunks, chunkCount)
        {

        }

    protected:
        ChunkArrayPointerT(const ChunkStructure_t*const chunkStructure, const Size_t chunkCount, const Size_t nodeCountPerChunk)
            : Base_t(chunkStructure, chunkCount * nodeCountPerChunk)
            , Array(chunkCount)
        {
        }

    public:
        using Base_t::IsVoid;
        using Base_t::IsStruct;
        using Base_t::IsNull;
        using Base_t::IsData;
        using Base_t::IsVoidNull;
        using Base_t::IsVoidData;
        using Base_t::IsStructNull;
        using Base_t::IsStructData;

        using Base_t::GetStructure;
        using Base_t::GetNodeCount;
        using Base_t::GetNodeCapacity;
        using Base_t::GetChunkCount;
        using Base_t::GetChunkCapacity;
        /// <summary>
        /// Number of elements in the array
        /// </summary>
        /// <returns></returns>
        Size_t GetChunkCount()const { return Array.ChunkCount; }
        Size_t GetChunkCapacity()const { return GetChunkCount(); }

        // TODO Array indexing must return copies
        const ChunkPointerElement_t operator[](Size_t index)const { return Array.Chunks[index]; }
        //// TODO Array indexing must return copies
        //ChunkPointerElement_t& operator[](Size_t index) { return Array.Chunks[index]; }
        // TODO Array indexing must return copies
        const ChunkPointerElement_t GetChunk(Size_t index)const { return Array.Chunks[index]; }
        //// TODO Array indexing must return copies
        //ChunkPointerElement_t GetChunk(Size_t index) { return Array.Chunks[index]; }
        const Chunk_t& operator*()const { return *this; }
        Chunk_t& operator*() { return *this; }
        const Chunk_t* operator->()const { return this; }
        Chunk_t* operator->() { return this; }
        const Chunk_t& GetChunk()const { return *this; }
        Chunk_t& GetChunk() { return *this; }

        static ChunkPointerInternal_t& GetInternalChunk(Self_t& chunkPointer) { return reinterpret_cast<ChunkPointerInternal_t&>(chunkPointer.GetChunk()); }
        static const ChunkPointerInternal_t& GetInternalChunk(const Self_t& chunkPointer) { return reinterpret_cast<const ChunkPointerInternal_t&>(chunkPointer.GetChunk()); }
        static ChunkPointerElementInternal_t& GetInternalChunkElement(Self_t& chunkPointer, const Size_t index) { return reinterpret_cast<ChunkPointerElementInternal_t&>(chunkPointer.Array.Chunks[index]); }
        static const ChunkPointerElementInternal_t& GetInternalChunkElement(const Self_t& chunkPointer, const Size_t index) { return reinterpret_cast<const ChunkPointerElementInternal_t&>(chunkPointer.Array.Chunks[index]); }
    protected:
        //ChunkPointerInternal_t& GetInternalChunk() { return reinterpret_cast<ChunkPointerInternal_t&>(GetChunk()); }
        //ChunkPointerElementInternal_t& GetInternalChunkElement(const Size_t index) { return reinterpret_cast<ChunkPointerElementInternal_t&>(Array.Chunks[index]); }
        const ChunkArrayExtension_t& GetArrayExtension()const { return Array; }
        ChunkArrayExtension_t& GetArrayExtension() { return Array; }

    public:
        static void ConstructChunkElementAndNodes(Self_t& chunkArray, const Size_t chunkIndex, const Size_t nodeFirstIndex, const Size_t nodeCount, void**const componentDataArray)
        {
            pnc_assert(chunkIndex >= 0);
            pnc_assert(chunkIndex < chunkArray.Array.ChunkCount);
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
                internalChunkElement.ComponentData[i] = componentType.Forward(internalChunkArray.ComponentData[i], nodeFirstIndex, chunkIndex);
            }
            
            if(nodeCount > 0)
                Node_t::ConstructAllComponentsUnsafe(internalChunkElement, 0, nodeCount);
            else
                Node_t::ConstructAllChunkComponentsUnsafe(internalChunkElement, 0);

        }

        static void DestructChunkElementAndNodes(Self_t& chunkArray, const Size_t chunkIndex)
        {
            pnc_assert(chunkIndex >= 0);
            pnc_assert(chunkIndex < chunkArray.Array.ChunkCount);

            ChunkPointerInternal_t& internalChunkArray = GetInternalChunk(chunkArray);
            pnc_assert(!internalChunkArray.IsNull());

            ChunkPointerElementInternal_t& internalChunkElement = GetInternalChunkElement(chunkArray, chunkIndex);
            if (internalChunkElement.NodeCount > 0)
                Node_t::DestructAllComponentsUnsafe(internalChunkElement, 0, internalChunkElement.NodeCount);
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
                internalElementTo.ComponentData = GetComponentDataArrayForChunk(elementIndexTo);

                // Copy Nodes
                if (nodeCountFrom > 0)
                    Node_t::CopyConstructAllComponentsForward(internalElementTo, internalElementFrom, 0, 0, nodeCountFrom);
            }
        }
        static void MoveElementAndNodesForward(Self_t& chunkArrayTo, const Size_t chunkIndexTo, Self_t& chunkArrayFrom, const Size_t chunkIndexFrom, const Size_t chunkCount)
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

    };
}
