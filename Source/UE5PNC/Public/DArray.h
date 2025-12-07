// MIT License
// Copyright (c) 2025 Stephanie Rancourt

#pragma once
#include "common.h"

namespace NiT
{
    /// <summary>
    /// Container of Chunk, Node and their component data.
    /// 
    /// Container with an fixed ChunkCount of allocated and constructed chunks, and a total number (NodeCount) of 
    /// allocated and constructed nodes.
    /// 
    /// A DArray is a fixed number of chunks totalling a fixed number of sequential nodes and their components in memory.
    /// </summary>
    template<typename TBase>
    struct DArray : public TBase
    {
    public:
        using Base_t = TBase;
        using Self_t = DArray<TBase>;
        using typename Base_t::Size_t;
        using typename Base_t::ComponentType_t;
        using typename Base_t::ChunkStructure_t;
        using typename Base_t::Node_t;
        using typename Base_t::ChunkPointerInternal_t;
        using typename Base_t::ChunkPointer_t;
        using typename Base_t::ArrayExtension_t;
        using typename Base_t::ChunkPointerInternal_t;
        using typename Base_t::ChunkPointerElementInternal_t;

        /// <summary>
        /// Create a VoidNull Container.
        /// </summary>
        DArray() = default;

        /// <summary>
        /// Create a StructNull Container
        /// </summary>
        DArray(const ChunkStructure_t* const chunkStructure)
            : Base_t(chunkStructure)
        {
        }

        /// <summary>
        /// Create a StructData Container with:
        ///     (ChunkCount) allocated and constructed chunks;
        ///     all chunk elements in the array have a NodeCount equal to NodeCountPerChunk;
        ///     totalling to the full array of allocated and constructed (NodeCount) nodes equal to ChunkCount * NodeCountPerChunk.
        /// </summary>
        DArray(const ChunkStructure_t* const chunkStructure,
               const ChunkCountT<       Size_t> chunkCount,
               const NodeCountPerChunkT<Size_t> nodeCountPerChunk)
            : Base_t(chunkStructure,
                     chunkCount,
                     ChunkCapacityT<Size_t>(chunkCount),
                     NodeCountT<    Size_t>(chunkCount * nodeCountPerChunk),
                     NodeCapacityT< Size_t>(chunkCount * nodeCountPerChunk),
                     nodeCountPerChunk)
        {
        }
#ifndef PNC_PROPS_STRICT
        DArray(const ChunkStructure_t*const chunkStructure,
               const Size_t chunkCount,
               const Size_t nodeCountPerChunk)
            : DArray(chunkStructure,
                     PropChunkCountT<      Size_t>(chunkCount),
                     PropNodeCountPerChunkT<Size_t>(nodeCountPerChunk))
        {
        }
#endif
        /// <summary>
        /// Create a StructData Container with:
        ///     (ChunkCount) allocated and constructed chunks;
        ///     all chunk elements in the array have a NodeCount equal to NodeCountPerChunk;
        ///     totalling to the full array of constructed (NodeCount) nodes equal to ChunkCount * NodeCountPerChunk;
        ///     and a full array of allocated (NodeCapacity) nodes that must be greater or equal than the full array NodeCount.
        /// Notes:
        ///     Chunk elements can increase/decrease node count up to the total NodeCapacity.
        ///     NodeCountPerChunk can be 0.
        /// </summary>
        DArray(const ChunkStructure_t* chunkStructure,
               const ChunkCountT<       Size_t> chunkCount,
               const NodeCountPerChunkT<Size_t> nodeCountPerChunk,
               const NodeCapacityT<     Size_t> nodeCapacity)
            : Base_t(chunkStructure,
                     chunkCount,
                     PropCountToCapacity(chunkCount),
                     PropNodeCountT<    Size_t>(chunkCount * nodeCountPerChunk),
                     PropNodeCapacityT< Size_t>(nodeCapacity),
                     nodeCountPerChunk)
        {
            ni_assert(nodeCapacity >= PropCountToCapacity(chunkCount * nodeCountPerChunk));
        }
#ifndef PNC_PROPS_STRICT
        DArray(const ChunkStructure_t* chunkStructure,
               const Size_t chunkCount,
               const Size_t nodeCountPerChunk,
               const Size_t nodeCapacity)
            : DArray(chunkStructure,
                     PropChunkCountT<       Size_t>(chunkCount),
                     PropNodeCountPerChunkT<Size_t>(nodeCountPerChunk),
                     PropNodeCapacityT<     Size_t>(nodeCapacity))
        {
        }
#endif
    };
}
