// MIT License
// Copyright (c) 2025 Stephanie Rancourt

#pragma once
#include "common.h"

namespace PNC
{

    template<typename TBase>
    struct DBucket : public TBase
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
        using typename Base_t::Chunk_t;

        DBucket() = default;
        DBucket(const StructurePtr<ChunkStructure_t>& chunkStructure)
            : Base_t(chunkStructure)
        {
        }

        DBucket(const StructurePtr<ChunkStructure_t>& chunkStructure, const NodeCapacityT<Size_t> nodeCapacity, const NodeCountT<Size_t> nodeCount = 0)
            : Base_t(chunkStructure, nodeCapacity, nodeCount)
        {
        }
    };
}