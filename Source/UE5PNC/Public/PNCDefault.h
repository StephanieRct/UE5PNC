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
#include "BucketChunk.h"
#include "BucketChunkArray.h"

#include "KindPointer.inl.h"

namespace PNC
{
    //template<typename TChunkStructure>
    //using BucketChunk = ChunkCapacityAllocationT<BucketChunkT<ChunkPointerT<TChunkStructure>>>;

    //template<typename TChunkStructure, typename TChunkPointerElement>
    //using BucketChunkArray = ChunkArrayCapacityAllocationT<BucketEvenChunkArrayT<ChunkArrayPointerT<ChunkStructure, TChunkPointerElement>>>;


    using Size_t = int32;
    using ComponentType = ComponentTypeT<Size_t>;
    using ComponentTypeSet = ComponentTypeSetT<Size_t>;
    using ChunkStructure = ChunkStructureT<Size_t>;

    using ChunkPointer = ChunkPointerT<ChunkStructure>;
    using Chunk = ChunkCapacityAllocationT<BucketChunkT<ChunkPointer>>;

    using ChunkArrayPointer = ChunkArrayPointerT<ChunkStructure, ChunkPointer>;
    using ChunkArray = ChunkArrayCapacityAllocationT<BucketEvenChunkArrayT<ChunkArrayPointer>>;

    using KChunkTreePointer = KChunkTreePointerT<ChunkStructure>;
    using KChunkTree = ChunkCapacityAllocationT<BucketChunkT<KChunkTreePointer>>;

    using KChunkArrayTreePointer = KChunkArrayTreePointerT<ChunkStructure, ChunkPointer>;
    using KChunkArrayTree = ChunkArrayCapacityAllocationT<BucketEvenChunkArrayT<KChunkArrayTreePointer>>;

    template<typename TAlgorithm>
    using AlgorithmRouter = Routing::AlgorithmRouterT<TAlgorithm>;
    template<typename TAlgorithm>
    using AlgorithmCacheRouter = Routing::AlgorithmCacheRouterT<TAlgorithm, ChunkStructure, Size_t>;

    template<typename TPipeline>
    using Pipeline = PipelineT<TPipeline, ChunkStructure, Size_t>;

    using CoParentInChunk = CoParentInChunkT<Size_t>;
    using CoSingleParentOutsideChunk = CoSingleParentOutsideChunkT<Size_t>;
    using CoChildrenInChunk = CoChildrenInChunkT<Size_t>;
    

    template<typename TComponentType>
    struct ComponentTypeRegistryT
    {
    public:
        using Self_t = ComponentTypeRegistryT<TComponentType>;
        using ComponentType_t = TComponentType;

    public:
        std::vector<std::unique_ptr<ComponentType_t>> ComponentTypes;
        std::unordered_map<const type_info*, Size_t> TypeToComponentTypeIndex;

