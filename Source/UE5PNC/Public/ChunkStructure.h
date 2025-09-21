// MIT License
// Copyright (c) 2025 Stephanie Rancourt

#pragma once
#include "common.h"
#include "ComponentTypeSet.h"

namespace PNC
{
    /// <summary>
    /// A ChunkStructure defines the types of components that compose a chunk.
    /// Pointers to ChunkStructure are used to uniquely identify the structure between chunks
    /// </summary>
    template< typename TSize>
    struct ChunkStructureT 
    {
    public:
        using Self = ChunkStructureT<TSize>;
        using Size_t = TSize;
        using ComponentTypeSet_t = ComponentTypeSetT<TSize>;
        using ComponentType_t = typename ComponentTypeSet_t::ComponentType_t;

    public:
        /// <summary>
        /// Set of component types this ChunkStructure defines
        /// </summary>
        ComponentTypeSet_t Components;

        /// <summary>
        /// Create a ChunkStructure from a list of ComponentType
        /// </summary>
        /// <param name="components"></param>
        ChunkStructureT(std::initializer_list<const ComponentType_t*> components) :Components(components) {}

        /// <summary>
        /// Get the index of a component type in the ComponentTypeSet of this ChunkStructure
        /// </summary>
        /// <param name="type"></param>
        /// <returns></returns>
        int GetComponentTypeIndexInChunk(const type_info* type)const { return Components.GetComponentTypeIndexInChunk(type); }
    };
}
