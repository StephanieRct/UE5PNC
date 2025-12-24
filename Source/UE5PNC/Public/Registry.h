// MIT License
// Copyright (c) 2025 Stephanie Rancourt

#pragma once
#include "Ni.h"

/// <summary>
/// Registries are used to scope the creation of various data.
/// </summary>
namespace NiT
{
    template<typename TComponentType>
    struct ComponentTypeRegistryT
    {
    public:
        using Self_t = ComponentTypeRegistryT<TComponentType>;
        using ComponentType_t = TComponentType;
        using Size_t = typename ComponentType_t::Size_t;

    public:
        std_vector<Unique_Ptr<ComponentType_t>> ComponentTypes;
        std_unordered_map<const type_info*, Size_t> TypeToComponentTypeIndex;

    public:
        template<typename T>
        const ComponentType_t* AddComponentType()
        {

            auto componentType = std_make_unique<ComponentType_t>((T*)nullptr, T::Owner);
            auto* ptr = componentType.get();
            ComponentTypes.push_back(std::move(componentType));
            TypeToComponentTypeIndex.insert({ &typeid(T), ComponentTypes.size() - 1 });
            return ptr;
        }
        template<typename T>
        const ComponentType_t* GetOrAddComponentType()
        {
            auto i = TypeToComponentTypeIndex.find(&typeid(T));
            if (i != TypeToComponentTypeIndex.end())
                return ComponentTypes[i->second].get();
            return AddComponentType<T>();
        }

        template<typename T>
        const ComponentType_t* GetComponentType()
        {
            auto i = TypeToComponentTypeIndex.find(&typeid(T));
            if (i != TypeToComponentTypeIndex.end())
                return ComponentTypes[i->second].get();
            return nullptr;
        }
    };


    template<typename TChunkStructure>
    struct ChunkStructureRegistryT
    {
    public:
        using Self_t = ChunkStructureRegistryT<TChunkStructure>;
        using ChunkStructure_t = TChunkStructure;
        using ComponentType_t = typename ChunkStructure_t::ComponentType_t;
        using ChunkStructureHasher_t = typename ChunkStructure_t::Hasher_t;
        using ChunkStructureEqualler_t = typename ChunkStructure_t::Equaler_t;
        using ComponentTypeRegistry_t = ComponentTypeRegistryT<ComponentType_t>;
        using Size_t = typename ChunkStructure_t::Size_t;
    public:
        // Keep an array of all our chunk structures
        std_vector<Unique_Ptr<ChunkStructure_t>> ChunkStructures;
        std_unordered_map< const ChunkStructure_t*, std::size_t, ChunkStructureHasher_t, ChunkStructureEqualler_t> ChunkStructureToIndex;

        const ChunkStructure_t* GetOrAddChunkStructure(const ComponentType_t* component)
        {
            auto chunkStructure = std_make_unique<ChunkStructure_t>(component);
            auto ptr = chunkStructure.get();
            auto i = ChunkStructureToIndex.find(ptr);
            if (i != ChunkStructureToIndex.end())
                return i->first;
            ChunkStructures.push_back(std::move(chunkStructure));
            ChunkStructureToIndex.insert(i, { ptr, ChunkStructures.size() - 1 });
            return ptr;
        }
        // Add a chunk structure from a list of component type
        const ChunkStructure_t* GetOrAddChunkStructure(const std::initializer_list<const ComponentType_t*>& aComponents)
        {
            auto chunkStructure = std_make_unique<ChunkStructure_t>(aComponents);
            auto ptr = chunkStructure.get();
            auto i = ChunkStructureToIndex.find(ptr);
            if (i != ChunkStructureToIndex.end())
                return i->first;
            ChunkStructures.push_back(std::move(chunkStructure));
            ChunkStructureToIndex.insert(i, { ptr, ChunkStructures.size() - 1 });
            return ptr;
        }


        template<typename... TComponentTypes>
        const ChunkStructure_t* GetOrAddChunkStructure(ComponentTypeRegistry_t& componentTypeRegistry)
        {
            std_vector<const ComponentType_t*> componentTypes(sizeof...(TComponentTypes));
            Size_t c = 0;
            ((componentTypes[c++] = componentTypeRegistry.GetOrAddComponentType<TComponentTypes>()), ...);

            auto chunkStructure = std_make_unique<ChunkStructure_t>(std::move(componentTypes));
            auto ptr = chunkStructure.get();
            auto i = ChunkStructureToIndex.find(ptr);
            if (i != ChunkStructureToIndex.end())
                return i->first;
            ChunkStructures.push_back(std::move(chunkStructure));
            ChunkStructureToIndex.insert(i, { ptr, ChunkStructures.size() - 1 });
            return ptr;
        }

    };

