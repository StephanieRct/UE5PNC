// MIT License
// Copyright (c) 2025 Stephanie Rancourt

#pragma once
#include "common.h"
#include "AlgorithmRunner.h"

namespace NiT
{
    /// <summary>
    /// Extend this template struct to write your own algorithm that processes any Chunk's Component data as input or output
    /// </summary>
    /// <typeparam name="TDerivedAlgorithm">Derived type. ex.: struct MyAlgo : public Algorithm<MyAlgo> {};</typeparam>
    template< typename TDerivedAlgorithm >
    struct Algorithm
    {
    public:
        using Self_t = Algorithm<TDerivedAlgorithm>;
        using Algorithm_t = TDerivedAlgorithm;

    public:
        /// <summary>  
        /// Will execute the algorithm on the chunk if all requirements are fulfilled and return true.
        /// Returns false if the chunk is null or if it doesn't fulfill the algorithm requirements.
        /// 
        /// TODO: Add a TryRun(const TChunkPointer& chunkPointer) version.
        /// </summary>
        /// <typeparam name="TContainer">Typename of the chunk to execute the algorithm on.</typeparam>
        /// <param name="chunk">The chunk to execute the algorithm on.</param>
        /// <returns>If it successfully executed the algorithm on the chunk.</returns>
        template<typename TContainer>
        bool TryRun(TContainer& container)const
        {
            return AlgorithmRunner<Algorithm_t, TContainer>::TryRun(*Impl(), container);
        }

        template<typename TContainer>
        bool TryRun(TContainer* container)
        {
            if (!container)
                return false;
            return TryRun(*container);
        }

        /// <summary>
        /// Route using a router and execute an algorithm on a chunk if all requirements are fulfilled and return true.
        /// </summary>
        /// <typeparam name="TRouter"></typeparam>
        /// <typeparam name="TContainer"></typeparam>
        /// <param name="router"></param>
        /// <param name="chunkPointer"></param>
        /// <returns></returns>
        template<typename TRouter, typename TContainer>
        bool TryRun(const TRouter& router, TContainer& container)const
        {
            return AlgorithmRunner<Algorithm_t, TContainer>::TryRun(router, *Impl(), container);
        }

        /// <summary>
        /// Will execute the algorithm on a matching chunk.
        /// The chunk must not be null and must match the algorithm or it halt execution
        /// 
        /// TODO: Add a Run(const TChunkPointer& chunkPointer) version.
        /// </summary>
        /// <typeparam name="TChunkPointer"></typeparam>
        /// <param name="chunk"></param>
        template<typename TContainer>
        void Run(TContainer& container)const
        {
            if (!TryRun(container))
            {
                ni_assertf(false, TEXT("Could not run algorithm '%hs' on chunk '%hs'. The chunk failed the algorithm requirements."), typeid(Algorithm_t).name(), typeid(TContainer).name());
            }
        }

        template<typename TContainer>
        void Run(TContainer* container)
        {
            if (!container)
            {
                ni_assertf(false, TEXT("Could not run algorithm '%hs' on null chunk '%hs'."), typeid(Algorithm_t).name(), typeid(TContainer).name());
            }
            return Run(*container);
        }


        /// <summary>
        /// Route using a router and execute an algorithm on a chunk
        /// The chunk must not be null and must match the algorithm or it halt execution
        /// 
        /// TODO: Add a Run(const TRouter& router, const TContainer& container) version.
        /// </summary>
        template<typename TRouter, typename TContainer>
        void Run(const TRouter& router, TContainer& container)const
        {
            if (!TryRun(router, container))
            {
                ni_assertf(false, TEXT("Could not run algorithm '%hs' on chunk '%hs'. The chunk failed the algorithm requirements."), typeid(Algorithm_t).name(), typeid(TContainer).name());
            }
        }

    private:
        Algorithm_t* Impl()const { return (const_cast<Algorithm_t*>(reinterpret_cast<const Algorithm_t*>(this))); }
    };
}
