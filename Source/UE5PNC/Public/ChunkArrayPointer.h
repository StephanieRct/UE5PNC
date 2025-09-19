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
    /// <typeparam name="TChunkType">Structure of the Chunk's Component data.</typeparam>
    /// <typeparam name="TChunkPointerElement">Type of the Chunk pointer in the Array.</typeparam>
    template<typename TChunkType, typename TChunkPointerElement = ChunkPointerT<TChunkType>>
    struct ChunkArrayPointerT : public ChunkPointerT<TChunkType>
    {
    public:
        using Base_t = ChunkPointerT<TChunkType>;
        using Self_t = ChunkArrayPointerT<TChunkType, TChunkPointerElement>;
        using ChunkType_t = TChunkType;
        using ChunkPointerElement_t = TChunkPointerElement;
        using Size_t = typename TChunkType::Size_t;

        /// <summary>
        /// Reinterprete_cast this object to this type to get read-write access to it's private data fields
        /// Use with caution.
        /// </summary>
        using Internal_t = ChunkArrayPointerInternalT<ChunkType_t, TChunkPointerElement>;

        /// <summary>
        /// Chunk_t is the type a ChunkPointer points to
        /// </summary>
        using Chunk_t = Self_t;

        /// <summary>
        /// Use the same ChunkArrayExtention_t as ChunkArrayPointerInternalT so both keep the same memory layout.
        /// </summary>
        using ChunkArrayExtention_t = typename ChunkArrayPointerInternalT<ChunkType_t, TChunkPointerElement>::ChunkArrayExtention_t;

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
        ChunkArrayPointerT()
        {
        }

        /// <summary>
        /// Contructs from its member data fields.
        /// </summary>
        /// <param name="chunkType">Structure of the Chunk's Component data.</param>
        /// <param name="componentData">Points to an array of Component data pointers created according to the ChunkType.</param>
        /// <param name="chunks">Points to an array of ChunkPointerElement_t the size of chunkCount or more.</param>
        /// <param name="chunkCount">Number of Chunks in the Array.</param>
        /// <param name="totalNodeCount">The total number of Nodes used by all Chunks in the Array.</param>
        ChunkArrayPointerT(const ChunkType_t* chunkType, void** componentData, ChunkPointerElement_t* chunks, Size_t chunkCount, Size_t totalNodeCount)
            : Base_t(chunkType, totalNodeCount, componentData)
            , Array(chunks, chunkCount)
        {

        }

    protected:
        ChunkArrayPointerT(const ChunkType_t* chunkType, Size_t totalNodeCount, Size_t chunkCount = 0)
            : Base_t(chunkType, totalNodeCount)
            , Array(chunkCount)
        {
        }

    public:
        /// <summary>
        /// Number of elements in the array
        /// </summary>
        /// <returns></returns>
        Size_t GetChunkCount()const { return Array.ChunkCount; }

        const ChunkPointerElement_t& operator[](Size_t index)const { return Array.Chunks[index]; }
        ChunkPointerElement_t& operator[](Size_t index) { return Array.Chunks[index]; }
        const Chunk_t& GetChunk(Size_t index)const { return Array.Chunks[index]; }
        Chunk_t& GetChunk(Size_t index) { return Array.Chunks[index]; }
        const Chunk_t& operator*()const { return *this; }
        Chunk_t& operator*() { return *this; }
        const Chunk_t* operator->()const { return this; }
        Chunk_t* operator->() { return this; }
        const Chunk_t& GetChunk()const { return *this; }
        Chunk_t& GetChunk() { return *this; }
    };
}
