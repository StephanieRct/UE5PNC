// MIT License
// Copyright (c) 2025 Stephanie Rancourt
// 
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
// 
// The above copyright notice and this permission notice shall be included in all
// copies or substantial portions of the Software.
// 
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
// SOFTWARE.

#pragma once
#include "common.h"
#include "ComponentType.h"
#include "ComponentTypeSet.h"
#include "ChunkStructure.h"
#include "ChunkPointer.h"
#include "ChunkCapacityAllocation.h"
#include "ChunkArrayPointer.h"
#include "ChunkArrayCapacityAllocation.h"
#include "KindPointer.h"
#include "KChunkArrayPointer.h"
#include "Algorithm.h"
#include "Pipeline.h"
#include "Components.h"
#include "routing\AlgorithmRouter.h"
#include "routing\AlgorithmCacheRouter.h"
#include "BucketChunkPointer.h"
#include "BucketChunkArrayPointer.h"

#include "KindPointer.inl.h"

namespace PNC
{
    //template<typename TChunkStructure>
    //using BucketChunk = ChunkCapacityAllocationT<BucketChunkPointerT<ChunkPointerT<TChunkStructure>>>;

    //template<typename TChunkStructure, typename TChunkPointerElement>
    //using BucketChunkArray = ChunkArrayCapacityAllocationT<BucketChunkArrayPointerT<ChunkArrayPointerT<ChunkStructure, TChunkPointerElement>>>;

    template<typename TChunkStructure>
    using ChunkT = ChunkCapacityAllocationT<ChunkPointerT<TChunkStructure>>;
    template<typename TChunkStructure, typename TAlgorithm>
    struct AlgorithmRunner< TChunkStructure, TAlgorithm, ChunkT<TChunkStructure>> : public AlgorithmRunnerChunk<TAlgorithm, ChunkPointerT<TChunkStructure>> {};

    template<typename TChunkStructure>
    using BucketT = ChunkCapacityAllocationT<BucketChunkPointerT<ChunkPointerT<TChunkStructure>>>;
    template<typename TChunkStructure, typename TAlgorithm>
    struct AlgorithmRunner< TChunkStructure, TAlgorithm, BucketT<TChunkStructure>> : public AlgorithmRunnerChunk<TAlgorithm, ChunkPointerT<TChunkStructure>> {};


    using Size_t = int32;
    using ComponentType = ComponentTypeT<Size_t>;
    using ComponentTypeSet = ComponentTypeSetT<Size_t>;
    using ChunkStructure = ChunkStructureT<Size_t>;

    using ChunkPointer = ChunkPointerT<ChunkStructure>;
    using Chunk = ChunkT<ChunkStructure>;
    using Bucket = BucketT<ChunkStructure>;






    template<typename TChunkStructure, typename TChunkPointerElement>
    using ChunkArrayT = ChunkArrayCapacityAllocationT<BucketChunkArrayPointerT<ChunkArrayPointerT<TChunkStructure, TChunkPointerElement>>>;

    template<typename TChunkStructure>
    using KChunkT = ChunkCapacityAllocationT<BucketChunkPointerT<KChunkPointerT<TChunkStructure>>>;

    template<typename TChunkStructure, typename TChunkPointerElement>
    using KChunkArrayT = ChunkArrayCapacityAllocationT<BucketChunkArrayPointerT<KChunkArrayPointerT<TChunkStructure, ChunkPointerT<TChunkStructure>>>>;

    template<typename TChunkStructure>
    using KChunkTreeT = ChunkCapacityAllocationT<BucketChunkPointerT<KChunkTreePointerT<TChunkStructure>>>;

    template<typename TChunkStructure, typename TChunkPointerElement>
    using KChunkArrayTreeT = ChunkArrayCapacityAllocationT<BucketChunkArrayPointerT<KChunkArrayTreePointerT<TChunkStructure, ChunkPointerT<TChunkStructure>>>>;





    using ChunkArrayPointer = ChunkArrayPointerT<ChunkStructure, ChunkPointer>;

