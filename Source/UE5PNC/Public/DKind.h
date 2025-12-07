// MIT License
// Copyright (c) 2025 Stephanie Rancourt

#pragma once
#include "common.h"
#include "Containers.h"

namespace NiT
{
    enum class ContainerKind 
    {
        None,
        Chunk,
        Bucket,
        Bunch,
        Array,
        ChunkTree,
        BucketTree,
        BunchTree,
        ArrayTree,
    };

    /// <summary>
    /// A DKind provides a way to identify the type of container and extract the ChunkPointer using GetChunk() or GetArray().
    /// </summary>
    template<typename TBase>
    struct DKind : public TBase
    {
    public:
        using Base_t = TBase;
        using Self_t = DKind<TBase>;
        using typename Base_t::Size_t;
        using typename Base_t::ChunkStructure_t;

        using ChunkPointer_t = ChunkPointerT<ChunkStructure_t>;
        using ChunkPointerElement_t = ChunkPointerT<ChunkStructure_t>;
        using ArrayPointer_t = ArrayPointerT<ChunkStructure_t, ChunkPointerElement_t>;

    protected:
        ContainerKind Kind;

    public:
        DKind()
            :Kind(ContainerKind::None)
        {
        }

#ifdef NI_MEMORYCLEANUP
        ~DKind()
        {
            ni_clean(Kind);
        }
#endif
        ContainerKind GetKind()const 
        {
            return Kind;
        }

        const ChunkStructure_t& GetStructure()const;

        bool IsTree()const
        {
            switch (Kind)
            {
            case ContainerKind::Chunk:
                return false;
            case ContainerKind::Array:
                return false;
            case ContainerKind::ChunkTree:
                return true;
            case ContainerKind::ArrayTree:
                return true;
            }
        }

        bool IsArray()const
        {
            switch (Kind)
            {
            case ContainerKind::Chunk:
                return false;
            case ContainerKind::Array:
                return true;
            case ContainerKind::ChunkTree:
                return false;
            case ContainerKind::ArrayTree:
                return true;
            }
        }
        const ChunkPointer_t& GetChunk()const;
        ChunkPointer_t& GetChunk();
        const ArrayPointer_t& GetArray()const;
        ArrayPointer_t& GetArray();

    protected:
        template<typename TProps>
        NI_DEBUG_NOINLINE DKind(const DPropsTag& tag, const TProps& props)
            : Kind(props.GetChunkKind())
        {
        }
    };

    template<typename TProps>
    struct DPropKind : public TProps
    {
        using Value_t = ContainerKind;
        ContainerKind Kind;
        ContainerKind GetChunkKind() const { return Kind; }
        DPropKind(const TProps& props, const ContainerKind kind) : TProps(props), Kind(kind) {}
    };

    template<>
    struct PropTraits<ContainerKind> : public PropTraitsDefault<DPropKind> {};
}
