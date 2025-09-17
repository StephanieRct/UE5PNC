// MIT License
// Copyright (c) 2025 Stephanie Rancourt

#pragma once
#include "common.h"
#include "IdentifiableTreePointer.h"

namespace PNC
{

    template<typename TChunkType>
    struct ChunkTreePointerT : public IdentifiableTreePointerT<TChunkType>
    {
    public:
        using Base_t = IdentifiableTreePointerT<TChunkType>;
        using ChunkType_t = TChunkType;
        using Size_t = typename ChunkType_t::Size_t;
        using ChunkPointer_t = ChunkPointerT<ChunkType_t>;
        using Chunk_t = ChunkPointer_t;
        using ChunkInternal_t = ChunkPointerInternalT<TChunkType>;

    protected:
        ChunkInternal_t Chunk;

    public:
        ChunkTreePointerT()
            : Base_t(ChunkKind_ChunkTree)
        {
        }

        ChunkTreePointerT(const ChunkType_t* chunkType, Size_t nodeCount, void** componentData)
            : Base_t(ChunkKind_ChunkTree)
            , Chunk(chunkType, nodeCount, componentData)
        {
        }

    protected:
        ChunkTreePointerT(const ChunkType_t* chunkType, Size_t nodeCount, void** componentData, ChunkKind kind)
            : Base_t(kind)
            , Chunk(chunkType, nodeCount, componentData)
        {
        }

        ChunkTreePointerT(const ChunkType_t* chunkType, Size_t nodeCount, ChunkKind kind = ChunkKind_ChunkTree)
            : Base_t(kind)
            , Chunk(chunkType, nodeCount)
        {
        }

    public:
        const Chunk_t& operator*()const { return (const Chunk_t&)Chunk; }
        Chunk_t& operator*() { return (Chunk_t&)Chunk; }
        const Chunk_t* operator->()const { return &(const Chunk_t&)Chunk; }
        Chunk_t* operator->() { return &(Chunk_t&)Chunk; }
        const Chunk_t& GetChunk()const { return (const Chunk_t&)Chunk; }
        Chunk_t& GetChunk() { return (Chunk_t&)Chunk; }

    };
}
