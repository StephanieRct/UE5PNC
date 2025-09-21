// MIT License
// Copyright (c) 2025 Stephanie Rancourt

#pragma once
#include "common.h"
#include "routing\AlgorithmCacheRouter.h"
#include "routing\AlgorithmRequirementMatchForChunkType.h"

namespace PNC
{
    /// <summary>
    /// Extend this template struct to write your own pipeline to process Chunks
    /// </summary>
    /// <typeparam name="TDerivedPipeline">Derived type. ex.: struct MyPipeline : public PipelineT<MyPipeline> {};</typeparam>
    /// <typeparam name="TChunkStructure"></typeparam>
    /// <typeparam name="TSize"></typeparam>
    template<typename TDerivedPipeline, typename TChunkStructure, typename TSize>
    struct PipelineT
    {
    public:
        using Self_t = PipelineT<TDerivedPipeline, TChunkStructure, TSize>;
        using Pipeline_t = TDerivedPipeline;
        using ChunkStructure_t = TChunkStructure;
        using Size_t = TSize;

    protected:
        using CacheMap_t = std::unordered_map<const ChunkStructure_t*, bool>;
        mutable CacheMap_t ChunkStructureMatching;

    public:

        template<typename TChunkStructure>
        bool Match(const TChunkStructure* chunkStructure)
        {
            assert(chunkStructure != null);
            auto iMatching = ChunkStructureMatching.find(chunkStructure);
            if (iMatching == ChunkStructureMatching.end())
            {
                bool bMatch = Impl()->Requirements(PipelineRequirementMatchForChunkType<TChunkStructure>(chunkStructure));
                iMatching = ChunkStructureMatching.insert(iMatching, typename CacheMap_t::value_type(chunkStructure, bMatch));
            }
            return iMatching->second;
        }

        template<typename TChunkPointer>
        bool TryRun(TChunkPointer& chunkPointer)
        {
            auto& chunk = *chunkPointer;
            assert(!chunk.IsNull());
            const auto* chunkStructure = &chunk.GetChunkStructure();
            if (!Match(chunkStructure))
                return false;
            Impl()->Execute(chunkPointer);
            return true;
        }

        template<typename TChunkPointer>
        void TryRun(TChunkPointer* chunkPointer) = delete;

        template<typename TChunkPointer>
        void Run(TChunkPointer& chunkPointer)
        {
            if (!TryRun(chunkPointer))
            {
                checkf(false, TEXT("Could not run pipeline '%hs' on chunk '%hs'. The chunk failed the pipeline requirements."), typeid(Pipeline_t).name(), typeid(TChunkPointer).name());
            }
        }
        template<typename TChunkPointer>
        void Run(TChunkPointer* chunkPointer) = delete;

    private:
        Pipeline_t* Impl() { return (reinterpret_cast<Pipeline_t*>(this)); }
    };

    template<typename TChunkStructure>
    struct PipelineRequirementMatchForChunkType
    {
    public:
        using ChunkType_t = TChunkStructure;

    protected:
        const TChunkStructure* ChunkStructure;

    public:
        PipelineRequirementMatchForChunkType(const TChunkStructure* chunkStructure)
            :ChunkStructure(chunkStructure)
        {
        }

        template<typename T>
        bool Algorithm(T& algorithm)
        {
            return algorithm.Requirements(Routing::AlgorithmRequirementMatchForChunkType<TChunkStructure>(ChunkStructure));
        }
    };
}