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
#include "Containers.h"
#include "KContainers.h"
#include "ContainersAlgorithmRunner.h"
#include "KContainersAlgorithmRunner.h"
#include "Pipeline.h"
#include "Components.h"
#include "routing\AlgorithmRouter.h"
#include "routing\AlgorithmCacheRouter.h"
#include "DKind.inl.h"

namespace PNC
{
    using Size_t = int32;
    using ComponentType = ComponentTypeT<Size_t>;
    using ComponentTypeSet = ComponentTypeSetT<Size_t>;

    /// <summary>
    /// Defines the list of component a container has.
    /// </summary>
    using ChunkStructure = ChunkStructureT<Size_t>;
    template<> struct PropTraits<const ChunkStructure*> : public PropTraitsDefault2<ChunkStructure, const ChunkStructure*, DStructurePtr> { };

    /// <summary>
    /// A ChunkPointer container of components for one(1) chunk and multiple nodes. 
    /// A ChunkPointer does not own its component data.
    /// </summary>
    using NChunkPointer = ChunkPointerT<ChunkStructure>;

    /// <summary>
    /// A Chunk container of components for one(1) chunk and multiple nodes. 
    /// A Chunk owns its component data.
    /// </summary>
    using NChunk = ChunkT<ChunkStructure>;

    /// <summary>
    /// A BucketPointer container of components for one(1) chunk and multiple nodes. 
    /// A BucketPointer does not own its component data.
    /// A BucketPointer has a fixed NodeCapacity of allocated nodes and a variable NodeCount of constructed nodes stating at index 0 that cannot overflow the capacity.
    /// </summary>
    using NBucketPointer = BucketPointerT<ChunkStructure>;

    /// <summary>
    /// A Bucket container of components for one(1) chunk and multiple nodes. 
    /// A Bucket owns its component data.
    /// A Bucket has a fixed NodeCapacity of allocated nodes and a variable NodeCount of constructed nodes stating at index 0 that cannot overflow the capacity.
    /// </summary>
    using NBucket = BucketT<ChunkStructure>;

    /// <summary>
    /// A BunchPointer container of components for one(1) chunk and multiple nodes. 
    /// A BunchPointer does not own its component data.
    /// A BunchPointer has a variable NodeCapacity of allocated nodes that grows when needed to fit a smaller or equal number of (NodeCount) constructed nodes. 
    /// </summary>
    using NBunchPointer = BunchPointerT<ChunkStructure>;

    /// <summary>
    /// A Bunch container of components for one(1) chunk and multiple nodes. 
    /// A Bunch owns its component data.
    /// A Bunch has a variable NodeCapacity of allocated nodes that grows when needed to fit a smaller or equal number of (NodeCount) constructed nodes. 
    /// </summary>
    using NBunch = BunchT<ChunkStructure>;

    /// <summary>
    /// An ArrayPointer container of components for multiple chunks and multiple nodes.
    /// An ArrayPointer does not own its component data.
    /// An ArrayPointer has a fixed ChunkCount of allocated and constructed chunks, and a total number (NodeCount) of allocated and constructed nodes.
    /// </summary>
    using NArrayPointer = ArrayPointerT<ChunkStructure, NChunkPointer>;

    /// <summary>
    /// An Array container of components for multiple chunks and multiple nodes.
    /// An Array owns its component data.
    /// An Array has a fixed ChunkCount of allocated and constructed chunks, and a total number (NodeCount) of allocated and constructed nodes.
    /// </summary>
    using NArray = ArrayT<ChunkStructure, NChunkPointer>;

    /// <summary>
    /// A UniformArray container of components for multiple chunks and multiple nodes.
    /// A UniformArray owns its component data.
    /// A UniformArray has a fixed ChunkCount of allocated and constructed chunks, and a total number (NodeCount) of allocated and constructed nodes.
    /// Each chunk elements in the UniformArray has the same NodeCount.
    /// </summary>
    using NUniformArray = UniformArrayT<ChunkStructure, NChunkPointer>;

    /// <summary>
    /// A KindPointer has a Kind data field usable to identify what kind of container follows the Kind data field in memory.
    /// </summary>
    using KindPointer = KindPointerT<ChunkStructure>;

    /// <summary>
    /// A ChunkPointer container of components for one(1) chunk and multiple nodes. 
    /// A ChunkPointer does not own its component data.
    /// A KChunkPointer is a KindPointer of value ContainerKind::Chunk
    /// </summary>
    using KChunkPointer = KChunkPointerT<ChunkStructure>;

    /// <summary>
    /// A ChunkPointer container of components for one(1) chunk and multiple nodes. 
    /// A ChunkPointer owns its component data.
    /// A KChunkPointer is a KindPointer of value ContainerKind::Chunk
    /// </summary>
    using KChunk = KChunkT<ChunkStructure>;

