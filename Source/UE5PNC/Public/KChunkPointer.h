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
    /// <typeparam name="TChunkStructure">Structure of the Chunk's Component data.</typeparam>
    /// <summary>
    template<typename TChunkStructure>
    struct KChunkPointerT : public KindPointerT<TChunkStructure>
    {
    public:
        using Base_t = KindPointerT<TChunkStructure>;
        using Self_t = KChunkPointerT<TChunkStructure>;
        using ChunkStructure_t = TChunkStructure;
        using Size_t = typename ChunkStructure_t::Size_t;
        using ChunkPointer_t = ChunkPointerT<TChunkStructure>;
        using Chunk_t = ChunkPointer_t;

    protected:
        ChunkPointer_t Chunk;

    public:
        /// <summary>
        /// Create a Null KChunkPointer without ChunkStructure
        /// </summary>
        KChunkPointerT()
        {
        }

        /// <summary>
        /// Contructs from its member data fields.
        /// </summary>
        /// <param name="chunkStructure">Structure of the Chunk's Component data.</param>
        /// <param name="nodeCount">Number of Nodes that are included by this pointer.</param>
        /// <param name="componentData">Points to an array of Component data pointers created according to the ChunkStructure.</param>
        KChunkPointerT(const ChunkStructure_t* chunkStructure, Size_t nodeCount, void** componentData)
            : Base_t()
            , Chunk(chunkStructure, nodeCount, componentData)
        {
        }

    protected:
        KChunkPointerT(const ChunkStructure_t* chunkStructure, Size_t nodeCount, void** componentData, ChunkKind kind)
            : Base_t(kind)
            , Chunk(chunkStructure, nodeCount, componentData)
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
