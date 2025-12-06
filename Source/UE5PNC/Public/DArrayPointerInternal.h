// MIT License
// Copyright (c) 2025 Stephanie Rancourt

#pragma once
#include "common.h"

namespace NiT
{
    /// <summary>
    /// Adds array extension to a base ChunkPointerInternal.
    /// </summary>
    /// <typeparam name="TArrayExtension"></typeparam>
    /// <typeparam name="TBase"></typeparam>
    template<typename TArrayExtension, typename TBase>
    struct DArrayPointerInternalT : public TBase
    {
    public:
        using Base_t = TBase;
        using Self_t = DArrayPointerInternalT<TArrayExtension, TBase>;
        using ArrayExtension_t = TArrayExtension;
        using typename Base_t::Size_t;
        using typename Base_t::ComponentType_t;
        using typename Base_t::ChunkStructure_t;
        using typename Base_t::Node_t;
        using ChunkPointerElement_t = ArrayExtension_t::ChunkPointerElement_t;
        using ChunkPointerElementInternal_t = typename ChunkPointerElement_t::ChunkPointerInternal_t;

    public:
        ArrayExtension_t Array;

        template<typename TProps>
        DArrayPointerInternalT(const DPropsTag& tag, const TProps& props)
            : Base_t(tag, props)
            , Array (tag, props)
        {
        }
    };

    template<typename TArrayExtension, typename TBase>
    struct DArrayPointerProtectedT : public TBase
    {
    public:
        using Base_t = TBase;
        using Self_t = DArrayPointerProtectedT<TArrayExtension, TBase>;
        using ArrayExtension_t = TArrayExtension;
        using typename Base_t::Size_t;
        using typename Base_t::ComponentType_t;
        using typename Base_t::ChunkStructure_t;
        using typename Base_t::Node_t;
        using ChunkPointerElement_t = ArrayExtension_t::ChunkPointerElement_t;
        using ChunkPointerElementInternal_t = typename ChunkPointerElement_t::ChunkPointerInternal_t;

    protected:
        ArrayExtension_t Array;

    public:
        template<typename TProps>
        DArrayPointerProtectedT(const DPropsTag& tag, const TProps& props)
            : Base_t(tag, props)
            , Array (tag, props)
        {
        }
    };
}
