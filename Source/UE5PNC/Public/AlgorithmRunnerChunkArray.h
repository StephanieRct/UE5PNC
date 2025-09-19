// MIT License
// Copyright (c) 2025 Stephanie Rancourt

#pragma once
#include "common.h"
#include "Routing\SetAlgorithmChunk.h"
#include "Routing\OffsetAlgorithmNode.h"
#include "Routing\AlgorithmRequirementMatchForChunkType.h"

namespace PNC
{
    /// <summary>
    /// Execute an algorithm on each element Chunks in Chunk array.
    /// </summary>
    /// <typeparam name="TAlgorithm"></typeparam>
    /// <typeparam name="TChunkArrayPointer"></typeparam>
    template< typename TAlgorithm, typename TChunkArrayPointer>
    struct AlgorithmRunnerChunkArray
    {
    public:
        using Algorithm_t = TAlgorithm;
        using ChunkArrayPointer_t = TChunkArrayPointer;
        using ChunkType_t = typename TChunkArrayPointer::ChunkType_t;
        using Size_t = typename TChunkArrayPointer::Size_t;

    public:
        /// <summary>
        /// Route and execute an algorithm on all element Chunks in the array.
        /// </summary>
        /// <param name="algorithm"></param>
        /// <param name="chunkPtr"></param>
        /// <returns></returns>
        static bool TryRun(Algorithm_t& algorithm, ChunkArrayPointer_t& chunkPtr)
        {
            auto& chunkArray = *chunkPtr;
            if (chunkArray.IsNull())
                return false;
            if (!algorithm.Requirements(Routing::SetAlgorithmChunk<ChunkArrayPointer_t>(&chunkPtr)))
                return false;
            for (Size_t i = 0; i < chunkArray.GetChunkCount(); ++i)
            {
                auto& chunk = chunkArray[i];
                auto nodeCount = chunk.GetNodeCount();
                algorithm.Execute(nodeCount);
                if (!algorithm.Requirements(Routing::OffsetAlgorithmNode<ChunkArrayPointer_t>(nodeCount)))
                    return false;
            }
            return true;
        }

        /// <summary>
        /// Route using a given router and execute an algorithm on each element Chunks in the array.
        /// </summary>
        /// <typeparam name="TRouter"></typeparam>
        /// <param name="router"></param>
        /// <param name="algorithm"></param>
        /// <param name="chunkPtr"></param>
        /// <returns></returns>
        template<typename TRouter>
        static bool TryRun(const TRouter& router, Algorithm_t& algorithm, ChunkArrayPointer_t& chunkPtr)
        {
            auto& chunkArray = *chunkPtr;
            assert_pnc(!chunkArray.IsNull());
            if (!router.RouteAlgorithm(algorithm, chunkPtr))
                return false;

            for (Size_t i = 0; i < chunkArray.GetChunkCount(); ++i)
            {
                auto& chunk = chunkArray[i];
                auto nodeCount = chunk.GetNodeCount();
                algorithm.Execute(nodeCount);
                bool nextOk = algorithm.Requirements(Routing::OffsetAlgorithmNode<ChunkArrayPointer_t>(nodeCount));
                assert_pnc(nextOk);
            }
            return true;
        }
    };
}
