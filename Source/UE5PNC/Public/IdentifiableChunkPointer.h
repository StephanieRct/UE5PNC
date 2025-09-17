// MIT License
// Copyright (c) 2025 Stephanie Rancourt

#pragma once
#include "common.h"
#include "IdentifiableChunk.h"

namespace PNC
{
    template<typename TChunkType>
    struct IdentifiableChunkPointerT : public IdentifiableChunkT<TChunkType>
    {
    public:
        using Base_t = IdentifiableChunkT<TChunkType>;
        using ChunkType_t = TChunkType;
        using Size_t = typename ChunkType_t::Size_t;
        using ChunkPointer_t = ChunkPointerT<TChunkType>;
        using Chunk_t = ChunkPointer_t;

    protected:
        ChunkPointer_t Chunk;

    public:
        IdentifiableChunkPointerT()
        {
        }

        IdentifiableChunkPointerT(const ChunkType_t* chunkType, Size_t nodeCount, void** componentData)
            : Base_t()
            , Chunk(chunkType, nodeCount, componentData)
        {
        }

    protected:
        IdentifiableChunkPointerT(const ChunkType_t* chunkType, Size_t nodeCount, void** componentData, ChunkKind kind)
            : Base_t(kind)
            , Chunk(chunkType, nodeCount, componentData)
        {
        }

        IdentifiableChunkPointerT(ChunkKind kind = ChunkKind_Chunk)
            : Base_t(kind)
            , Chunk()
        {
        }

    public:
        const Chunk_t& operator*()const { return Chunk; }
        Chunk_t& operator*() { return Chunk; }
        const Chunk_t* operator->()const { return &Chunk; }
        Chunk_t* operator->() { return &Chunk; }
        const Chunk_t& GetChunk()const { return Chunk; }
        Chunk_t& GetChunk() { return Chunk; }

        //const ChunkType_t& GetChunkType()const { return Chunk.GetChunkType(); }
    };
}
