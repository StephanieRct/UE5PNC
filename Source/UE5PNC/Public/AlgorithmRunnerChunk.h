// MIT License
// Copyright (c) 2025 Stephanie Rancourt

#pragma once
#include "common.h"
#include "DChunkPointer.h"
#include "Routing\SetAlgorithmChunk.h"
#include "Routing\OffsetAlgorithmNode.h"

namespace NiT
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
            ni_assert(!container.IsNull());
            if (!router.RouteAlgorithm(algorithm, container))
                return false;
            const auto nodeCount = container.GetNodeCount();
            algorithm.Execute(nodeCount);
            return true;
        }
    };
}
