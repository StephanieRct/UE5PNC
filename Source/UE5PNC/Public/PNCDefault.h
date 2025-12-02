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
    using ChunkStructure = ChunkStructureT<Size_t>;

    using ChunkPointer = ChunkPointerT<ChunkStructure>;
    using Chunk = ChunkT<ChunkStructure>;
    using BucketPointer = BucketPointerT<ChunkStructure>;
    using Bucket = BucketT<ChunkStructure>;
    using BunchPointer = BunchPointerT<ChunkStructure>;
    using Bunch = BunchT<ChunkStructure>;

    using UniformChunkArray = UniformArrayT<ChunkStructure, ChunkPointer>;
    using ChunkArrayPointer = ArrayPointerT<ChunkStructure, ChunkPointer>;
    using ChunkArray = ArrayT<ChunkStructure, ChunkPointer>;


    using KChunkTree = KChunkTreeT<ChunkStructure>;

    using KChunkPointer = KChunkPointerT<ChunkStructure>;
    using KChunk = KChunkT<ChunkStructure>;
    using KBucket = KBucketT<ChunkStructure>;
    using KBunch = KBunchT<ChunkStructure>;

    using KChunkArrayPointer = KChunkArrayPointerT<ChunkStructure, ChunkPointer>;
    using KChunkArray = KChunkArrayT<ChunkStructure, ChunkPointer>;

    using KChunkTreePointer = KChunkTreePointerT<ChunkStructure>;
    using KChunkTree = KChunkTreeT<ChunkStructure>;
    using KBucketTreePointer = KBucketTreePointerT<ChunkStructure>;
    using KBucketTree = KBucketTreeT<ChunkStructure>;
    using KBunchTreePointer = KBunchTreePointerT<ChunkStructure>;
    using KBunchTree = KBunchTreeT<ChunkStructure>;

    using KChunkArrayTreePointer = KChunkArrayTreePointerT<ChunkStructure, ChunkPointer>;
    using KChunkArrayTree = KChunkArrayTreeT<ChunkStructure, ChunkPointer>;
    using KBucketBarrelTree = KBucketBarrelTreeT<ChunkStructure, ChunkPointer>;
    //KBucketBarrelTree



    template<typename TAlgorithm>
    using AlgorithmRouter = Routing::AlgorithmRouterT<TAlgorithm>;
    template<typename TAlgorithm>
    using AlgorithmCacheRouter = Routing::AlgorithmCacheRouterT<TAlgorithm, ChunkStructure, Size_t>;

    template<typename TPipeline>
    using Pipeline = PipelineT<TPipeline, ChunkStructure, Size_t>;

    using CoParentInChunk = CoParentInChunkT<Size_t>;
    using CoSingleParentOutsideChunk = CoSingleParentOutsideChunkT<Size_t>;
    using CoChildrenInChunk = CoChildrenInChunkT<Size_t>;
}


namespace PNC 
{

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