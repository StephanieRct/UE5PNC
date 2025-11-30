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

    //using PropChunkKind = Prop<ChunkKind, PropId::ChunkKind>;

    template<typename TProps>
    struct DPropChunkKind : public TProps
    {
        using Value_t = ChunkKind;
        
        ChunkKind Kind;
        ChunkKind GetChunkKind() const { return Kind; }

        DPropChunkKind(const TProps& props, const ChunkKind kind)
            : TProps(props)
            , Kind(kind)
        {
        }
    };

    template<>
    struct PropTraits<ChunkKind> : public PropTraitsDefault<DPropChunkKind>
    {
        //template<typename TBase>
        //static DPropChunkKind<TBase> Decorate(const TBase& baseCopy, const ChunkKind value)
        //{
        //    return DPropChunkKind<TBase>(baseCopy, value);
        //}
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

        using Chunk_t = ChunkPointerT<ChunkStructure_t>;//ChunkPointerInternalT<TChunkStructure>;
        using ChunkPointerElement_t = ChunkPointerT<ChunkStructure_t>;
        //using ArrayExtension_t = ChunkArrayExtensionT<TChunkStructure, ChunkPointerElement_t>;
        using ChunkArray_t = ChunkArrayPointerT<ChunkStructure_t, ChunkPointerElement_t>;//DArrayPointerInternalT<ArrayExtension_t, ChunkPointerInternalT<TChunkStructure>>;

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

        template<typename TProps>
        __declspec(noinline) KindPointerT(const DPropsTag& tag, const TProps& props)
            : Kind(props.GetChunkKind())
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
        //const Chunk_t& operator*()const { return GetChunk(); }
        //Chunk_t& operator*() { return GetChunk(); }
        //const Chunk_t* operator->()const { return &GetChunk(); }
        //Chunk_t* operator->() { return &GetChunk(); }
        const Chunk_t& GetChunk()const;
        Chunk_t& GetChunk();
        const ChunkArray_t& GetChunkArray()const;
        ChunkArray_t& GetChunkArray();
    };


    /// <summary>
    /// Force a compile-time ChunkKind value.
    /// The most derived decorator of the same type will take precedence
    /// </summary>
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

    protected:
        template<typename TProps>
        __declspec(noinline) DKindT(const DPropsTag& tag, const TProps& props)
            : Base_t(tag, AppendPropSingle(props, TKind))
        {
        }
    public:

    };
}
