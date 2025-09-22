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
#include "ChunkAllocation.h"
#include "ChunkArrayPointer.h"
#include "ChunkArrayAllocation.h"
#include "KindPointer.h"
#include "KChunkArrayPointer.h"
#include "Algorithm.h"
#include "Pipeline.h"
#include "Components.h"
#include "routing\AlgorithmRouter.h"
#include "routing\AlgorithmCacheRouter.h"
#include "KindPointer.inl.h"

namespace PNC
{
    using Size_t = int32;
    using ComponentType = ComponentTypeT<Size_t>;
    using ComponentTypeSet = ComponentTypeSetT<Size_t>;
    using ChunkStructure = ChunkStructureT<Size_t>;

    using ChunkPointer = ChunkPointerT<ChunkStructure>;
    using Chunk = ChunkAllocationT<ChunkPointerT<ChunkStructure>>;

    using ChunkArrayPointer = ChunkArrayPointerT<ChunkStructure, ChunkPointer>;
    using ChunkArray = ChunkArrayAllocationT<ChunkArrayPointer>;

    using KChunkTreePointer = KChunkTreePointerT<ChunkStructure>;
    using KChunkTree = ChunkAllocationT<KChunkTreePointer>;

    using KChunkArrayTreePointer = KChunkArrayTreePointerT<ChunkStructure, ChunkPointer>;
    using KChunkArrayTree = ChunkArrayAllocationT< KChunkArrayTreePointer>;

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