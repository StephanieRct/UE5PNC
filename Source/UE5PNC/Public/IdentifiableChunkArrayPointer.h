// MIT License
// Copyright (c) 2025 Stephanie Rancourt

#pragma once
#include "common.h"
#include "IdentifiableChunkPointer.h"

namespace PNC
{
    template<typename TChunkType, typename TChunkPointerElement>// = ChunkPointerT<TChunkType>>
    struct IdentifiableChunkArrayPointerT : public IdentifiableChunkPointerT<TChunkType>
    {
    public:
        using Base_t = IdentifiableChunkPointerT<TChunkType>;
        using ChunkType_t = TChunkType;
        using ChunkPointerElement_t = TChunkPointerElement;
        using Size_t = typename ChunkType_t::Size_t;
        using Chunk_t = ChunkArrayPointerT<ChunkType_t, ChunkPointerElement_t>;
        using ChunkArrayExtention_t = ChunkArrayExtentionT<ChunkType_t, ChunkPointerElement_t>;

    protected:
        ChunkArrayExtention_t Array;

    public:
        IdentifiableChunkArrayPointerT()
        {
        }

        IdentifiableChunkArrayPointerT(const ChunkType_t* chunkType, void** componentData, ChunkPointerElement_t* chunks, Size_t chunkCount, Size_t capacityPerChunk)
            : Base_t(chunkType, chunkCount* capacityPerChunk, componentData)
            , Array(chunks, chunkCount)
        {
        }

    protected:
        IdentifiableChunkArrayPointerT(const ChunkType_t* chunkType, void** componentData, ChunkPointerElement_t* chunks, Size_t chunkCount, Size_t capacityPerChunk, ChunkKind kind)
            : Base_t(chunkType, chunkCount* capacityPerChunk, componentData, kind)
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
