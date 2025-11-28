// MIT License
// Copyright (c) 2025 Stephanie Rancourt

#pragma once
#include "common.h"
#include "Containers.h"

//#include "routing\AlgorithmRouter.h"
//#include "routing\AlgorithmCacheRouter.h"
#include "Algorithm.h"
#include "AlgorithmRunnerChunk.h"
#include "AlgorithmRunnerChunkArray.h"

namespace PNC
{

    template<typename TChunkStructure, typename TAlgorithm>
    struct AlgorithmRunner< TChunkStructure, TAlgorithm, ChunkT<TChunkStructure>>
        : public AlgorithmRunnerChunk<TAlgorithm, ChunkPointerT<TChunkStructure>> {
    };


    template<typename TChunkStructure, typename TAlgorithm>
    struct AlgorithmRunner< TChunkStructure, TAlgorithm, BucketPointerT<TChunkStructure>> :
        public AlgorithmRunnerChunk<TAlgorithm, ChunkPointerT<TChunkStructure>> {
    };



    template<typename TChunkStructure, typename TAlgorithm>
    struct AlgorithmRunner< TChunkStructure, TAlgorithm, BucketT<TChunkStructure>>
        : public AlgorithmRunnerChunk<TAlgorithm, ChunkPointerT<TChunkStructure>> {
    };

    template<typename TChunkStructure, typename TAlgorithm>
    struct AlgorithmRunner< TChunkStructure, TAlgorithm, BunchPointerT<TChunkStructure>> :
        public AlgorithmRunnerChunk<TAlgorithm, ChunkPointerT<TChunkStructure>> {
    };


    template<typename TChunkStructure, typename TAlgorithm>
    struct AlgorithmRunner< TChunkStructure, TAlgorithm, BunchT<TChunkStructure>>
        : public AlgorithmRunnerChunk<TAlgorithm, ChunkPointerT<TChunkStructure>> {
    };


    template<typename TChunkStructure, typename TAlgorithm, typename TChunkPointerElement>
    struct AlgorithmRunner< TChunkStructure, TAlgorithm, UniformChunkArrayT<TChunkStructure, TChunkPointerElement>>
        : public AlgorithmRunnerChunkArray<TAlgorithm, ChunkArrayPointerT<TChunkStructure, TChunkPointerElement>> {
    };
}
