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
    /// <typeparam name="TChunkStructure">Structure of the chunk's component data.</typeparam>
    /// <typeparam name="TChunkPointerElement">Structure of the chunk pointer in the array.</typeparam>
    template<typename TChunkStructure, typename TChunkPointerElement = ChunkPointerT<TChunkStructure>>
    struct ChunkArrayPointerInternalT : public ChunkPointerInternalT<TChunkStructure>
    {
    public:
        using Base_t = ChunkPointerInternalT<TChunkStructure>;
        using Self_t = ChunkArrayPointerInternalT<TChunkStructure, TChunkPointerElement>;
        using ChunkStructure_t = TChunkStructure;
        using ChunkPointerElement_t = TChunkPointerElement;
        using Size_t = typename ChunkStructure_t::Size_t;
        using ChunkArrayExtention_t = ChunkArrayExtentionT<ChunkStructure_t, ChunkPointerElement_t>;

    public:
        ChunkArrayExtention_t Array;
    };
}
