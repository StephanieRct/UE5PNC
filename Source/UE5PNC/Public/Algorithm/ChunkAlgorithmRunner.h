// MIT License
// Copyright (c) 2025 Stephanie Rancourt

#pragma once
#include "common.h"
#include "SetAlgorithmChunk.h"
#include "OffsetAlgorithmNode.h"
#include "AlgorithmRequirementMatchForChunkType.h"

namespace PNC
{

    /// <summary>
    /// Execute an algorithm on a chunk. 
    /// If TChunkPointer is an array pointer (ChunkArrayPointer), the algorithm will execute only once on the whole array as a single chunk.
    /// Use ChunkArrayAlgorithmRunner to execute the algorithm on each element of an array pointer
    /// </summary>
    /// <typeparam name="TChunkAlgorithm"></typeparam>
    /// <typeparam name="TChunkPointer"></typeparam>
    template< typename TChunkAlgorithm, typename TChunkPointer>
    struct ChunkAlgorithmRunner
    {
    public:
        using ChunkAlgorithm_t = TChunkAlgorithm;
        using ChunkPointer_t = TChunkPointer;

        static bool TryRun(TChunkAlgorithm& algorithm, ChunkPointer_t& chunkPtr)
        {
            auto& chunk = *chunkPtr;
            if (chunk.IsNull())
                return false;
            if (!algorithm.Requirements(SetAlgorithmChunk<ChunkPointer_t>(&chunkPtr)))
                return false;
            algorithm.Execute(chunk.GetNodeCount());
            return true;
        }

        template<typename TRouting>
        static bool TryRunWithRouting(const TRouting& routing, ChunkAlgorithm_t& algorithm, ChunkPointer_t& chunkPtr)
        {
            auto& chunk = *chunkPtr;
            assert_pnc(!chunk.IsNull());
            if (!routing.RouteAlgorithm(algorithm, chunkPtr))
                return false;
            algorithm.Execute(chunk.GetNodeCount());
            return true;
        }
        template<typename TRouting>
        static void RunWithRouting(const TRouting& routing, ChunkAlgorithm_t& algorithm, ChunkPointer_t& chunkPtr)
        {
            if (!TryRunWithRouting(routing, algorithm, chunkPtr))
            {
                checkf(false, TEXT("Could not run algorithm '%hs' on chunk '%hs'. The chunk failed the algorithm requirements."), typeid(ChunkAlgorithm_t).name(), typeid(ChunkPointer_t).name());
            }
        }
    };

    /// <summary>
    /// Execute an algorithm on each element chunk in a chunk array.
    /// </summary>
    /// <typeparam name="TChunkAlgorithm"></typeparam>
    /// <typeparam name="TChunkArrayPointer"></typeparam>
    template< typename TChunkAlgorithm, typename TChunkArrayPointer>
    struct ChunkArrayAlgorithmRunner
    {
    public:
        using ChunkAlgorithm_t = TChunkAlgorithm;
        using ChunkArrayPointer_t = TChunkArrayPointer;
        using ChunkType_t = typename TChunkArrayPointer::ChunkType_t;
        using Size_t = typename TChunkArrayPointer::Size_t;

        static bool TryRun(ChunkAlgorithm_t& algorithm, ChunkArrayPointer_t& chunkPtr)
        {
            auto& chunkArray = *chunkPtr;
            if (chunkArray.IsNull())
                return false;
            if (!algorithm.Requirements(SetAlgorithmChunk<ChunkArrayPointer_t>(&chunkPtr)))
                return false;
            for (Size_t i = 0; i < chunkArray.GetChunkCount(); ++i)
            {
                auto& chunk = chunkArray[i];
                auto nodeCount = chunk.GetNodeCount();
                algorithm.Execute(nodeCount);
                if (!algorithm.Requirements(OffsetAlgorithmNode<ChunkArrayPointer_t>(nodeCount)))
                    return false;
            }
            return true;
        }

