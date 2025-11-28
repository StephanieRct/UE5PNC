// MIT License
// Copyright (c) 2025 Stephanie Rancourt

#pragma once
#include "common.h"

namespace PNC
{
    template<typename TBase>
    struct DChunkBarrel : public TBase
    {
    public:
        using Base_t = TBase;
        using Self_t = DChunkBarrel<TBase>;

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

        DChunkBarrel() = default;
        DChunkBarrel(const StructurePtr<ChunkStructure_t>& chunkStructure)
            : Base_t(chunkStructure)
        {
        }


        // Full Uniform Barrel
        DChunkBarrel(const StructurePtr<ChunkStructure_t>& chunkStructure,
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

        //// Empty Barrel
        //DChunkBarrel(const StructurePtr<ChunkStructure_t>& chunkStructure,
        //    const ChunkCapacityT<Size_t> chunkCapacity,
        //    const NodeCapacityT<Size_t> nodeCapacity)
        //    : Base_t(chunkStructure,
        //             ChunkCountT<Size_t>(0),
        //             chunkCapacity,
        //             NodeCountT<Size_t>(0),
        //             nodeCapacity,
        //             NodeCountPerChunkT<Size_t>(0))
        //{
        //}

        // Empty, Partial or Full Uniform array
        // nodeCountPerChunk may be 0.
        DChunkBarrel(const StructurePtr<ChunkStructure_t>& chunkStructure,
            const ChunkCapacityT<Size_t> chunkCapacity,
            const NodeCountPerChunkT<Size_t> nodeCountPerChunk,
            const ChunkCountT<Size_t> chunkCount)
            : Base_t(chunkStructure,
                chunkCount,
                chunkCapacity,
                NodeCountT<Size_t>(chunkCount * nodeCountPerChunk),
                NodeCapacityT<Size_t>(chunkCapacity * nodeCountPerChunk),
                nodeCountPerChunk)
        {
            pnc_assert(chunkCapacity >= 0);
            pnc_assert(nodeCountPerChunk >= 0);
            pnc_assert(chunkCount >= 0);
            pnc_assert(chunkCount <= chunkCapacity);
        }

        // Empty, Partial or Full Uniform array with additional node capacity.
        // nodeCountPerChunk may be 0.
        // Chunk elements can increade/decrease node count up to the total node capacity.
        DChunkBarrel(const StructurePtr<ChunkStructure_t>& chunkStructure,
            const ChunkCapacityT<Size_t> chunkCapacity,
            const NodeCountPerChunkT<Size_t> nodeCountPerChunk,
            const ChunkCountT<Size_t> chunkCount,
            const NodeCapacityT<Size_t> nodeCapacity)
            : Base_t(chunkStructure,
                chunkCount,
                chunkCapacity,
                NodeCountT<Size_t>(chunkCount * nodeCountPerChunk),
                nodeCapacity,
                nodeCountPerChunk)
        {
        }
    };
}