    using KChunkPointer = KChunkPointerT<ChunkStructure>;
    using KChunkArrayPointer = KChunkArrayPointerT<ChunkStructure, ChunkPointer>;

    using KChunkTreePointer = KChunkTreePointerT<ChunkStructure>;
    using KChunkArrayTreePointer = KChunkArrayTreePointerT<ChunkStructure, ChunkPointer>;


    using ChunkArray = ChunkArrayT<ChunkStructure, ChunkPointer>;

    using KChunk = KChunkT<ChunkStructure>;
    using KChunkArray = KChunkArrayT<ChunkStructure, ChunkPointer>;

    using KChunkTree = KChunkTreeT<ChunkStructure>;
    using KChunkArrayTree = KChunkArrayTreeT<ChunkStructure, ChunkPointer>;




    template<typename TAlgorithm>
    using AlgorithmRouter = Routing::AlgorithmRouterT<TAlgorithm>;
    template<typename TAlgorithm>
    using AlgorithmCacheRouter = Routing::AlgorithmCacheRouterT<TAlgorithm, ChunkStructure, Size_t>;

    template<typename TPipeline>
    using Pipeline = PipelineT<TPipeline, ChunkStructure, Size_t>;

    using CoParentInChunk = CoParentInChunkT<Size_t>;
    using CoSingleParentOutsideChunk = CoSingleParentOutsideChunkT<Size_t>;
    using CoChildrenInChunk = CoChildrenInChunkT<Size_t>;
    //


    template<typename TComponentType>
    struct ComponentTypeRegistryT
    {
    public:
        using Self_t = ComponentTypeRegistryT<TComponentType>;
        using ComponentType_t = TComponentType;

    public:
        Vector<Unique_Ptr<ComponentType_t>> ComponentTypes;
        HashMap<const type_info*, Size_t> TypeToComponentTypeIndex;

    public:
        template<typename T>
        const ComponentType_t* AddComponentType()
        {
            
            auto componentType = Make_Unique<ComponentType_t>((T*)nullptr, T::Owner);
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

    public:
        // Keep an array of all our chunk structures
        Vector<Unique_Ptr<ChunkStructure_t>> ChunkStructures;
        HashMap< const ChunkStructure_t*, std::size_t, ChunkStructureHasher_t, ChunkStructureEqualler_t> ChunkStructureToIndex;

        const ChunkStructure_t* GetOrAddChunkStructure(const ComponentType_t* component)
        {
            auto chunkStructure = Make_Unique<ChunkStructure_t>(component);
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
            auto chunkStructure = Make_Unique<ChunkStructure_t>(aComponents);
            auto ptr = chunkStructure.get();
            auto i = ChunkStructureToIndex.find(ptr);
            if (i != ChunkStructureToIndex.end())
                return i->first;
            ChunkStructures.push_back(std::move(chunkStructure));
            ChunkStructureToIndex.insert(i, { ptr, ChunkStructures.size() - 1 });
            return ptr;
        }


        template<typename... TComponentTypes>
        const ChunkStructure* GetOrAddChunkStructure(ComponentTypeRegistry_t& componentTypeRegistry)
        {
            Vector<const ComponentType_t*> componentTypes(sizeof...(TComponentTypes));
            Size_t c = 0;
            ((componentTypes[c++] = componentTypeRegistry.GetOrAddComponentType<TComponentTypes>()), ...);

            auto chunkStructure = Make_Unique<ChunkStructure_t>(std::move(componentTypes));
            auto ptr = chunkStructure.get();
            auto i = ChunkStructureToIndex.find(ptr);
            if (i != ChunkStructureToIndex.end())
                return i->first;
            ChunkStructures.push_back(std::move(chunkStructure));
            ChunkStructureToIndex.insert(i, { ptr, ChunkStructures.size() - 1 });
            return ptr;
        }

    };

    //template<typename TChunk>
    //struct ChunkRegistryT
    //{
    //public:
    //    using Self_t = ChunkRegistryT<TChunkStructure>;
    //    using Chunk_t = TChunk;
    //    using ChunkStructure_t = typename Chunk_t::ChunkStructure_t;

