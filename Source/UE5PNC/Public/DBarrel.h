// MIT License
// Copyright (c) 2025 Stephanie Rancourt

#pragma once
#include "common.h"

namespace NiT
{
    //TODO complete
    template<typename TBase>
    struct DBarrel : public TBase
    {
    public:
        using Base_t = TBase;
        using Self_t = DBarrel<TBase>;

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

        DBarrel() = default;
        DBarrel(const ChunkStructure_t* const chunkStructure)
            : Base_t(chunkStructure)
        {
        }


        // Full Uniform Barrel
        DBarrel(const ChunkStructure_t* const chunkStructure,
                const ChunkCountT<       Size_t> chunkCount,
                const NodeCountPerChunkT<Size_t> nodeCountPerChunk)
            : Base_t(chunkStructure,
                     chunkCount,
                     PropChunkCapacityT<Size_t>(chunkCount),
                     PropNodeCountT<    Size_t>(chunkCount * nodeCountPerChunk),
                     PropNodeCapacityT< Size_t>(chunkCount * nodeCountPerChunk),
                     nodeCountPerChunk)
        {
        }

        // Empty, Partial or Full Uniform array
        // nodeCountPerChunk may be 0.
        DBarrel(const ChunkStructure_t* const chunkStructure,
                const ChunkCapacityT<    Size_t> chunkCapacity,
                const NodeCountPerChunkT<Size_t> nodeCountPerChunk,
                const ChunkCountT<       Size_t> chunkCount)
            : Base_t(chunkStructure,
                     chunkCount,
                     chunkCapacity,
                     PropNodeCountT<   Size_t>(chunkCount *    nodeCountPerChunk),
                     PropNodeCapacityT<Size_t>(chunkCapacity * nodeCountPerChunk),
                     nodeCountPerChunk)
        {
            ni_assert(chunkCapacity >= 0);
            ni_assert(nodeCountPerChunk >= 0);
            ni_assert(chunkCount >= 0);
            ni_assert(chunkCount <= chunkCapacity);
        }

        // Empty, Partial or Full Uniform array with additional node capacity.
        // nodeCountPerChunk may be 0.
        // Chunk elements can increade/decrease node count up to the total node capacity.
        DBarrel(const ChunkStructure_t* const chunkStructure,
                const ChunkCapacityT<    Size_t> chunkCapacity,
                const NodeCountPerChunkT<Size_t> nodeCountPerChunk,
                const ChunkCountT<       Size_t> chunkCount,
                const NodeCapacityT<     Size_t> nodeCapacity)
            : Base_t(chunkStructure,
                     chunkCount,
                     chunkCapacity,
                     PropNodeCountT<Size_t>(chunkCount * nodeCountPerChunk),
                     nodeCapacity,
                     nodeCountPerChunk)
        {
        }
    };
}
