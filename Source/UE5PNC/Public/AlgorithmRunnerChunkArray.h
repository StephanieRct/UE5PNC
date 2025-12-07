// MIT License
// Copyright (c) 2025 Stephanie Rancourt

#pragma once
#include "common.h"
#include "Routing\SetAlgorithmChunk.h"
#include "Routing\OffsetAlgorithmNode.h"

namespace NiT
{
    /// <summary>
    /// Execute an algorithm on each element Chunks in Chunk array.
    /// </summary>
    template< typename TAlgorithm, typename TContainer>
    struct AlgorithmRunnerChunkArray
    {
    public:
        using Algorithm_t = TAlgorithm;
        using Container_t = TContainer;
        using ChunkStructure_t = typename TContainer::ChunkStructure_t;
        using Size_t = typename TContainer::Size_t;

    public:
        /// <summary>
        /// Route and execute an algorithm on all element Chunks in the array.
        /// </summary>
        /// <param name="algorithm"></param>
        /// <param name="chunkPtr"></param>
        /// <returns></returns>
        static bool TryRun(Algorithm_t& algorithm, Container_t& container)
        {
            if (container.IsNull())
                return false;
            if (!algorithm.Requirements(Routing::SetAlgorithmChunk<Container_t>(&container)))
                return false;
            for (Size_t i = 0; i < container.GetChunkCount(); ++i)
            {
                auto& chunkElement = container[i];
                auto elementNodeCount = chunkElement.GetNodeCount();
                algorithm.Execute(elementNodeCount);
                if (!algorithm.Requirements(Routing::OffsetAlgorithmNode<Container_t>(elementNodeCount)))
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
        static bool TryRun(const TRouter& router, Algorithm_t& algorithm, Container_t& container)
        {
            ni_assert(!container.IsNull());
            if (!router.RouteAlgorithm(algorithm, container))
                return false;

            for (Size_t i = 0; i < container.GetChunkCount(); ++i)
            {
                auto& chunkElement = container[i];
                auto elementNnodeCount = chunkElement.GetNodeCount();
                algorithm.Execute(elementNnodeCount);
                bool nextOk = algorithm.Requirements(Routing::OffsetAlgorithmNode<Container_t>(elementNnodeCount));
                ni_assert(nextOk);
            }
            return true;
        }
    };
}