    /// <summary>
    /// A BucketPointer container of components for one(1) chunk and multiple nodes. 
    /// A BucketPointer does not own its component data.
    /// A BucketPointer has a fixed NodeCapacity of allocated nodes and a variable NodeCount of constructed nodes stating at index 0 that cannot overflow the capacity.
    /// A KBucketPointer is a KindPointer of value ContainerKind::Chunk
    /// </summary>
    using KBucketPointer = KBucketPointerT<ChunkStructure>;

    /// <summary>
    /// A Bucket container of components for one(1) chunk and multiple nodes. 
    /// A Bucket owns its component data.
    /// A Bucket has a fixed NodeCapacity of allocated nodes and a variable NodeCount of constructed nodes stating at index 0 that cannot overflow the capacity.
    /// A KBucket is a KindPointer of value ContainerKind::Chunk
    /// </summary>
    using KBucket = KBucketT<ChunkStructure>;

    /// <summary>
    /// A BunchPointer container of components for one(1) chunk and multiple nodes. 
    /// A BunchPointer does not own its component data.
    /// A BunchPointer has a variable NodeCapacity of allocated nodes that grows when needed to fit a smaller or equal number of (NodeCount) constructed nodes. 
    /// A KBunchPointer is a KindPointer of value ContainerKind::Chunk
    /// </summary>
    using KBunchPointer = KBunchPointerT<ChunkStructure>;

    /// <summary>
    /// A Bunch container of components for one(1) chunk and multiple nodes. 
    /// A Bunch owns its component data.
    /// A Bunch has a variable NodeCapacity of allocated nodes that grows when needed to fit a smaller or equal number of (NodeCount) constructed nodes. 
    /// A KBunch is a KindPointer of value ContainerKind::Chunk
    /// </summary>
    using KBunch = KBunchT<ChunkStructure>;

    /// <summary>
    /// An ArrayPointer container of components for multiple chunks and multiple nodes.
    /// An ArrayPointer does not own its component data.
    /// An ArrayPointer has a fixed ChunkCount of allocated and constructed chunks, and a total number (NodeCount) of allocated and constructed nodes.
    /// A KArrayPointer is a KindPointer of value ContainerKind::Array
    /// </summary>
    using KArrayPointer = KArrayPointerT<ChunkStructure, NChunkPointer>;

    /// <summary>
    /// An Array container of components for multiple chunks and multiple nodes.
    /// An Array own its component data.
    /// An Array has a fixed ChunkCount of allocated and constructed chunks, and a total number (NodeCount) of allocated and constructed nodes.
    /// A KArray is a KindPointer of value ContainerKind::ChunkArray
    /// </summary>
    using KArray = KArrayT<ChunkStructure, NChunkPointer>;

    /// <summary>
    /// A KTreePointer has a Parent/Children set of pointers to other KindPointer containers.
    /// </summary>
    using KTreePointer = KTreePointerT<ChunkStructure>;

    /// <summary>
    /// A KChunkTreePointer container of components for one(1) chunk and multiple nodes. 
    /// A KChunkTreePointer does not own its component data.
    /// A KChunkTreePointer is a KindPointer of value ContainerKind::ChunkTree
    /// A KChunkTreePointer has a Parent/Children set of pointers to other KindPointer containers.
    /// </summary>
    using KChunkTreePointer = KChunkTreePointerT<ChunkStructure>;

    /// <summary>
    /// A KChunkTree container of components for one(1) chunk and multiple nodes. 
    /// A KChunkTree owns its component data.
    /// A KChunkTree is a KindPointer of value ContainerKind::ChunkTree
    /// A KChunkTree has a Parent/Children set of pointers to other KindPointer containers.
    /// </summary>
    using KChunkTree = KChunkTreeT<ChunkStructure>;

    /// <summary>
    /// A KBucketTreePointer container of components for one(1) chunk and multiple nodes. 
    /// A KBucketTreePointer does not own its component data.
    /// A KBucketTreePointer has a fixed NodeCapacity of allocated nodes and a variable NodeCount of constructed nodes stating at index 0 that cannot overflow the capacity.
    /// A KBucketTreePointer is a KindPointer of value ContainerKind::ChunkTree
    /// A KBucketTreePointer has a Parent/Children set of pointers to other KindPointer containers.
    /// </summary>
    using KBucketTreePointer = KBucketTreePointerT<ChunkStructure>;

    /// <summary>
    /// A KBucketTree container of components for one(1) chunk and multiple nodes. 
    /// A KBucketTree owns its component data.
    /// A KBucketTree has a fixed NodeCapacity of allocated nodes and a variable NodeCount of constructed nodes stating at index 0 that cannot overflow the capacity.
    /// A KBucketTree is a KindPointer of value ContainerKind::ChunkTree
    /// A KBucketTree has a Parent/Children set of pointers to other KindPointer containers.
    /// </summary>
    using KBucketTree = KBucketTreeT<ChunkStructure>;