        template<typename TRouting>
        static bool TryRunWithRouting(const TRouting& routing, ChunkAlgorithm_t& algorithm, ChunkArrayPointer_t& chunkPtr)
        {
            auto& chunkArray = *chunkPtr;
            assert_pnc(!chunkArray.IsNull());
            if (!routing.RouteAlgorithm(algorithm, chunkPtr))
                return false;

            for (Size_t i = 0; i < chunkArray.GetChunkCount(); ++i)
            {
                auto& chunk = chunkArray[i];
                auto nodeCount = chunk.GetNodeCount();
                algorithm.Execute(nodeCount);
                bool nextOk = algorithm.Requirements(OffsetAlgorithmNode<ChunkArrayPointer_t>(nodeCount));
                assert_pnc(nextOk);
            }
            return true;
        }
        template<typename TRouting>
        static void RunWithRouting(const TRouting& routing, ChunkAlgorithm_t& algorithm, ChunkArrayPointer_t& chunkPtr)
        {
            if (!TryRunWithRouting(routing, algorithm, chunkPtr))
            {
                checkf(false, TEXT("Could not run algorithm '%hs' on chunk '%hs'. The chunk failed the algorithm requirements."), typeid(ChunkAlgorithm_t).name(), typeid(ChunkArrayPointer_t).name());
            }
        }
    };


    template<typename TChunkType, typename TChunkAlgorithm, typename TIdentifiableChunkPointer = IdentifiableChunkT<TChunkType>>
    struct IdentifiableChunkAlgorithmRunner
    {
    public:
        using ChunkType_t = TChunkType;
        using ChunkAlgorithm_t = TChunkAlgorithm;
        using IdentifiableChunkPointer_t = TIdentifiableChunkPointer;
        using Size_t = typename ChunkType_t::Size_t;

        using ChunkPointer_t = ChunkPointerT<ChunkType_t>;
        using IdentifiableChunk_t = IdentifiableChunkT<ChunkType_t>;
        using ChunkArrayPointer_t = ChunkArrayPointerT<ChunkType_t, ChunkPointer_t>;
        //using IdentifiableChunkPointer_t = IdentifiableChunkPointerT<ChunkType_t>;
        using IdentifiableChunkArrayPointer_t = IdentifiableChunkArrayPointerT<ChunkType_t, ChunkPointer_t>;
        using ChunkTreePointer_t = ChunkTreePointerT<ChunkType_t>;
        using ChunkArrayTreePointer_t = ChunkArrayTreePointerT<ChunkType_t, ChunkPointer_t>;


        static bool TryRun(ChunkAlgorithm_t& algorithm, IdentifiableChunk_t& chunkPtr)
        {
            switch (chunkPtr.Kind)
            {
            case ChunkKind_Chunk:
                return ChunkAlgorithmRunner<ChunkAlgorithm_t, ChunkPointer_t>::TryRun(algorithm, *(IdentifiableChunkPointer_t&)chunkPtr);
            case ChunkKind_ChunkArray:
                return ChunkArrayAlgorithmRunner<ChunkAlgorithm_t, ChunkArrayPointer_t>::TryRun(algorithm, *(IdentifiableChunkArrayPointer_t&)chunkPtr);
            case ChunkKind_ChunkTree:
                return ChunkAlgorithmRunner<ChunkAlgorithm_t, ChunkTreePointer_t>::TryRun(algorithm, (ChunkTreePointer_t&)chunkPtr);
            case ChunkKind_ChunkArrayTree:
                return ChunkArrayAlgorithmRunner<ChunkAlgorithm_t, ChunkArrayTreePointer_t>::TryRun(algorithm, (ChunkArrayTreePointer_t&)chunkPtr);
            }
            return true;
        }