    //public:
    //    std::list<std::unique_ptr<Chunk_t>> Chunks;

    //    // Add a chunk with a given chunk structure and capacity.
    //    KChunkTree& Add(const ChunkStructure_t* chunkStructure, const std::initializer_list<const ComponentType_t*>& args)
    //    {
    //        auto* chunkStructure = std::make_unique<Chunk_t>(aComponents);
    //        return *Chunks[Chunks.Add(MakeUnique<KChunkTree>(chunkStructure, capacity, size))].Get();
    //    }

    //    // Add a chunk with a given chunk structure and capacity.
    //    KChunkArrayTree& NewChunkArray(const ChunkStructure_t* chunkStructure, size_t nodeCapacityPerChunk, size_t chunkCapacity, size_t chunkCount = 0, size_t nodeCountPerChunk = 0)
    //    {
    //        return *ChunkArrays[ChunkArrays.Add(MakeUnique<KChunkArrayTree>(chunkStructure, nodeCapacityPerChunk, chunkCapacity, chunkCount, nodeCountPerChunk))].Get();
    //    }
    //};
    
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
        HashSet<Chunk_t*> Chunks;
        HashSet<ChunkArray_t*> ChunkArrays;

    public:

        ChunkRegistryT() = default;

        // TODO: Make copyable
        ChunkRegistryT(const ChunkRegistryT& other) = delete;
        ChunkRegistryT& operator=(const ChunkRegistryT& other) = delete;

        ChunkRegistryT(ChunkRegistryT&& other) = default;
        ChunkRegistryT& operator=(ChunkRegistryT&& other) = default;

        ~ChunkRegistryT()
        {
            for(Chunk_t* chunk : Chunks)
                pnc_delete_dirty(chunk);
            for (ChunkArray_t* chunk : ChunkArrays)
                pnc_delete_dirty(chunk);
        }


    public:
        // Add a chunk with a given chunk structure and capacity.
        Chunk_t* NewChunk(const ChunkStructure_t* const  chunkStructure, const Size_t capacity, const Size_t size = 0)
        {
            Chunk_t*const chunk = pnc_new(Chunk_t)(chunkStructure, capacity, size);
            Chunks.insert(chunk);
            return chunk;
        }

        // Add a chunk with a given chunk structure and capacity.
        ChunkArray_t* NewChunkArray(const ChunkStructure_t* const  chunkStructure, const Size_t nodeCapacityPerChunk, const Size_t chunkCapacity, const Size_t chunkCount = 0, const Size_t nodeCountPerChunk = 0)
        {
            ChunkArray_t* const chunk = pnc_new(ChunkArray_t)(chunkStructure, nodeCapacityPerChunk, chunkCapacity, chunkCount, nodeCountPerChunk);
            ChunkArrays.insert(chunk);
            return chunk;
        }

        bool DeleteChunk(Chunk_t* const chunk)
        {
            if (Chunks.erase(chunk) > 0)
            {
                pnc_delete_dirty(chunk);
                return true;
            }
            return false;
        }

        bool DeleteChunk(ChunkArray_t* const chunk)
        {
            if (ChunkArrays.erase(chunk) > 0)
            {
                pnc_delete_dirty(chunk);
                return true;
            }
            return false;
        }

    };



    using ComponentTypeRegistry = ComponentTypeRegistryT<ComponentType>;
    using ChunkStructureRegistry = ChunkStructureRegistryT<ChunkStructure>;
    using ChunkTreeRegistry = ChunkRegistryT<KChunkTree, KChunkArrayTree>;


#pragma region Hash
    //using Hash_t = std::size_t;
    //struct CoHash : public NodeComponent
    //{
    //public:
    //    Hash_t Value;
    //};

    //template<typename TChunkStructure, typename TComponent, typename THasher = std::hash<TComponent>>
    //struct HashChunkPointerT : public ChunkPointerT<TChunkStructure>
    //{
    //public:
    //    using Base_t = ChunkPointerT<TChunkStructure>;
    //    using Self_t = HashChunkPointerT<TChunkStructure, TComponent, THasher>;
    //    using ChunkStructure_t = TChunkStructure;
    //    using Component_t = TComponent;
    //    using Hasher_t = THasher;
    //    using Size_t = typename ChunkStructure_t::Size_t;

