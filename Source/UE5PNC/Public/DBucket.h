// MIT License
// Copyright (c) 2025 Stephanie Rancourt

#pragma once
#include "common.h"

namespace PNC
{
    /// <summary>
    /// Container of Node and their component data.
    /// 
    /// Container with a variable NodeCount of constructed nodes smaller or equal to a fixed NodeCapacity of allocated nodes and their components.
    /// A Bucket has a fix capacity of allocated nodes and a variable number of constructed nodes stating at index 0 that cannot overflow the capacity. 
    /// 
    /// Each Component data is allocated sequentially in memory with the same NodeCapacity
    /// </summary>
    template<typename TBase>
    struct DBucket : public TBase
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
        /// Create a VoidNull Container.
        /// </summary>
        DBucket() = default;

        /// <summary>
        /// Create a StructNull Container
        /// </summary>
        DBucket(const StructurePtr<ChunkStructure_t>& chunkStructure)
            : Base_t(chunkStructure)
        {
        }

        /// <summary>
        /// Create a StructData Container with (NodeCapacity) allocated nodes and a smaller or equal (NodeCount) number of constructed nodes.
        /// </summary>
        DBucket(const StructurePtr<ChunkStructure_t>& chunkStructure, 
                const NodeCapacityT<Size_t> nodeCapacity, 
                const NodeCountT   <Size_t> nodeCount = 0)
            : Base_t(chunkStructure, nodeCapacity, nodeCount)
        {
        }
    };
}
