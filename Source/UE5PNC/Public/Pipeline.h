// MIT License
// Copyright (c) 2025 Stephanie Rancourt

#pragma once
#include "common.h"

namespace PNC
{
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
            return algorithm.Requirements(AlgorithmRequirementMatchForChunkType<TChunkType>(ChunkType));
        }
    };

    template<typename TPipeline, typename TChunkType, typename TSize>
    struct PipelineT
    {
    public:
        using ChunkType_t = TChunkType;
        using Size_t = TSize;
        using Pipeline_t = TPipeline;

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

    template<typename TSize>
    struct AlgorithmRouteT
    {
    public:
        using Size_t = TSize;

    public:
        std::vector<Size_t> Components;

        Size_t operator[](Size_t routeIndex)const
        {
            return Components[routeIndex];
        }

        void AddRoute(Size_t componentTypeIndexInChunk)
        {
            Components.push_back(componentTypeIndexInChunk);
        }
        void MarkMismatch()
        {
            Components.clear();
        }
        bool IsMismatch()const
        {
            return Components.size() == 0;
        }
    };

    template<typename TChunkPointer, typename TSize>
    struct AlgorithmRouteToCacheT : public SetAlgorithmChunk<TChunkPointer>
    {
    public:
        using Base_t = SetAlgorithmChunk<TChunkPointer>;
        using ChunkPointer_t = TChunkPointer;
        using Size_t = TSize;
        using AlgorithmRoute_t = AlgorithmRouteT<Size_t>;

    public:
        bool MatchForChunk;
    protected:
        AlgorithmRoute_t* Route;

    public:
        AlgorithmRouteToCacheT(ChunkPointer_t* chunkPointer, AlgorithmRoute_t* route)
            : Base_t(chunkPointer)
            , MatchForChunk(true)
            , Route(route)
        {
        }

        template<typename T>
        bool Component(T*& component)
        {
            auto& chunk = this->ChunkPointer->GetChunk();
            const auto& chunkType = chunk.GetChunkType();
            auto componentTypeIndexInChunk = chunkType.GetComponentTypeIndexInChunk(&typeid(T));
            Route->AddRoute(componentTypeIndexInChunk);

            if (componentTypeIndexInChunk == -1)
            {
                component = nullptr;
                MatchForChunk = false;
                return false;
            }
            component = (T*)chunk.GetComponentData(componentTypeIndexInChunk);
            return true;
        }

    };

    template<typename TChunkPointer, typename TSize>
    struct AlgorithmRouteWithCacheT : public SetAlgorithmChunk<TChunkPointer>
    {
    public:
        using Base_t = SetAlgorithmChunk<TChunkPointer>;
        using ChunkPointer_t = TChunkPointer;
        using Size_t = TSize;
        using AlgorithmRoute_t = typename AlgorithmRouteT<TSize>;

    protected:
        AlgorithmRoute_t* Route;
        Size_t CurrentComponentRoute;

    public:
        AlgorithmRouteWithCacheT(ChunkPointer_t* chunkPointer, AlgorithmRoute_t* route)
            : Base_t(chunkPointer)
            , Route(route)
            , CurrentComponentRoute(0)
        {
        }

        template<typename T>
        bool Component(T*& component)
        {
            auto componentTypeIndexInChunk = (*Route)[CurrentComponentRoute];
            ++CurrentComponentRoute;
            // Make sure the cache is valid
            //assert(componentTypeIndexInChunk == Chunk->GetChunkType().GetComponentTypeIndexInChunk(&typeid(T)));

            if (componentTypeIndexInChunk == (Size_t)-1)
                return false;

            auto& chunk = this->ChunkPointer->GetChunk();
            component = (T*)chunk.GetComponentData(componentTypeIndexInChunk);
            return true;
        }

    };

    template<typename TAlgorithm, typename TChunkType, typename TSize>
    struct AlgorithmRoutingCacheT 
    {
    public:
        using Size_t = TSize;
        using ChunkType_t = TChunkType;
        using Algorithm_t = TAlgorithm;
        using AlgorithmRoute_t = AlgorithmRouteT<TSize>;

    protected:
        using Map_t = std::unordered_map<const ChunkType_t*, AlgorithmRoute_t*>;
        mutable Map_t Cache;
        mutable std::list<AlgorithmRoute_t> CachedRoutes;

    public:
        AlgorithmRoutingCacheT() {}
        // Non-copyable
        AlgorithmRoutingCacheT(const AlgorithmRoutingCacheT&) = delete;
        AlgorithmRoutingCacheT& operator=(const AlgorithmRoutingCacheT&) = delete;

        template<typename T>
        bool Requirements(T req)
        {
            return ((TAlgorithm*)nullptr)->Requirements(req);
        }

        template<typename TChunkPointer>
        bool RouteAlgorithm(Algorithm_t& algorithm, TChunkPointer& chunkPointer) const
        {
            //TRACE_CPUPROFILER_EVENT_SCOPE(TEXT("Routing"));
            auto& chunk = *chunkPointer;
            const ChunkType_t* chunkType = &chunk.GetChunkType();
            
            typename Map_t::iterator i = Cache.find(chunkType);
            if (i == Cache.end())
            {
                CachedRoutes.push_back(AlgorithmRoute_t());
                AlgorithmRoute_t * route = &CachedRoutes.back();
                Cache[chunkType] = route;
                AlgorithmRouteToCacheT<TChunkPointer, Size_t> routeToCache(&chunkPointer, route);
                bool matches = algorithm.Requirements<AlgorithmRouteToCacheT<TChunkPointer, Size_t>&>(routeToCache);
                //UE_LOG(LogTemp, Warning, TEXT("Algorithm '%hs' matches: %i, components: %i"), typeid(Algorithm_t).name(), matches, route->Components.size());
                if (!routeToCache.MatchForChunk)
                {
                    route->MarkMismatch();
                    return false;
                }
                return matches;
            }
            else
            {
                //return algorithm.Requirements(SetAlgorithmChunk<TChunk>(&chunk));
                ////UE_LOG(LogTemp, Warning, TEXT("Algorithm '%hs' cahed matches: %i, components: %i"), typeid(Algorithm_t).name(), i->second->IsMismatch(), i->second->Components.size());
                if (i->second->IsMismatch())
                    return false;
                AlgorithmRouteWithCacheT<TChunkPointer, Size_t> router(&chunkPointer, i->second);
                return algorithm.Requirements<AlgorithmRouteWithCacheT<TChunkPointer, Size_t>&>(router);
            }
        }
    };
}