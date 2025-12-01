// MIT License
// Copyright (c) 2025 Stephanie Rancourt

#pragma once
#include "common.h"

namespace PNC
{
    template<typename TBase>
    struct DBunch : public TBase
    {
    public:
        using Base_t = TBase;
        using Self_t = DChunk<TBase>;

        using typename Base_t::Size_t;
        using typename Base_t::ComponentType_t;
        using typename Base_t::ChunkStructure_t;
        using typename Base_t::Node_t;
        using typename Base_t::ChunkPointerInternal_t;
        using typename Base_t::ChunkPointer_t;

        DBunch() = default;
        DBunch(const StructurePtr<ChunkStructure_t>& chunkStructure)
            : Base_t(chunkStructure)
        {
        }

        DBunch(const StructurePtr<ChunkStructure_t>& chunkStructure, NodeCapacityT<Size_t> nodeCapacity, NodeCountT<Size_t> nodeCount = 0)
            : Base_t(chunkStructure, nodeCapacity, nodeCount)
        {
        }
    };
}