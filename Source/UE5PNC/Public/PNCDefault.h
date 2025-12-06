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

namespace NiT
{
    template<typename TChunkStructure, typename TSize>
    struct ContainersT 
    {
        using Size_t = TSize;

        /// <summary>
        /// Defines the list of component a container has.
        /// </summary>
        using ChunkStructure_t = TChunkStructure;

        using ComponentType_t = typename ChunkStructure_t::ComponentType_t;

        using ComponentTypeSet_t = typename ChunkStructure_t::ComponentTypeSet_t;
        
        /// <summary>
        /// Define who owns a component
        /// </summary>
        using ComponentOwner_t = NiT::ComponentOwner;

        /// <summary>
        /// Extend this type to create a component owned by nodes in a chunks.
        /// Each node in a chunk will have exactly one(1) instance of all their NodeComponents.
        /// </summary>
        using NodeComponent_t = NiT::NodeComponent;

        /// <summary>
        /// Extend this type to create a component owned by chunks.
        /// Each chunk with this component will have exactly one(1) instance of all their ChunkComponents
        /// </summary>
        using ChunkComponent_t = NiT::ChunkComponent;

        /// <summary>
        /// A ChunkPointer container of components for one(1) chunk and multiple nodes. 
        /// A ChunkPointer does not own its component data.
        /// Figure: |#####|
        /// Layout: Structure*, void**CDA, NodeCount
        /// </summary>
        using NChunkPointer_t = NiT::ChunkPointerT<ChunkStructure_t>;

        /// <summary>
        /// A Chunk container of components for one(1) chunk and multiple nodes. 
        /// A Chunk owns its component data.
        /// Figure: |#####|
        /// Layout: Structure*, void**CDA, NodeCount
        /// </summary>
        using NChunk_t = NiT::ChunkT<ChunkStructure_t>;

        /// <summary>
        /// A BucketPointer container of components for one(1) chunk and multiple nodes. 
        /// A BucketPointer does not own its component data.
        /// A BucketPointer has a fixed NodeCapacity of allocated nodes and a variable NodeCount of constructed nodes stating at index 0 that cannot overflow the capacity.
        /// Figure: |###__|
        /// Layout: Structure*, void**CDA, NodeCount, NodeCapacity
        /// </summary>
        using NBucketPointer_t = NiT::BucketPointerT<ChunkStructure_t>;

        /// <summary>
        /// A Bucket container of components for one(1) chunk and multiple nodes. 
        /// A Bucket owns its component data.
        /// A Bucket has a fixed NodeCapacity of allocated nodes and a variable NodeCount of constructed nodes stating at index 0 that cannot overflow the capacity.
        /// Figure: |###__|
        /// Layout: Structure*, void**CDA, NodeCount, NodeCapacity
        /// </summary>
        using NBucket_t = NiT::BucketT<ChunkStructure_t>;

        /// <summary>
        /// A BunchPointer container of components for one(1) chunk and multiple nodes. 
        /// A BunchPointer does not own its component data.
        /// A BunchPointer has a variable NodeCapacity of allocated nodes that grows when needed to fit a smaller or equal number of (NodeCount) constructed nodes. 
        /// Figure: |###__|...
        /// </summary>
        using NBunchPointer_t = NiT::BunchPointerT<ChunkStructure_t>;

        /// <summary>
        /// A Bunch container of components for one(1) chunk and multiple nodes. 
        /// A Bunch owns its component data.
        /// A Bunch has a variable NodeCapacity of allocated nodes that grows when needed to fit a smaller or equal number of (NodeCount) constructed nodes. 
        /// Figure: |###__|...
        /// Layout: Structure*, void**CDA, NodeCount, NodeCapacity
        /// </summary>
        using NBunch_t = NiT::BunchT<ChunkStructure_t>;

        /// <summary>
        /// An ArrayPointer container of components for multiple chunks and multiple nodes.
        /// An ArrayPointer does not own its component data.
        /// An ArrayPointer has a fixed ChunkCount of allocated and constructed chunks, and a total number (NodeCount) of allocated and constructed nodes.
        /// Figure: |####|###|######|
        /// Layout: Structure*, void**CDA, NodeCount, NodeCapacity
        /// </summary>
        using NArrayPointer_t = NiT::ArrayPointerT<ChunkStructure_t, NChunkPointer_t>;

        /// <summary>
        /// An Array container of components for multiple chunks and multiple nodes.
        /// An Array owns its component data.
        /// An Array has a fixed ChunkCount of allocated and constructed chunks, and a total number (NodeCount) of allocated and constructed nodes.
        /// Figure: |####|###|######|
        /// Layout: Structure*, void**CDA, NodeCount, ChunkPointer*, ChunkCount
        /// </summary>
        using NArray_t = NiT::ArrayT<ChunkStructure_t, NChunkPointer_t>;

        /// <summary>
        /// A UniformArray container of components for multiple chunks and multiple nodes.
        /// A UniformArray owns its component data.
        /// A UniformArray has a fixed ChunkCount of allocated and constructed chunks, and a total number (NodeCount) of allocated and constructed nodes.
        /// Each chunk elements in the UniformArray has the same NodeCount.
        /// Figure: |###|###|###|###|
        /// Layout: Structure*, void**CDA, NodeCount, ChunkPointer*, ChunkCount
        /// </summary>
        using NUniformArray_t = NiT::UniformArrayT<ChunkStructure_t, NChunkPointer_t>;

        /// <summary>
        /// A KindPointer has a Kind data field usable to identify what kind of container follows the Kind data field in memory.
        /// Layout: Kind
        /// </summary>
        using KindPointer_t = NiT::KindPointerT<ChunkStructure_t>;

        /// <summary>
        /// A ChunkPointer container of components for one(1) chunk and multiple nodes. 
        /// A ChunkPointer does not own its component data.
        /// A KChunkPointer is a KindPointer of value ContainerKind::Chunk
        /// Figure: |#####|
        /// Layout: Kind, Structure*, void**CDA, NodeCount
        /// </summary>
        using KChunkPointer_t = NiT::KChunkPointerT<ChunkStructure_t>;

        /// <summary>
        /// A ChunkPointer container of components for one(1) chunk and multiple nodes. 
        /// A ChunkPointer owns its component data.
        /// A KChunkPointer is a KindPointer of value ContainerKind::Chunk
        /// Figure: |#####|
        /// Layout: Kind, Structure*, void**CDA, NodeCount
        /// </summary>
        using KChunk_t = NiT::KChunkT<ChunkStructure_t>;

        /// <summary>
        /// A BucketPointer container of components for one(1) chunk and multiple nodes. 
        /// A BucketPointer does not own its component data.
        /// A BucketPointer has a fixed NodeCapacity of allocated nodes and a variable NodeCount of constructed nodes stating at index 0 that cannot overflow the capacity.
        /// A KBucketPointer is a KindPointer of value ContainerKind::Chunk
        /// Figure: |###__|
        /// Layout: Kind, Structure*, void**CDA, NodeCount, NodeCapacity
        /// </summary>
        using KBucketPointer_t = NiT::KBucketPointerT<ChunkStructure_t>;

        /// <summary>
        /// A Bucket container of components for one(1) chunk and multiple nodes. 
        /// A Bucket owns its component data.
        /// A Bucket has a fixed NodeCapacity of allocated nodes and a variable NodeCount of constructed nodes stating at index 0 that cannot overflow the capacity.
        /// A KBucket is a KindPointer of value ContainerKind::Chunk
        /// Figure: |###__|
        /// Layout: Kind, Structure*, void**CDA, NodeCount, NodeCapacity
        /// </summary>
        using KBucket_t = NiT::KBucketT<ChunkStructure_t>;

