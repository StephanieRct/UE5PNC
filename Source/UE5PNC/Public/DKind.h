// MIT License
// Copyright (c) 2025 Stephanie Rancourt

#pragma once
#include "common.h"
#include "Containers.h"

namespace PNC
{
    enum class ContainerKind 
    {
        None,
        Chunk,
        Bucket,
        Bunch,
        ChunkArray,
        ChunkTree,
        BucketTree,
        BunchTree,
        ChunkArrayTree,
    };

    // TODO add template parameter for TKind (ContainerKind)
    template<typename TProps>
    struct DPropKind : public TProps
    {
        using Value_t = ContainerKind;
        ContainerKind Kind;
        ContainerKind GetChunkKind() const { return Kind; }
        DPropKind(const TProps& props, const ContainerKind kind)
            : TProps(props)
            , Kind(kind)
        {
        }
    };

    template<>
    struct PropTraits<ContainerKind> : public PropTraitsDefault<DPropKind> { };

    // TODO add template parameter for TKind (ContainerKind)
    /// <summary>
    /// A DKind provides a way to identify the type of container and extract the ChunkPointer using GetChunkPointer.
    /// </summary>
    template<typename TBase>
    struct DKind : public TBase
    {
    public:
        using Base_t = TBase;
        using Self_t = DKind<TBase>;
        using typename Base_t::ChunkStructure_t;
        using typename Base_t::Size_t;

        using ChunkPointer_t = ChunkPointerT<ChunkStructure_t>;
        using ChunkPointerElement_t = ChunkPointerT<ChunkStructure_t>;
        using ArrayPointer_t = ArrayPointerT<ChunkStructure_t, ChunkPointerElement_t>;

    protected:
        /// <summary>
        /// The kind of the Chunk being pointed at.
        /// </summary>
        ContainerKind Kind;

    protected:
        DKind()
            :Kind(ContainerKind::None)
        {
        }

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
            case ContainerKind::ChunkArray:
                return false;
            case ContainerKind::ChunkTree:
                return true;
            case ContainerKind::ChunkArrayTree:
                return true;
            }
        }

        bool IsArray()const
        {
            switch (Kind)
            {
            case ContainerKind::Chunk:
                return false;
            case ContainerKind::ChunkArray:
                return true;
            case ContainerKind::ChunkTree:
                return false;
            case ContainerKind::ChunkArrayTree:
                return true;
            }
        }
        const ChunkPointer_t& GetChunk()const;
        ChunkPointer_t& GetChunk();
        const ArrayPointer_t& GetChunkArray()const;
        ArrayPointer_t& GetChunkArray();
    };
}
