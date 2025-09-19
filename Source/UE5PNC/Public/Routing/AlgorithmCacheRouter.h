// MIT License
// Copyright (c) 2025 Stephanie Rancourt

#pragma once
#include "common.h"
#include "SetAlgorithmChunk.h"
#include "AlgorithmRequirementFulfiller.h"

namespace PNC::Routing
{

    template<typename TSize>
    struct RouteT
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
    struct RouteAlgorithmToCacheT : public SetAlgorithmChunk<TChunkPointer>
    {
    public:
        using Base_t = SetAlgorithmChunk<TChunkPointer>;
        using ChunkPointer_t = TChunkPointer;
        using Size_t = TSize;
        using AlgorithmRoute_t = RouteT<Size_t>;

    public:
        bool MatchForChunk;
    protected:
        AlgorithmRoute_t* Route;

    public:
        RouteAlgorithmToCacheT(ChunkPointer_t* chunkPointer, AlgorithmRoute_t* route)
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
    struct RouteAlgorithmWithCacheT : public SetAlgorithmChunk<TChunkPointer>
    {
    public:
        using Base_t = SetAlgorithmChunk<TChunkPointer>;
        using ChunkPointer_t = TChunkPointer;
        using Size_t = TSize;
        using AlgorithmRoute_t = typename RouteT<TSize>;

    protected:
        AlgorithmRoute_t* Route;
        Size_t CurrentComponentRoute;

    public:
        RouteAlgorithmWithCacheT(ChunkPointer_t* chunkPointer, AlgorithmRoute_t* route)
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

    /// <summary>
    /// Route algorithms and cache the routes for each ChunkTypes the algorithm matches with.
    /// </summary>
    /// <typeparam name="TAlgorithm"></typeparam>
    /// <typeparam name="TChunkType"></typeparam>
    /// <typeparam name="TSize"></typeparam>
    template<typename TAlgorithm, typename TChunkType, typename TSize>
    struct AlgorithmCacheRouterT : public AlgorithmRequirementFulfiller
    {
    public:
        using Size_t = TSize;
        using ChunkType_t = TChunkType;
        using Algorithm_t = TAlgorithm;
        using AlgorithmRoute_t = RouteT<TSize>;

    protected:
        using Map_t = std::unordered_map<const ChunkType_t*, AlgorithmRoute_t*>;
        mutable Map_t Cache;
        mutable std::list<AlgorithmRoute_t> CachedRoutes;

    public:
        AlgorithmCacheRouterT() {}
        // Non-copyable
        AlgorithmCacheRouterT(const AlgorithmCacheRouterT&) = delete;
        AlgorithmCacheRouterT& operator=(const AlgorithmCacheRouterT&) = delete;

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
            using AlgorithmRouteToCache_t = RouteAlgorithmToCacheT<TChunkPointer, Size_t>;
            using AlgorithmRouteWithCache_t = RouteAlgorithmWithCacheT<TChunkPointer, Size_t>;
            //TRACE_CPUPROFILER_EVENT_SCOPE(TEXT("Routing"));
            auto& chunk = *chunkPointer;
            const ChunkType_t* chunkType = &chunk.GetChunkType();

            typename Map_t::iterator i = Cache.find(chunkType);
            if (i == Cache.end())
            {
                CachedRoutes.push_back(AlgorithmRoute_t());
                AlgorithmRoute_t* route = &CachedRoutes.back();
                Cache[chunkType] = route;
                AlgorithmRouteToCache_t routeToCache(&chunkPointer, route);
                bool matches = algorithm.template Requirements<AlgorithmRouteToCache_t&>(routeToCache);
                if (!routeToCache.MatchForChunk)
                {
                    route->MarkMismatch();
                    return false;
                }
                return matches;
            }
            else
            {
                if (i->second->IsMismatch())
                    return false;
                AlgorithmRouteWithCache_t router(&chunkPointer, i->second);
                return algorithm.template Requirements<AlgorithmRouteWithCache_t&>(router);
            }
        }

        template<typename TChunkPointer>
        bool TryRun(const Algorithm_t& algorithm, TChunkPointer& chunkPointer) const
        {
            return algorithm.TryRun(*this, chunkPointer);
        }

        template<typename TChunkPointer>
        bool TryRun(TChunkPointer& chunkPointer) const
        {
            return Algorithm_t().TryRun(*this, chunkPointer);
        }

        template<typename TChunkPointer>
        void Run(const Algorithm_t& algorithm, TChunkPointer& chunkPointer) const
        {
            algorithm.Run(*this, chunkPointer);
        }

        template<typename TChunkPointer>
        void Run(TChunkPointer& chunkPointer) const
        {
            Algorithm_t().Run(*this, chunkPointer);
        }
    };

}