        /// <summary>
        /// A BunchPointer container of components for one(1) chunk and multiple nodes. 
        /// A BunchPointer does not own its component data.
        /// A BunchPointer has a variable NodeCapacity of allocated nodes that grows when needed to fit a smaller or equal number of (NodeCount) constructed nodes. 
        /// A KBunchPointer is a KindPointer of value ContainerKind::Chunk
        /// Figure: |###__|...
        /// Layout: Kind, Structure*, void**CDA, NodeCount, NodeCapacity
        /// </summary>
        using KBunchPointer_t = NiT::KBunchPointerT<ChunkStructure_t>;

        /// <summary>
        /// A Bunch container of components for one(1) chunk and multiple nodes. 
        /// A Bunch owns its component data.
        /// A Bunch has a variable NodeCapacity of allocated nodes that grows when needed to fit a smaller or equal number of (NodeCount) constructed nodes. 
        /// A KBunch is a KindPointer of value ContainerKind::Chunk
        /// Figure: |###__|...
        /// Layout: Kind, Structure*, void**CDA, NodeCount, NodeCapacity
        /// </summary>
        using KBunch_t = NiT::KBunchT<ChunkStructure_t>;

        /// <summary>
        /// An ArrayPointer container of components for multiple chunks and multiple nodes.
        /// An ArrayPointer does not own its component data.
        /// An ArrayPointer has a fixed ChunkCount of allocated and constructed chunks, and a total number (NodeCount) of allocated and constructed nodes.
        /// A KArrayPointer is a KindPointer of value ContainerKind::Array
        /// Figure: |####|###|######|
        /// Layout: Kind, Structure*, void**CDA, NodeCount, ChunkPointer*, ChunkCount
        /// </summary>
        using KArrayPointer_t = NiT::KArrayPointerT<ChunkStructure_t, NChunkPointer_t>;

        /// <summary>
        /// An Array container of components for multiple chunks and multiple nodes.
        /// An Array own its component data.
        /// An Array has a fixed ChunkCount of allocated and constructed chunks, and a total number (NodeCount) of allocated and constructed nodes.
        /// A KArray is a KindPointer of value ContainerKind::ChunkArray
        /// Figure: |####|###|######|
        /// Layout: Kind, Structure*, void**CDA, NodeCount, ChunkPointer*, ChunkCount
        /// </summary>
        using KArray_t = NiT::KArrayT<ChunkStructure_t, NChunkPointer_t>;

        /// <summary>
        /// A KTreePointer has a Parent/Children set of pointers to other KindPointer containers.
        /// </summary>
        using KTreePointer_t = NiT::KTreePointerT<ChunkStructure_t>;

        /// <summary>
        /// A KChunkTreePointer container of components for one(1) chunk and multiple nodes. 
        /// A KChunkTreePointer does not own its component data.
        /// A KChunkTreePointer is a KindPointer of value ContainerKind::ChunkTree
        /// A KChunkTreePointer has a Parent/Children set of pointers to other KindPointer containers.
        /// Figure: |#####|
        /// Layout: Kind, Parent*, FirstChild*, PrevivousSibling*, NextSibling*, Structure*, void**CDA, NodeCount
        /// </summary>
        using KChunkTreePointer_t = NiT::KChunkTreePointerT<ChunkStructure_t>;

        /// <summary>
        /// A KChunkTree container of components for one(1) chunk and multiple nodes. 
        /// A KChunkTree owns its component data.
        /// A KChunkTree is a KindPointer of value ContainerKind::ChunkTree
        /// A KChunkTree has a Parent/Children set of pointers to other KindPointer containers.
        /// Figure: |#####|
        /// Layout: Kind, 
        ///         Parent*, FirstChild*, PrevivousSibling*, NextSibling*, 
        ///         Structure*, void**CDA, NodeCount
        /// </summary>
        using KChunkTree_t = NiT::KChunkTreeT<ChunkStructure_t>;

        /// <summary>
        /// A KBucketTreePointer container of components for one(1) chunk and multiple nodes. 
        /// A KBucketTreePointer does not own its component data.
        /// A KBucketTreePointer has a fixed NodeCapacity of allocated nodes and a variable NodeCount of constructed nodes stating at index 0 that cannot overflow the capacity.
        /// A KBucketTreePointer is a KindPointer of value ContainerKind::ChunkTree
        /// A KBucketTreePointer has a Parent/Children set of pointers to other KindPointer containers.
        /// Figure: |###__|
        /// Layout: Kind, 
        ///         Parent*, FirstChild*, PrevivousSibling*, NextSibling*, 
        ///         Structure*, void**CDA, NodeCount, NodeCapacity
        /// </summary>
        using KBucketTreePointer_t = NiT::KBucketTreePointerT<ChunkStructure_t>;

        /// <summary>
        /// A KBucketTree container of components for one(1) chunk and multiple nodes. 
        /// A KBucketTree owns its component data.
        /// A KBucketTree has a fixed NodeCapacity of allocated nodes and a variable NodeCount of constructed nodes stating at index 0 that cannot overflow the capacity.
        /// A KBucketTree is a KindPointer of value ContainerKind::ChunkTree
        /// A KBucketTree has a Parent/Children set of pointers to other KindPointer containers.
        /// Figure: |###__|
        /// Layout: Kind, 
        ///         Parent*, FirstChild*, PrevivousSibling*, NextSibling*, 
        ///         Structure*, void**CDA, NodeCount, NodeCapacity
        /// </summary>
        using KBucketTree_t = NiT::KBucketTreeT<ChunkStructure_t>;

        /// <summary>
        /// A KBunchTreePointer container of components for one(1) chunk and multiple nodes. 
        /// A KBunchTreePointer does not own its component data.
        /// A KBunchTreePointer has a variable NodeCapacity of allocated nodes that grows when needed to fit a smaller or equal number of (NodeCount) constructed nodes. 
        /// A KBunchTreePointer is a KindPointer of value ContainerKind::ChunkTree
        /// A KBunchTreePointer has a Parent/Children set of pointers to other KindPointer containers.
        /// Figure: |###__|...
        /// Layout: Kind, 
        ///         Parent*, FirstChild*, PrevivousSibling*, NextSibling*, 
        ///         Structure*, void**CDA, NodeCount, NodeCapacity
        /// </summary>
        using KBunchTreePointer_t = NiT::KBunchTreePointerT<ChunkStructure_t>;

        /// <summary>
        /// A KBunchTree container of components for one(1) chunk and multiple nodes. 
        /// A KBunchTree owns its component data.
        /// A KBunchTree has a variable NodeCapacity of allocated nodes that grows when needed to fit a smaller or equal number of (NodeCount) constructed nodes. 
        /// A KBunchTree is a KindPointer of value ContainerKind::ChunkTree
        /// A KBunchTree has a Parent/Children set of pointers to other KindPointer containers.
        /// Figure: |###__|...
        /// Layout: Kind, 
        ///         Parent*, FirstChild*, PrevivousSibling*, NextSibling*, 
        ///         Structure*, void**CDA, NodeCount, NodeCapacity
        /// </summary>
        using KBunchTree_t = NiT::KBunchTreeT<ChunkStructure_t>;

        /// <summary>
        /// A KArrayTreePointer container of components for multiple chunks and multiple nodes.
        /// A KArrayTreePointer does not own its component data.
        /// A KArrayTreePointer has a fixed ChunkCount of allocated and constructed chunks, and a total number (NodeCount) of allocated and constructed nodes.
        /// A KArrayTreePointer is a KindPointer of value ContainerKind::ArrayTree
        /// A KArrayTreePointer has a Parent/Children set of pointers to other KindPointer containers.
        /// Figure: |####|###|######|
        /// Layout: Kind, 
        ///         Parent*, FirstChild*, PrevivousSibling*, NextSibling*, 
        ///         Structure*, void**CDA, NodeCount, ChunkPointer*, ChunkCount
        /// </summary>
        using KArrayTreePointer_t = NiT::KArrayTreePointerT<ChunkStructure_t, NChunkPointer_t>;

