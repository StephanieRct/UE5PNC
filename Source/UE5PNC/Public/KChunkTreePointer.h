// MIT License
// Copyright (c) 2025 Stephanie Rancourt

#pragma once
#include "common.h"
#include "KTreePointer.h"
#include "ChunkPointer.h"

namespace PNC
{
    template<typename TChunkStructure>
    using KChunkTreePointerT = DChunkPointer<ChunkPointerT<TChunkStructure>, 
                               DKindT<ChunkKind::ChunkTree, 
                               KTreePointerT<TChunkStructure>>>;
    template<typename TChunkStructure>
    using KChunkTreeT = DOwnT<DBucketPointerT<KChunkTreePointerT<TChunkStructure>>>;

    //// TODO replace with PNCDefault using
    //template<typename TChunkStructure>
    //struct KChunkTreePointerT : public DChunkPointer<ChunkPointerT<TChunkStructure>, KTreePointerT<TChunkStructure>>
    //{
    //public:
    //    using Base_t = DChunkPointer<ChunkPointerT<TChunkStructure>, KTreePointerT<TChunkStructure>>;
    //    using Self_t = KChunkTreePointerT<TChunkStructure>;
    //    using ChunkStructure_t = TChunkStructure;
    //    using Size_t = typename ChunkStructure_t::Size_t;
    //    using Chunk_t = ChunkPointerT<ChunkStructure_t>; // Structure this pointer is pointing at
    //    using Node_t = typename Chunk_t::Node_t;
    //    using ChunkPointer_t = ChunkPointerT<ChunkStructure_t>;
    //    using ChunkPointerInternal_t = typename ChunkPointer_t::ChunkPointerInternal_t;

    //public:
    //    /// <summary>
    //    /// Create a Null KChunkTreePointer without ChunkStructure
    //    /// </summary>
    //    KChunkTreePointerT()
    //        : Base_t(ChunkKind::ChunkTree)
    //    {
    //    }

    //    /// <summary>
    //    /// Contructs the Chunk from its member data fields.
    //    /// </summary>
    //    /// <param name="chunkStructure">Structure of the Chunk's Component data.</param>
    //    /// <param name="nodeCount">Number of Nodes that are included by this pointer.</param>
    //    /// <param name="componentData">Points to an array of Component data pointers created according to the ChunkStructure.</param>
    //    KChunkTreePointerT(const ChunkStructure_t* chunkStructure, const Size_t nodeCount, void** const  componentData)
    //        : Base_t(chunkStructure, nodeCount, componentData, ChunkKind::ChunkTree)
    //    {
    //    }

    //    KChunkTreePointerT(const KChunkTreePointerT&) = delete;
    //    KChunkTreePointerT(const KChunkTreePointerT&&) = delete;
    //    KChunkTreePointerT& operator=(const KChunkTreePointerT&) = delete;

    //protected:
    //    KChunkTreePointerT(const ChunkStructure_t* chunkStructure, const Size_t nodeCount, void** const  componentData, const ChunkKind kind)
    //        : Base_t(chunkStructure, nodeCount, componentData, kind)
    //    {
    //    }

    //    KChunkTreePointerT(const ChunkStructure_t* chunkStructure, const Size_t nodeCount, const ChunkKind kind = ChunkKind::ChunkTree)
    //        : Base_t(chunkStructure, nodeCount, kind)
    //    {
    //    }


    //};
    //template<typename TChunkStructure>
    //struct KBucketTreePointerT : public DChunkPointer<ChunkPointerT<TChunkStructure>, KTreePointerT<TChunkStructure>>
    //{
    //public:
    //    using Base_t = DChunkPointer<ChunkPointerT<TChunkStructure>, KTreePointerT<TChunkStructure>>;
    //    using Self_t = KBucketTreePointerT<TChunkStructure>;
    //    using ChunkStructure_t = TChunkStructure;
    //    using Size_t = typename ChunkStructure_t::Size_t;
    //    using Chunk_t = ChunkPointerT<ChunkStructure_t>; // Structure this pointer is pointing at
    //    using Node_t = typename Chunk_t::Node_t;
    //    using ChunkPointer_t = ChunkPointerT<ChunkStructure_t>;
    //    using ChunkPointerInternal_t = typename ChunkPointer_t::ChunkPointerInternal_t;

    //public:
    //    /// <summary>
    //    /// Create a Null KChunkTreePointer without ChunkStructure
    //    /// </summary>
    //    KBucketTreePointerT()
    //        : Base_t(ChunkKind::BucketTree)
    //    {
    //    }

    //    /// <summary>
    //    /// Contructs the Chunk from its member data fields.
    //    /// </summary>
    //    /// <param name="chunkStructure">Structure of the Chunk's Component data.</param>
    //    /// <param name="nodeCount">Number of Nodes that are included by this pointer.</param>
    //    /// <param name="componentData">Points to an array of Component data pointers created according to the ChunkStructure.</param>
    //    KBucketTreePointerT(const ChunkStructure_t* chunkStructure, const Size_t nodeCount, void** const  componentData)
    //        : Base_t(chunkStructure, nodeCount, componentData, ChunkKind::ChunkTrBucketTreeee)
    //    {
    //    }

    //    KBucketTreePointerT(const KBucketTreePointerT&) = delete;
    //    KBucketTreePointerT(const KBucketTreePointerT&&) = delete;
    //    KBucketTreePointerT& operator=(const KBucketTreePointerT&) = delete;

    //protected:
    //    KBucketTreePointerT(const ChunkStructure_t* chunkStructure, const Size_t nodeCount, void** const  componentData, const ChunkKind kind)
    //        : Base_t(chunkStructure, nodeCount, componentData, kind)
    //    {
    //    }

    //    KBucketTreePointerT(const ChunkStructure_t* chunkStructure, const Size_t nodeCount, const ChunkKind kind = ChunkKind::BucketTree)
    //        : Base_t(chunkStructure, nodeCount, kind)
    //    {
    //    }
    //};
}
