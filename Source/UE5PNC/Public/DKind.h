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
    struct PropTraits<ChunkKind> : public PropTraitsDefault<DPropChunkKind> { };

    // TODO add template parameter for TKind (ChunkKind)
    /// <summary>
    /// A KindPointer is an abstract Pointer with a differnt implementation according to the Chunk's kind.
    /// </summary>
    template<typename TBase>
    struct DKind : public TBase
    {
    public:
        using Base_t = TBase;
        using Self_t = DKind<TBase>;
        using typename Base_t::ChunkStructure_t;
        using typename Base_t::Size_t;

        using Chunk_t = ChunkPointerT<ChunkStructure_t>;
        using ChunkPointerElement_t = ChunkPointerT<ChunkStructure_t>;
        using ChunkArray_t = ChunkArrayPointerT<ChunkStructure_t, ChunkPointerElement_t>;

    protected:
        /// <summary>
        /// The kind of the Chunk being pointed at.
        /// </summary>
        ChunkKind Kind;

    protected:
        DKind()
            :Kind(ChunkKind::None)
        {
        }
        //KindPointerT(ChunkKind kind)
        //    :Kind(kind)
        //{
        //}

        template<typename TProps>
        PNC_DEBUG_NOINLINE DKind(const DPropsTag& tag, const TProps& props)
            : Kind(props.GetChunkKind())
        {
        }

#ifdef PNC_MEMORYCLEANUP
        ~DKind()
        {
            pnc_clean(Kind);
        }
#endif
    public:
        // TODO: IsNull
        // TODO: GetStructure
        // TODO: GetComponentData
        // TODO: GetNodeCount
        ChunkKind GetKind()const 
        {
            return Kind;
        }

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

    // TODO Remove To DOfKind
    /// <summary>
    /// Force a compile-time ChunkKind value.
    /// The most derived decorator of the same type will take precedence
    /// </summary>
    template<ChunkKind TKind, typename TBase>
    struct DOfKind : public TBase
    {
    public:
        using Base_t = TBase;
        using Self_t = DOfKind<TKind, TBase>;
        using ChunkStructure_t = Base_t::ChunkStructure_t;

        DOfKind()
            :Base_t(TKind)
        {
        }

    protected:
        template<typename TProps>
        PNC_DEBUG_NOINLINE DOfKind(const DPropsTag& tag, const TProps& props)
            : Base_t(tag, AppendPropSingle(props, TKind))
        {
        }
    public:

    };




    //template<template<typename> typename TDecorator0>
    //struct ContainerHasKindT
    //{
    //    template<typename TContainer>
    //    consteval static auto HasDecorator()
    //    {
    //        return Select((TContainer*)nullptr);
    //    }

    //    template<typename TBase>
    //    consteval static bool Select(const TDecorator0<TBase>* d)
    //    {
    //        return true;
    //    }
    //    template<typename TBase, template<typename> typename TDecoratorOther>
    //    consteval static bool Select(TDecoratorOther<TBase>* d)
    //    {
    //        return false;
    //    }
    //    template<typename TBase, typename TValueOther, template<typename, typename> typename TDecoratorOther>
    //    consteval static bool Select(TDecoratorOther<TValueOther, TBase>* d)
    //    {
    //        return false;
    //    }
    //};

    //template<typename TContainer>
    //consteval bool ContainerHasKind()
    //{
    //    return ContainerHasDecorator0T< TDecorator>::HasDecorator<TContainer>();
    //}
}