        /// <summary>
        /// A KArrayTree container of components for multiple chunks and multiple nodes.
        /// A KArrayTree owns its component data.
        /// A KArrayTree has a fixed ChunkCount of allocated and constructed chunks, and a total number (NodeCount) of allocated and constructed nodes.
        /// A KArrayTree is a KindPointer of value ContainerKind::ArrayTree
        /// A KArrayTree has a Parent/Children set of pointers to other KindPointer containers.
        /// Figure: |####|###|######|
        /// Layout: Kind, 
        ///         Parent*, FirstChild*, PrevivousSibling*, NextSibling*, 
        ///         Structure*, void**CDA, NodeCount, ChunkPointer*, ChunkCount
        /// </summary>
        using KArrayTree_t = NiT::KArrayTreeT<ChunkStructure_t, NChunkPointer_t>;

        // TODO complete
        using KBucketBarrelTree_t = NiT::KBucketBarrelTreeT<ChunkStructure_t, NChunkPointer_t>;

        // TODO Move to algo file
        using Algorithm_t = NiT::Algorithm;

        /// <summary>
        /// Caches the component data routing indices for each chunk structure the algorithm is executed with.
        /// Use in your pipelines to avoid re-routing each component for all chunks of the same structure.
        /// May improve performance, especially for large structures.
        /// </summary>
        /// <typeparam name="TAlgorithm"></typeparam>
        template<typename TAlgorithm>
        using AlgorithmCacheRouter_t = NiT::Routing::AlgorithmCacheRouterT<TAlgorithm, ChunkStructure_t, Size_t>;

        /// <summary>
        /// An AlgorithmRouter equivalent of AlgorithmCacheRouter that does not cache component data routing indices.
        /// Use in your pipeline as a replacement of AlgorithmCacheRouter
        /// </summary>
        /// <typeparam name="TAlgorithm"></typeparam>
        template<typename TAlgorithm>
        using AlgorithmRouter_t = NiT::Routing::AlgorithmRouterT<TAlgorithm>;

        /// <summary>
        /// Extend this struct to implement a custom pipeline.
        /// A pipeline is responsible to dispatch the execution of various algorithm on incoming containers
        /// </summary>
        /// <typeparam name="TPipeline"></typeparam>
        template<typename TPipeline>
        using Pipeline_t = NiT::PipelineT<TPipeline, ChunkStructure_t, Size_t>;

        using CoParentInChunk_t = NiT::CoParentInChunkT<Size_t>;
        using CoSingleParentOutsideChunk_t = NiT::CoSingleParentOutsideChunkT<Size_t>;
        using CoChildrenInChunk_t = NiT::CoChildrenInChunkT<Size_t>;

        template<typename TIn> NiT::NodeCapacityT        <Size_t> PropNodeCapacity        (const TIn& value) { return NiT::PropNodeCapacityT        <Size_t, TIn>(value); }
        template<typename TIn> NiT::NodeCountT           <Size_t> PropNodeCount           (const TIn& value) { return NiT::PropNodeCountT           <Size_t, TIn>(value); }
        template<typename TIn> NiT::ChunkCapacityT       <Size_t> PropChunkCapacity       (const TIn& value) { return NiT::PropChunkCapacityT       <Size_t, TIn>(value); }
        template<typename TIn> NiT::ChunkCountT          <Size_t> PropChunkCount          (const TIn& value) { return NiT::PropChunkCountT          <Size_t, TIn>(value); }
        template<typename TIn> NiT::NodeCapacityPerChunkT<Size_t> PropNodeCapacityPerChunk(const TIn& value) { return NiT::PropNodeCapacityPerChunkT<Size_t, TIn>(value); }
        template<typename TIn> NiT::NodeCountPerChunkT   <Size_t> PropNodeCountPerChunk   (const TIn& value) { return NiT::PropNodeCountPerChunkT   <Size_t, TIn>(value); }

        using NodeCapacity_t =         NiT::NodeCapacityT<        Size_t>;
        using NodeCount_t =            NiT::NodeCountT<           Size_t>;
        using ChunkCapacity_t =        NiT::ChunkCapacityT<       Size_t>;
        using ChunkCount_t =           NiT::ChunkCountT<          Size_t>;
        using NodeCapacityPerChunk_t = NiT::NodeCapacityPerChunkT<Size_t>;
        using NodeCountPerChunk_t =    NiT::NodeCountPerChunkT<   Size_t>;


        // TODO experimental/remove
        template<typename TComponent>
        using ComponentPointer_t = NiT::ComponentPointerT<ChunkStructure_t, TComponent>;

        // TODO experimental/remove
        template<typename TComponent>
        using ComponentPointerRO_t = NiT::ComponentPointerT<ChunkStructure_t, TComponent>;

    };
}

namespace Ni::Containers
{
    using Size_t = int32;

    using Containers_t = NiT::ContainersT<NiT::ChunkStructureT<Size_t>, Size_t>;

    using ComponentType = Containers_t::ComponentType_t;
    using ComponentTypeSet = Containers_t::ComponentTypeSet_t;

    /// <summary>
    /// Define who owns a component
    /// </summary>
    using NiT::ComponentOwner;

    /// <summary>
    /// Extend this type to create a component owned by nodes in a chunks.
    /// Each node in a chunk will have exactly one(1) instance of all their NodeComponents.
    /// </summary>
    using NiT::NodeComponent;

    /// <summary>
    /// Extend this type to create a component owned by chunks.
    /// Each chunk with this component will have exactly one(1) instance of all their ChunkComponents
    /// </summary>
    using NiT::ChunkComponent;

    /// <summary>
    /// Defines the list of component a container has.
    /// </summary>
    using ChunkStructure = NiT::ChunkStructureT<Size_t>;

    /// <summary>
    /// A ChunkPointer container of components for one(1) chunk and multiple nodes. 
    /// A ChunkPointer does not own its component data.
    /// Figure: |#####|
    /// Layout: Structure*, void**CDA, NodeCount
    /// </summary>
    using NChunkPointer = NiT::ChunkPointerT<ChunkStructure>;

    /// <summary>
    /// A Chunk container of components for one(1) chunk and multiple nodes. 
    /// A Chunk owns its component data.
    /// Figure: |#####|
    /// Layout: Structure*, void**CDA, NodeCount
    /// </summary>
    using NChunk = NiT::ChunkT<ChunkStructure>;

    /// <summary>
    /// A BucketPointer container of components for one(1) chunk and multiple nodes. 
    /// A BucketPointer does not own its component data.
    /// A BucketPointer has a fixed NodeCapacity of allocated nodes and a variable NodeCount of constructed nodes stating at index 0 that cannot overflow the capacity.
    /// Figure: |###__|
    /// Layout: Structure*, void**CDA, NodeCount, NodeCapacity
    /// </summary>
    using NBucketPointer = NiT::BucketPointerT<ChunkStructure>;

    /// <summary>
    /// A Bucket container of components for one(1) chunk and multiple nodes. 
    /// A Bucket owns its component data.
    /// A Bucket has a fixed NodeCapacity of allocated nodes and a variable NodeCount of constructed nodes stating at index 0 that cannot overflow the capacity.
    /// Figure: |###__|
    /// Layout: Structure*, void**CDA, NodeCount, NodeCapacity
    /// </summary>
    using NBucket = NiT::BucketT<ChunkStructure>;

