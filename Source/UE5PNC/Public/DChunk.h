// MIT License
// Copyright (c) 2025 Stephanie Rancourt

#pragma once
#include "common.h"

namespace PNC
{

    template<typename TBase>
    struct DChunk : public TBase
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

        DChunk() = default;
        DChunk(const StructurePtr<ChunkStructure_t>& chunkStructure)
            : Base_t(chunkStructure)
        {
        }
        //DChunk(const StructurePtr<ChunkStructure_t>& chunkStructure, const NodeCountT<Size_t> nodeCount, void** componentData)
        //    : Base_t(chunkStructure, nodeCount, componentData)
        //{
        //}

        DChunk(const StructurePtr<ChunkStructure_t>& chunkStructure, const NodeCountT<Size_t> nodeCount)
            : Base_t(chunkStructure, 
                nodeCount,
                PropCountToCapacity(nodeCount))
        {
        }

    };
}