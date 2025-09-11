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
#include "ChunkType.h"
#include "Chunk.h"
#include "Algorithm.h"
#include "Pipeline.h"
#include "Components.h"
#include "ChunkTree.h"

namespace PNC
{
    using Size_t = int32;

    using ComponentType = ComponentTypeT<Size_t>;
    using ComponentTypeSet = ComponentTypeSetT<Size_t>;
    using ChunkType = ChunkTypeT<Size_t>;

    using ChunkPointer = ChunkPointerT<ChunkType>;
    using Chunk = ChunkT<ChunkType>;

    using ChunkArrayPointer = ChunkArrayPointerT<ChunkPointer>;
    using ChunkArray = ChunkArrayT<ChunkPointer>;

    using CoParentInChunk = CoParentInChunkT<Size_t>;
    using CoSingleParentOutsideChunk = CoSingleParentOutsideChunkT<Size_t>;
    
    using CoChildrenInChunk = CoChildrenInChunkT<Size_t>;

    template<typename TAlgorithm>
    using AlgorithmRoutingCache = AlgorithmRoutingCacheT<TAlgorithm, ChunkType, Size_t>;

    template<typename TPipeline>
    using Pipeline = PipelineT<TPipeline, ChunkType, Size_t>;

    using CoParentInOrOutsideIndex = CoParentInOrOutsideIndexT<Size_t>;




    /// <summary>
    /// A IdentifiableChunk can be reinterpret_cast-ed to ChunkTreePointer if its kind is ChunkKind_ChunkTree
    /// </summary>
    using ChunkTreePointer = ChunkTreePointerT<ChunkPointer>;

    /// <summary>
    /// A IdentifiableChunk or ChunkTreePointer can be reinterpret_cast-ed to ChunkArrayTreePointer if its kind is ChunkKind_ChunkArrayTree
    /// </summary>
    using ChunkArrayTreePointer = ChunkTreePointerT<ChunkArrayPointer>;


    using ChunkTree = ChunkTreeT<ChunkType>;
    using ChunkArrayTree = ChunkArrayTreeT<ChunkPointer>;

}