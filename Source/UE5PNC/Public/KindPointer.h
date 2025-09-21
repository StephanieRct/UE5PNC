// MIT License
// Copyright (c) 2025 Stephanie Rancourt

#pragma once
#include "common.h"

namespace PNC
{
    enum ChunkKind
    {
        /// <summary>
        /// The Chunk is a ChunkPointer
        /// </summary>
        ChunkKind_Chunk,

        /// <summary>
        /// The Chunk is a ChunkArrayPointer
        /// </summary>
        ChunkKind_ChunkArray,

        /// <summary>
        /// The Chunk is a KChunkTreePointer
        /// </summary>
        ChunkKind_ChunkTree,

        /// <summary>
        /// The Chunk is a KChunkArrayTreePointer
        /// </summary>
        ChunkKind_ChunkArrayTree,
    };

    /// <summary>
    /// A KindPointer is an abstract Pointer with a differnt implementation according to the Chunk's kind.
    /// </summary>
    /// <typeparam name="TChunkStructure">Structure of the Chunk's Component data.</typeparam>
    template<typename TChunkStructure>
    struct KindPointerT
    {
    public:
        using Self_t = KindPointerT<TChunkStructure>;
        using ChunkStructure_t = TChunkStructure;
        using Size_t = typename ChunkStructure_t::Size_t;
        using ChunkPointer_t = ChunkPointerT<TChunkStructure>;
        using Chunk_t = ChunkPointerT<TChunkStructure>;
        using ChunkArray_t = ChunkArrayPointerT<ChunkStructure_t, ChunkPointerT<TChunkStructure>>;

    public:
        /// <summary>
        /// The kind of the Chunk being pointed at.
        /// </summary>
        ChunkKind Kind;

    protected:
        KindPointerT(ChunkKind kind)
            :Kind(kind)
        {
        }

    public:
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

        bool IsArray()const
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
    };
}
