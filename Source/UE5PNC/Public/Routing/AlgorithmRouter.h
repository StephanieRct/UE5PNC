// MIT License
// Copyright (c) 2025 Stephanie Rancourt

#pragma once
#include "common.h"
#include "SetAlgorithmChunk.h"

namespace PNC::Routing
{
    /// <summary>
    /// Route an algorithm using the
    /// </summary>
    /// <typeparam name="TAlgorithm"></typeparam>
    template<typename TAlgorithm>
    struct AlgorithmRouterT : public AlgorithmRequirementFulfiller
    {
    public:
        using Algorithm_t = TAlgorithm;

    public:
        /// <summary>
        /// A AlgorithmCacheRouterT can be passed as an Algorithm inside a Pipeline so it must
        /// passthrough the algorithm requirements.
        /// </summary>
        /// <typeparam name="T"></typeparam>
        /// <param name="req"></param>
        /// <returns></returns>
        template<typename T>
        bool Requirements(T req)
        {
            return ((TAlgorithm*)nullptr)->Requirements(req);
        }

        template<typename TChunkPointer>
        bool RouteAlgorithm(Algorithm_t& algorithm, TChunkPointer& chunkPointer) const
        {
            return algorithm.Requirements(SetAlgorithmChunk<TChunkPointer>());
        }

        template<typename TChunkPointer>
        bool TryRun(const Algorithm_t& algorithm, TChunkPointer& chunkPointer) const
        {
            return algorithm.TryRun(*this, chunkPointer);
        }

        template<typename TChunkPointer>
        void Run(const Algorithm_t& algorithm, TChunkPointer& chunkPointer) const
        {
            algorithm.Run(*this, chunkPointer);
        }
    };
}