    /// <summary>
    /// A BunchPointer container of components for one(1) chunk and multiple nodes. 
    /// A BunchPointer does not own its component data.
    /// A BunchPointer has a variable NodeCapacity of allocated nodes that grows when needed to fit a smaller or equal number of (NodeCount) constructed nodes. 
    /// Figure: |###__|...
    /// </summary>
    using NBunchPointer = NiT::BunchPointerT<ChunkStructure>;

    /// <summary>
    /// A Bunch container of components for one(1) chunk and multiple nodes. 
    /// A Bunch owns its component data.
    /// A Bunch has a variable NodeCapacity of allocated nodes that grows when needed to fit a smaller or equal number of (NodeCount) constructed nodes. 
    /// Figure: |###__|...
    /// Layout: Structure*, void**CDA, NodeCount, NodeCapacity
    /// </summary>
    using NBunch = NiT::BunchT<ChunkStructure>;

    /// <summary>
    /// An ArrayPointer container of components for multiple chunks and multiple nodes.
    /// An ArrayPointer does not own its component data.
    /// An ArrayPointer has a fixed ChunkCount of allocated and constructed chunks, and a total number (NodeCount) of allocated and constructed nodes.
    /// Figure: |####|###|######|
    /// Layout: Structure*, void**CDA, NodeCount, NodeCapacity
    /// </summary>
    using NArrayPointer = NiT::ArrayPointerT<ChunkStructure, NChunkPointer>;

    /// <summary>
    /// An Array container of components for multiple chunks and multiple nodes.
    /// An Array owns its component data.
    /// An Array has a fixed ChunkCount of allocated and constructed chunks, and a total number (NodeCount) of allocated and constructed nodes.
    /// Figure: |####|###|######|
    /// Layout: Structure*, void**CDA, NodeCount, ChunkPointer*, ChunkCount
    /// </summary>
    using NArray = NiT::ArrayT<ChunkStructure, NChunkPointer>;

    /// <summary>
    /// A UniformArray container of components for multiple chunks and multiple nodes.
    /// A UniformArray owns its component data.
    /// A UniformArray has a fixed ChunkCount of allocated and constructed chunks, and a total number (NodeCount) of allocated and constructed nodes.
    /// Each chunk elements in the UniformArray has the same NodeCount.
    /// Figure: |###|###|###|###|
    /// Layout: Structure*, void**CDA, NodeCount, ChunkPointer*, ChunkCount
    /// </summary>
    using NUniformArray = NiT::UniformArrayT<ChunkStructure, NChunkPointer>;

    /// <summary>
    /// A KindPointer has a Kind data field usable to identify what kind of container follows the Kind data field in memory.
    /// Layout: Kind
    /// </summary>
    using KindPointer = NiT::KindPointerT<ChunkStructure>;

    /// <summary>
    /// A ChunkPointer container of components for one(1) chunk and multiple nodes. 
    /// A ChunkPointer does not own its component data.
    /// A KChunkPointer is a KindPointer of value ContainerKind::Chunk
    /// Figure: |#####|
    /// Layout: Kind, Structure*, void**CDA, NodeCount
    /// </summary>
    using KChunkPointer = NiT::KChunkPointerT<ChunkStructure>;

    /// <summary>
    /// A ChunkPointer container of components for one(1) chunk and multiple nodes. 
    /// A ChunkPointer owns its component data.
    /// A KChunkPointer is a KindPointer of value ContainerKind::Chunk
    /// Figure: |#####|
    /// Layout: Kind, Structure*, void**CDA, NodeCount
    /// </summary>
    using KChunk = NiT::KChunkT<ChunkStructure>;

    /// <summary>
    /// A BucketPointer container of components for one(1) chunk and multiple nodes. 
    /// A BucketPointer does not own its component data.
    /// A BucketPointer has a fixed NodeCapacity of allocated nodes and a variable NodeCount of constructed nodes stating at index 0 that cannot overflow the capacity.
    /// A KBucketPointer is a KindPointer of value ContainerKind::Chunk
    /// Figure: |###__|
    /// Layout: Kind, Structure*, void**CDA, NodeCount, NodeCapacity
    /// </summary>
    using KBucketPointer = NiT::KBucketPointerT<ChunkStructure>;

    /// <summary>
    /// A Bucket container of components for one(1) chunk and multiple nodes. 
    /// A Bucket owns its component data.
    /// A Bucket has a fixed NodeCapacity of allocated nodes and a variable NodeCount of constructed nodes stating at index 0 that cannot overflow the capacity.
    /// A KBucket is a KindPointer of value ContainerKind::Chunk
    /// Figure: |###__|
    /// Layout: Kind, Structure*, void**CDA, NodeCount, NodeCapacity
    /// </summary>
    using KBucket = NiT::KBucketT<ChunkStructure>;

    /// <summary>
    /// A BunchPointer container of components for one(1) chunk and multiple nodes. 
    /// A BunchPointer does not own its component data.
    /// A BunchPointer has a variable NodeCapacity of allocated nodes that grows when needed to fit a smaller or equal number of (NodeCount) constructed nodes. 
    /// A KBunchPointer is a KindPointer of value ContainerKind::Chunk
    /// Figure: |###__|...
    /// Layout: Kind, Structure*, void**CDA, NodeCount, NodeCapacity
    /// </summary>
    using KBunchPointer = NiT::KBunchPointerT<ChunkStructure>;

    /// <summary>
    /// A Bunch container of components for one(1) chunk and multiple nodes. 
    /// A Bunch owns its component data.
    /// A Bunch has a variable NodeCapacity of allocated nodes that grows when needed to fit a smaller or equal number of (NodeCount) constructed nodes. 
    /// A KBunch is a KindPointer of value ContainerKind::Chunk
    /// Figure: |###__|...
    /// Layout: Kind, Structure*, void**CDA, NodeCount, NodeCapacity
    /// </summary>
    using KBunch = NiT::KBunchT<ChunkStructure>;

    /// <summary>
    /// An ArrayPointer container of components for multiple chunks and multiple nodes.
    /// An ArrayPointer does not own its component data.
    /// An ArrayPointer has a fixed ChunkCount of allocated and constructed chunks, and a total number (NodeCount) of allocated and constructed nodes.
    /// A KArrayPointer is a KindPointer of value ContainerKind::Array
    /// Figure: |####|###|######|
    /// Layout: Kind, Structure*, void**CDA, NodeCount, ChunkPointer*, ChunkCount
    /// </summary>
    using KArrayPointer = NiT::KArrayPointerT<ChunkStructure, NChunkPointer>;

    /// <summary>
    /// An Array container of components for multiple chunks and multiple nodes.
    /// An Array own its component data.
    /// An Array has a fixed ChunkCount of allocated and constructed chunks, and a total number (NodeCount) of allocated and constructed nodes.
    /// A KArray is a KindPointer of value ContainerKind::ChunkArray
    /// Figure: |####|###|######|
    /// Layout: Kind, Structure*, void**CDA, NodeCount, ChunkPointer*, ChunkCount
    /// </summary>
    using KArray = NiT::KArrayT<ChunkStructure, NChunkPointer>;

    /// <summary>
    /// A KTreePointer has a Parent/Children set of pointers to other KindPointer containers.
    /// </summary>
    using KTreePointer = NiT::KTreePointerT<ChunkStructure>;

