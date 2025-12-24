// MIT License
// Copyright (c) 2025 Stephanie Rancourt

#pragma once
#include "common.h"
#include "ComponentType.h"

namespace NiT
{
    /// <summary>
    /// A set of component types.
    /// </summary>
    /// <typeparam name="TSize"></typeparam>
    template< typename TSize = size_t>
    struct ComponentTypeSetT
    {
    public:
        using Self_t = ComponentTypeSetT<TSize>;
        using Size_t = TSize;
        using ComponentType_t = ComponentTypeT<Size_t>;

    private:
        std::size_t Hash;
        std_vector<const ComponentType_t*> ComponentTypes;
        std_unordered_map<const type_info*, Size_t> TypeToComponentTypeIndexInChunk;

    public:

        std::size_t GetHash()const
        {
            return Hash;
        }

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

        ComponentTypeSetT()
            : Hash(0)
            , ComponentTypes()
            , TypeToComponentTypeIndexInChunk()
        {
        }

        ComponentTypeSetT(const ComponentType_t* componentType)
            :ComponentTypes(1)
        {
            ComponentTypes.push_back(componentType);
            Normalize();
        }
        /// <summary>
        /// Create a ComponentTypeSet from a list of ComponentType
        /// </summary>
        /// <param name="types"></param>
        ComponentTypeSetT(const std::initializer_list<const ComponentType_t*>& types)
            :ComponentTypes(types)
        {
            Normalize();
        }

        ComponentTypeSetT(std_vector<const ComponentType_t*>&& types)
            :ComponentTypes(std::move(types))
        {
            Normalize();
        }

        template<typename... TComponentTypes>
        ComponentTypeSetT(TComponentTypes... componentTypes)
            : ComponentTypes(sizeof...(componentTypes))
        {
            Size_t c = 0;
            ((ComponentTypes[c++] = componentTypes), ...);
            Normalize();
        }


        bool operator==(const Self_t& other)const { return IsSame(other); }
        bool operator!=(const Self_t& other)const { return !IsSame(other); }
        bool operator<(const Self_t& other)const
        {
            auto sizeThis = ComponentTypes.size();
            auto sizeOther = other.ComponentTypes.size();
            if (sizeThis < sizeOther)
                return true;
            if (sizeThis > sizeOther)
                return false;
            return std::memcmp(ComponentTypes.begin(), other.ComponentTypes.begin(), sizeof(const ComponentType_t*) * ComponentTypes.size()) < 0;
        }
        bool operator<=(const Self_t& other)const
        {
            auto sizeThis = ComponentTypes.size();
            auto sizeOther = other.ComponentTypes.size();
            if (sizeThis < sizeOther)
                return true;
            if (sizeThis > sizeOther)
                return false;
            return std::memcmp(ComponentTypes.begin(), other.ComponentTypes.begin(), sizeof(const ComponentType_t*) * ComponentTypes.size()) <= 0;
        }
        bool operator>(const Self_t& other)const { return !operator<=(other); }
        bool operator>=(const Self_t& other)const { return !operator<(other); }
        

        struct Hasher_t
        {
            std::size_t operator()(const Self_t* a) const
            {
                return a ? a->Hash : (std::numeric_limits< std::size_t>::max() >> 1);
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
    public:

        /// <summary>
        /// Sort components in the set
        /// </summary>
        void Normalize()
        {
            std::sort(ComponentTypes.begin(), ComponentTypes.end());
            UpdateMap();
            Hash = InternalHasher_t()(*this);
        }

        /// <summary>
        /// If both sets include the exact same elements
        /// </summary>
        /// <param name="other"></param>
        /// <returns></returns>
        bool IsSame(const Self_t& other)const
        {
            if (ComponentTypes.size() != other.ComponentTypes.size())
                return false;
            return std::memcmp(ComponentTypes.data(), other.ComponentTypes.data(), sizeof(const ComponentType_t*) * ComponentTypes.size()) == 0;
        }

        /// <summary>
        /// Get the index of a component type_info in the set.
        /// Will return -1 if the component type is not present in the set.
        /// </summary>
        /// <param name="type">type_info of the component type. ex. &typeid(MyComponent)</param>
        /// <returns>return index of the component type in the set or -1 if not found.</returns>
        Size_t GetComponentTypeIndexInChunk(const type_info* const type)const
        {
            auto i = TypeToComponentTypeIndexInChunk.find(type);
            if (i == TypeToComponentTypeIndexInChunk.end())
                return -1;
            return i->second;
        }
        Size_t GetComponentTypeIndexInChunk(const ComponentType_t*const type)const
        {
            return GetComponentTypeIndexInChunk(type->GetTypeInfo());
        }

        template<typename TPredicate>
        void SubSet(Self_t* const resultMemory, TPredicate predicate)
        {
            std_vector<const ComponentType_t*> subComponentTypes;
            for (Size_t i = 0; i < ComponentTypes.size(); ++i)
            {
                if (predicate(ComponentTypes[i]))
                {
                    subComponentTypes.push_back(ComponentTypes[i]);
                }
            }
            new (resultMemory) Self_t(std::move(subComponentTypes));
        }

        template<typename TPredicate>
        void SubSetIndex(std_vector<Size_t>* resultMemory, TPredicate predicate)
        {
            std_vector<Size_t> subComponentTypes;
            for (Size_t i = 0; i < ComponentTypes.size(); ++i)
            {
                if (predicate(ComponentTypes[i]))
                {
                    subComponentTypes.push_back(i);
                }
            }
            new (resultMemory) std_vector<Size_t>(std::move(subComponentTypes));
        }

    private:
        struct InternalHasher_t
        {
            std::size_t operator()(const Self_t& a) const
            {
                std::size_t h = 0;
                for (std::size_t i = 0; i < a.ComponentTypes.size(); ++i)
                {
                    h ^= std::hash<const ComponentType_t*>()(a.ComponentTypes[i]) + 0x9e3779b9 + (h << 6) + (h >> 2);
                }
                return h;
            }
        };
        void UpdateMap() 
        {
            std::sort(ComponentTypes.begin(), ComponentTypes.end());
            TypeToComponentTypeIndexInChunk.clear();
            for (int i = 0; i < ComponentTypes.size(); ++i) 
            {
                TypeToComponentTypeIndexInChunk[ComponentTypes[i]->GetTypeInfo()] = i;
            }
            Hash = InternalHasher_t()(*this);
        }
    };
}