    /// <summary>
    /// A KBunchTreePointer container of components for one(1) chunk and multiple nodes. 
    /// A KBunchTreePointer does not own its component data.
    /// A KBunchTreePointer has a variable NodeCapacity of allocated nodes that grows when needed to fit a smaller or equal number of (NodeCount) constructed nodes. 
    /// A KBunchTreePointer is a KindPointer of value ContainerKind::ChunkTree
    /// A KBunchTreePointer has a Parent/Children set of pointers to other KindPointer containers.
    /// </summary>
    using KBunchTreePointer = KBunchTreePointerT<ChunkStructure>;

    /// <summary>
    /// A KBunchTree container of components for one(1) chunk and multiple nodes. 
    /// A KBunchTree owns its component data.
    /// A KBunchTree has a variable NodeCapacity of allocated nodes that grows when needed to fit a smaller or equal number of (NodeCount) constructed nodes. 
    /// A KBunchTree is a KindPointer of value ContainerKind::ChunkTree
    /// A KBunchTree has a Parent/Children set of pointers to other KindPointer containers.
    /// </summary>
    using KBunchTree = KBunchTreeT<ChunkStructure>;

    /// <summary>
    /// A KArrayTreePointer container of components for multiple chunks and multiple nodes.
    /// A KArrayTreePointer does not own its component data.
    /// A KArrayTreePointer has a fixed ChunkCount of allocated and constructed chunks, and a total number (NodeCount) of allocated and constructed nodes.
    /// A KArrayTreePointer is a KindPointer of value ContainerKind::ArrayTree
    /// A KArrayTreePointer has a Parent/Children set of pointers to other KindPointer containers.
    /// </summary>
    using KArrayTreePointer = KArrayTreePointerT<ChunkStructure, NChunkPointer>;

    /// <summary>
    /// A KArrayTree container of components for multiple chunks and multiple nodes.
    /// A KArrayTree owns its component data.
    /// A KArrayTree has a fixed ChunkCount of allocated and constructed chunks, and a total number (NodeCount) of allocated and constructed nodes.
    /// A KArrayTree is a KindPointer of value ContainerKind::ArrayTree
    /// A KArrayTree has a Parent/Children set of pointers to other KindPointer containers.
    /// </summary>
    using KArrayTree = KArrayTreeT<ChunkStructure, NChunkPointer>;

    // TODO complete
    using KBucketBarrelTree = KBucketBarrelTreeT<ChunkStructure, NChunkPointer>;

    /// <summary>
    /// Caches the component data routing indices for each chunk structure the algorithm is executed with.
    /// Use in your pipelines to avoid re-routing each component for all chunks of the same structure.
    /// May improve performance, especially for large structures.
    /// </summary>
    /// <typeparam name="TAlgorithm"></typeparam>
    template<typename TAlgorithm>
    using AlgorithmCacheRouter = Routing::AlgorithmCacheRouterT<TAlgorithm, ChunkStructure, Size_t>;

    /// <summary>
    /// An AlgorithmRouter equivalent of AlgorithmCacheRouter that does not cache component data routing indices.
    /// Use in your pipeline as a replacement of AlgorithmCacheRouter
    /// </summary>
    /// <typeparam name="TAlgorithm"></typeparam>
    template<typename TAlgorithm>
    using AlgorithmRouter = Routing::AlgorithmRouterT<TAlgorithm>;

    /// <summary>
    /// Extend this struct to implement a custom pipeline.
    /// A pipeline is responsible to dispatch the execution of various algorithm on incoming containers
    /// </summary>
    /// <typeparam name="TPipeline"></typeparam>
    template<typename TPipeline>
    using Pipeline = PipelineT<TPipeline, ChunkStructure, Size_t>;

    using CoParentInChunk = CoParentInChunkT<Size_t>;
    using CoSingleParentOutsideChunk = CoSingleParentOutsideChunkT<Size_t>;
    using CoChildrenInChunk = CoChildrenInChunkT<Size_t>;

    template<typename TIn> NodeCapacityT        <Size_t> PropNodeCapacity        (const TIn& value) { return PropNodeCapacityT        <Size_t, TIn>(value); }
    template<typename TIn> NodeCountT           <Size_t> PropNodeCount           (const TIn& value) { return PropNodeCountT           <Size_t, TIn>(value); }
    template<typename TIn> ChunkCapacityT       <Size_t> PropChunkCapacity       (const TIn& value) { return PropChunkCapacityT       <Size_t, TIn>(value); }
    template<typename TIn> ChunkCountT          <Size_t> PropChunkCount          (const TIn& value) { return PropChunkCountT          <Size_t, TIn>(value); }
    template<typename TIn> NodeCapacityPerChunkT<Size_t> PropNodeCapacityPerChunk(const TIn& value) { return PropNodeCapacityPerChunkT<Size_t, TIn>(value); }
    template<typename TIn> NodeCountPerChunkT   <Size_t> PropNodeCountPerChunk   (const TIn& value) { return PropNodeCountPerChunkT   <Size_t, TIn>(value); }
}


namespace PNC 
{
    // TODO experimental/remove
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


}