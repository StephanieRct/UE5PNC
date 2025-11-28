// MIT License
// Copyright (c) 2025 Stephanie Rancourt

#pragma once
#include "common.h"
#include "Containers.h"

namespace PNC
{
    enum class ChunkKind 
    {
        None,
        /// <summary>
        /// The Chunk is a ChunkPointer
        /// </summary>
        Chunk,
        Bucket,
        Bunch,

        /// <summary>
        /// The Chunk is a ChunkArrayPointer
        /// </summary>
        ChunkArray,

        /// <summary>
        /// The Chunk is a KChunkTreePointer
        /// </summary>
        ChunkTree,
        BucketTree,
        BunchTree,

        /// <summary>
        /// The Chunk is a KChunkArrayTreePointer
        /// </summary>
        ChunkArrayTree,
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
        KindPointerT()
            :Kind(ChunkKind::None)
        {
        }
        KindPointerT(ChunkKind kind)
            :Kind(kind)
        {
        }

#ifdef PNC_MEMORYCLEANUP
        ~KindPointerT()
        {
            pnc_clean(Kind);
        }
#endif
    public:
        // TODO: IsNull
        // TODO: GetStructure
        // TODO: GetComponentData
        // TODO: GetNodeCount

        bool IsTree()const
        {
            switch (Kind)
            {
            case ChunkKind::Chunk:
                return false;
            case ChunkKind::ChunkArray:
                return false;
            case ChunkKind::ChunkTree:
                return true;
            case ChunkKind::ChunkArrayTree:
                return true;
            }
        }

        bool IsArray()const
        {
            switch (Kind)
            {
            case ChunkKind::Chunk:
                return false;
            case ChunkKind::ChunkArray:
                return true;
            case ChunkKind::ChunkTree:
                return false;
            case ChunkKind::ChunkArrayTree:
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


    template<ChunkKind TKind, typename TBase>
    struct DKindT : public TBase
    {
    public:
        using Base_t = TBase;
        using Self_t = DKindT<TKind, TBase>;
        using ChunkStructure_t = Base_t::ChunkStructure_t;

        DKindT()
            :Base_t(TKind)
        {
        }

        //template<class... TBaseCTorArgumentTypes>
        //DKindT(const StructurePtr<ChunkStructure_t>& chunkStructure, TBaseCTorArgumentTypes&&... args)
        //    : Base_t(chunkStructure, TKind, args...)
        //{
        //}

        template<class... TBaseCTorArgumentTypes>
        DKindT(const StructurePtr<ChunkStructure_t>& chunkStructure, const ChunkKind overrideKind, TBaseCTorArgumentTypes&&... args)
            : Base_t(chunkStructure, overrideKind, args...)
        {
        }

        DKindT(const ChunkKind overrideKind)
            :Base_t(overrideKind)
        {
        }
        template<class... TBaseCTorArgumentTypes>
        DKindT(const ChunkKind overrideKind, TBaseCTorArgumentTypes&&... args)
            : Base_t(overrideKind, args...)
        {
        }
    };
}