    //public:

    //    HashChunkPointerT()
    //        : Base_t()
    //    {
    //    }
    //    HashChunkPointerT(const ChunkStructure_t* chunkStructure, Size_t nodeCount, void** componentData)
    //        : Base_t(chunkStructure, nodeCount, componentData)
    //    {
    //    }

    //    Size_t Add(const Component_t& value)
    //    {
    //        auto hash = Hasher_t()(value);
    //        Size_t index = hash % this->GetNodeCount();
    //    }

    //protected:
    //    HashChunkPointerT(const ChunkStructure_t* chunkStructure, Size_t nodeCount)
    //        : Base_t(chunkStructure, nodeCount)
    //    {
    //    }

    //    void InitHashComponent() 
    //    {
    //        CoHash* hash = this->GetComponentData<CoHash>();
    //        for (Size_t i = 0; i < this->GetNodeCount(); ++i)
    //        {
    //            hash[0]
    //        }
    //    }
    //};
#pragma endregion
#pragma region ComponentChunkPointer
    //template<typename TChunkStructure, typename TComponent>
    //struct ComponentChunkPointerT : protected ChunkPointerT<TChunkStructure>
    //{
    //public:
    //    using Base_t = ChunkPointerT<TChunkStructure>;
    //    using Self_t = ComponentChunkPointerT<TChunkStructure, TComponent>;
    //    using ChunkStructure_t = TChunkStructure;
    //    using Component_t = TComponent;
    //    using Size_t = typename ChunkStructure_t::Size_t;

    //protected:
    //    TComponent* Component;

    //public:
    //    ComponentChunkPointerT()
    //        : Base_t()
    //        , Component(nullptr)
    //    {
    //    }

    //    ComponentChunkPointerT(const ChunkStructure_t* const chunkStructure, const Size_t nodeCount, void const** componentData, TComponent const* component)
    //        : Base_t(chunkStructure, nodeCount, componentData)
    //        , Component(component)
    //    {
    //    }

    //protected:
    //    ComponentChunkPointerT(const ChunkStructure_t* const chunkStructure, const Size_t nodeCount)
    //        : Base_t(chunkStructure, nodeCount)
    //        , Component(nullptr)
    //    {
    //    }

    //public:
    //    const Component_t& operator[](const Size_t nodeIndex)const
    //    {
    //        pnc_assert(nodeIndex >= 0);
    //        pnc_assert(nodeIndex < GetNodeCount());
    //        return Component[nodeIndex];
    //    }

    //    Component_t& operator[](const Size_t nodeIndex)
    //    {
    //        pnc_assert(nodeIndex >= 0);
    //        pnc_assert(nodeIndex < GetNodeCount());
    //        return Component[nodeIndex];
    //    }

    //    const Component_t& operator*()const
    //    {
    //        pnc_assert(GetNodeCount() > 0);
    //        return *Component;
    //    }

    //    Component_t& operator*()
    //    {
    //        pnc_assert(GetNodeCount() > 0);
    //        return *Component;
    //    }

    //    const Component_t* operator->()const
    //    {
    //        pnc_assert(GetNodeCount() > 0);
    //        return Component;
    //    }

    //    Component_t* operator->()
    //    {
    //        pnc_assert(GetNodeCount() > 0);
    //        return Component;
    //    }
    //};
    //template<typename TComponent>
    //using ComponentChunkPointer = ComponentChunkPointerT<ChunkStructure, TComponent>;
#pragma endregion




    template<typename TChunkStructure, typename TComponent>
    struct ComponentPointerT
    {
    public:
        using Self_t = ComponentPointerT<TChunkStructure, TComponent>;
        using ChunkStructure_t = TChunkStructure;
        using Component_t = TComponent;
        using Size_t = typename ChunkStructure_t::Size_t;
        using Chunk_t = ChunkPointerT<ChunkStructure_t>;