    template<typename TChunk, typename TChunkArray>
    struct ChunkRegistryT
    {
    public:
        using Self_t = ChunkRegistryT<TChunk, TChunkArray>;
        using Chunk_t = TChunk;
        using ChunkArray_t = TChunkArray;
        using ChunkStructure_t = typename Chunk_t::ChunkStructure_t;
        using Size_t = typename ChunkStructure_t::Size_t;

    public:
        std_unordered_set<Chunk_t*> Chunks;
        std_unordered_set<ChunkArray_t*> ChunkArrays;

    public:

        ChunkRegistryT() = default;

        // TODO: Make copyable
        ChunkRegistryT(const ChunkRegistryT& other) = delete;
        ChunkRegistryT& operator=(const ChunkRegistryT& other) = delete;

        ChunkRegistryT(ChunkRegistryT&& other) = default;
        ChunkRegistryT& operator=(ChunkRegistryT&& other) = default;

        ~ChunkRegistryT()
        {
            for (Chunk_t* chunk : Chunks)
                ni_delete_dirty(chunk);
            for (ChunkArray_t* chunk : ChunkArrays)
                ni_delete_dirty(chunk);
        }


    public:
        // Add a chunk with a given chunk structure and capacity.
        Chunk_t* NewChunk(const ChunkStructure_t* const chunkStructure, const NodeCountT<Size_t> nodeCount)
        {
            Chunk_t* const chunk = ni_new(Chunk_t)(chunkStructure, nodeCount);
            Chunks.insert(chunk);
            return chunk;
        }

        // Add a chunk with a given chunk structure and capacity.
        ChunkArray_t* NewChunkArray(const ChunkStructure_t* const chunkStructure, const ChunkCountT<Size_t> chunkCount, const NodeCountPerChunkT<Size_t> nodeCountPerChunk)
        {
            ChunkArray_t* const chunk = ni_new(ChunkArray_t)(chunkStructure, chunkCount, nodeCountPerChunk);
            ChunkArrays.insert(chunk);
            return chunk;
        }

        bool DeleteChunk(Chunk_t* const chunk)
        {
            if (Chunks.erase(chunk) > 0)
            {
                ni_delete_dirty(chunk);
                return true;
            }
            return false;
        }

        bool DeleteChunk(ChunkArray_t* const chunk)
        {
            if (ChunkArrays.erase(chunk) > 0)
            {
                ni_delete_dirty(chunk);
                return true;
            }
            return false;
        }

    };

    template<typename TChunkStructure>
    struct PncRegistryT
    {
    public:
        using Self_t = PncRegistryT<TChunkStructure>;
        using ChunkStructure_t = TChunkStructure;

        using Size_t = typename ChunkStructure_t::Size_t;
        using ComponentType_t = typename ChunkStructure_t::ComponentType_t;
        using ComponentTypeSet_t = typename ChunkStructure_t::ComponentTypeSet_t;

        using ChunkPointer_t = ChunkPointerT<ChunkStructure_t>;
        using ChunkArrayPointer_t = ArrayPointerT<ChunkStructure_t, ChunkPointer_t>;

        using KChunkPointer_t = KChunkPointerT<ChunkStructure_t>;
        using KChunkArrayPointer_t = KArrayPointerT<ChunkStructure_t, ChunkPointer_t>;

        using KChunkTreePointer_t = KChunkTreePointerT<ChunkStructure_t>;
        using KChunkArrayTreePointer_t = KArrayTreePointerT<ChunkStructure_t, ChunkPointer_t>;


        using Chunk_t = BunchT<ChunkStructure_t>;
        using ChunkArray_t = ArrayT<ChunkStructure_t, ChunkPointer_t>;

        using KChunk_t = KBunchT<ChunkStructure_t>;
        using KChunkArray_t = KArrayT<ChunkStructure_t, ChunkPointer_t>;

        using KChunkTree_t = KBunchTreeT<ChunkStructure_t>;
        using KChunkArrayTree_t = KArrayTreeT<ChunkStructure_t, ChunkPointer_t>;

        using ComponentTypeRegistry_t = ComponentTypeRegistryT<ComponentType_t>;
        using ChunkStructureRegistry_t = ChunkStructureRegistryT<ChunkStructure_t>;
        using ChunkRegistry_t = ChunkRegistryT<Chunk_t, ChunkArray_t>;
        using KChunkRegistry_t = ChunkRegistryT<KChunk_t, KChunkArray_t>;
        using KChunkTreeRegistry_t = ChunkRegistryT<KChunkTree_t, KChunkArrayTree_t>;




