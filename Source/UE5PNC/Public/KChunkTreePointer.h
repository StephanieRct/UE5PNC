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
    /// <typeparam name="TChunkStructure">Structure of the Chunk's Component data.</typeparam>
    /// <summary>
    template<typename TChunkStructure>
    struct KChunkTreePointerT : public KTreePointerT<TChunkStructure>
    {
    public:
        using Base_t = KTreePointerT<TChunkStructure>;
        using Self_t = KChunkTreePointerT<TChunkStructure>;
        using ChunkStructure_t = TChunkStructure;
        using Size_t = typename ChunkStructure_t::Size_t;
        using Chunk_t = ChunkPointerT<ChunkStructure_t>; // Structure this pointer is pointing at

    protected:
        using ChunkInternal_t = ChunkPointerInternalT<TChunkStructure>;
        ChunkInternal_t Chunk;

    public:
        /// <summary>
        /// Create a Null KChunkTreePointer without ChunkStructure
        /// </summary>
        KChunkTreePointerT()
            : Base_t(ChunkKind_ChunkTree)
        {
        }

        /// <summary>
        /// Contructs the Chunk from its member data fields.
        /// </summary>
        /// <param name="chunkStructure">Structure of the Chunk's Component data.</param>
        /// <param name="nodeCount">Number of Nodes that are included by this pointer.</param>
        /// <param name="componentData">Points to an array of Component data pointers created according to the ChunkStructure.</param>
        KChunkTreePointerT(const ChunkStructure_t* chunkStructure, Size_t nodeCount, void** componentData)
            : Base_t(ChunkKind_ChunkTree)
            , Chunk(chunkStructure, nodeCount, componentData)
        {
        }

        KChunkTreePointerT(const KChunkTreePointerT&) = delete;
        KChunkTreePointerT(const KChunkTreePointerT&&) = delete;
        KChunkTreePointerT& operator=(const KChunkTreePointerT&) = delete;

    protected:
        KChunkTreePointerT(const ChunkStructure_t* chunkStructure, Size_t nodeCount, void** componentData, ChunkKind kind)
            : Base_t(kind)
            , Chunk(chunkStructure, nodeCount, componentData)
        {
        }

        KChunkTreePointerT(const ChunkStructure_t* chunkStructure, Size_t nodeCount, ChunkKind kind = ChunkKind_ChunkTree)
            : Base_t(kind)
            , Chunk(chunkStructure, nodeCount)
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
