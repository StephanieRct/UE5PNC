// MIT License
// Copyright (c) 2025 Stephanie Rancourt

#pragma once
#include "common.h"

namespace NiT
{
    // TODO complete
    template<typename TBase>
    struct DCarryPointer : public TBase
    {
    public:
        using Base_t = TBase;
        using Self_t = DCarryPointer<TBase>;
        using typename Base_t::Size_t;
        using typename Base_t::ComponentType_t;
        using typename Base_t::ChunkStructure_t;
        using typename Base_t::Node_t;
        using typename Base_t::ChunkPointerInternal_t;

    public:
        using Base_t::Base_t;
        using Base_t::GetNodeCount;
        using Base_t::GetNodeCapacity;
        using Base_t::GetChunkCapacity;
        using Base_t::AddNodes;

        // TODO override AddChunks so it grows when reaching capacity
    protected:
        using TBase::SetNodeCapacity;
    };
}
