// MIT License
// Copyright (c) 2025 Stephanie Rancourt

#pragma once
#include "common.h"
#include "DChunkPointer.h"
#include "Routing\SetAlgorithmChunk.h"
#include "Routing\OffsetAlgorithmNode.h"

namespace PNC
{
    template<typename TContainer>
    struct AlgorithmRunnerChunk
    {
        /// <summary>
        /// Route and execute an algorithm on a chunk
        /// </summary>
        template<typename TAlgorithm>
        static bool TryRun(TAlgorithm& algorithm, TContainer& container)
        {
            if (container.IsNull())
                return false;
            if (!algorithm.Requirements(Routing::SetAlgorithmChunk<TContainer>(&container)))
                return false;
            const auto nodeCount = container.GetNodeCount();
            algorithm.Execute(nodeCount);
            return true;
        }

        /// <summary>
        /// Route using a router and execute an algorithm on a chunk
        /// </summary>
        template<typename TRouter, typename TAlgorithm>
        static bool TryRun(const TRouter& router, TAlgorithm& algorithm, TContainer& container)
        {
            pnc_assert(!container.IsNull());
            if (!router.RouteAlgorithm(algorithm, container))
                return false;
            const auto nodeCount = container.GetNodeCount();
            algorithm.Execute(nodeCount);
            return true;
        }
    };
    //struct AlgorithmRunnerDChunkPointer
    //{
    //    /// <summary>
    //    /// Route and execute an algorithm on a chunk
    //    /// </summary>
    //    /// <param name="algorithm"></param>
    //    /// <param name="chunkPtr"></param>
    //    /// <returns></returns>
    //    template<typename TAlgorithm, typename TChunkPointer, typename TBase>
    //    static bool TryRun(TAlgorithm& algorithm, DChunkPointer<TChunkPointer, TBase>& container)
    //    {
    //        auto& chunk = container.GetChunk();
    //        if (chunk.IsNull())
    //            return false;
    //        if (!algorithm.Requirements(Routing::SetAlgorithmChunk<DChunkPointer<TChunkPointer, TBase>>(&container)))
    //            return false;
    //        algorithm.Execute(chunk.GetNodeCount());
    //        return true;
    //    }

    //    /// <summary>
    //    /// Route using a router and execute an algorithm on a chunk
    //    /// </summary>
    //    /// <typeparam name="TRouter"></typeparam>
    //    /// <param name="router"></param>
    //    /// <param name="algorithm"></param>
    //    /// <param name="chunkPtr"></param>
    //    /// <returns></returns>
    //    template<typename TRouter, typename TAlgorithm, typename TChunkPointer, typename TBase>
    //    static bool TryRun(const TRouter& router, TAlgorithm& algorithm, DChunkPointer<TChunkPointer, TBase>& container)
    //    {
    //        auto& chunk = container.GetChunk();
    //        pnc_assert(!chunk.IsNull());
    //        if (!router.RouteAlgorithm(algorithm, container))
    //            return false;
    //        algorithm.Execute(chunk.GetNodeCount());
    //        return true;
    //    }
    //};

    ///// <summary>
    ///// Execute an algorithm on a Chunk. 
    ///// If TChunkPointer is an array pointer (ChunkArrayPointer), the algorithm will execute only once on the whole array as a single Chunk.
    ///// Use ChunkArrayAlgorithmRunner to execute the algorithm on each elements of an array pointer
    ///// </summary>
    ///// <typeparam name="TAlgorithm"></typeparam>
    ///// <typeparam name="TChunkPointer"></typeparam>
    //template< typename TAlgorithm, typename TChunkPointer>
    //struct AlgorithmRunnerChunk
    //{
    //public:
    //    using Self_t = AlgorithmRunnerChunk<TAlgorithm, TChunkPointer>;
    //    using Algorithm_t = TAlgorithm;
    //    using ChunkPointer_t = TChunkPointer;

    //public:
    //    /// <summary>
    //    /// Route and execute an algorithm on a chunk
    //    /// </summary>
    //    /// <param name="algorithm"></param>
    //    /// <param name="chunkPtr"></param>
    //    /// <returns></returns>
    //    static bool TryRun(TAlgorithm& algorithm, ChunkPointer_t& chunkPtr)
    //    {
    //        auto& chunk = chunkPtr.GetChunk();
    //        if (chunk.IsNull())
    //            return false;
    //        if (!algorithm.Requirements(Routing::SetAlgorithmChunk<ChunkPointer_t>(&chunkPtr)))
    //            return false;
    //        algorithm.Execute(chunk.GetNodeCount());
    //        return true;
    //    }

    //    /// <summary>
    //    /// Route using a router and execute an algorithm on a chunk
    //    /// </summary>
    //    /// <typeparam name="TRouter"></typeparam>
    //    /// <param name="router"></param>
    //    /// <param name="algorithm"></param>
    //    /// <param name="chunkPtr"></param>
    //    /// <returns></returns>
    //    template<typename TRouter>
    //    static bool TryRun(const TRouter& router, TAlgorithm& algorithm, ChunkPointer_t& chunkPtr)
    //    {
    //        auto& chunk = chunkPtr.GetChunk();
    //        pnc_assert(!chunk.IsNull());
    //        if (!router.RouteAlgorithm(algorithm, chunkPtr))
    //            return false;
    //        algorithm.Execute(chunk.GetNodeCount());
    //        return true;
    //    }
    //};
}
