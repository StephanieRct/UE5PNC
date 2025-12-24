// MIT License
// Copyright (c) 2025 Stephanie Rancourt

#pragma once
#include "common.h"

namespace NiT
{
    // TODO complete
    template<typename TBase>
    struct DBucketArray : public TBase
    {
    public:
        using Base_t = TBase;
        using Self_t = DBucketArray<TBase>;

        using typename Base_t::Size_t;
        using typename Base_t::ComponentType_t;
        using typename Base_t::ChunkStructure_t;
        using typename Base_t::Node_t;
        using typename Base_t::ChunkPointerInternal_t;
        using typename Base_t::ChunkPointer_t;
        using typename Base_t::Chunk_t;
        using typename Base_t::ArrayExtension_t;
        using typename Base_t::ChunkPointerInternal_t;
        using typename Base_t::ChunkPointerElementInternal_t;

        DBucketArray() = default;
        DBucketArray(const ChunkStructure_t* const chunkStructure)
            : Base_t(chunkStructure)
        {
        }
        DBucketArray(const ChunkStructure_t* const chunkStructure,
                     const ChunkCountT          <Size_t> chunkCount,
                     const NodeCapacityPerChunkT<Size_t> nodeCapacityPerChunk,
                     const NodeCountPerChunkT   <Size_t> nodeCountPerChunk)
            : Base_t(chunkStructure,
                     PropCountToCapacity(chunkCount),
                     chunkCount,
                     nodeCapacityPerChunk,
                     nodeCountPerChunk,
                     PropNodeCount   (chunkCount * nodeCapacityPerChunk),
                     PropNodeCapacity(chunkCount * nodeCapacityPerChunk))
        {
        }
    };
}
