// MIT License
// Copyright (c) 2025 Stephanie Rancourt

#pragma once
#include "common.h"
#include "AlgorithmRunnerChunk.h"
#include "AlgorithmRunnerChunkArray.h"

namespace PNC
{
    /// <summary>
    /// Execute an algorithm on a KindPointer Chunk
    /// It will retreive the pointed Chunk according to the kind of KindPointer and
    /// execute the algorithm using the appropriate AlgorithmRunner Chunk/ChunkArray
    /// </summary>
    /// <typeparam name="TChunkStructure"></typeparam>
    /// <typeparam name="TAlgorithm"></typeparam>
    template<typename TChunkStructure, typename TAlgorithm>
    struct AlgorithmRunnerKindPointerSwitch
    {
    public:
        using Self_t = AlgorithmRunnerKindPointerSwitch<TChunkStructure, TAlgorithm>;
        using ChunkStructure_t = TChunkStructure;
        using Algorithm_t = TAlgorithm;
        using Size_t = typename ChunkStructure_t::Size_t;
        using KindPointer_t = KindPointerT<ChunkStructure_t>;
        using ChunkPointer_t = ChunkPointerT<ChunkStructure_t>;
        using ChunkArrayPointer_t = ChunkArrayPointerT<ChunkStructure_t, ChunkPointer_t>;
        using KChunkPointer_t = KChunkPointerT<ChunkStructure_t>;
        using KChunkArrayPointer_t = KChunkArrayPointerT<ChunkStructure_t, ChunkPointer_t>;
        using KChunkTreePointer_t = KChunkTreePointerT<ChunkStructure_t>;
        using KChunkArrayTreePointer_t = KChunkArrayTreePointerT<ChunkStructure_t, ChunkPointer_t>;

    public:
        /// <summary>
        /// Route and execute an algorithm on a chunk
        /// </summary>
        /// <param name="algorithm"></param>
        /// <param name="chunkPtr"></param>
        /// <returns></returns>
        static bool TryRun(Algorithm_t& algorithm, KindPointer_t& chunkPtr)
        {
            switch (chunkPtr.Kind)
            {
            case ChunkKind_Chunk:
                return AlgorithmRunnerChunk<Algorithm_t, ChunkPointer_t>::TryRun(algorithm, *(KChunkPointer_t&)chunkPtr);
            case ChunkKind_ChunkArray:
                return AlgorithmRunnerChunkArray<Algorithm_t, ChunkArrayPointer_t>::TryRun(algorithm, *(KChunkArrayPointer_t&)chunkPtr);
            case ChunkKind_ChunkTree:
                return AlgorithmRunnerChunk<Algorithm_t, KChunkTreePointer_t>::TryRun(algorithm, (KChunkTreePointer_t&)chunkPtr);
            case ChunkKind_ChunkArrayTree:
                return AlgorithmRunnerChunkArray<Algorithm_t, KChunkArrayTreePointer_t>::TryRun(algorithm, (KChunkArrayTreePointer_t&)chunkPtr);
            }
            return true;
        }

        /// <summary>
        /// Route using a router and execute an algorithm on a chunk
        /// </summary>
        /// <typeparam name="TRouter"></typeparam>
        /// <param name="router"></param>
        /// <param name="algorithm"></param>
        /// <param name="chunkPtr"></param>
        /// <returns></returns>
        template<typename TRouter>
        static bool TryRun(const TRouter& router, Algorithm_t& algorithm, KindPointer_t& chunkPtr)
        {
            switch (chunkPtr.Kind)
            {
            case ChunkKind_Chunk:
                return AlgorithmRunnerChunk<Algorithm_t, ChunkPointer_t>::TryRun(router, algorithm, *(KChunkPointer_t&)chunkPtr);
            case ChunkKind_ChunkArray:
                return AlgorithmRunnerChunkArray<Algorithm_t, ChunkArrayPointer_t>::TryRun(router, algorithm, *(KChunkArrayPointer_t&)chunkPtr);
            case ChunkKind_ChunkTree:
                return AlgorithmRunnerChunk<Algorithm_t, KChunkTreePointer_t>::TryRun(router, algorithm, (KChunkTreePointer_t&)chunkPtr);
            case ChunkKind_ChunkArrayTree:
                return AlgorithmRunnerChunkArray<Algorithm_t, KChunkArrayTreePointer_t>::TryRun(router, algorithm, (KChunkArrayTreePointer_t&)chunkPtr);
            }
            return true;
        }
    };
}
