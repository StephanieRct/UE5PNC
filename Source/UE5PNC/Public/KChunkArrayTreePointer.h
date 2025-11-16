// MIT License
// Copyright (c) 2025 Stephanie Rancourt

#pragma once
#include "common.h"
#include "KChunkTreePointer.h"
#include "ChunkArrayExtension.h"

namespace PNC
{
    /// <summary>
    /// A KChunkArrayTreePointer is a KTreePointer pointing to an array of ChunkPointers whose Component data are adjacent in memory.
    /// A KChunkArrayTreePointer is itself a KChunkTreePointer pointing to the beginning of the Component data, 
    /// effectively pointing to the first Chunk in the array.
    /// </summary>
    /// <typeparam name="TChunkStructure">Structure of the Chunk's Component data.</typeparam>
    /// <typeparam name="TChunkPointerElement">Structure of the Chunk pointer in the array.</typeparam>
    template<typename TChunkStructure, typename TChunkPointerElement>
    struct KChunkArrayTreePointerT : public KChunkTreePointerT<TChunkStructure>
    {
    public:
        using Base_t = KChunkTreePointerT<TChunkStructure>;
        using Self_t = KChunkArrayTreePointerT<TChunkStructure, TChunkPointerElement>;
        using ChunkStructure_t = TChunkStructure;
        using ChunkPointerElement_t = TChunkPointerElement;
        using Size_t = typename ChunkPointerElement_t::Size_t;
        using Chunk_t = ChunkArrayPointerT<ChunkStructure_t, ChunkPointerElement_t>; // Structure this pointer points to

        using ChunkArrayPointer_t = Chunk_t;
        using ChunkArrayExtension_t = ChunkArrayExtensionT<ChunkStructure_t, ChunkPointerElement_t>;

        using ChunkPointerInternal_t = typename ChunkArrayPointer_t::ChunkPointerInternal_t;
        using ChunkPointerElementInternal_t = typename ChunkPointerElement_t::ChunkPointerInternal_t;

    protected:
        ChunkArrayExtension_t Array;

    public:
        /// <summary>
        /// Create a Null KChunkArrayTreePointer without ChunkStructure
        /// </summary>
        KChunkArrayTreePointerT()
            : Base_t(ChunkKind_ChunkArrayTree)
        {
        }

        /// <summary>
        /// Contructs the Chunk from its member data fields.
        /// </summary>
        /// <param name="chunkStructure">Structure of the Chunk's Component data.</param>
        /// <param name="componentData">Points to an array of Component data pointers created according to the ChunkStructure.</param>
        /// <param name="chunks">Points to an array of ChunkPointerElement_t the size of chunkCount or more.</param>
        /// <param name="chunkCount">Number of Chunks in the Array.</param>
        /// <param name="totalNodeCount">The total number of Nodes used by all Chunks in the array.</param>
        KChunkArrayTreePointerT(const ChunkStructure_t* chunkStructure, void** componentData, ChunkPointerElement_t* chunks, Size_t chunkCount, Size_t totalNodeCount)
            : Base_t(chunkStructure, componentData, totalNodeCount, ChunkKind_ChunkArrayTree)
            , Array(chunks, chunkCount)
        {
        }

    protected:
        KChunkArrayTreePointerT(const ChunkStructure_t* chunkStructure, void** componentData, ChunkPointerElement_t* chunks, Size_t chunkCount, Size_t totalNodeCount, ChunkKind kind)
            : Base_t(chunkStructure, componentData, totalNodeCount, kind)
            , Array(chunks, chunkCount)
        {
        }

        KChunkArrayTreePointerT(const ChunkStructure_t* chunkStructure, const Size_t chunkCount, const Size_t nodeCountPerChunk, ChunkKind kind = ChunkKind_ChunkArrayTree)
            : Base_t(chunkStructure, chunkCount * nodeCountPerChunk, kind)
            , Array(chunkCount)
        {
        }

    public:

        Size_t GetChunkCount()const { return Array.ChunkCount; }
        const Chunk_t& GetChunk(Size_t index)const { return Array.Chunks[index]; }
        Chunk_t& GetChunk(Size_t index) { return Array.Chunks[index]; }
        const ChunkPointerElement_t& operator[](Size_t index)const { return Array.Chunks[index]; }
        ChunkPointerElement_t& operator[](Size_t index) { return Array.Chunks[index]; }

        const Chunk_t& GetChunk()const { return (const Chunk_t&)Base_t::GetChunk(); }
        Chunk_t& GetChunk() { return (Chunk_t&)Base_t::GetChunk(); }
        const Chunk_t& operator*()const { return GetChunk(); }
        Chunk_t& operator*() { return GetChunk(); }
        const Chunk_t* operator->()const { return &GetChunk(); }
        Chunk_t* operator->() { return &GetChunk(); }

        static ChunkPointerInternal_t& GetInternalChunk(Self_t& chunkPointer) { return ChunkArrayPointer_t::GetInternalChunk(chunkPointer.GetChunk()); }
        static ChunkPointerElementInternal_t& GetInternalChunkElement(Self_t& chunkPointer, const Size_t index) { return reinterpret_cast<ChunkPointerElementInternal_t&>(chunkPointer.Array.Chunks[index]); }
    //protected:
    //    ChunkPointerInternal_t& GetInternalChunk() { return reinterpret_cast<ChunkPointerInternal_t&>(GetChunk()); }
    //    ChunkPointerElementInternal_t& GetInternalChunkElement(const Size_t index) { return reinterpret_cast<ChunkPointerElementInternal_t&>(Array.Chunks[index]); }

    protected:

        static void ConstructChunkElementAndNodes(Self_t& chunkArray, const Size_t chunkIndex, const Size_t nodeFirstIndex, const Size_t nodeCount, void** const componentDataArray)
        {
            ChunkArrayPointer_t::ConstructChunkElementAndNodes(chunkArray.GetChunk(), chunkIndex, nodeFirstIndex, nodeCount, componentDataArray);
        }

        static void DestructChunkElementAndNodes(Self_t& chunkArray, const Size_t chunkIndex)
        {
            ChunkArrayPointer_t::DestructChunkElementAndNodes(chunkArray.GetChunk(), chunkIndex);
        }
    };
}
