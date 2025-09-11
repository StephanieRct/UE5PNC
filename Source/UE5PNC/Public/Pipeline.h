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
        typedef TChunkType TChunkType_t;

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
        typedef TChunkType ChunkType_t;
        typedef TSize Size_t;
        typedef TPipeline Pipeline_t;

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



        template<typename TChunk>
        bool TryRun(TChunk& chunk)
        {
            assert(!chunk.IsNull());
            const auto* chunkType = &chunk.GetChunkType();
            if (!Match(chunkType))
                return false;
            Impl()->Execute(chunk);
            return true;
        }

        template<typename TChunk>
        void TryRun(TChunk* chunk) = delete;


        template<typename TChunk>
        void Run(TChunk& chunk)
        {
            if (!TryRun(chunk))
            {
                checkf(false, TEXT("Could not run pipeline '%hs' on chunk '%hs'. The chunk failed the pipeline requirements."), typeid(Pipeline_t).name(), typeid(TChunk).name());
            }
        }
        template<typename TChunk>
        void Run(TChunk* chunk) = delete;



        //template<typename TChunk, typename TChunkType>
        //bool TryRunSameType(TChunk** chunk, Size_t count, const TChunkType* chunkType)
        //{
        //    assert(chunk != null);
        //    assert(chunkType != null);
        //    auto iMatching = ChunkTypeMatching.find(chunkType);
        //    if (iMatching == ChunkTypeMatching.end())
        //    {
        //        bool bMatch = Impl()->Requirements(PipelineRequirementMatchForChunkType<TChunkType>(chunkType));
        //        iMatching = ChunkTypeMatching.insert(iMatching, typename CacheMap_t::value_type(chunkType, bMatch));
        //    }
        //    if (!iMatching->second)
        //        return false;
        //    Impl()->Execute(chunk, count);
        //    return true;
        //}
        //template<typename TChunk>
        //void Run(TChunk** chunk, Size_t count)
    private:
        Pipeline_t* Impl() { return (reinterpret_cast<Pipeline_t*>(this)); }
    };


    template<typename TSize>
    struct AlgorithmRouteT
    {
    public:
        typedef TSize Size_t;

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

    template<typename TChunk, typename TSize>
    struct AlgorithmRouteToCacheT
    {
    public:
        typedef TChunk Chunk_t;
        typedef TSize Size_t;
        typedef AlgorithmRouteT<Size_t> AlgorithmRoute_t;

    public:
        bool MatchForChunk;
    protected:
        Chunk_t* Chunk;
        AlgorithmRoute_t* Route;

    public:
        AlgorithmRouteToCacheT(Chunk_t* chunk, AlgorithmRoute_t* route)
            : MatchForChunk(true)
            , Chunk(chunk)
            , Route(route)
        {
        }

        template<typename T>
        bool Component(T*& component)
        {
            const auto& chunkType = Chunk->GetChunkType();
            auto componentTypeIndexInChunk = chunkType.GetComponentTypeIndexInChunk(&typeid(T));
            Route->AddRoute(componentTypeIndexInChunk);

            if (componentTypeIndexInChunk == -1)
            {
                component = nullptr;
                MatchForChunk = false;
                return false;
            }
            component = (T*)Chunk->GetComponentData(componentTypeIndexInChunk);
            return true;
        }

        template<typename T>
        bool ParentComponent(T*& component)
        {
            auto parentChunk = Chunk->GetParentChunk();
            if (parentChunk == nullptr)
                return false;
            const auto& chunkType = parentChunk->GetChunkType();
            auto index = chunkType.GetComponentTypeIndexInChunk(&typeid(T));
            if (index < 0)
                return false;
            component = (T*)parentChunk->GetComponentData(index);
            return true;
        }
        bool ChunkIndex(Size_t& index)
        {
            index = 0;
            return true;
        }
    };

    template<typename TChunk, typename TSize>
    struct AlgorithmRouteWithCacheT
    {
    public:
        typedef TChunk Chunk_t;
        typedef TSize Size_t;
        typedef typename AlgorithmRouteT<TSize> AlgorithmRoute_t;

    protected:
        Chunk_t* Chunk;
        AlgorithmRoute_t* Route;
        Size_t CurrentComponentRoute;

    public:
        AlgorithmRouteWithCacheT(Chunk_t* chunk, AlgorithmRoute_t* route)
            : Chunk(chunk)
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

            component = (T*)Chunk->GetComponentData(componentTypeIndexInChunk);
            return true;
        }

        template<typename T>
        bool ParentComponent(T*& component)
        {
            auto parentChunk = Chunk->GetParentChunk();
            if (parentChunk == nullptr)
                return false;
            const auto& chunkType = parentChunk->GetChunkType();
            auto index = chunkType.GetComponentTypeIndexInChunk(&typeid(T));
            if (index < 0)
                return false;
            component = (T*)parentChunk->GetComponentData(index);
            return true;
        }
        bool ChunkIndex(Size_t& index)
        {
            index = 0;
            return true;
        }
    };


    template<typename TAlgorithm, typename TChunkType, typename TSize>
    struct AlgorithmRoutingCacheT
    {
    public:
        typedef TSize Size_t;
        typedef TChunkType ChunkType_t;
        typedef TAlgorithm Algorithm_t;
        typedef AlgorithmRouteT<TSize> AlgorithmRoute_t;

    protected:
        typedef std::unordered_map<const ChunkType_t*, AlgorithmRoute_t*> Map_t;
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

        template<typename TChunk>
        bool RouteAlgorithm(Algorithm_t& algorithm, TChunk& chunk) const
        {
            //TRACE_CPUPROFILER_EVENT_SCOPE(TEXT("Routing"));
            const ChunkType_t* chunkType = &chunk.GetChunkType();
            
            typename Map_t::iterator i = Cache.find(chunkType);
            if (i == Cache.end())
            {
                CachedRoutes.push_back(AlgorithmRoute_t());
                AlgorithmRoute_t * route = &CachedRoutes.back();
                Cache[chunkType] = route;
                AlgorithmRouteToCacheT<TChunk, Size_t> routeToCache(&chunk, route);
                bool matches = algorithm.Requirements<AlgorithmRouteToCacheT<TChunk, Size_t>&>(routeToCache);
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
                AlgorithmRouteWithCacheT<TChunk, Size_t> router(&chunk, i->second);
                return algorithm.Requirements<AlgorithmRouteWithCacheT<TChunk, Size_t>&>(router);
            }
        }
    };
}