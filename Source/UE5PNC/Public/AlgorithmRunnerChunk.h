// MIT License
// Copyright (c) 2025 Stephanie Rancourt

#pragma once
#include "common.h"
#include "Routing\SetAlgorithmChunk.h"
#include "Routing\OffsetAlgorithmNode.h"

namespace PNC
{
    /// <summary>
    /// Execute an algorithm on a Chunk. 
    /// If TChunkPointer is an array pointer (ChunkArrayPointer), the algorithm will execute only once on the whole array as a single Chunk.
    /// Use ChunkArrayAlgorithmRunner to execute the algorithm on each elements of an array pointer
    /// </summary>
    /// <typeparam name="TAlgorithm"></typeparam>
    /// <typeparam name="TChunkPointer"></typeparam>
    template< typename TAlgorithm, typename TChunkPointer>
    struct AlgorithmRunnerChunk
    {
    public:
        using Self_t = AlgorithmRunnerChunk<TAlgorithm, TChunkPointer>;
        using Algorithm_t = TAlgorithm;
        using ChunkPointer_t = TChunkPointer;

    public:
        /// <summary>
        /// Route and execute an algorithm on a chunk
        /// </summary>
        /// <param name="algorithm"></param>
        /// <param name="chunkPtr"></param>
        /// <returns></returns>
        static bool TryRun(TAlgorithm& algorithm, ChunkPointer_t& chunkPtr)
        {
            auto& chunk = *chunkPtr;
            if (chunk.IsNull())
                return false;
            if (!algorithm.Requirements(Routing::SetAlgorithmChunk<ChunkPointer_t>(&chunkPtr)))
                return false;
            algorithm.Execute(chunk.GetNodeCount());
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
        static bool TryRun(const TRouter& router, TAlgorithm& algorithm, ChunkPointer_t& chunkPtr)
        {
            auto& chunk = *chunkPtr;
            assert_pnc(!chunk.IsNull());
            if (!router.RouteAlgorithm(algorithm, chunkPtr))
                return false;
            algorithm.Execute(chunk.GetNodeCount());
            return true;
        }
    };
}