    protected:
        Chunk_t* Chunk;
        TComponent* Component;

    public:
        ComponentPointerT()
            : Chunk(nullptr)
            , Component(nullptr)
        {
        }

        ComponentPointerT(Chunk_t* const chunk)
            : Chunk(chunk)
            , Component(nullptr)
        {
            Update();
        }

        ComponentPointerT(Chunk_t* const chunk, Component_t* const component)
            : Chunk(chunk)
            , Component(component)
        {
            if (chunk)
            {
                pnc_assert(component);
            }
            else 
            {
                pnc_assert(!component);
            }
            Update();
        }

    public:
        // Must be called before using it if the chunk's component memory moved
        void Update()
        {
            if (!Chunk) return;
            Component = Chunk->GetComponentData<Component_t>();
            pnc_assert(Component);
        }
        Chunk_t* GetChunk()const { return Chunk; }
        Component_t& operator[](const Size_t nodeIndex)const
        {
            pnc_assert(nodeIndex >= 0);
            pnc_assert(nodeIndex < Chunk->GetNodeCount());
            return Component[nodeIndex];
        }

        Component_t& operator*()const
        {
            pnc_assert(Chunk->GetNodeCount() > 0);
            return *Component;
        }

        Component_t* operator->()const
        {
            pnc_assert(Chunk->GetNodeCount() > 0);
            return Component;
        }

    };

    template<typename TComponent>
    using ComponentPointer = ComponentPointerT<ChunkStructure, TComponent>;

    // TODO
    template<typename TComponent>
    using ComponentPointerRO = ComponentPointerT<ChunkStructure, TComponent>;





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
        using ChunkArrayPointer_t = ChunkArrayPointerT<ChunkStructure_t, ChunkPointer_t>;

        using KChunkPointer_t = KChunkPointerT<ChunkStructure_t>;
        using KChunkArrayPointer_t = KChunkArrayPointerT<ChunkStructure_t, ChunkPointer_t>;

        using KChunkTreePointer_t = KChunkTreePointerT<ChunkStructure_t>;
        using KChunkArrayTreePointer_t = KChunkArrayTreePointerT<ChunkStructure_t, ChunkPointer_t>;


        using Chunk_t = ChunkT<ChunkStructure_t>;
        using ChunkArray_t = ChunkArrayT<ChunkStructure_t, ChunkPointer_t>;

        using KChunk_t = KChunkT<ChunkStructure_t>;
        using KChunkArray_t = KChunkArrayT<ChunkStructure_t, ChunkPointer_t>;

        using KChunkTree_t = KChunkTreeT<ChunkStructure_t>;
        using KChunkArrayTree_t = KChunkArrayTreeT<ChunkStructure_t, ChunkPointer_t>;

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

        Chunk_t* NewChunk(const ChunkStructure_t* const chunkStructure, const Size_t nodeCapacity, const Size_t nodeCount = 0)
        {
            return ChunkRegistry.NewChunk(chunkStructure, nodeCapacity, nodeCount);
        }

        ChunkArray_t* NewChunkArray(const ChunkStructure_t* const chunkStructure, const Size_t nodeCapacityPerChunk, const Size_t chunkCapacity, const Size_t chunkCount = 0, const Size_t nodeCountPerChunk = 0)
        {
            return ChunkRegistry.NewChunkArray(chunkStructure, nodeCapacityPerChunk, chunkCapacity, chunkCount, nodeCountPerChunk);
        }

        KChunk_t* AddKChunk(const ChunkStructure_t* const chunkStructure, const Size_t nodeCapacity, const Size_t nodeCount = 0)
        {
            return KChunkRegistry.NewChunk(chunkStructure, nodeCapacity, nodeCount);
        }

        KChunkArray_t* AddKChunkArray(const ChunkStructure_t* const chunkStructure, const Size_t nodeCapacityPerChunk, const Size_t chunkCapacity, const Size_t chunkCount = 0, const Size_t nodeCountPerChunk = 0)
        {
            return KChunkRegistry.NewChunkArray(chunkStructure, nodeCapacityPerChunk, chunkCapacity, chunkCount, nodeCountPerChunk);
        }

