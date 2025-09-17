// MIT License
// Copyright (c) 2025 Stephanie Rancourt

#pragma once
#include "common.h"
#include "Algorithm\ChunkAlgorithmRunner.h"

namespace PNC
{
    /// <summary>
    /// Extend this template struct to write your own algorithm that processes any chunk's component data as input or output
    /// </summary>
    /// <typeparam name="TChunkAlgorithm">The typename of your extended struct. ex.: struct MyAlgo : public ChunkAlgorithm<MyAlgo> {};</typeparam>
    template< typename TChunkAlgorithm >
    struct ChunkAlgorithm
    {
    public:
        using ChunkAlgorithm_t = TChunkAlgorithm;

    public:
        /// <summary>
        /// Will execute the algorithm on the chunk if all requirements are fulfilled and return true.
        /// Returns false if the chunk is null or if it doesn't fulfill the algorithm requirements.
        /// </summary>
        /// <typeparam name="TChunk">Typename of the chunk to execute the algorithm on.</typeparam>
        /// <param name="chunk">The chunk to execute the algorithm on.</param>
        /// <returns>If it successfully executed the algorithm on the chunk.</returns>
        template<typename TChunkPointer>
        bool TryRun(TChunkPointer& chunkPointer)
        {
            return IdentifiableChunkAlgorithmRunner<typename TChunkPointer::ChunkType_t, ChunkAlgorithm_t, TChunkPointer>::TryRun(*Impl(), chunkPointer);
        }
        template<typename TChunkPointer>
        void TryRun(TChunkPointer* chunkPointer) = delete;

        /// <summary>
        /// Will execute the algorithm on a matching chunk.
        /// The chunk must not be null and must match the algorithm or it halt execution
        /// </summary>
        /// <typeparam name="TChunkPointer"></typeparam>
        /// <param name="chunk"></param>
        template<typename TChunkPointer>
        void Run(TChunkPointer& chunkPointer)
        {
            if (!TryRun(chunkPointer))
            {
                checkf(false, TEXT("Could not run algorithm '%hs' on chunk '%hs'. The chunk failed the algorithm requirements."), typeid(ChunkAlgorithm_t).name(), typeid(TChunkPointer).name());
            }
        }
        template<typename TChunkPointer>
        void Run(TChunkPointer* chunkPointer) = delete;


        template<typename TRouting, typename TChunkPointer>
        bool TryRunWithRouting(const TRouting& routing, TChunkPointer& chunkPointer)
        {
            return IdentifiableChunkAlgorithmRunner<typename TChunkPointer::ChunkType_t, ChunkAlgorithm_t, TChunkPointer>::TryRunWithRouting(routing, *Impl(), chunkPointer);
        }
        template<typename TRouting, typename TChunkPointer>
        void RunWithRouting(const TRouting& routing, TChunkPointer& chunkPointer)
        {
            return IdentifiableChunkAlgorithmRunner<typename TChunkPointer::ChunkType_t, ChunkAlgorithm_t, TChunkPointer>::RunWithRouting(routing, *Impl(), chunkPointer);
        }

    private:
        ChunkAlgorithm_t* Impl() { return (reinterpret_cast<ChunkAlgorithm_t*>(this)); }
    };
}
