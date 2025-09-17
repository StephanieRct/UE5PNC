// MIT License
// Copyright (c) 2025 Stephanie Rancourt

#pragma once
#include "common.h"

namespace PNC
{
    /// <summary>
    /// Extent a ChunkPointer to include the required data field to point to an array of chunks.
    /// </summary>
    /// <typeparam name="TChunkType">Structure of the chunk's component data.</typeparam>
    /// <typeparam name="TChunkPointerElement">Type of the chunk pointer in the array.</typeparam>
    template<typename TChunkType, typename TChunkPointerElement>
    struct ChunkArrayExtentionT
    {
    public:
        using Self = ChunkArrayExtentionT<TChunkType, TChunkPointerElement>;
        using ChunkType_t = TChunkType;
        using ChunkPointerElement_t = TChunkPointerElement;
        using Size_t = typename ChunkType_t::Size_t;

    public:
        /// <summary>
        /// Points to an array of ChunkPointerElement_t with at least ChunkCount elements
        /// </summary>
        ChunkPointerElement_t* Chunks;

        /// <summary>
        /// Number of chunk in the Chunks array
        /// </summary>
        Size_t ChunkCount;

    public:
        /// <summary>
        /// Create a null pointer.
        /// </summary>
        /// <param name="chunkCount"></param>
        ChunkArrayExtentionT(Size_t chunkCount = 0)
            : Chunks(nullptr)
            , ChunkCount(chunkCount)
        {
        }

        /// <summary>
        /// Contructs from its member data fields.
        /// </summary>
        /// <param name="chunks">Array of ChunkPointerElement_t with at least chunkCount elements.</param>
        /// <param name="chunkCount">Number of chunk in chunks array</param>
        ChunkArrayExtentionT(ChunkPointerElement_t* chunks, Size_t chunkCount)
            : Chunks(chunks)
            , ChunkCount(chunkCount)
        {
        }
    };
}