        KChunkTree_t* AddKChunkTree(const ChunkStructure_t* const chunkStructure, const Size_t nodeCapacity, const Size_t nodeCount = 0)
        {
            return KChunkTreeRegistry.NewChunk(chunkStructure, nodeCapacity, nodeCount);
        }

        KChunkArrayTree_t* AddKChunkArrayTree(const ChunkStructure_t* const chunkStructure, const Size_t nodeCapacityPerChunk, const Size_t chunkCapacity, const Size_t chunkCount = 0, const Size_t nodeCountPerChunk = 0)
        {
            return KChunkTreeRegistry.NewChunkArray(chunkStructure, nodeCapacityPerChunk, chunkCapacity, chunkCount, nodeCountPerChunk);
        }

        bool DeleteChunk(Chunk_t* const chunk) { return ChunkRegistry.DeleteChunk(chunk); }
        bool DeleteChunk(ChunkArray_t* const chunk) { return ChunkRegistry.DeleteChunk(chunk); }
        bool DeleteChunk(KChunk_t* const chunk) { return KChunkRegistry.DeleteChunk(chunk); }
        bool DeleteChunk(KChunkArray_t* const chunk) { return KChunkRegistry.DeleteChunk(chunk); }
        bool DeleteChunk(KChunkTree_t* const chunk) { return KChunkTreeRegistry.DeleteChunk(chunk); }
        bool DeleteChunk(KChunkArrayTree_t* const chunk) { return KChunkTreeRegistry.DeleteChunk(chunk); }
    };

    using PncRegistry = PncRegistryT<ChunkStructure>;
}



#pragma region old code
//template<typename T, std::size_t tChunkSize = 16>
//struct FreeList
//{
//public:
//    union Node 
//    {
//        Node* NextFree;
//        T Value;
//    };
//    std::vector<Node*> Chunks;
//    Node* NextFree = nullptr;

//public:
//    FreeList() = default;

//    // TODO: Make copyable
//    FreeList(const FreeList& other) = delete;
//    FreeList& operator=(const FreeList& other) = delete;


//    FreeList(const FreeList&& other)
//        : Chunks(std::move(other.Chunks))
//        , NextFree(other.NextFree)
//    {
//        other.NextFree = nullptr;
//    }

//    FreeList& operator=(FreeList&& other)
//    {
//        Chunks = std::move(other.Chunks);
//        NextFree = other.NextFree;
//        other.NextFree = nullptr;
//        return *this;
//    }

//    ~FreeList()
//    {
//        for (Node* node in Chunks)
//            pnc_free_clean(node, nodeSize * tChunkSize, nodeAlignment);
//    }
//public:
//    void* AcquireMemory()
//    {
//        if (!NextFree)
//            NewChunk();
//        Node* node = NextFree;
//        NextFree = node->NextFree;
//        return node;
//    }

//    void ReleaseMemory(const T* const element)
//    {
//        checkSlow(Owns(element));
//        if (!element)
//            return;
//        Node* node = (Node*)element;
//        node->NextFree = NextFree;
//        NextFree = node;
//    }


//    void ShrinkToFitTODO()
//    {
//        // TODO remove empty chunks.
//    }

//    bool Owns(const T* const element)const
//    {
//        for (Node* node : Chunks)
//            if (element >= node && element < (node + tChunkSize))
//                return true;
//        return false;
//    }
//protected:
//    void NewChunk()
//    {
//        std::size_t nodeSize = std::max(sizeof(T), sizeof(Node*));
//        std::size_t nodeAlignment = std::max(alignof(T), alignof(Node*));
//        Node* chunk = (Node*) pnc_alloc(nodeSize * tChunkSize, nodeAlignment);
//        std::size_t i = 0
//        while(i < tChunkSize-1)
//            chunk[i].NextFree = &chunk[++i];
//        chunk[i].NextFree = NextFree;
//        Chunks.push_back(chunk);
//    }
//};
#pragma endregion