    /// <summary>
    /// A KChunkTreePointer container of components for one(1) chunk and multiple nodes. 
    /// A KChunkTreePointer does not own its component data.
    /// A KChunkTreePointer is a KindPointer of value ContainerKind::ChunkTree
    /// A KChunkTreePointer has a Parent/Children set of pointers to other KindPointer containers.
    /// Figure: |#####|
    /// Layout: Kind, Parent*, FirstChild*, PrevivousSibling*, NextSibling*, Structure*, void**CDA, NodeCount
    /// </summary>
    using KChunkTreePointer = NiT::KChunkTreePointerT<ChunkStructure>;

    /// <summary>
    /// A KChunkTree container of components for one(1) chunk and multiple nodes. 
    /// A KChunkTree owns its component data.
    /// A KChunkTree is a KindPointer of value ContainerKind::ChunkTree
    /// A KChunkTree has a Parent/Children set of pointers to other KindPointer containers.
    /// Figure: |#####|
    /// Layout: Kind, 
    ///         Parent*, FirstChild*, PrevivousSibling*, NextSibling*, 
    ///         Structure*, void**CDA, NodeCount
    /// </summary>
    using KChunkTree = NiT::KChunkTreeT<ChunkStructure>;

    /// <summary>
    /// A KBucketTreePointer container of components for one(1) chunk and multiple nodes. 
    /// A KBucketTreePointer does not own its component data.
    /// A KBucketTreePointer has a fixed NodeCapacity of allocated nodes and a variable NodeCount of constructed nodes stating at index 0 that cannot overflow the capacity.
    /// A KBucketTreePointer is a KindPointer of value ContainerKind::ChunkTree
    /// A KBucketTreePointer has a Parent/Children set of pointers to other KindPointer containers.
    /// Figure: |###__|
    /// Layout: Kind, 
    ///         Parent*, FirstChild*, PrevivousSibling*, NextSibling*, 
    ///         Structure*, void**CDA, NodeCount, NodeCapacity
    /// </summary>
    using KBucketTreePointer = NiT::KBucketTreePointerT<ChunkStructure>;

    /// <summary>
    /// A KBucketTree container of components for one(1) chunk and multiple nodes. 
    /// A KBucketTree owns its component data.
    /// A KBucketTree has a fixed NodeCapacity of allocated nodes and a variable NodeCount of constructed nodes stating at index 0 that cannot overflow the capacity.
    /// A KBucketTree is a KindPointer of value ContainerKind::ChunkTree
    /// A KBucketTree has a Parent/Children set of pointers to other KindPointer containers.
    /// Figure: |###__|
    /// Layout: Kind, 
    ///         Parent*, FirstChild*, PrevivousSibling*, NextSibling*, 
    ///         Structure*, void**CDA, NodeCount, NodeCapacity
    /// </summary>
    using KBucketTree = NiT::KBucketTreeT<ChunkStructure>;

    /// <summary>
    /// A KBunchTreePointer container of components for one(1) chunk and multiple nodes. 
    /// A KBunchTreePointer does not own its component data.
    /// A KBunchTreePointer has a variable NodeCapacity of allocated nodes that grows when needed to fit a smaller or equal number of (NodeCount) constructed nodes. 
    /// A KBunchTreePointer is a KindPointer of value ContainerKind::ChunkTree
    /// A KBunchTreePointer has a Parent/Children set of pointers to other KindPointer containers.
    /// Figure: |###__|...
    /// Layout: Kind, 
    ///         Parent*, FirstChild*, PrevivousSibling*, NextSibling*, 
    ///         Structure*, void**CDA, NodeCount, NodeCapacity
    /// </summary>
    using KBunchTreePointer = NiT::KBunchTreePointerT<ChunkStructure>;

    /// <summary>
    /// A KBunchTree container of components for one(1) chunk and multiple nodes. 
    /// A KBunchTree owns its component data.
    /// A KBunchTree has a variable NodeCapacity of allocated nodes that grows when needed to fit a smaller or equal number of (NodeCount) constructed nodes. 
    /// A KBunchTree is a KindPointer of value ContainerKind::ChunkTree
    /// A KBunchTree has a Parent/Children set of pointers to other KindPointer containers.
    /// Figure: |###__|...
    /// Layout: Kind, 
    ///         Parent*, FirstChild*, PrevivousSibling*, NextSibling*, 
    ///         Structure*, void**CDA, NodeCount, NodeCapacity
    /// </summary>
    using KBunchTree = NiT::KBunchTreeT<ChunkStructure>;

    /// <summary>
    /// A KArrayTreePointer container of components for multiple chunks and multiple nodes.
    /// A KArrayTreePointer does not own its component data.
    /// A KArrayTreePointer has a fixed ChunkCount of allocated and constructed chunks, and a total number (NodeCount) of allocated and constructed nodes.
    /// A KArrayTreePointer is a KindPointer of value ContainerKind::ArrayTree
    /// A KArrayTreePointer has a Parent/Children set of pointers to other KindPointer containers.
    /// Figure: |####|###|######|
    /// Layout: Kind, 
    ///         Parent*, FirstChild*, PrevivousSibling*, NextSibling*, 
    ///         Structure*, void**CDA, NodeCount, ChunkPointer*, ChunkCount
    /// </summary>
    using KArrayTreePointer = NiT::KArrayTreePointerT<ChunkStructure, NChunkPointer>;

    /// <summary>
    /// A KArrayTree container of components for multiple chunks and multiple nodes.
    /// A KArrayTree owns its component data.
    /// A KArrayTree has a fixed ChunkCount of allocated and constructed chunks, and a total number (NodeCount) of allocated and constructed nodes.
    /// A KArrayTree is a KindPointer of value ContainerKind::ArrayTree
    /// A KArrayTree has a Parent/Children set of pointers to other KindPointer containers.
    /// Figure: |####|###|######|
    /// Layout: Kind, 
    ///         Parent*, FirstChild*, PrevivousSibling*, NextSibling*, 
    ///         Structure*, void**CDA, NodeCount, ChunkPointer*, ChunkCount
    /// </summary>
    using KArrayTree = NiT::KArrayTreeT<ChunkStructure, NChunkPointer>;

    // TODO complete
    using KBucketBarrelTree = NiT::KBucketBarrelTreeT<ChunkStructure, NChunkPointer>;

    // TODO Move to algo file
    using NiT::Algorithm;

    /// <summary>
    /// Caches the component data routing indices for each chunk structure the algorithm is executed with.
    /// Use in your pipelines to avoid re-routing each component for all chunks of the same structure.
    /// May improve performance, especially for large structures.
    /// </summary>
    /// <typeparam name="TAlgorithm"></typeparam>
    template<typename TAlgorithm>
    using AlgorithmCacheRouter = NiT::Routing::AlgorithmCacheRouterT<TAlgorithm, ChunkStructure, Size_t>;

    /// <summary>
    /// An AlgorithmRouter equivalent of AlgorithmCacheRouter that does not cache component data routing indices.
    /// Use in your pipeline as a replacement of AlgorithmCacheRouter
    /// </summary>
    /// <typeparam name="TAlgorithm"></typeparam>
    template<typename TAlgorithm>
    using AlgorithmRouter = NiT::Routing::AlgorithmRouterT<TAlgorithm>;

    /// <summary>
    /// Extend this struct to implement a custom pipeline.
    /// A pipeline is responsible to dispatch the execution of various algorithm on incoming containers
    /// </summary>
    /// <typeparam name="TPipeline"></typeparam>
    template<typename TPipeline>
    using Pipeline = NiT::PipelineT<TPipeline, ChunkStructure, Size_t>;

    using CoParentInChunk = NiT::CoParentInChunkT<Size_t>;
    using CoSingleParentOutsideChunk = NiT::CoSingleParentOutsideChunkT<Size_t>;
    using CoChildrenInChunk = NiT::CoChildrenInChunkT<Size_t>;

