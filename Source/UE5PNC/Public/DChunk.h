// MIT License
// Copyright (c) 2025 Stephanie Rancourt

#pragma once
#include "common.h"

namespace NiT
{
    /// <summary>
    /// Container of Node and their component data.
    /// 
    /// Container with a fixed NodeCount of allocated and constructed nodes and their components.
    /// A Chunk is a fixed number of constructed nodes.
    /// 
    /// Each Component data is allocated sequentially in memory with the same NodeCount
    /// </summary>
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

        /// <summary>
        /// Create a VoidNull Chunk.
        /// </summary>
        DChunk() = default;

        /// <summary>
        /// Create a StructNull Chunk
        /// </summary>
        DChunk(const ChunkStructure_t* const chunkStructure)
            : Base_t(chunkStructure)
        {
        }

        /// <summary>
        /// Create a StructData Chunk with (NodeCount) allocated and constructed nodes.
        /// </summary>
        DChunk(const ChunkStructure_t*const chunkStructure, 
               const NodeCountT<Size_t> nodeCount)
            : Base_t(chunkStructure, 
                     nodeCount,
                     PropCountToCapacity(nodeCount))
        {
        }
#ifndef PNC_PROPS_STRICT
        /// <summary>
        /// Create a StructData Chunk with (NodeCount) allocated and constructed nodes.
        /// </summary>
        DChunk(const ChunkStructure_t*const chunkStructure, 
               const Size_t nodeCount)
            : DChunk(chunkStructure, PropNodeCountT<Size_t>(nodeCount))
        {
        }
#endif
    };
}
