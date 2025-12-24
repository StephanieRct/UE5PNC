// MIT License
// Copyright (c) 2025 Stephanie Rancourt

#pragma once
#include "common.h"

namespace NiT
{
    // TODO complete
    template<typename TBase>
    struct DBucketBarrel : public TBase
    {
    public:
        using Base_t = TBase;
        using Self_t = DBucketBarrel<TBase>;
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

        DBucketBarrel() = default;
        DBucketBarrel(const ChunkStructure_t* const chunkStructure)
            : Base_t(chunkStructure)
        {
        }
        DBucketBarrel(const ChunkStructure_t* const chunkStructure,
                      const ChunkCapacityT       <Size_t> chunkCapacity,
                      const ChunkCountT          <Size_t> chunkCount,
                      const NodeCapacityPerChunkT<Size_t> nodeCapacityPerChunk,
                      const NodeCountPerChunkT   <Size_t> nodeCountPerChunk)
            : Base_t(chunkStructure, 
                     chunkCapacity, 
                     chunkCount, 
                     nodeCapacityPerChunk, 
                     nodeCountPerChunk,
                     PropNodeCount   (chunkCount    * nodeCapacityPerChunk),
                     PropNodeCapacity(chunkCapacity * nodeCapacityPerChunk))
        {
        }
    };
}