    public:
        template<typename T>
        const ComponentType_t* AddComponentType()
        {
            auto componentType = std::make_unique<ComponentType_t>((T*)nullptr, T::Owner);
            auto ptr = componentType.get();
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
        using ChunkStructureEqualler_t = typename ChunkStructure_t::Equaller_t;
        using ComponentTypeRegistry_t = ComponentTypeRegistryT<ComponentType_t>;

    public:
        // Keep an array of all our chunk structures
        std::vector<std::unique_ptr<ChunkStructure_t>> ChunkStructures;
        std::unordered_map< const ChunkStructure_t*, std::size_t, ChunkStructureHasher_t, ChunkStructureEqualler_t> ChunkStructureToIndex;

        const ChunkStructure_t* GetOrAddChunkStructure(const ComponentType_t* component)
        {
            auto chunkStructure = std::make_unique<ChunkStructure_t>(component);
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
            auto chunkStructure = std::make_unique<ChunkStructure_t>(aComponents);
            auto ptr = chunkStructure.get();
            auto i = ChunkStructureToIndex.find(ptr);
            if (i != ChunkStructureToIndex.end())
                return i->first;
            ChunkStructures.push_back(std::move(chunkStructure));
            ChunkStructureToIndex.insert(i, { ptr, ChunkStructures.size() - 1 });
            return ptr;
        }


        template<typename... TComponentTypes>
        const PNC::ChunkStructure* GetOrAddChunkStructure(ComponentTypeRegistry_t& componentTypeRegistry)
        {
            std::vector<const ComponentType_t*> componentTypes(sizeof...(TComponentTypes));
            Size_t c = 0;
            ([&]()
            {
                componentTypes[c++] = componentTypeRegistry.GetOrAddComponentType<TComponentTypes>();
            } (), ...);
            auto chunkStructure = std::make_unique<ChunkStructure_t>(std::move(componentTypes));
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
    //    PNC::KChunkTree& Add(const ChunkStructure_t* chunkStructure, const std::initializer_list<const ComponentType_t*>& args)
    //    {
    //        auto* chunkStructure = std::make_unique<Chunk_t>(aComponents);
    //        return *Chunks[Chunks.Add(MakeUnique<PNC::KChunkTree>(chunkStructure, capacity, size))].Get();
    //    }

    //    // Add a chunk with a given chunk structure and capacity.
    //    PNC::KChunkArrayTree& AddChunkArray(const ChunkStructure_t* chunkStructure, size_t nodeCapacityPerChunk, size_t chunkCapacity, size_t chunkCount = 0, size_t nodeCountPerChunk = 0)
    //    {
    //        return *ChunkArrays[ChunkArrays.Add(MakeUnique<PNC::KChunkArrayTree>(chunkStructure, nodeCapacityPerChunk, chunkCapacity, chunkCount, nodeCountPerChunk))].Get();
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

    public:
        std::list<std::unique_ptr<Chunk_t>> Chunk;
        std::list<std::unique_ptr<ChunkArray_t>> ChunkArray;

        // Add a chunk with a given chunk structure and capacity.
        Chunk_t* AddChunk(const ChunkStructure_t* chunkStructure, size_t capacity, size_t size = 0)
        {
            auto chunk = std::make_unique<Chunk_t>(chunkStructure, capacity, size);
            auto ptr = chunk.get();
            Chunk.push_back(std::move(chunk));
            return ptr;
        }

        // Add a chunk with a given chunk structure and capacity.
        ChunkArray_t* AddChunkArray(const ChunkStructure_t* chunkStructure, size_t nodeCapacityPerChunk, size_t chunkCapacity, size_t chunkCount = 0, size_t nodeCountPerChunk = 0)
        {
            auto chunk = std::make_unique<ChunkArray_t>(chunkStructure, nodeCapacityPerChunk, chunkCapacity, chunkCount, nodeCountPerChunk);
            auto ptr = chunk.get();
            ChunkArray.push_back(std::move(chunk));
            return ptr;
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
    //        assert_pnc(nodeIndex >= 0);
    //        assert_pnc(nodeIndex < GetNodeCount());
    //        return Component[nodeIndex];
    //    }

    //    Component_t& operator[](const Size_t nodeIndex)
    //    {
    //        assert_pnc(nodeIndex >= 0);
    //        assert_pnc(nodeIndex < GetNodeCount());
    //        return Component[nodeIndex];
    //    }

    //    const Component_t& operator*()const
    //    {
    //        assert_pnc(GetNodeCount() > 0);
    //        return *Component;
    //    }

    //    Component_t& operator*()
    //    {
    //        assert_pnc(GetNodeCount() > 0);
    //        return *Component;
    //    }

    //    const Component_t* operator->()const
    //    {
    //        assert_pnc(GetNodeCount() > 0);
    //        return Component;
    //    }

    //    Component_t* operator->()
    //    {
    //        assert_pnc(GetNodeCount() > 0);
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
                assert_pnc(component);
            }
            else 
            {
                assert_pnc(!component);
            }
            Update();
        }

    public:
        // Must be called before using it if the chunk's component memory moved
        void Update()
        {
            if (!Chunk) return;
            Component = Chunk->GetComponentData<Component_t>();
            assert_pnc(Component);
        }
        Chunk_t* GetChunk()const { return Chunk; }
        Component_t& operator[](const Size_t nodeIndex)const
        {
            assert_pnc(nodeIndex >= 0);
            assert_pnc(nodeIndex < Chunk->GetNodeCount());
            return Component[nodeIndex];
        }

        Component_t& operator*()const
        {
            assert_pnc(Chunk->GetNodeCount() > 0);
            return *Component;
        }

        Component_t* operator->()const
        {
            assert_pnc(Chunk->GetNodeCount() > 0);
            return Component;
        }

    };

    template<typename TComponent>
    using ComponentPointer = ComponentPointerT<ChunkStructure, TComponent>;

    // TODO
    template<typename TComponent>
    using ComponentPointerRO = ComponentPointerT<ChunkStructure, TComponent>;
}