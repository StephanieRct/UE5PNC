// MIT License
// Copyright (c) 2025 Stephanie Rancourt

#pragma once
#include "common.h"
#include "SetAlgorithmChunk.h"
#include "AlgorithmRequirementFulfiller.h"

namespace NiT::Routing
{

    template<typename TSize>
    struct RouteT
    {
    public:
        using Self_t = RouteT<TSize>;
        using Size_t = TSize;

    public:
        std_vector<Size_t> Components;

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

    template<typename TContainer, typename TSize>
    struct RouteAlgorithmToCacheT : public SetAlgorithmChunk<TContainer>
    {
    public:
        using Base_t = SetAlgorithmChunk<TContainer>;
        using Self_t = RouteAlgorithmToCacheT<TContainer, TSize>;
        using Container_t = TContainer;
        using Size_t = TSize;
        using AlgorithmRoute_t = RouteT<Size_t>;

    public:
        bool MatchForChunk;
    protected:
        AlgorithmRoute_t* Route;

    public:
        RouteAlgorithmToCacheT(Container_t* container, AlgorithmRoute_t* route)
            : Base_t(container)
            , MatchForChunk(true)
            , Route(route)
        {
        }

        template<typename T>
        bool Component(T*& component)
        {
            const auto& chunkStructure = this->Container->GetStructure();
            auto componentTypeIndexInChunk = chunkStructure.GetComponentTypeIndexInChunk(&typeid(T));
            Route->AddRoute(componentTypeIndexInChunk);

            if (componentTypeIndexInChunk == -1)
            {
                component = nullptr;
                MatchForChunk = false;
                return false;
            }
            component = (T*)this->Container->GetComponentData(componentTypeIndexInChunk);
            return true;
        }

    };

    template<typename TContainer, typename TSize>
    struct RouteAlgorithmWithCacheT : public SetAlgorithmChunk<TContainer>
    {
    public:
        using Base_t = SetAlgorithmChunk<TContainer>;
        using Self_t = RouteAlgorithmWithCacheT<TContainer, TSize>;
        using Container_t = TContainer;
        using Size_t = TSize;
        using AlgorithmRoute_t = typename RouteT<TSize>;

    protected:
        AlgorithmRoute_t* Route;
        Size_t CurrentComponentRoute;

    public:
        RouteAlgorithmWithCacheT(Container_t* container, AlgorithmRoute_t* route)
            : Base_t(container)
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
            //assert(componentTypeIndexInChunk == Chunk->GetStructure().GetComponentTypeIndexInChunk(&typeid(T)));

            if (componentTypeIndexInChunk == (Size_t)-1)
                return false;

            component = (T*)this->Container->GetComponentData(componentTypeIndexInChunk);
            return true;
        }

    };

    /// <summary>
    /// Route algorithms and cache the routes for each ChunkStructures the algorithm matches with.
    /// </summary>
    /// <typeparam name="TAlgorithm"></typeparam>
    /// <typeparam name="TChunkStructure"></typeparam>
    /// <typeparam name="TSize"></typeparam>
    template<typename TAlgorithm, typename TChunkStructure, typename TSize>
    struct AlgorithmCacheRouterT : public AlgorithmRequirementFulfiller
    {
    public:
        using Self_t = AlgorithmCacheRouterT<TAlgorithm, TChunkStructure, TSize>;
        using Base_t = AlgorithmRequirementFulfiller;
        using Algorithm_t = TAlgorithm;
        using ChunkStructure_t = TChunkStructure;
        using Size_t = TSize;

    protected:
        using AlgorithmRoute_t = RouteT<TSize>;
        using Map_t = std_unordered_map<const ChunkStructure_t*, AlgorithmRoute_t*>;
        mutable Map_t Cache;
        mutable std_list<AlgorithmRoute_t> CachedRoutes;

    public:
        AlgorithmCacheRouterT() {}
        // Non-copyable
        AlgorithmCacheRouterT(const AlgorithmCacheRouterT&) = delete;
        AlgorithmCacheRouterT& operator=(const AlgorithmCacheRouterT&) = delete;

        AlgorithmCacheRouterT(AlgorithmCacheRouterT&&) = default;
        AlgorithmCacheRouterT& operator=(AlgorithmCacheRouterT&&) = default;
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

        template<typename TContainer>
        bool RouteAlgorithm(Algorithm_t& algorithm, TContainer& container) const
        {
            using AlgorithmRouteToCache_t = RouteAlgorithmToCacheT<TContainer, Size_t>;
            using AlgorithmRouteWithCache_t = RouteAlgorithmWithCacheT<TContainer, Size_t>;
            //TRACE_CPUPROFILER_EVENT_SCOPE(TEXT("Routing"));
            const ChunkStructure_t* chunkStructure = &container.GetStructure();

            typename Map_t::iterator i = Cache.find(chunkStructure);
            if (i == Cache.end())
            {
                CachedRoutes.push_back(AlgorithmRoute_t());
                AlgorithmRoute_t* route = &CachedRoutes.back();
                Cache[chunkStructure] = route;
                AlgorithmRouteToCache_t routeToCache(&container, route);
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
                AlgorithmRouteWithCache_t router(&container, i->second);
                return algorithm.template Requirements<AlgorithmRouteWithCache_t&>(router);
            }
        }

        template<typename TContainer>
        bool TryRun(const Algorithm_t& algorithm, TContainer& container) const
        {
            return algorithm.TryRun(*this, container);
        }

        template<typename TContainer>
        bool TryRun(TContainer& container) const
        {
            return Algorithm_t().TryRun(*this, container);
        }

        template<typename TContainer>
        void Run(const Algorithm_t& algorithm, TContainer& container) const
        {
            algorithm.Run(*this, container);
        }

        template<typename TContainer>
        void Run(TContainer& container) const
        {
            Algorithm_t().Run(*this, container);
        }
    };

}
