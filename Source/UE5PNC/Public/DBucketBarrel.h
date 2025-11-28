// MIT License
// Copyright (c) 2025 Stephanie Rancourt

#pragma once
#include "common.h"

namespace PNC
{

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
        DBucketBarrel(const StructurePtr<ChunkStructure_t>& chunkStructure)
            : Base_t(chunkStructure)
        {
        }
        DBucketBarrel(const StructurePtr<ChunkStructure_t>& chunkStructure,
                      const ChunkCapacityT<Size_t> chunkCapacity,
                      const NodeCapacityPerChunkT<Size_t> nodeCapacityPerChunk,
                      const ChunkCountT<Size_t> chunkCount,
                      const NodeCountPerChunkT<Size_t> nodeCountPerChunk)
            : Base_t(chunkStructure, 
                chunkCapacity, 
                chunkCount, 
                nodeCapacityPerChunk, 
                nodeCountPerChunk,
                NodeCountT<Size_t>(chunkCount * nodeCapacityPerChunk),
                NodeCapacityT<Size_t>(chunkCapacity* nodeCapacityPerChunk))
        {
        }
    };
}