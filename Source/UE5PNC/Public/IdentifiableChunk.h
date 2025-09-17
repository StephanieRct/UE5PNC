// MIT License
// Copyright (c) 2025 Stephanie Rancourt

#pragma once
#include "common.h"

namespace PNC
{
    enum ChunkKind
    {
        ChunkKind_Chunk,
        ChunkKind_ChunkArray,

        ChunkKind_ChunkTree,
        ChunkKind_ChunkArrayTree,
    };

    template<typename TChunkType>
    struct IdentifiableChunkT
    {
    public:
        using ChunkType_t = TChunkType;
        using Size_t = typename ChunkType_t::Size_t;
        using ChunkPointer_t = ChunkPointerT<TChunkType>;
        using ChunkArrayPointer_t = ChunkArrayPointerT<ChunkType_t, ChunkPointer_t>;
        using Chunk_t = ChunkPointer_t;
        using ChunkArray_t = ChunkArrayPointer_t;
    public:
        ChunkKind Kind;

    public:
        IdentifiableChunkT(ChunkKind kind)
            :Kind(kind)
        {
        }

        bool IsTree()const
        {
            switch (Kind)
            {
            case PNC::ChunkKind_Chunk:
                return false;
            case PNC::ChunkKind_ChunkArray:
                return false;
            case PNC::ChunkKind_ChunkTree:
                return true;
            case PNC::ChunkKind_ChunkArrayTree:
                return true;
            }
        }

        bool isArray()const
        {
            switch (Kind)
            {
            case PNC::ChunkKind_Chunk:
                return false;
            case PNC::ChunkKind_ChunkArray:
                return true;
            case PNC::ChunkKind_ChunkTree:
                return false;
            case PNC::ChunkKind_ChunkArrayTree:
                return true;
            }
        }

        const Chunk_t& operator*()const { return GetChunk(); }
        Chunk_t& operator*() { return GetChunk(); }
        const Chunk_t* operator->()const { return &GetChunk(); }
        Chunk_t* operator->() { return &GetChunk(); }

        const Chunk_t& GetChunk()const;
        Chunk_t& GetChunk();
        const ChunkArray_t& GetChunkArray()const;
        ChunkArray_t& GetChunkArray();

        //Size_t GetNodeCount()const { return GetChunk().GetNodeCount(); }
        //const ChunkType_t& GetChunkType()const { return GetChunk().GetChunkType(); }
        //bool IsNull()const { return GetChunk().IsNull(); }

        //operator ChunkPointer_t&() { return GetChunkPointer(); }
        //operator const ChunkPointer_t& ()const { return GetChunkPointer(); }
    };

}
