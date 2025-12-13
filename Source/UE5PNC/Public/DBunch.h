// MIT License
// Copyright (c) 2025 Stephanie Rancourt

#pragma once
#include "common.h"

namespace NiT
{
    /// <summary>
    /// Container of Node and their component data.
    /// 
    /// Container with a variable NodeCount of constructed nodes smaller or equal to a variable (growing by multiple of 2) NodeCapacity of allocated nodes and their components.
    /// A Bunch has a variable capacity of allocated nodes that grows when needed to fit a smaller or equal number of constructed nodes. 
    /// 
    /// Each Component data is allocated sequentially in memory with the same NodeCapacity
    /// </summary>
    template<typename TBase>
    struct DBunch : public TBase
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
        DBunch() = default;

        /// <summary>
        /// Create a StructNull Container
        /// </summary>
        DBunch(const ChunkStructure_t* const chunkStructure)
            : Base_t(chunkStructure)
        {
        }

        /// <summary>
        /// Create a StructData Container with (NodeCapacity) allocated nodes and a smaller or equal (NodeCount) number of constructed nodes.
        /// </summary>
        DBunch(const ChunkStructure_t* const chunkStructure, 
               const NodeCapacityT<Size_t> nodeCapacity, 
               const NodeCountT<   Size_t> nodeCount = NodeCountT<Size_t>::V_0())
            : Base_t(chunkStructure, nodeCapacity, nodeCount)
        {
        }
#ifndef PNC_PROPS_STRICT
        DBunch(const ChunkStructure_t* const chunkStructure,
               const Size_t nodeCapacity,
               const Size_t nodeCount = (Size_t)0)
            : DBunch(chunkStructure, 
                     PropNodeCapacityT<Size_t>(nodeCapacity), 
                     PropNodeCountT<   Size_t>(nodeCount))
        {
        }
#endif
    };
}
