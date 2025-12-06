// MIT License
// Copyright (c) 2025 Stephanie Rancourt

#pragma once
#include "common.h"
#include "ComponentTypeSet.h"

namespace NiT
{
    /// <summary>
    /// A ChunkStructure defines the types of components that compose a chunk.
    /// Pointers to ChunkStructure are used to uniquely identify the structure between chunks
    /// </summary>
    template< typename TSize>
    struct ChunkStructureT 
    {
    public:
        using Self_t = ChunkStructureT<TSize>;
        using Size_t = TSize;
        using ComponentTypeSet_t = ComponentTypeSetT<TSize>;
        using ComponentType_t = typename ComponentTypeSet_t::ComponentType_t;

    public:
        /// <summary>
        /// Set of component types this ChunkStructure defines
        /// </summary>
        ComponentTypeSet_t Components;

        std_vector<Size_t> DefaultConstructibleNodeIndex;
        std_vector<Size_t> DefaultConstructibleChunkIndex;
        std_vector<Size_t> DestructibleNodeIndex;
        std_vector<Size_t> DestructibleChunkIndex;
        std_vector<Size_t> NodeComponentIndex;
        std_vector<Size_t> ChunkComponentIndex;
    public:

        /// <summary>
        /// Create a ChunkStructure from a list of ComponentType
        /// </summary>
        /// <param name="components"></param>
        ChunkStructureT(std::initializer_list<const ComponentType_t*> components) 
            :Components(components) 
        {
            Update();
        }

        ChunkStructureT(std_vector<const ComponentType_t*>&& types)
            :Components(std::move(types))
        {
            Update();
        }

        template<typename... TComponentTypes>
        ChunkStructureT(TComponentTypes... componentTypes)
            : Components(componentTypes...)
        {
            Update();
        }

        bool operator==(const Self_t& other)const { return IsSame(other); }
        bool operator!=(const Self_t& other)const { return !IsSame(other); }

    public:

        Size_t GetComponentCount() const { return Components.GetSize(); }
        const ComponentType_t& GetComponentType(const Size_t index)const
        {
            return *Components[index];
        }
        /// <summary>
        /// Get the index of a component type in the ComponentTypeSet of this ChunkStructure
        /// </summary>
        /// <param name="type"></param>
        /// <returns></returns>
        Size_t GetComponentTypeIndexInChunk(const type_info* type)const { return Components.GetComponentTypeIndexInChunk(type); }

        Size_t GetComponentTypeIndexInChunk(const ComponentType_t* const type)const { return Components.GetComponentTypeIndexInChunk(type); }

        /// <summary>
        /// If both structures are equal
        /// </summary>
        /// <param name="other"></param>
        /// <returns></returns>
        bool IsSame(const Self_t& other)const
        {
            return Components.IsSame(other.Components);
        }

    public:
        struct Hasher_t
        {
            std::size_t operator()(const Self_t* a) const
            {
                return a ? a->Components.GetHash() : (std::numeric_limits< std::size_t>::max() >> 1);
            }
        };
        struct Equaler_t
        {
            bool operator()(const Self_t* a, const Self_t* b) const
            {
                if (!a)
                    return !b;
                if (!b) return false;
                return a->IsSame(*b);
            }
        };
    protected:
        void Update() 
        {
            Components.SubSetIndex(&DefaultConstructibleNodeIndex, [](const ComponentType_t* c) { return c->IsNonTrivialConstruct() && c->IsNodeComponent(); });
            Components.SubSetIndex(&DestructibleNodeIndex, [](const ComponentType_t* c) { return c->IsNonTrivialDestruct() && c->IsNodeComponent(); });
            Components.SubSetIndex(&DefaultConstructibleChunkIndex, [](const ComponentType_t* c) { return c->IsNonTrivialConstruct() && c->IsChunkComponent(); });
            Components.SubSetIndex(&DestructibleChunkIndex, [](const ComponentType_t* c) { return c->IsNonTrivialDestruct() && c->IsChunkComponent(); });
            Components.SubSetIndex(&NodeComponentIndex, [](const ComponentType_t* c) { return c->IsNodeComponent(); });
            Components.SubSetIndex(&ChunkComponentIndex, [](const ComponentType_t* c) { return c->IsChunkComponent(); });
        }
    };
}
