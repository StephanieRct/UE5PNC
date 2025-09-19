// MIT License
// Copyright (c) 2025 Stephanie Rancourt

#pragma once
#include "common.h"
#include "ComponentType.h"

namespace PNC
{
    /// <summary>
    /// A set of component types.
    /// </summary>
    /// <typeparam name="TSize"></typeparam>
    template< typename TSize = size_t>
    struct ComponentTypeSetT
    {
    public:
        typedef TSize Size_t;
        typedef ComponentTypeT<Size_t> ComponentType_t;

    private:
        std::vector<const ComponentType_t*> ComponentTypes;
        std::unordered_map<const type_info*, Size_t> TypeToComponentTypeIndexInChunk;

    public:
        /// <summary>
        /// Get the number of component types in this set.
        /// </summary>
        Size_t GetSize()const { return ComponentTypes.size(); }

        /// <summary>
        /// Get the component type at an index less than the set's size.
        /// </summary>
        const ComponentType_t* operator[](Size_t index)const
        {
            return ComponentTypes[index];
        }

    public:
        /// <summary>
        /// Create a ComponentTypeSet from a list of ComponentType
        /// </summary>
        /// <param name="aTypes"></param>
        ComponentTypeSetT(const std::initializer_list<const ComponentType_t*>& aTypes)
            :ComponentTypes(aTypes) 
        {
            UpdateMap();
        }

    public:
        /// <summary>
        /// Get the index of a component type_info in the set.
        /// Will return -1 if the component type is not present in the set.
        /// </summary>
        /// <param name="type">type_info of the component type. ex. &typeid(MyComponent)</param>
        /// <returns>return index of the component type in the set or -1 if not found.</returns>
        Size_t GetComponentTypeIndexInChunk(const type_info* type)const 
        {
            auto i = TypeToComponentTypeIndexInChunk.find(type);
            if (i == TypeToComponentTypeIndexInChunk.end())
                return -1;
            return i->second;
        }

    private:
        void UpdateMap() 
        {
            TypeToComponentTypeIndexInChunk.clear();
            for (int i = 0; i < ComponentTypes.size(); ++i) 
            {
                TypeToComponentTypeIndexInChunk[ComponentTypes[i]->TypeInfo] = i;
            }
        }
    };
}