        template<typename TRouting>
        static bool TryRunWithRouting(const TRouting& routing, ChunkAlgorithm_t& algorithm, IdentifiableChunk_t& chunkPtr)
        {
            switch (chunkPtr.Kind)
            {
            case ChunkKind_Chunk:
                return ChunkAlgorithmRunner<ChunkAlgorithm_t, ChunkPointer_t>::TryRunWithRouting(routing, algorithm, *(IdentifiableChunkPointer_t&)chunkPtr);
            case ChunkKind_ChunkArray:
                return ChunkArrayAlgorithmRunner<ChunkAlgorithm_t, ChunkArrayPointer_t>::TryRunWithRouting(routing, algorithm, *(IdentifiableChunkArrayPointer_t&)chunkPtr);
            case ChunkKind_ChunkTree:
                return ChunkAlgorithmRunner<ChunkAlgorithm_t, ChunkTreePointer_t>::TryRunWithRouting(routing, algorithm, (ChunkTreePointer_t&)chunkPtr);
            case ChunkKind_ChunkArrayTree:
                return ChunkArrayAlgorithmRunner<ChunkAlgorithm_t, ChunkArrayTreePointer_t>::TryRunWithRouting(routing, algorithm, (ChunkArrayTreePointer_t&)chunkPtr);
            }
            return true;
        }

        template<typename TRouting>
        static void RunWithRouting(const TRouting& routing, ChunkAlgorithm_t& algorithm, IdentifiableChunk_t& chunkPtr)
        {
            switch (chunkPtr.Kind)
            {
            case ChunkKind_Chunk:
                ChunkAlgorithmRunner<ChunkAlgorithm_t, ChunkPointer_t>::RunWithRouting(routing, algorithm, *(IdentifiableChunkPointer_t&)chunkPtr);
                break;
            case ChunkKind_ChunkArray:
                ChunkArrayAlgorithmRunner<ChunkAlgorithm_t, ChunkArrayPointer_t>::RunWithRouting(routing, algorithm, *(IdentifiableChunkArrayPointer_t&)chunkPtr);
                break;
            case ChunkKind_ChunkTree:
                ChunkAlgorithmRunner<ChunkAlgorithm_t, ChunkTreePointer_t>::RunWithRouting(routing, algorithm, (ChunkTreePointer_t&)chunkPtr);
                break;
            case ChunkKind_ChunkArrayTree:
                ChunkArrayAlgorithmRunner<ChunkAlgorithm_t, ChunkArrayTreePointer_t>::RunWithRouting(routing, algorithm, (ChunkArrayTreePointer_t&)chunkPtr);
                break;
            }
        }
    };


    //template< typename TChunkAlgorithm, typename TChunkPointer>
    //struct IdentifiableChunkAlgorithmRunner
    //{
    //public:
    //    using ChunkAlgorithm_t = TChunkAlgorithm;
    //    using ChunkPointer_t = TChunkPointer;
    //    using ChunkType_t = typename ChunkPointer_t::ChunkType_t;
    //    using Size_t = typename ChunkPointer_t::Size_t;

    //    using IdentifiableChunk_t = IdentifiableChunkT<ChunkType_t>;
    //    using ChunkArrayPointer_t = ChunkArrayPointerT<ChunkType_t, ChunkPointer_t>;
    //    using IdentifiableChunkPointer_t = IdentifiableChunkPointerT<ChunkType_t>;
    //    using IdentifiableChunkArrayPointer_t = IdentifiableChunkArrayPointerT<ChunkType_t, ChunkPointer_t>;
    //    using ChunkTreePointer_t = ChunkTreePointerT<ChunkType_t>;
    //    using ChunkArrayTreePointer_t = ChunkArrayTreePointerT<ChunkType_t, ChunkPointer_t>;


