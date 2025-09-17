// MIT License
// Copyright (c) 2025 Stephanie Rancourt

#pragma once
#include "common.h"
#include "ChunkPointerInternal.h"
#include "ChunkArrayExtention.h"

namespace PNC
{
    /// <summary>
    /// A ChunkArrayPointerInternal points to an array of chunk's component data memory within a range of nodes.
    /// Used to get read-write access to a ChunkArrayPointer's data fields.
    /// Both ChunkArrayPointer and ChunkArrayPointerInternal must have the same overlapping memory layout so
    /// a reinterpret_cast<ChunkArrayPointerInternalT>(ChunkArrayPointer) is valid.
    /// </summary>
    /// <typeparam name="TChunkType">Structure of the chunk's component data.</typeparam>
    /// <typeparam name="TChunkPointerElement">Type of the chunk pointer in the array.</typeparam>
    template<typename TChunkType, typename TChunkPointerElement = ChunkPointerT<TChunkType>>
    struct ChunkArrayPointerInternalT : public ChunkPointerInternalT<TChunkType>
    {
    public:
        using Base_t = ChunkPointerInternalT<TChunkType>;
        using Self_t = ChunkArrayPointerInternalT<TChunkType, TChunkPointerElement>;
        using ChunkType_t = TChunkType;
        using ChunkPointerElement_t = TChunkPointerElement;
        using Size_t = typename ChunkType_t::Size_t;
        using ChunkArrayExtention_t = ChunkArrayExtentionT<ChunkType_t, ChunkPointerElement_t>;

    public:
        ChunkArrayExtention_t Array;
    };
}