    template<typename TIn> NiT::NodeCapacityT        <Size_t> PropNodeCapacity        (const TIn& value) { return NiT::PropNodeCapacityT        <Size_t, TIn>(value); }
    template<typename TIn> NiT::NodeCountT           <Size_t> PropNodeCount           (const TIn& value) { return NiT::PropNodeCountT           <Size_t, TIn>(value); }
    template<typename TIn> NiT::ChunkCapacityT       <Size_t> PropChunkCapacity       (const TIn& value) { return NiT::PropChunkCapacityT       <Size_t, TIn>(value); }
    template<typename TIn> NiT::ChunkCountT          <Size_t> PropChunkCount          (const TIn& value) { return NiT::PropChunkCountT          <Size_t, TIn>(value); }
    template<typename TIn> NiT::NodeCapacityPerChunkT<Size_t> PropNodeCapacityPerChunk(const TIn& value) { return NiT::PropNodeCapacityPerChunkT<Size_t, TIn>(value); }
    template<typename TIn> NiT::NodeCountPerChunkT   <Size_t> PropNodeCountPerChunk   (const TIn& value) { return NiT::PropNodeCountPerChunkT   <Size_t, TIn>(value); }

    using NodeCapacity =         NiT::NodeCapacityT<        Size_t>;
    using NodeCount =            NiT::NodeCountT<           Size_t>;
    using ChunkCapacity =        NiT::ChunkCapacityT<       Size_t>;
    using ChunkCount =           NiT::ChunkCountT<          Size_t>;
    using NodeCapacityPerChunk = NiT::NodeCapacityPerChunkT<Size_t>;
    using NodeCountPerChunk =    NiT::NodeCountPerChunkT<   Size_t>;


    // TODO experimental/remove
    template<typename TComponent>
    using ComponentPointer = NiT::ComponentPointerT<ChunkStructure, TComponent>;

    // TODO experimental/remove
    template<typename TComponent>
    using ComponentPointerRO = NiT::ComponentPointerT<ChunkStructure, TComponent>;

}

