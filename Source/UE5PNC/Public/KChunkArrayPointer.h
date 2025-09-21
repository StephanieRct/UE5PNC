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
    /// <typeparam name="TChunkStructure">Structure of the Chunk's Component data.</typeparam>
    /// <typeparam name="TChunkPointerElement">Structure of the Chunk pointer in the array.</typeparam>
    template<typename TChunkStructure, typename TChunkPointerElement>
    struct KChunkArrayPointerT : public KChunkPointerT<TChunkStructure>
    {
    public:
        using Base_t = KChunkPointerT<TChunkStructure>;
        using Self_t = KChunkArrayPointerT<TChunkStructure, TChunkPointerElement>;
        using ChunkStructure_t = TChunkStructure;
        using ChunkPointerElement_t = TChunkPointerElement;
        using Size_t = typename ChunkStructure_t::Size_t;
        using Chunk_t = ChunkArrayPointerT<ChunkStructure_t, ChunkPointerElement_t>; // The type this pointer is pointing to

        /// <summary>
        /// Use the same ChunkArrayExtention_t as ChunkArrayPointerT so both keep the same memory layout.
        /// </summary>
        using ChunkArrayExtention_t = typename ChunkArrayPointerT<ChunkStructure_t, TChunkPointerElement>::ChunkArrayExtention_t;

    protected:
        /// <summary>
        /// Contains pointer to the array of Chunks
        /// </summary>
        ChunkArrayExtention_t Array;

    public:
        /// <summary>
        /// Create a Null ChunkArrayPointer without ChunkStructure.
        /// IsNull() will evaluate to true.
        /// </summary>
        KChunkArrayPointerT()
        {
        }

        /// <summary>
        /// Contructs from its member data fields.
        /// </summary>
        /// <param name="chunkStructure">Structure of the Chunk's Component data.</param>
        /// <param name="componentData">Points to an array of Component data pointers created according to the ChunkStructure.</param>
        /// <param name="chunks">Points to an array of ChunkPointerElement_t the size of chunkCount or more.</param>
        /// <param name="chunkCount">Number of Chunks in the Array.</param>
        /// <param name="totalNodeCount">The total number of Nodes used by all Chunks in the array.</param>
        KChunkArrayPointerT(const ChunkStructure_t* chunkStructure, void** componentData, ChunkPointerElement_t* chunks, Size_t chunkCount, Size_t totalNodeCount)
            : Base_t(chunkStructure, totalNodeCount, componentData)
            , Array(chunks, chunkCount)
        {
        }

    protected:
        KChunkArrayPointerT(const ChunkStructure_t* chunkStructure, void** componentData, ChunkPointerElement_t* chunks, Size_t chunkCount, Size_t totalNodeCount, ChunkKind kind)
            : Base_t(chunkStructure, totalNodeCount, componentData, kind)
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
