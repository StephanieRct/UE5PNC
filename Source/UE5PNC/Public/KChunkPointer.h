// MIT License
// Copyright (c) 2025 Stephanie Rancourt

#pragma once
#include "common.h"
#include "KindPointer.h"

namespace PNC
{
    /// <summary>
    /// A KChunkPointer is a KindPointer to a Chunk's Component data memory within a range of Nodes.
    /// A KChunkPointer is also a Chunk by itself and provides access to the Component data.
    /// Copying a KChunkPointer does not copy the data.
    /// </summary>
    /// <typeparam name="TChunkType">Structure of the Chunk's Component data.</typeparam>
    /// <summary>
    template<typename TChunkType>
    struct KChunkPointerT : public KindPointerT<TChunkType>
    {
    public:
        using Base_t = KindPointerT<TChunkType>;
        using Self_t = KChunkPointerT<TChunkType>;
        using ChunkType_t = TChunkType;
        using Size_t = typename ChunkType_t::Size_t;
        using ChunkPointer_t = ChunkPointerT<TChunkType>;
        using Chunk_t = ChunkPointer_t;

    protected:
        ChunkPointer_t Chunk;

    public:
        /// <summary>
        /// Create a Null KChunkPointer without ChunkType
        /// </summary>
        KChunkPointerT()
        {
        }

        /// <summary>
        /// Contructs from its member data fields.
        /// </summary>
        /// <param name="chunkType">Structure of the Chunk's Component data.</param>
        /// <param name="nodeCount">Number of Nodes that are included by this pointer.</param>
        /// <param name="componentData">Points to an array of Component data pointers created according to the ChunkType.</param>
        KChunkPointerT(const ChunkType_t* chunkType, Size_t nodeCount, void** componentData)
            : Base_t()
            , Chunk(chunkType, nodeCount, componentData)
        {
        }

    protected:
        KChunkPointerT(const ChunkType_t* chunkType, Size_t nodeCount, void** componentData, ChunkKind kind)
            : Base_t(kind)
            , Chunk(chunkType, nodeCount, componentData)
        {
        }

        KChunkPointerT(ChunkKind kind = ChunkKind_Chunk)
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
    };
}