    public:
        ComponentTypeRegistry_t ComponentTypeRegistry;
        ChunkStructureRegistry_t ChunkStructureRegistry;
        ChunkRegistry_t ChunkRegistry;
        KChunkRegistry_t KChunkRegistry;
        KChunkTreeRegistry_t KChunkTreeRegistry;

    public:
        // Add a component type we can use for our centipede chunks
        template<typename T>
        const ComponentType_t* GetOrAddComponentType() { return ComponentTypeRegistry.GetOrAddComponentType<T>(); }

        const ChunkStructure_t* GetOrAddChunkStructure(const ComponentType_t* const component)
        {
            return ChunkStructureRegistry.GetOrAddChunkStructure(component);
        }

        // Add a chunk structure from a list of component type
        const ChunkStructure_t* GetOrAddChunkStructure(const std::initializer_list<const ComponentType_t*>& aComponents)
        {
            return ChunkStructureRegistry.GetOrAddChunkStructure(aComponents);
        }

        // Add a chunk structure from a parameter pack of component type
        template<typename... TComponentTypes>
        const ChunkStructure_t* GetOrAddChunkStructure(TComponentTypes... args)
        {
            return ChunkStructureRegistry.GetOrAddChunkStructure({ args... });
        }

        template<typename... TComponentTypes>
        const ChunkStructure_t* GetOrAddChunkStructure()
        {
            return ChunkStructureRegistry.GetOrAddChunkStructure<TComponentTypes...>(ComponentTypeRegistry);
        }

        Chunk_t* NewChunk(const ChunkStructure_t* const chunkStructure, const NodeCountT<Size_t> nodeCount)
        {
            return ChunkRegistry.NewChunk(chunkStructure, nodeCount);
        }

        ChunkArray_t* NewChunkArray(const ChunkStructure_t* const chunkStructure, const ChunkCountT<Size_t> chunkCount, const NodeCountPerChunkT<Size_t> nodeCountPerChunk)
        {
            return ChunkRegistry.NewChunkArray(chunkStructure, chunkCount, nodeCountPerChunk);
        }

        KChunk_t* AddKChunk(const ChunkStructure_t* const chunkStructure, const NodeCountT<Size_t> nodeCount)
        {
            return KChunkRegistry.NewChunk(chunkStructure, nodeCount);
        }

        KChunkArray_t* AddKChunkArray(const ChunkStructure_t* const chunkStructure, const ChunkCountT<Size_t> chunkCount, const NodeCountPerChunkT<Size_t> nodeCountPerChunk)
        {
            return KChunkRegistry.NewChunkArray(chunkStructure, chunkCount, nodeCountPerChunk);
        }

        KChunkTree_t* AddKChunkTree(const ChunkStructure_t* const chunkStructure, const NodeCountT<Size_t> nodeCount)
        {
            return KChunkTreeRegistry.NewChunk(chunkStructure, nodeCount);
        }

        KChunkArrayTree_t* AddKChunkArrayTree(const ChunkStructure_t* const chunkStructure, const ChunkCountT<Size_t> chunkCount, const NodeCountPerChunkT<Size_t> nodeCountPerChunk)
        {
            return KChunkTreeRegistry.NewChunkArray(chunkStructure, chunkCount, nodeCountPerChunk);
        }

        bool DeleteChunk(Chunk_t* const chunk) { return ChunkRegistry.DeleteChunk(chunk); }
        bool DeleteChunk(ChunkArray_t* const chunk) { return ChunkRegistry.DeleteChunk(chunk); }
        bool DeleteChunk(KChunk_t* const chunk) { return KChunkRegistry.DeleteChunk(chunk); }
        bool DeleteChunk(KChunkArray_t* const chunk) { return KChunkRegistry.DeleteChunk(chunk); }
        bool DeleteChunk(KChunkTree_t* const chunk) { return KChunkTreeRegistry.DeleteChunk(chunk); }
        bool DeleteChunk(KChunkArrayTree_t* const chunk) { return KChunkTreeRegistry.DeleteChunk(chunk); }
    };

}
namespace Ni
{
    using ComponentTypeRegistry = NiT::ComponentTypeRegistryT<ComponentType>;
    using ChunkStructureRegistry = NiT::ChunkStructureRegistryT<ChunkStructure>;
    using ChunkTreeRegistry = NiT::ChunkRegistryT<KChunkTree, KArrayTree>;
    using PncRegistry = NiT::PncRegistryT<ChunkStructure>;
}
