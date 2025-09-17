// MIT License
// Copyright (c) 2025 Stephanie Rancourt

#pragma once
#include "common.h"
#include "ChunkTreePointer.h"
#include "ChunkArrayExtention.h"

namespace PNC
{
    template<typename TChunkType, typename TChunkPointerElement>// = ChunkPointerT<TChunkType>>
    struct ChunkArrayTreePointerT : public ChunkTreePointerT<TChunkType>
    {
    public:
        using Base_t = ChunkTreePointerT<TChunkType>;
        using ChunkPointerElement_t = TChunkPointerElement;
        using ChunkType_t = TChunkType;
        using Size_t = typename ChunkPointerElement_t::Size_t;
        using ChunkArrayPointer_t = ChunkArrayPointerT<ChunkType_t, ChunkPointerElement_t>;
        using ChunkPointer_t = ChunkArrayPointer_t;
        using Chunk_t = ChunkArrayPointer_t;

    protected:
        using ChunkArrayExtention_t = ChunkArrayExtentionT<ChunkType_t, ChunkPointerElement_t>;
        ChunkArrayExtention_t Array;

    public:
        ChunkArrayTreePointerT()
            : Base_t(ChunkKind_ChunkArrayTree)
        {
        }

        ChunkArrayTreePointerT(const ChunkType_t* chunkType, void** componentData, ChunkPointerElement_t* chunks, Size_t chunkCount, Size_t totalNodeCount)
            : Base_t(chunkType, componentData, totalNodeCount, ChunkKind_ChunkArrayTree)
            , Array(chunks, chunkCount)
        {
        }

    protected:
        ChunkArrayTreePointerT(const ChunkType_t* chunkType, void** componentData, ChunkPointerElement_t* chunks, Size_t chunkCount, Size_t totalNodeCount, ChunkKind kind)
            : Base_t(chunkType, componentData, totalNodeCount, kind)
            , Array(chunks, chunkCount)
        {
        }

        ChunkArrayTreePointerT(const ChunkType_t* chunkType, Size_t totalNodeCount, Size_t chunkCount = 0, ChunkKind kind = ChunkKind_ChunkArrayTree)
            : Base_t(chunkType, totalNodeCount, kind)
            , Array(chunkCount)
        {
        }

    public:
        const Chunk_t& operator*()const { return GetChunk(); }
        Chunk_t& operator*() { return GetChunk(); }
        const Chunk_t* operator->()const { return &GetChunk(); }
        Chunk_t* operator->() { return &GetChunk(); }
        const Chunk_t& GetChunk()const { return (const ChunkArrayPointer_t&)Base_t::GetChunk(); }
        Chunk_t& GetChunk() { return (ChunkArrayPointer_t&)Base_t::GetChunk(); }
    };

}
