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
    /// <typeparam name="TChunkType"></typeparam>
    /// <typeparam name="TChunkAlgorithm"></typeparam>
    template<typename TChunkType, typename TChunkAlgorithm>
    struct AlgorithmRunnerKindPointerSwitch
    {
    public:
        using Self_t = AlgorithmRunnerKindPointerSwitch<TChunkType, TChunkAlgorithm>;
        using ChunkType_t = TChunkType;
        using ChunkAlgorithm_t = TChunkAlgorithm;
        using Size_t = typename ChunkType_t::Size_t;
        using KindPointer_t = KindPointerT<ChunkType_t>;
        using ChunkPointer_t = ChunkPointerT<ChunkType_t>;
        using ChunkArrayPointer_t = ChunkArrayPointerT<ChunkType_t, ChunkPointer_t>;
        using KChunkPointer_t = KChunkPointerT<ChunkType_t>;
        using KChunkArrayPointer_t = KChunkArrayPointerT<ChunkType_t, ChunkPointer_t>;
        using KChunkTreePointer_t = KChunkTreePointerT<ChunkType_t>;
        using KChunkArrayTreePointer_t = KChunkArrayTreePointerT<ChunkType_t, ChunkPointer_t>;

    public:
        /// <summary>
        /// Route and execute an algorithm on a chunk
        /// </summary>
        /// <param name="algorithm"></param>
        /// <param name="chunkPtr"></param>
        /// <returns></returns>
        static bool TryRun(ChunkAlgorithm_t& algorithm, KindPointer_t& chunkPtr)
        {
            switch (chunkPtr.Kind)
            {
            case ChunkKind_Chunk:
                return AlgorithmRunnerChunk<ChunkAlgorithm_t, ChunkPointer_t>::TryRun(algorithm, *(KChunkPointer_t&)chunkPtr);
            case ChunkKind_ChunkArray:
                return AlgorithmRunnerChunkArray<ChunkAlgorithm_t, ChunkArrayPointer_t>::TryRun(algorithm, *(KChunkArrayPointer_t&)chunkPtr);
            case ChunkKind_ChunkTree:
                return AlgorithmRunnerChunk<ChunkAlgorithm_t, KChunkTreePointer_t>::TryRun(algorithm, (KChunkTreePointer_t&)chunkPtr);
            case ChunkKind_ChunkArrayTree:
                return AlgorithmRunnerChunkArray<ChunkAlgorithm_t, KChunkArrayTreePointer_t>::TryRun(algorithm, (KChunkArrayTreePointer_t&)chunkPtr);
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
        static bool TryRun(const TRouter& router, ChunkAlgorithm_t& algorithm, KindPointer_t& chunkPtr)
        {
            switch (chunkPtr.Kind)
            {
            case ChunkKind_Chunk:
                return AlgorithmRunnerChunk<ChunkAlgorithm_t, ChunkPointer_t>::TryRun(router, algorithm, *(KChunkPointer_t&)chunkPtr);
            case ChunkKind_ChunkArray:
                return AlgorithmRunnerChunkArray<ChunkAlgorithm_t, ChunkArrayPointer_t>::TryRun(router, algorithm, *(KChunkArrayPointer_t&)chunkPtr);
            case ChunkKind_ChunkTree:
                return AlgorithmRunnerChunk<ChunkAlgorithm_t, KChunkTreePointer_t>::TryRun(router, algorithm, (KChunkTreePointer_t&)chunkPtr);
            case ChunkKind_ChunkArrayTree:
                return AlgorithmRunnerChunkArray<ChunkAlgorithm_t, KChunkArrayTreePointer_t>::TryRun(router, algorithm, (KChunkArrayTreePointer_t&)chunkPtr);
            }
            return true;
        }
    };
}
