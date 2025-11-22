// MIT License
// Copyright (c) 2025 Stephanie Rancourt

#pragma once
#include "common.h"

namespace PNC
{
    template<typename TArrayExtension, typename TBase>
    struct DArrayPointerInternalT : public TBase
    {
    public:
        using Base_t = TBase;
        using Self_t = DArrayPointerInternalT<TArrayExtension, TBase>;
        using ArrayExtension_t = TArrayExtension;
        using ChunkPointerElement_t = ArrayExtension_t::ChunkPointerElement_t;
        using typename Base_t::Size_t;
        using typename Base_t::ComponentType_t;
        using typename Base_t::ChunkStructure_t;
        using typename Base_t::Node_t;
        using Chunk_t = Self_t;
        using ChunkPointerElementInternal_t = typename ChunkPointerElement_t::ChunkPointerInternal_t;

    public:
        ArrayExtension_t Array;
    };
}