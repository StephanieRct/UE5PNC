// MIT License
// Copyright (c) 2025 Stephanie Rancourt

#pragma once
#include "common.h"
#include "KChunkPointer.h"
#include "ChunkArrayPointer.h"

namespace PNC
{
    /// <summary>
    /// A KChunkArrayPointer is a KindPointer pointing to an array of ChunkPointers whose Component data are adjacent in memory.
    /// A KChunkArrayPointer is itself a KChunkPointer pointing to the beginning of the Component data, 
    /// effectively pointing to the first Chunk in the array.
    /// </summary>
    /// <typeparam name="TChunkType">Structure of the Chunk's Component data.</typeparam>
    /// <typeparam name="TChunkPointerElement">Type of the Chunk pointer in the array.</typeparam>
    template<typename TChunkType, typename TChunkPointerElement>
    struct KChunkArrayPointerT : public KChunkPointerT<TChunkType>
    {
    public:
        using Base_t = KChunkPointerT<TChunkType>;
        using Self_t = KChunkArrayPointerT<TChunkType, TChunkPointerElement>;
        using ChunkType_t = TChunkType;
        using ChunkPointerElement_t = TChunkPointerElement;
        using Size_t = typename ChunkType_t::Size_t;
        using Chunk_t = ChunkArrayPointerT<ChunkType_t, ChunkPointerElement_t>; // The type this pointer is pointing to

        /// <summary>
        /// Use the same ChunkArrayExtention_t as ChunkArrayPointerT so both keep the same memory layout.
        /// </summary>
        using ChunkArrayExtention_t = typename ChunkArrayPointerT<ChunkType_t, TChunkPointerElement>::ChunkArrayExtention_t;

    protected:
        /// <summary>
        /// Contains pointer to the array of Chunks
        /// </summary>
        ChunkArrayExtention_t Array;

    public:
        /// <summary>
        /// Create a Null ChunkArrayPointer without ChunkType.
        /// IsNull() will evaluate to true.
        /// </summary>
        KChunkArrayPointerT()
        {
        }

        /// <summary>
        /// Contructs from its member data fields.
        /// </summary>
        /// <param name="chunkType">Structure of the Chunk's Component data.</param>
        /// <param name="componentData">Points to an array of Component data pointers created according to the ChunkType.</param>
        /// <param name="chunks">Points to an array of ChunkPointerElement_t the size of chunkCount or more.</param>
        /// <param name="chunkCount">Number of Chunks in the Array.</param>
        /// <param name="totalNodeCount">The total number of Nodes used by all Chunks in the array.</param>
        KChunkArrayPointerT(const ChunkType_t* chunkType, void** componentData, ChunkPointerElement_t* chunks, Size_t chunkCount, Size_t totalNodeCount)
            : Base_t(chunkType, totalNodeCount, componentData)
            , Array(chunks, chunkCount)
        {
        }

    protected:
        KChunkArrayPointerT(const ChunkType_t* chunkType, void** componentData, ChunkPointerElement_t* chunks, Size_t chunkCount, Size_t totalNodeCount, ChunkKind kind)
            : Base_t(chunkType, totalNodeCount, componentData, kind)
            , Array(chunks, chunkCount)
        {
        }

    public:
        const Chunk_t& operator*()const { return GetChunk(); }
        Chunk_t& operator*() { return GetChunk(); }
        const Chunk_t* operator->()const { return &GetChunk(); }
        Chunk_t* operator->() { return &GetChunk(); }
        const Chunk_t& GetChunk()const { return (const Chunk_t&)Base_t::GetChunk(); }
        Chunk_t& GetChunk() { return (Chunk_t&)Base_t::GetChunk(); }
    };
}
