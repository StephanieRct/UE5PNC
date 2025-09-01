// MIT License
// Copyright (c) 2025 Stephanie Rancourt

#pragma once
#include "common.h"
#include "ComponentTypeSet.h"

namespace PNC
{
    /// <summary>
    /// A chunk type defines the structure of a chunk.
    /// Pointers to ChunkType are used to uniquely identify ChunkType between chunks
    /// </summary>
    template< typename TSize = size_t>
    struct ChunkTypeT 
    {
    public:
        typedef TSize Size_t;
        typedef ComponentTypeSetT<TSize> ComponentTypeSet_t;
        typedef typename ComponentTypeSet_t::ComponentType_t ComponentType_t;

    public:

        /// <summary>
        /// Set of component types this chunk type defines
        /// </summary>
        ComponentTypeSet_t Components;

        /// <summary>
        /// Create a ChunkType from a list of ComponentType
        /// </summary>
        /// <param name="components"></param>
        ChunkTypeT(std::initializer_list<const ComponentType_t*> components) :Components(components) {}

        /// <summary>
        /// Get the index of a component type in the ComponentTypeSet of this ChunkType
        /// </summary>
        /// <param name="type"></param>
        /// <returns></returns>
        int GetComponentTypeIndexInChunk(const type_info* type)const { return Components.GetComponentTypeIndexInChunk(type); }
    };

}