    //    static bool TryRun(ChunkAlgorithm_t& algorithm, IdentifiableChunk_t& chunkPtr)
    //    {
    //        switch (chunkPtr.Kind)
    //        {
    //        case ChunkKind_Chunk:
    //            return ChunkAlgorithmRunner<ChunkAlgorithm_t, ChunkPointer_t>::TryRun(algorithm, *(IdentifiableChunkPointer_t&)chunkPtr);
    //        case ChunkKind_ChunkArray:
    //            return ChunkAlgorithmRunner<ChunkAlgorithm_t, ChunkArrayPointer_t>::TryRun(algorithm, *(IdentifiableChunkArrayPointer_t&)chunkPtr);
    //        case ChunkKind_ChunkTree:
    //            return ChunkAlgorithmRunner<ChunkAlgorithm_t, ChunkTreePointer_t>::TryRun(algorithm, (ChunkTreePointer_t&)chunkPtr);
    //        case ChunkKind_ChunkArrayTree:
    //            return ChunkAlgorithmRunner<ChunkAlgorithm_t, ChunkArrayTreePointer_t>::TryRun(algorithm, (ChunkArrayTreePointer_t&)chunkPtr);
    //        }
    //        return true;
    //    }

    //    template<typename TRouting>
    //    static bool TryRunWithRouting(const TRouting& routing, ChunkAlgorithm_t& algorithm, IdentifiableChunk_t& chunkPtr)
    //    {
    //        switch (chunkPtr.Kind)
    //        {
    //        case ChunkKind_Chunk:
    //            return ChunkAlgorithmRunner<ChunkAlgorithm_t, ChunkPointer_t>::TryRunWithRouting(routing, algorithm, *(IdentifiableChunkPointer_t&)chunkPtr);
    //        case ChunkKind_ChunkArray:
    //            return ChunkAlgorithmRunner<ChunkAlgorithm_t, ChunkArrayPointer_t>::TryRunWithRouting(routing, algorithm, *(IdentifiableChunkArrayPointer_t&)chunkPtr);
    //        case ChunkKind_ChunkTree:
    //            return ChunkAlgorithmRunner<ChunkAlgorithm_t, ChunkTreePointer_t>::TryRunWithRouting(routing, algorithm, (ChunkTreePointer_t&)chunkPtr);
    //        case ChunkKind_ChunkArrayTree:
    //            return ChunkAlgorithmRunner<ChunkAlgorithm_t, ChunkArrayTreePointer_t>::TryRunWithRouting(routing, algorithm, (ChunkArrayTreePointer_t&)chunkPtr);
    //        }
    //        return true;
    //    }

    //    template<typename TRouting>
    //    static void RunWithRouting(const TRouting& routing, ChunkAlgorithm_t& algorithm, IdentifiableChunk_t& chunkPtr)
    //    {
    //        switch (chunkPtr.Kind)
    //        {
    //        case ChunkKind_Chunk:
    //            ChunkAlgorithmRunner<ChunkAlgorithm_t, ChunkPointer_t>::RunWithRouting(routing, algorithm, *(IdentifiableChunkPointer_t&)chunkPtr);
    //            break;
    //        case ChunkKind_ChunkArray:
    //            ChunkAlgorithmRunner<ChunkAlgorithm_t, ChunkArrayPointer_t>::RunWithRouting(routing, algorithm, *(IdentifiableChunkArrayPointer_t&)chunkPtr);
    //            break;
    //        case ChunkKind_ChunkTree:
    //            ChunkAlgorithmRunner<ChunkAlgorithm_t, ChunkTreePointer_t>::RunWithRouting(routing, algorithm, (ChunkTreePointer_t&)chunkPtr);
    //            break;
    //        case ChunkKind_ChunkArrayTree:
    //            ChunkAlgorithmRunner<ChunkAlgorithm_t, ChunkArrayTreePointer_t>::RunWithRouting(routing, algorithm, (ChunkArrayTreePointer_t&)chunkPtr);
    //            break;
    //        }
    //    }
    //};

    //// IdentifiableTreePointerT
    //template< typename TChunkAlgorithm, typename TChunkType>
    //struct IdentifiableChunkAlgorithmRunner<TChunkAlgorithm, IdentifiableTreePointerT<TChunkType>>
    //{
    //public:
    //    using ChunkAlgorithm_t = TChunkAlgorithm;
    //    using ChunkType_t = TChunkType;
    //    using Size_t = typename TChunkType::Size_t;
    //    using IdentifiableTreePointer_t = IdentifiableTreePointerT<ChunkType_t>;
    //    using ChunkPointer_t = ChunkPointerT<ChunkType_t>;
    //    using ChunkArrayPointer_t = ChunkArrayPointerT<ChunkPointer_t>;
    //    using ChunkTreePointer_t = ChunkTreePointerT<ChunkType_t>;
    //    using ChunkArrayTreePointer_t = ChunkArrayTreePointerT<ChunkType_t, ChunkPointer_t>;


