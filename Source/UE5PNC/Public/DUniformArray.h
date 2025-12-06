// MIT License
// Copyright (c) 2025 Stephanie Rancourt

#pragma once
#include "common.h"

namespace NiT
{

    template<typename TBase>
    struct DUniformArray : public TBase
    {
    public:
        using Base_t = TBase;
        using Self_t = DUniformArray<TBase>;

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

        DUniformArray() = default;
        DUniformArray(const ChunkStructure_t* const chunkStructure)
            : Base_t(chunkStructure)
        {
        }

        DUniformArray(const ChunkStructure_t* const chunkStructure,
            const ChunkCountT<Size_t> chunkCount,
            const NodeCountPerChunkT<Size_t> nodeCountPerChunk)
            : Base_t(chunkStructure, 
                     chunkCount, 
                     ChunkCapacityT<Size_t>(chunkCount),
                     NodeCountT<Size_t>(chunkCount * nodeCountPerChunk),
                     NodeCapacityT<Size_t>(chunkCount* nodeCountPerChunk),
                     nodeCountPerChunk)
        {
        }
    };
}