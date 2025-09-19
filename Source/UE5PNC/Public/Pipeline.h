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
    /// <typeparam name="TChunkType"></typeparam>
    /// <typeparam name="TSize"></typeparam>
    template<typename TDerivedPipeline, typename TChunkType, typename TSize>
    struct PipelineT
    {
    public:
        using Self_t = PipelineT<TDerivedPipeline, TChunkType, TSize>;
        using Pipeline_t = TDerivedPipeline;
        using ChunkType_t = TChunkType;
        using Size_t = TSize;

    protected:
        typedef std::unordered_map<const ChunkType_t*, bool> CacheMap_t;
        mutable CacheMap_t ChunkTypeMatching;

    public:

        template<typename TChunkType>
        bool Match(const TChunkType* chunkType)
        {
            assert(chunkType != null);
            auto iMatching = ChunkTypeMatching.find(chunkType);
            if (iMatching == ChunkTypeMatching.end())
            {
                bool bMatch = Impl()->Requirements(PipelineRequirementMatchForChunkType<TChunkType>(chunkType));
                iMatching = ChunkTypeMatching.insert(iMatching, typename CacheMap_t::value_type(chunkType, bMatch));
            }
            return iMatching->second;
        }

        template<typename TChunkPointer>
        bool TryRun(TChunkPointer& chunkPointer)
        {
            auto& chunk = *chunkPointer;
            assert(!chunk.IsNull());
            const auto* chunkType = &chunk.GetChunkType();
            if (!Match(chunkType))
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

    template<typename TChunkType>
    struct PipelineRequirementMatchForChunkType
    {
    public:
        using TChunkType_t = TChunkType;

    protected:
        const TChunkType* ChunkType;

    public:
        PipelineRequirementMatchForChunkType(const TChunkType* chunkType)
            :ChunkType(chunkType)
        {
        }

        template<typename T>
        bool Algorithm(T& algorithm)
        {
            return algorithm.Requirements(Routing::AlgorithmRequirementMatchForChunkType<TChunkType>(ChunkType));
        }
    };
}