    //    static bool TryRun(ChunkAlgorithm_t& algorithm, IdentifiableTreePointer_t& chunkPtr)
    //    {
    //        switch (chunkPtr.Kind)
    //        {
    //        case ChunkKind_Chunk:
    //        case ChunkKind_ChunkArray:
    //            checkNoEntry();
    //            return false;
    //        case ChunkKind_ChunkTree:
    //            return ChunkAlgorithmRunner<ChunkAlgorithm_t, ChunkTreePointer_t>::TryRun(algorithm, (ChunkTreePointer_t&)chunkPtr);
    //        case ChunkKind_ChunkArrayTree:
    //            return ChunkAlgorithmRunner<ChunkAlgorithm_t, ChunkArrayTreePointer_t>::TryRun(algorithm, (ChunkArrayTreePointer_t&)chunkPtr);
    //        }
    //        return true;
    //    }

    //    template<typename TRouting>
    //    static bool TryRunWithRouting(const TRouting& routing, ChunkAlgorithm_t& algorithm, IdentifiableTreePointer_t& chunkPtr)
    //    {
    //        switch (chunkPtr.Kind)
    //        {
    //        case ChunkKind_Chunk:
    //        case ChunkKind_ChunkArray:
    //            checkNoEntry();
    //            return false;
    //        case ChunkKind_ChunkTree:
    //            return ChunkAlgorithmRunner<ChunkAlgorithm_t, ChunkTreePointer_t>::TryRunWithRouting(routing, algorithm, (ChunkTreePointer_t&)chunkPtr);
    //        case ChunkKind_ChunkArrayTree:
    //            return ChunkAlgorithmRunner<ChunkAlgorithm_t, ChunkArrayTreePointer_t>::TryRunWithRouting(routing, algorithm, (ChunkArrayTreePointer_t&)chunkPtr);
    //        }
    //        return true;
    //    }

    //    template<typename TRouting>
    //    static void RunWithRouting(const TRouting& routing, ChunkAlgorithm_t& algorithm, IdentifiableTreePointer_t& chunkPtr)
    //    {
    //        switch (chunkPtr.Kind)
    //        {
    //        case ChunkKind_Chunk:
    //        case ChunkKind_ChunkArray:
    //            checkNoEntry();
    //            break;
    //        case ChunkKind_ChunkTree:
    //            ChunkAlgorithmRunner<ChunkAlgorithm_t, ChunkTreePointer_t>::RunWithRouting(routing, algorithm, (ChunkTreePointer_t&)chunkPtr);
    //            break;
    //        case ChunkKind_ChunkArrayTree:
    //            ChunkAlgorithmRunner<ChunkAlgorithm_t, ChunkArrayTreePointer_t>::RunWithRouting(routing, algorithm, (ChunkArrayTreePointer_t&)chunkPtr);
    //            break;
    //        }
    //    }
    //};

    //// IdentifiableChunkPointerT
    //template< typename TChunkAlgorithm, typename TChunkType>
    //struct IdentifiableChunkAlgorithmRunner<TChunkAlgorithm, IdentifiableChunkPointerT<TChunkType>>
    //{
    //public:
    //    using ChunkAlgorithm_t = TChunkAlgorithm;
    //    using ChunkType_t = TChunkType;
    //    using Size_t = typename TChunkType::Size_t;
    //    using IdentifiableChunkPointer_t = IdentifiableChunkPointerT<ChunkType_t>;
    //    using ChunkPointer_t = ChunkPointerT<ChunkType_t>;
    //    using ChunkArrayPointer_t = ChunkArrayPointerT<ChunkPointer_t>;
    //    using IdentifiableChunkPointer_t = IdentifiableChunkPointerT<ChunkType_t>;
    //    using IdentifiableChunkArrayPointer_t = IdentifiableChunkArrayPointerT<ChunkType_t, ChunkPointer_t>;

