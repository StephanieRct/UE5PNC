// MIT License
// Copyright (c) 2025 Stephanie Rancourt

#pragma once
#include "common.h"
#include "KChunkTreePointer.h"
#include "ChunkArrayExtention.h"

namespace PNC
{
    /// <summary>
    /// A KChunkArrayTreePointer is a KTreePointer pointing to an array of ChunkPointers whose Component data are adjacent in memory.
    /// A KChunkArrayTreePointer is itself a KChunkTreePointer pointing to the beginning of the Component data, 
    /// effectively pointing to the first Chunk in the array.
    /// </summary>
    /// <typeparam name="TChunkType">Structure of the Chunk's Component data.</typeparam>
    /// <typeparam name="TChunkPointerElement">Type of the Chunk pointer in the array.</typeparam>
    template<typename TChunkType, typename TChunkPointerElement>
    struct KChunkArrayTreePointerT : public KChunkTreePointerT<TChunkType>
    {
    public:
        using Base_t = KChunkTreePointerT<TChunkType>;
        using Self_t = KChunkArrayTreePointerT<TChunkType, TChunkPointerElement>;
        using ChunkType_t = TChunkType;
        using ChunkPointerElement_t = TChunkPointerElement;
        using Size_t = typename ChunkPointerElement_t::Size_t;
        using Chunk_t = ChunkArrayPointerT<ChunkType_t, ChunkPointerElement_t>; // Type this pointer points to

    protected:
        using ChunkArrayExtention_t = ChunkArrayExtentionT<ChunkType_t, ChunkPointerElement_t>;
        ChunkArrayExtention_t Array;

    public:
        /// <summary>
        /// Create a Null KChunkArrayTreePointer without ChunkType
        /// </summary>
        KChunkArrayTreePointerT()
            : Base_t(ChunkKind_ChunkArrayTree)
        {
        }

        /// <summary>
        /// Contructs the Chunk from its member data fields.
        /// </summary>
        /// <param name="chunkType">Structure of the Chunk's Component data.</param>
        /// <param name="componentData">Points to an array of Component data pointers created according to the ChunkType.</param>
        /// <param name="chunks">Points to an array of ChunkPointerElement_t the size of chunkCount or more.</param>
        /// <param name="chunkCount">Number of Chunks in the Array.</param>
        /// <param name="totalNodeCount">The total number of Nodes used by all Chunks in the array.</param>
        KChunkArrayTreePointerT(const ChunkType_t* chunkType, void** componentData, ChunkPointerElement_t* chunks, Size_t chunkCount, Size_t totalNodeCount)
            : Base_t(chunkType, componentData, totalNodeCount, ChunkKind_ChunkArrayTree)
            , Array(chunks, chunkCount)
        {
        }

    protected:
        KChunkArrayTreePointerT(const ChunkType_t* chunkType, void** componentData, ChunkPointerElement_t* chunks, Size_t chunkCount, Size_t totalNodeCount, ChunkKind kind)
            : Base_t(chunkType, componentData, totalNodeCount, kind)
            , Array(chunks, chunkCount)
        {
        }

        KChunkArrayTreePointerT(const ChunkType_t* chunkType, Size_t totalNodeCount, Size_t chunkCount = 0, ChunkKind kind = ChunkKind_ChunkArrayTree)
            : Base_t(chunkType, totalNodeCount, kind)
            , Array(chunkCount)
        {
        }

    public:
        const ChunkPointerElement_t& operator[](Size_t index)const { return Array.Chunks[index]; }
        ChunkPointerElement_t& operator[](Size_t index) { return Array.Chunks[index]; }
        const Chunk_t& GetChunk(Size_t index)const { return Array.Chunks[index]; }
        Chunk_t& GetChunk(Size_t index) { return Array.Chunks[index]; }
        const Chunk_t& operator*()const { return GetChunk(); }
        Chunk_t& operator*() { return GetChunk(); }
        const Chunk_t* operator->()const { return &GetChunk(); }
        Chunk_t* operator->() { return &GetChunk(); }
        const Chunk_t& GetChunk()const { return (const Chunk_t&)Base_t::GetChunk(); }
        Chunk_t& GetChunk() { return (Chunk_t&)Base_t::GetChunk(); }
    };
}
