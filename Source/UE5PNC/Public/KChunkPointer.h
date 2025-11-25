// MIT License
// Copyright (c) 2025 Stephanie Rancourt

#pragma once
#include "common.h"
#include "KindPointer.h"
#include "ChunkPointer.h"

namespace PNC
{
    template<typename TChunkStructure>
    using KChunkPointerT = DChunkPointer<ChunkPointerInternalT<TChunkStructure>, KindPointerT<TChunkStructure>>;
    ////TODO Remove
    ///// <summary>
    ///// A KChunkPointer is a KindPointer to a Chunk's Component data memory within a range of Nodes.
    ///// A KChunkPointer is also a Chunk by itself and provides access to the Component data.
    ///// Copying a KChunkPointer does not copy the data.
    ///// </summary>
    ///// <typeparam name="TChunkStructure">Structure of the Chunk's Component data.</typeparam>
    ///// <summary>
    //template<typename TChunkStructure>
    //struct KChunkPointerT : public DChunkPointer<ChunkPointerT<TChunkStructure>, KindPointerT<TChunkStructure>>
    //{
    //public:
    //    using Base_t = DChunkPointer<ChunkPointerT<TChunkStructure>, KindPointerT<TChunkStructure>>;
    //    using Self_t = KChunkPointerT<TChunkStructure>;
    //    using ChunkStructure_t = TChunkStructure;
    //    using Size_t = typename ChunkStructure_t::Size_t;
    //    using ChunkPointer_t = ChunkPointerT<TChunkStructure>;
    //    using Chunk_t = ChunkPointer_t;
    //    using Node_t = NodeT<TChunkStructure>;
    //    using ChunkPointerInternal_t = typename ChunkPointer_t::ChunkPointerInternal_t;

    //public:
    //    /// <summary>
    //    /// Create a Null KChunkPointer without ChunkStructure
    //    /// </summary>
    //    KChunkPointerT()
    //    {
    //    }

    //    /// <summary>
    //    /// Contructs from its member data fields.
    //    /// </summary>
    //    /// <param name="chunkStructure">Structure of the Chunk's Component data.</param>
    //    /// <param name="nodeCount">Number of Nodes that are included by this pointer.</param>
    //    /// <param name="componentData">Points to an array of Component data pointers created according to the ChunkStructure.</param>
    //    KChunkPointerT(const ChunkStructure_t* chunkStructure, Size_t nodeCount, void** componentData)
    //        : Base_t(chunkStructure, nodeCount, componentData)
    //    {
    //    }

    //protected:
    //    KChunkPointerT(const ChunkStructure_t* chunkStructure, Size_t nodeCount, void** componentData, ChunkKind kind)
    //        : Base_t(chunkStructure, nodeCount, componentData, kind)
    //    {
    //    }

    //    KChunkPointerT(ChunkKind kind = ChunkKind::Chunk)
    //        : Base_t(kind)
    //    {
    //    }

    //};
}