    //public:
    //    static bool TryRun(ChunkAlgorithm_t& algorithm, IdentifiableChunkPointer_t& chunkPtr)
    //    {
    //        switch (chunkPtr.Kind)
    //        {
    //        case ChunkKind_Chunk:
    //            return ChunkAlgorithmRunner<ChunkAlgorithm_t, ChunkPointer_t>::TryRun(algorithm, *(IdentifiableChunkPointer_t&)chunkPtr);
    //        case ChunkKind_ChunkArray:
    //            return ChunkAlgorithmRunner<ChunkAlgorithm_t, ChunkArrayPointer_t>::TryRun(algorithm, *(IdentifiableChunkArrayPointer_t&)chunkPtr);
    //        case ChunkKind_ChunkTree:
    //        case ChunkKind_ChunkArrayTree:
    //            checkNoEntry();
    //            return false;
    //        }
    //        return true;
    //    }

    //    template<typename TRouting>
    //    static bool TryRunWithRouting(const TRouting& routing, ChunkAlgorithm_t& algorithm, IdentifiableChunkPointer_t& chunkPtr)
    //    {
    //        switch (chunkPtr.Kind)
    //        {
    //        case ChunkKind_Chunk:
    //            return ChunkAlgorithmRunner<ChunkAlgorithm_t, ChunkPointer_t>::TryRunWithRouting(routing, algorithm, *(IdentifiableChunkPointer_t&)chunkPtr);
    //        case ChunkKind_ChunkArray:
    //            return ChunkAlgorithmRunner<ChunkAlgorithm_t, ChunkArrayPointer_t>::TryRunWithRouting(routing, algorithm, *(IdentifiableChunkArrayPointer_t&)chunkPtr);
    //        case ChunkKind_ChunkTree:
    //        case ChunkKind_ChunkArrayTree:
    //            checkNoEntry();
    //            return false;
    //        }
    //        return true;
    //    }

    //    template<typename TRouting>
    //    static void RunWithRouting(const TRouting& routing, ChunkAlgorithm_t& algorithm, IdentifiableChunkPointer_t& chunkPtr)
    //    {
    //        switch (chunkPtr.Kind)
    //        {
    //        case ChunkKind_Chunk:
    //            return ChunkAlgorithmRunner<ChunkAlgorithm_t, ChunkPointer_t>::RunWithRouting(routing, algorithm, *(IdentifiableChunkPointer_t&)chunkPtr);
    //        case ChunkKind_ChunkArray:
    //            return ChunkAlgorithmRunner<ChunkAlgorithm_t, ChunkArrayPointer_t>::RunWithRouting(routing, algorithm, *(IdentifiableChunkArrayPointer_t&)chunkPtr);
    //        case ChunkKind_ChunkTree:
    //        case ChunkKind_ChunkArrayTree:
    //            checkNoEntry();
    //            return;
    //        }
    //    }
    //};

    //// IdentifiableChunkArrayPointerT
    //template< typename TChunkAlgorithm, typename TChunkType, typename TChunkPointerElement>
    //struct IdentifiableChunkAlgorithmRunner<TChunkAlgorithm, IdentifiableChunkArrayPointerT<TChunkType, TChunkPointerElement>>
    //    : public IdentifiableChunkAlgorithmRunner<TChunkAlgorithm, IdentifiableChunkPointerT<TChunkType>>
    //{
    //};

    //// ChunkTreePointerT
    //template< typename TChunkAlgorithm, typename TChunkType>
    //struct IdentifiableChunkAlgorithmRunner<TChunkAlgorithm, ChunkTreePointerT<TChunkType>>
    //    : public IdentifiableChunkAlgorithmRunner<TChunkAlgorithm, IdentifiableTreePointerT<TChunkType>>
    //{
    //};

