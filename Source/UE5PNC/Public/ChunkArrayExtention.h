// MIT License
// Copyright (c) 2025 Stephanie Rancourt

#pragma once
#include "common.h"

namespace PNC
{
    template<typename TChunkType, typename TChunkPointerElement>// = ChunkPointerT<TChunkType>>
    struct ChunkArrayExtentionT
    {
    public:
        using ChunkPointerElement_t = TChunkPointerElement;
        using ChunkType_t = TChunkType;
        using Size_t = typename TChunkPointerElement::Size_t;

    public:
        ChunkPointerElement_t* Chunks;
        Size_t ChunkCount;

    public:
        ChunkArrayExtentionT(Size_t chunkCount = 0)
            : Chunks(nullptr)
            , ChunkCount(chunkCount)
        {
        }

        ChunkArrayExtentionT(ChunkPointerElement_t* chunks, Size_t chunkCount)
            : Chunks(chunks)
            , ChunkCount(chunkCount)
        {
        }
    };
}