//
//namespace Ni::Containers
//{
//    using Size_t = int32;
//    using ComponentType = NiT::ComponentTypeT<Size_t>;
//    using ComponentTypeSet = NiT::ComponentTypeSetT<Size_t>;
//
//    /// <summary>
//    /// Define who owns a component
//    /// </summary>
//    using NiT::ComponentOwner;
//
//    /// <summary>
//    /// Extend this type to create a component owned by nodes in a chunks.
//    /// Each node in a chunk will have exactly one(1) instance of all their NodeComponents.
//    /// </summary>
//    using NiT::NodeComponent;
//
//    /// <summary>
//    /// Extend this type to create a component owned by chunks.
//    /// Each chunk with this component will have exactly one(1) instance of all their ChunkComponents
//    /// </summary>
//    using NiT::ChunkComponent;
//
//    /// <summary>
//    /// Defines the list of component a container has.
//    /// </summary>
//    using ChunkStructure = NiT::ChunkStructureT<Size_t>;
//
//    /// <summary>
//    /// A ChunkPointer container of components for one(1) chunk and multiple nodes. 
//    /// A ChunkPointer does not own its component data.
//    /// Figure: |#####|
//    /// Layout: Structure*, void**CDA, NodeCount
//    /// </summary>
//    using NChunkPointer = NiT::ChunkPointerT<ChunkStructure>;
//
//    /// <summary>
//    /// A Chunk container of components for one(1) chunk and multiple nodes. 
//    /// A Chunk owns its component data.
//    /// Figure: |#####|
//    /// Layout: Structure*, void**CDA, NodeCount
//    /// </summary>
//    using NChunk = NiT::ChunkT<ChunkStructure>;
//
//    /// <summary>
//    /// A BucketPointer container of components for one(1) chunk and multiple nodes. 
//    /// A BucketPointer does not own its component data.
//    /// A BucketPointer has a fixed NodeCapacity of allocated nodes and a variable NodeCount of constructed nodes stating at index 0 that cannot overflow the capacity.
//    /// Figure: |###__|
//    /// Layout: Structure*, void**CDA, NodeCount, NodeCapacity
//    /// </summary>
//    using NBucketPointer = NiT::BucketPointerT<ChunkStructure>;
//
//    /// <summary>
//    /// A Bucket container of components for one(1) chunk and multiple nodes. 
//    /// A Bucket owns its component data.
//    /// A Bucket has a fixed NodeCapacity of allocated nodes and a variable NodeCount of constructed nodes stating at index 0 that cannot overflow the capacity.
//    /// Figure: |###__|
//    /// Layout: Structure*, void**CDA, NodeCount, NodeCapacity
//    /// </summary>
//    using NBucket = NiT::BucketT<ChunkStructure>;
//
//    /// <summary>
//    /// A BunchPointer container of components for one(1) chunk and multiple nodes. 
//    /// A BunchPointer does not own its component data.
//    /// A BunchPointer has a variable NodeCapacity of allocated nodes that grows when needed to fit a smaller or equal number of (NodeCount) constructed nodes. 
//    /// Figure: |###__|...
//    /// </summary>
//    using NBunchPointer = NiT::BunchPointerT<ChunkStructure>;
//
//    /// <summary>
//    /// A Bunch container of components for one(1) chunk and multiple nodes. 
//    /// A Bunch owns its component data.
//    /// A Bunch has a variable NodeCapacity of allocated nodes that grows when needed to fit a smaller or equal number of (NodeCount) constructed nodes. 
//    /// Figure: |###__|...
//    /// Layout: Structure*, void**CDA, NodeCount, NodeCapacity
//    /// </summary>
//    using NBunch = NiT::BunchT<ChunkStructure>;
//
//    /// <summary>
//    /// An ArrayPointer container of components for multiple chunks and multiple nodes.
//    /// An ArrayPointer does not own its component data.
//    /// An ArrayPointer has a fixed ChunkCount of allocated and constructed chunks, and a total number (NodeCount) of allocated and constructed nodes.
//    /// Figure: |####|###|######|
//    /// Layout: Structure*, void**CDA, NodeCount, NodeCapacity
//    /// </summary>
//    using NArrayPointer = NiT::ArrayPointerT<ChunkStructure, NChunkPointer>;
//
//    /// <summary>
//    /// An Array container of components for multiple chunks and multiple nodes.
//    /// An Array owns its component data.
//    /// An Array has a fixed ChunkCount of allocated and constructed chunks, and a total number (NodeCount) of allocated and constructed nodes.
//    /// Figure: |####|###|######|
//    /// Layout: Structure*, void**CDA, NodeCount, ChunkPointer*, ChunkCount
//    /// </summary>
//    using NArray = NiT::ArrayT<ChunkStructure, NChunkPointer>;
//
//    /// <summary>
//    /// A UniformArray container of components for multiple chunks and multiple nodes.
//    /// A UniformArray owns its component data.
//    /// A UniformArray has a fixed ChunkCount of allocated and constructed chunks, and a total number (NodeCount) of allocated and constructed nodes.
//    /// Each chunk elements in the UniformArray has the same NodeCount.
//    /// Figure: |###|###|###|###|
//    /// Layout: Structure*, void**CDA, NodeCount, ChunkPointer*, ChunkCount
//    /// </summary>
//    using NUniformArray = NiT::UniformArrayT<ChunkStructure, NChunkPointer>;
//
//    /// <summary>
//    /// A KindPointer has a Kind data field usable to identify what kind of container follows the Kind data field in memory.
//    /// Layout: Kind
//    /// </summary>
//    using KindPointer = NiT::KindPointerT<ChunkStructure>;
//
//    /// <summary>
//    /// A ChunkPointer container of components for one(1) chunk and multiple nodes. 
//    /// A ChunkPointer does not own its component data.
//    /// A KChunkPointer is a KindPointer of value ContainerKind::Chunk
//    /// Figure: |#####|
//    /// Layout: Kind, Structure*, void**CDA, NodeCount
//    /// </summary>
//    using KChunkPointer = NiT::KChunkPointerT<ChunkStructure>;
//
//    /// <summary>
//    /// A ChunkPointer container of components for one(1) chunk and multiple nodes. 
//    /// A ChunkPointer owns its component data.
//    /// A KChunkPointer is a KindPointer of value ContainerKind::Chunk
//    /// Figure: |#####|
//    /// Layout: Kind, Structure*, void**CDA, NodeCount
//    /// </summary>
//    using KChunk = NiT::KChunkT<ChunkStructure>;
//
//    /// <summary>
//    /// A BucketPointer container of components for one(1) chunk and multiple nodes. 
//    /// A BucketPointer does not own its component data.
//    /// A BucketPointer has a fixed NodeCapacity of allocated nodes and a variable NodeCount of constructed nodes stating at index 0 that cannot overflow the capacity.
//    /// A KBucketPointer is a KindPointer of value ContainerKind::Chunk
//    /// Figure: |###__|
//    /// Layout: Kind, Structure*, void**CDA, NodeCount, NodeCapacity
//    /// </summary>
//    using KBucketPointer = NiT::KBucketPointerT<ChunkStructure>;
//
//    /// <summary>
//    /// A Bucket container of components for one(1) chunk and multiple nodes. 
//    /// A Bucket owns its component data.
//    /// A Bucket has a fixed NodeCapacity of allocated nodes and a variable NodeCount of constructed nodes stating at index 0 that cannot overflow the capacity.
//    /// A KBucket is a KindPointer of value ContainerKind::Chunk
//    /// Figure: |###__|
//    /// Layout: Kind, Structure*, void**CDA, NodeCount, NodeCapacity
//    /// </summary>
//    using KBucket = NiT::KBucketT<ChunkStructure>;
//
//    /// <summary>
//    /// A BunchPointer container of components for one(1) chunk and multiple nodes. 
//    /// A BunchPointer does not own its component data.
//    /// A BunchPointer has a variable NodeCapacity of allocated nodes that grows when needed to fit a smaller or equal number of (NodeCount) constructed nodes. 
//    /// A KBunchPointer is a KindPointer of value ContainerKind::Chunk
//    /// Figure: |###__|...
//    /// Layout: Kind, Structure*, void**CDA, NodeCount, NodeCapacity
//    /// </summary>
//    using KBunchPointer = NiT::KBunchPointerT<ChunkStructure>;
//
//    /// <summary>
//    /// A Bunch container of components for one(1) chunk and multiple nodes. 
//    /// A Bunch owns its component data.
//    /// A Bunch has a variable NodeCapacity of allocated nodes that grows when needed to fit a smaller or equal number of (NodeCount) constructed nodes. 
//    /// A KBunch is a KindPointer of value ContainerKind::Chunk
//    /// Figure: |###__|...
//    /// Layout: Kind, Structure*, void**CDA, NodeCount, NodeCapacity
//    /// </summary>
//    using KBunch = NiT::KBunchT<ChunkStructure>;
//
//    /// <summary>
//    /// An ArrayPointer container of components for multiple chunks and multiple nodes.
//    /// An ArrayPointer does not own its component data.
//    /// An ArrayPointer has a fixed ChunkCount of allocated and constructed chunks, and a total number (NodeCount) of allocated and constructed nodes.
//    /// A KArrayPointer is a KindPointer of value ContainerKind::Array
//    /// Figure: |####|###|######|
//    /// Layout: Kind, Structure*, void**CDA, NodeCount, ChunkPointer*, ChunkCount
//    /// </summary>
//    using KArrayPointer = NiT::KArrayPointerT<ChunkStructure, NChunkPointer>;
//
//    /// <summary>
//    /// An Array container of components for multiple chunks and multiple nodes.
//    /// An Array own its component data.
//    /// An Array has a fixed ChunkCount of allocated and constructed chunks, and a total number (NodeCount) of allocated and constructed nodes.
//    /// A KArray is a KindPointer of value ContainerKind::ChunkArray
//    /// Figure: |####|###|######|
//    /// Layout: Kind, Structure*, void**CDA, NodeCount, ChunkPointer*, ChunkCount
//    /// </summary>
//    using KArray = NiT::KArrayT<ChunkStructure, NChunkPointer>;
//
//    /// <summary>
//    /// A KTreePointer has a Parent/Children set of pointers to other KindPointer containers.
//    /// </summary>
//    using KTreePointer = NiT::KTreePointerT<ChunkStructure>;
//
//    /// <summary>
//    /// A KChunkTreePointer container of components for one(1) chunk and multiple nodes. 
//    /// A KChunkTreePointer does not own its component data.
//    /// A KChunkTreePointer is a KindPointer of value ContainerKind::ChunkTree
//    /// A KChunkTreePointer has a Parent/Children set of pointers to other KindPointer containers.
//    /// Figure: |#####|
//    /// Layout: Kind, Parent*, FirstChild*, PrevivousSibling*, NextSibling*, Structure*, void**CDA, NodeCount
//    /// </summary>
//    using KChunkTreePointer = NiT::KChunkTreePointerT<ChunkStructure>;
//
//    /// <summary>
//    /// A KChunkTree container of components for one(1) chunk and multiple nodes. 
//    /// A KChunkTree owns its component data.
//    /// A KChunkTree is a KindPointer of value ContainerKind::ChunkTree
//    /// A KChunkTree has a Parent/Children set of pointers to other KindPointer containers.
//    /// Figure: |#####|
//    /// Layout: Kind, 
//    ///         Parent*, FirstChild*, PrevivousSibling*, NextSibling*, 
//    ///         Structure*, void**CDA, NodeCount
//    /// </summary>
//    using KChunkTree = NiT::KChunkTreeT<ChunkStructure>;
//
//    /// <summary>
//    /// A KBucketTreePointer container of components for one(1) chunk and multiple nodes. 
//    /// A KBucketTreePointer does not own its component data.
//    /// A KBucketTreePointer has a fixed NodeCapacity of allocated nodes and a variable NodeCount of constructed nodes stating at index 0 that cannot overflow the capacity.
//    /// A KBucketTreePointer is a KindPointer of value ContainerKind::ChunkTree
//    /// A KBucketTreePointer has a Parent/Children set of pointers to other KindPointer containers.
//    /// Figure: |###__|
//    /// Layout: Kind, 
//    ///         Parent*, FirstChild*, PrevivousSibling*, NextSibling*, 
//    ///         Structure*, void**CDA, NodeCount, NodeCapacity
//    /// </summary>
//    using KBucketTreePointer = NiT::KBucketTreePointerT<ChunkStructure>;
//
//    /// <summary>
//    /// A KBucketTree container of components for one(1) chunk and multiple nodes. 
//    /// A KBucketTree owns its component data.
//    /// A KBucketTree has a fixed NodeCapacity of allocated nodes and a variable NodeCount of constructed nodes stating at index 0 that cannot overflow the capacity.
//    /// A KBucketTree is a KindPointer of value ContainerKind::ChunkTree
//    /// A KBucketTree has a Parent/Children set of pointers to other KindPointer containers.
//    /// Figure: |###__|
//    /// Layout: Kind, 
//    ///         Parent*, FirstChild*, PrevivousSibling*, NextSibling*, 
//    ///         Structure*, void**CDA, NodeCount, NodeCapacity
//    /// </summary>
//    using KBucketTree = NiT::KBucketTreeT<ChunkStructure>;
//
//    /// <summary>
//    /// A KBunchTreePointer container of components for one(1) chunk and multiple nodes. 
//    /// A KBunchTreePointer does not own its component data.
//    /// A KBunchTreePointer has a variable NodeCapacity of allocated nodes that grows when needed to fit a smaller or equal number of (NodeCount) constructed nodes. 
//    /// A KBunchTreePointer is a KindPointer of value ContainerKind::ChunkTree
//    /// A KBunchTreePointer has a Parent/Children set of pointers to other KindPointer containers.
//    /// Figure: |###__|...
//    /// Layout: Kind, 
//    ///         Parent*, FirstChild*, PrevivousSibling*, NextSibling*, 
//    ///         Structure*, void**CDA, NodeCount, NodeCapacity
//    /// </summary>
//    using KBunchTreePointer = NiT::KBunchTreePointerT<ChunkStructure>;
//
//    /// <summary>
//    /// A KBunchTree container of components for one(1) chunk and multiple nodes. 
//    /// A KBunchTree owns its component data.
//    /// A KBunchTree has a variable NodeCapacity of allocated nodes that grows when needed to fit a smaller or equal number of (NodeCount) constructed nodes. 
//    /// A KBunchTree is a KindPointer of value ContainerKind::ChunkTree
//    /// A KBunchTree has a Parent/Children set of pointers to other KindPointer containers.
//    /// Figure: |###__|...
//    /// Layout: Kind, 
//    ///         Parent*, FirstChild*, PrevivousSibling*, NextSibling*, 
//    ///         Structure*, void**CDA, NodeCount, NodeCapacity
//    /// </summary>
//    using KBunchTree = NiT::KBunchTreeT<ChunkStructure>;
//
//    /// <summary>
//    /// A KArrayTreePointer container of components for multiple chunks and multiple nodes.
//    /// A KArrayTreePointer does not own its component data.
//    /// A KArrayTreePointer has a fixed ChunkCount of allocated and constructed chunks, and a total number (NodeCount) of allocated and constructed nodes.
//    /// A KArrayTreePointer is a KindPointer of value ContainerKind::ArrayTree
//    /// A KArrayTreePointer has a Parent/Children set of pointers to other KindPointer containers.
//    /// Figure: |####|###|######|
//    /// Layout: Kind, 
//    ///         Parent*, FirstChild*, PrevivousSibling*, NextSibling*, 
//    ///         Structure*, void**CDA, NodeCount, ChunkPointer*, ChunkCount
//    /// </summary>
//    using KArrayTreePointer = NiT::KArrayTreePointerT<ChunkStructure, NChunkPointer>;
//
//    /// <summary>
//    /// A KArrayTree container of components for multiple chunks and multiple nodes.
//    /// A KArrayTree owns its component data.
//    /// A KArrayTree has a fixed ChunkCount of allocated and constructed chunks, and a total number (NodeCount) of allocated and constructed nodes.
//    /// A KArrayTree is a KindPointer of value ContainerKind::ArrayTree
//    /// A KArrayTree has a Parent/Children set of pointers to other KindPointer containers.
//    /// Figure: |####|###|######|
//    /// Layout: Kind, 
//    ///         Parent*, FirstChild*, PrevivousSibling*, NextSibling*, 
//    ///         Structure*, void**CDA, NodeCount, ChunkPointer*, ChunkCount
//    /// </summary>
//    using KArrayTree = NiT::KArrayTreeT<ChunkStructure, NChunkPointer>;
//
//    // TODO complete
//    using KBucketBarrelTree = NiT::KBucketBarrelTreeT<ChunkStructure, NChunkPointer>;
//
//    // TODO Move to algo file
//    using NiT::Algorithm;
//
//    /// <summary>
//    /// Caches the component data routing indices for each chunk structure the algorithm is executed with.
//    /// Use in your pipelines to avoid re-routing each component for all chunks of the same structure.
//    /// May improve performance, especially for large structures.
//    /// </summary>
//    /// <typeparam name="TAlgorithm"></typeparam>
//    template<typename TAlgorithm>
//    using AlgorithmCacheRouter = NiT::Routing::AlgorithmCacheRouterT<TAlgorithm, ChunkStructure, Size_t>;
//
//    /// <summary>
//    /// An AlgorithmRouter equivalent of AlgorithmCacheRouter that does not cache component data routing indices.
//    /// Use in your pipeline as a replacement of AlgorithmCacheRouter
//    /// </summary>
//    /// <typeparam name="TAlgorithm"></typeparam>
//    template<typename TAlgorithm>
//    using AlgorithmRouter = NiT::Routing::AlgorithmRouterT<TAlgorithm>;
//
//    /// <summary>
//    /// Extend this struct to implement a custom pipeline.
//    /// A pipeline is responsible to dispatch the execution of various algorithm on incoming containers
//    /// </summary>
//    /// <typeparam name="TPipeline"></typeparam>
//    template<typename TPipeline>
//    using Pipeline = NiT::PipelineT<TPipeline, ChunkStructure, Size_t>;
//
//    using CoParentInChunk = NiT::CoParentInChunkT<Size_t>;
//    using CoSingleParentOutsideChunk = NiT::CoSingleParentOutsideChunkT<Size_t>;
//    using CoChildrenInChunk = NiT::CoChildrenInChunkT<Size_t>;
//
//    template<typename TIn> NiT::NodeCapacityT        <Size_t> PropNodeCapacity        (const TIn& value) { return NiT::PropNodeCapacityT        <Size_t, TIn>(value); }
//    template<typename TIn> NiT::NodeCountT           <Size_t> PropNodeCount           (const TIn& value) { return NiT::PropNodeCountT           <Size_t, TIn>(value); }
//    template<typename TIn> NiT::ChunkCapacityT       <Size_t> PropChunkCapacity       (const TIn& value) { return NiT::PropChunkCapacityT       <Size_t, TIn>(value); }
//    template<typename TIn> NiT::ChunkCountT          <Size_t> PropChunkCount          (const TIn& value) { return NiT::PropChunkCountT          <Size_t, TIn>(value); }
//    template<typename TIn> NiT::NodeCapacityPerChunkT<Size_t> PropNodeCapacityPerChunk(const TIn& value) { return NiT::PropNodeCapacityPerChunkT<Size_t, TIn>(value); }
//    template<typename TIn> NiT::NodeCountPerChunkT   <Size_t> PropNodeCountPerChunk   (const TIn& value) { return NiT::PropNodeCountPerChunkT   <Size_t, TIn>(value); }
//
//    using NodeCapacity =         NiT::NodeCapacityT<        Size_t>;
//    using NodeCount =            NiT::NodeCountT<           Size_t>;
//    using ChunkCapacity =        NiT::ChunkCapacityT<       Size_t>;
//    using ChunkCount =           NiT::ChunkCountT<          Size_t>;
//    using NodeCapacityPerChunk = NiT::NodeCapacityPerChunkT<Size_t>;
//    using NodeCountPerChunk =    NiT::NodeCountPerChunkT<   Size_t>;
//
//
//    // TODO experimental/remove
//    template<typename TComponent>
//    using ComponentPointer = NiT::ComponentPointerT<ChunkStructure, TComponent>;
//
//    // TODO experimental/remove
//    template<typename TComponent>
//    using ComponentPointerRO = NiT::ComponentPointerT<ChunkStructure, TComponent>;
//
//}

namespace NiT
{
    template<> struct PropTraits<const Ni::Containers::ChunkStructure*> : public PropTraitsDefault2<Ni::Containers::ChunkStructure, const Ni::Containers::ChunkStructure*, DStructurePtr> {};
}