    //// ChunkArrayTreePointerT
    //template< typename TChunkAlgorithm, typename TChunkType, typename TChunkPointerElement>
    //struct IdentifiableChunkAlgorithmRunner<TChunkAlgorithm, ChunkArrayTreePointerT<TChunkType, TChunkPointerElement>>
    //    : public IdentifiableChunkAlgorithmRunner<TChunkAlgorithm, IdentifiableTreePointerT<TChunkType>>
    //{
    //};

    //// ChunkPointerT
    //template< typename TChunkAlgorithm, typename TChunkType>
    //struct IdentifiableChunkAlgorithmRunner<TChunkAlgorithm, ChunkPointerT<TChunkType>>
    //    : public ChunkAlgorithmRunner<TChunkAlgorithm, ChunkPointerT<TChunkType>>
    //{
    //};

    //// ChunkArrayPointerT
    //template< typename TChunkAlgorithm, typename TChunkType, typename TChunkPointerElement>
    //struct IdentifiableChunkAlgorithmRunner<TChunkAlgorithm, ChunkArrayPointerT<TChunkType, TChunkPointerElement>>
    //    : public ChunkAlgorithmRunner<TChunkAlgorithm, ChunkArrayPointerT<TChunkType, TChunkPointerElement>>
    //{
    //};


    //// ChunkT
    //template< typename TChunkAlgorithm, typename TChunkType>
    //struct IdentifiableChunkAlgorithmRunner<TChunkAlgorithm, ChunkT<TChunkType>>
    //    : public ChunkAlgorithmRunner<TChunkAlgorithm, ChunkPointerT<TChunkType>>
    //{
    //};

    //// ChunkTreeT
    //template< typename TChunkAlgorithm, typename TChunkType>
    //struct IdentifiableChunkAlgorithmRunner<TChunkAlgorithm, ChunkTreeT<TChunkType>>
    //    : public IdentifiableChunkAlgorithmRunner<TChunkAlgorithm, IdentifiableTreePointerT<TChunkType>>
    //{
    //};





    //template< typename TChunkAlgorithm, typename TChunkPointerElement>
    //struct ChunkAlgorithmRunner<TChunkAlgorithm, ChunkArrayPointerT<TChunkPointerElement>>
    //{
    //public:
    //    using ChunkType_t = typename TChunkPointerElement::ChunkType_t;
    //    using Size_t = typename TChunkPointerElement::Size_t;
    //    using ChunkPointerElement_t = TChunkPointerElement;
    //    using ChunkAlgorithm_t = TChunkAlgorithm;
    //    using ChunkArrayPointer_t = ChunkArrayPointerT<TChunkPointerElement>;

    //    static bool TryRun(ChunkAlgorithm_t& algorithm, ChunkArrayPointer_t& chunkPtr)
    //    {
    //        auto& chunkArray = *chunkPtr;
    //        if (chunkArray.IsNull())
    //            return false;
    //        if (!algorithm.Requirements(SetAlgorithmChunk<ChunkArrayPointer_t>(&chunkPtr)))
    //            return false;
    //        for (Size_t i = 0; i < chunkArray.GetChunkCount(); ++i)
    //        {
    //            auto& chunk = chunkArray[i];
    //            auto nodeCount = chunk.GetNodeCount();
    //            algorithm.Execute(nodeCount);
    //            if (!algorithm.Requirements(OffsetAlgorithmNode<ChunkArrayPointer_t>(nodeCount)))
    //                return false;
    //        }
    //        return true;
    //    }

    //    template<typename TRouting>
    //    static bool TryRunWithRouting(const TRouting& routing, ChunkAlgorithm_t& algorithm, ChunkArrayPointer_t& chunkPtr)
    //    {
    //        auto& chunkArray = *chunkPtr;
    //        assert_pnc(!chunkArray.IsNull());
    //        if (!routing.RouteAlgorithm(algorithm, chunkPtr))
    //            return false;

