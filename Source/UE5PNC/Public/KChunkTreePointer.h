// MIT License
// Copyright (c) 2025 Stephanie Rancourt

#pragma once
#include "common.h"
#include "KTreePointer.h"

namespace PNC
{
    /// <summary>
    /// A KChunkTreePointer is a KindPointer to a Chunk that is part of a Tree of Chunks.
    /// A KChunkTreePointer is non-copyable as it may be pointed to by other KTreePointers.
    /// </summary>
    /// <typeparam name="TChunkType">Structure of the Chunk's Component data.</typeparam>
    /// <summary>
    template<typename TChunkType>
    struct KChunkTreePointerT : public KTreePointerT<TChunkType>
    {
    public:
        using Base_t = KTreePointerT<TChunkType>;
        using Self_t = KChunkTreePointerT<TChunkType>;
        using ChunkType_t = TChunkType;
        using Size_t = typename ChunkType_t::Size_t;
        using Chunk_t = ChunkPointerT<ChunkType_t>; // Type this pointer is pointing at

    protected:
        using ChunkInternal_t = ChunkPointerInternalT<TChunkType>;
        ChunkInternal_t Chunk;

    public:
        /// <summary>
        /// Create a Null KChunkTreePointer without ChunkType
        /// </summary>
        KChunkTreePointerT()
            : Base_t(ChunkKind_ChunkTree)
        {
        }

        /// <summary>
        /// Contructs the Chunk from its member data fields.
        /// </summary>
        /// <param name="chunkType">Structure of the Chunk's Component data.</param>
        /// <param name="nodeCount">Number of Nodes that are included by this pointer.</param>
        /// <param name="componentData">Points to an array of Component data pointers created according to the ChunkType.</param>
        KChunkTreePointerT(const ChunkType_t* chunkType, Size_t nodeCount, void** componentData)
            : Base_t(ChunkKind_ChunkTree)
            , Chunk(chunkType, nodeCount, componentData)
        {
        }

        KChunkTreePointerT(const KChunkTreePointerT&) = delete;
        KChunkTreePointerT(const KChunkTreePointerT&&) = delete;
        KChunkTreePointerT& operator=(const KChunkTreePointerT&) = delete;

    protected:
        KChunkTreePointerT(const ChunkType_t* chunkType, Size_t nodeCount, void** componentData, ChunkKind kind)
            : Base_t(kind)
            , Chunk(chunkType, nodeCount, componentData)
        {
        }

        KChunkTreePointerT(const ChunkType_t* chunkType, Size_t nodeCount, ChunkKind kind = ChunkKind_ChunkTree)
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
