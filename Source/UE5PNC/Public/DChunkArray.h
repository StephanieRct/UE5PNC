// MIT License
// Copyright (c) 2025 Stephanie Rancourt

#pragma once
#include "common.h"

namespace PNC
{
    template<typename TBase>
    struct DChunkArray : public TBase
    {
    public:
        using Base_t = TBase;
        using Self_t = DChunkArray<TBase>;

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

        DChunkArray() = default;
        DChunkArray(const StructurePtr<ChunkStructure_t>& chunkStructure)
            : Base_t(chunkStructure)
        {
        }


        // Uniform Array
        DChunkArray(const StructurePtr<ChunkStructure_t>& chunkStructure,
            const ChunkCountT<Size_t> chunkCount,
            const NodeCountPerChunkT<Size_t> nodeCountPerChunk)
            : Base_t(chunkStructure,
                chunkCount,
                ChunkCapacityT<Size_t>(chunkCount),
                NodeCountT<Size_t>(chunkCount* nodeCountPerChunk),
                NodeCapacityT<Size_t>(chunkCount* nodeCountPerChunk),
                nodeCountPerChunk)
        {
        }

        // Uniform Array with additional node capacity.
        // nodeCountPerChunk may be 0.
        // Chunk elements can increade/decrease node count up to the total node capacity.
        DChunkArray(const StructurePtr<ChunkStructure_t>& chunkStructure,
            const ChunkCountT<Size_t> chunkCount,
            const NodeCountPerChunkT<Size_t> nodeCountPerChunk,
            const NodeCapacityT<Size_t> nodeCapacity)
            : Base_t(chunkStructure,
                chunkCount,
                ChunkCapacityT<Size_t>(chunkCount),
                NodeCountT<Size_t>(nodeCapacity),
                NodeCapacityT<Size_t>(nodeCapacity),
                nodeCountPerChunk)
        {
            pnc_assert(nodeCapacity >= PropCountToCapacity(chunkCount * nodeCountPerChunk));
        }

    };
}