    //        for (Size_t i = 0; i < chunkArray.GetChunkCount(); ++i)
    //        {
    //            auto& chunk = chunkArray[i];
    //            auto nodeCount = chunk.GetNodeCount();
    //            algorithm.Execute(nodeCount);
    //            bool nextOk = algorithm.Requirements(OffsetAlgorithmNode<ChunkArrayPointer_t>(nodeCount));
    //            assert_pnc(nextOk);
    //        }
    //        return true;
    //    }
    //    template<typename TRouting>
    //    static void RunWithRouting(const TRouting& routing, ChunkAlgorithm_t& algorithm, ChunkArrayPointer_t& chunkPtr)
    //    {
    //        if (!TryRunWithRouting(routing, algorithm, chunkPtr))
    //        {
    //            checkf(false, TEXT("Could not run algorithm '%hs' on chunk '%hs'. The chunk failed the algorithm requirements."), typeid(ChunkAlgorithm_t).name(), typeid(ChunkArrayPointer_t).name());
    //        }
    //    }
    //};

    //template< typename TChunkAlgorithm, typename TChunkType, typename TChunkPointerElement>
    //struct ChunkAlgorithmRunner<TChunkAlgorithm, ChunkArrayTreePointerT<TChunkType, TChunkPointerElement>>
    //{
    //public:
    //    using ChunkAlgorithm_t = TChunkAlgorithm;
    //    using ChunkType_t = TChunkType;
    //    using ChunkPointerElement_t = TChunkPointerElement;
    //    using Size_t = typename TChunkType::Size_t;
    //    using ChunkArrayTreePointer_t = ChunkArrayTreePointerT<ChunkType_t, TChunkPointerElement>;

    //    static bool TryRun(ChunkAlgorithm_t& algorithm, ChunkArrayTreePointer_t& chunkPtr)
    //    {
    //        auto& chunkArray = *chunkPtr;
    //        if (chunkArray.IsNull())
    //            return false;
    //        if (!algorithm.Requirements(SetAlgorithmChunk<ChunkArrayTreePointer_t>(&chunkPtr)))
    //            return false;
    //        for (Size_t i = 0; i < chunkArray.GetChunkCount(); ++i)
    //        {
    //            auto& chunk = chunkArray[i];
    //            auto nodeCount = chunk.GetNodeCount();
    //            algorithm.Execute(nodeCount);
    //            if (!algorithm.Requirements(OffsetAlgorithmNode<ChunkArrayTreePointer_t>(nodeCount)))
    //                return false;
    //        }
    //        return true;
    //    }

    //    template<typename TRouting>
    //    static bool TryRunWithRouting(const TRouting& routing, ChunkAlgorithm_t& algorithm, ChunkArrayTreePointer_t& chunkPtr)
    //    {
    //        auto& chunkArray = *chunkPtr;
    //        assert_pnc(!chunkArray.IsNull());
    //        if (!routing.RouteAlgorithm(algorithm, chunkPtr))
    //            return false;

    //        for (Size_t i = 0; i < chunkArray.GetChunkCount(); ++i)
    //        {
    //            auto& chunk = chunkArray[i];
    //            auto nodeCount = chunk.GetNodeCount();
    //            algorithm.Execute(nodeCount);
    //            bool nextOk = algorithm.Requirements(OffsetAlgorithmNode<ChunkArrayTreePointer_t>(nodeCount));
    //            assert_pnc(nextOk);
    //        }
    //        return true;
    //    }
    //    template<typename TRouting>
    //    static void RunWithRouting(const TRouting& routing, ChunkAlgorithm_t& algorithm, ChunkArrayTreePointer_t& chunkPtr)
    //    {
    //        if (!TryRunWithRouting(routing, algorithm, chunkPtr))
    //        {
    //            checkf(false, TEXT("Could not run algorithm '%hs' on chunk '%hs'. The chunk failed the algorithm requirements."), typeid(ChunkAlgorithm_t).name(), typeid(ChunkArrayTreePointer_t).name());
    //        }
    //    }
    //};

}
