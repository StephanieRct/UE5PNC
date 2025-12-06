// MIT License
// Copyright (c) 2025 Stephanie Rancourt

#pragma once
#include "common.h"
#include "AlgorithmRunnerChunk.h"
#include "AlgorithmRunnerChunkArray.h"
#include "DKind.h"
#include "KContainers.h"

namespace NiT
{
    /// <summary>
    /// Execute an algorithm on a DKind container
    /// It will retreive the container's ChunkPointer according to the kind in DKind and
    /// execute the algorithm using the appropriate AlgorithmRunner Chunk/Array
    /// </summary>
    template<typename TAlgorithm, typename TContainer>
    struct AlgorithmRunnerKindPointerSwitch
    {
    public:
        using Self_t = AlgorithmRunnerKindPointerSwitch<TAlgorithm, TContainer>;
        using Algorithm_t = TAlgorithm;
        using Container_t = TContainer;
        using ChunkStructure_t = typename TContainer::ChunkStructure_t;
        using Size_t = typename Container_t::Size_t;

        using KindPointer_t = KindPointerT<ChunkStructure_t>;
        using ChunkPointer_t = ChunkPointerT<ChunkStructure_t>;
        using ChunkArrayPointer_t = ArrayPointerT<ChunkStructure_t, ChunkPointer_t>;
        using KChunkPointer_t = KChunkPointerT<ChunkStructure_t>;
        using KChunkArrayPointer_t = KArrayPointerT<ChunkStructure_t, ChunkPointer_t>;
        using KChunkTreePointer_t = KChunkTreePointerT<ChunkStructure_t>;
        using KChunkArrayTreePointer_t = KArrayTreePointerT<ChunkStructure_t, ChunkPointer_t>;

    public:
        /// <summary>
        /// Route and execute an algorithm on a chunk
        /// </summary>
        /// <param name="algorithm"></param>
        /// <param name="container"></param>
        /// <returns></returns>
        static bool TryRun(Algorithm_t& algorithm, Container_t& container)
        {
            switch (container.GetKind())
            {
            case ContainerKind::Chunk:
                return AlgorithmRunnerChunk<KChunkPointer_t>::TryRun(algorithm, (KChunkPointer_t&)container);
            case ContainerKind::Array:
                return AlgorithmRunnerChunkArray<Algorithm_t, KChunkArrayPointer_t>::TryRun(algorithm, (KChunkArrayPointer_t&)container);
            case ContainerKind::ChunkTree:
                return AlgorithmRunnerChunk<KChunkTreePointer_t>::TryRun(algorithm, (KChunkTreePointer_t&)container);
            case ContainerKind::ArrayTree:
                return AlgorithmRunnerChunkArray<Algorithm_t, KChunkArrayTreePointer_t>::TryRun(algorithm, (KChunkArrayTreePointer_t&)container);
            }
            return true;
        }

        /// <summary>
        /// Route using a router and execute an algorithm on a chunk
        /// </summary>
        /// <typeparam name="TRouter"></typeparam>
        /// <param name="router"></param>
        /// <param name="algorithm"></param>
        /// <param name="container"></param>
        /// <returns></returns>
        template<typename TRouter>
        static bool TryRun(const TRouter& router, Algorithm_t& algorithm, Container_t& container)
        {
            switch (container.GetKind())
            {
            case ContainerKind::Chunk:
                return AlgorithmRunnerChunk<KChunkPointer_t>::TryRun(router, algorithm, (KChunkPointer_t&)container);
            case ContainerKind::Array:
                return AlgorithmRunnerChunkArray<Algorithm_t, KChunkArrayPointer_t>::TryRun(router, algorithm, (KChunkArrayPointer_t&)container);
            case ContainerKind::ChunkTree:
                return AlgorithmRunnerChunk<KChunkTreePointer_t>::TryRun(router, algorithm, (KChunkTreePointer_t&)container);
            case ContainerKind::ArrayTree:
                return AlgorithmRunnerChunkArray<Algorithm_t, KChunkArrayTreePointer_t>::TryRun(router, algorithm, (KChunkArrayTreePointer_t&)container);
            }
            return true;
        }
    };
}
