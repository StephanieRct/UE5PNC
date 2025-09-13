// MIT License
// Copyright (c) 2025 Stephanie Rancourt

#pragma once
#include "common.h"

#include "ChunkTree.h"

namespace PNC 
{
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

    template< typename TChunkAlgorithm, typename TChunkPointerElement>
    struct ChunkAlgorithmRunner<TChunkAlgorithm, ChunkArrayPointerT<TChunkPointerElement>>
    {
    public:
        using ChunkType_t = typename TChunkPointerElement::ChunkType_t;
        using Size_t = typename TChunkPointerElement::Size_t;
        using ChunkPointerElement_t = TChunkPointerElement;
        using ChunkAlgorithm_t = TChunkAlgorithm;
        using ChunkArrayPointer_t = ChunkArrayPointerT<TChunkPointerElement>;

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

    template< typename TChunkAlgorithm, typename TChunkPointerElement>
    struct ChunkAlgorithmRunner<TChunkAlgorithm, ChunkArrayTreePointerT<TChunkPointerElement>>
    {
    public:
        using ChunkType_t = typename TChunkPointerElement::ChunkType_t;
        using Size_t = typename TChunkPointerElement::Size_t;
        using ChunkPointerElement_t = TChunkPointerElement;
        using ChunkAlgorithm_t = TChunkAlgorithm;
        using ChunkArrayTreePointer_t = ChunkArrayTreePointerT<TChunkPointerElement>;

        static bool TryRun(ChunkAlgorithm_t& algorithm, ChunkArrayTreePointer_t& chunkPtr)
        {
            auto& chunkArray = *chunkPtr;
            if (chunkArray.IsNull())
                return false;
            if (!algorithm.Requirements(SetAlgorithmChunk<ChunkArrayTreePointer_t>(&chunkPtr)))
                return false;
            for (Size_t i = 0; i < chunkArray.GetChunkCount(); ++i)
            {
                auto& chunk = chunkArray[i];
                auto nodeCount = chunk.GetNodeCount();
                algorithm.Execute(nodeCount);
                if (!algorithm.Requirements(OffsetAlgorithmNode<ChunkArrayTreePointer_t>(nodeCount)))
                    return false;
            }
            return true;
        }

        template<typename TRouting>
        static bool TryRunWithRouting(const TRouting& routing, ChunkAlgorithm_t& algorithm, ChunkArrayTreePointer_t& chunkPtr)
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
                bool nextOk = algorithm.Requirements(OffsetAlgorithmNode<ChunkArrayTreePointer_t>(nodeCount));
                assert_pnc(nextOk);
            }
            return true;
        }
        template<typename TRouting>
        static void RunWithRouting(const TRouting& routing, ChunkAlgorithm_t& algorithm, ChunkArrayTreePointer_t& chunkPtr)
        {
            if (!TryRunWithRouting(routing, algorithm, chunkPtr))
            {
                checkf(false, TEXT("Could not run algorithm '%hs' on chunk '%hs'. The chunk failed the algorithm requirements."), typeid(ChunkAlgorithm_t).name(), typeid(ChunkArrayTreePointer_t).name());
            }
        }
    };

    template< typename TChunkAlgorithm, typename TChunkPointer>
    struct IdentifiableChunkAlgorithmRunner
    {
    public:
        using ChunkAlgorithm_t = TChunkAlgorithm;
        using ChunkType_t = typename TChunkPointer::ChunkType_t;

        using Size_t = typename TChunkPointer::Size_t;

        using IdentifiableChunk_t = IdentifiableChunkT<ChunkType_t>;

        

        using ChunkPointer_t = ChunkPointerT<ChunkType_t>;
        using ChunkArrayPointer_t = ChunkArrayPointerT<ChunkPointer_t>;
        using IdentifiableChunkPointer_t = IdentifiableChunkPointerT<ChunkType_t>;
        using IdentifiableChunkArrayPointer_t = IdentifiableChunkArrayPointerT<ChunkPointer_t>;
        using ChunkTreePointer_t = ChunkTreePointerT<ChunkType_t>;
        using ChunkArrayTreePointer_t = ChunkArrayTreePointerT<ChunkPointer_t>;
        

        static bool TryRun(ChunkAlgorithm_t& algorithm, IdentifiableChunk_t& chunkPtr)
        {
            switch (chunkPtr.Kind)
            {
            case ChunkKind_Chunk:
                return ChunkAlgorithmRunner<ChunkAlgorithm_t, ChunkPointer_t>::TryRun(algorithm, *(IdentifiableChunkPointer_t&)chunkPtr);
            case ChunkKind_ChunkArray:
                return ChunkAlgorithmRunner<ChunkAlgorithm_t, ChunkArrayPointer_t>::TryRun(algorithm, *(IdentifiableChunkArrayPointer_t&)chunkPtr);
            case ChunkKind_ChunkTree:
                return ChunkAlgorithmRunner<ChunkAlgorithm_t, ChunkTreePointer_t>::TryRun(algorithm, (ChunkTreePointer_t&)chunkPtr);
            case ChunkKind_ChunkArrayTree:
                return ChunkAlgorithmRunner<ChunkAlgorithm_t, ChunkArrayTreePointer_t>::TryRun(algorithm, (ChunkArrayTreePointer_t&)chunkPtr);
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
                return ChunkAlgorithmRunner<ChunkAlgorithm_t, ChunkArrayPointer_t>::TryRunWithRouting(routing, algorithm, *(IdentifiableChunkArrayPointer_t&)chunkPtr);
            case ChunkKind_ChunkTree:
                return ChunkAlgorithmRunner<ChunkAlgorithm_t, ChunkTreePointer_t>::TryRunWithRouting(routing, algorithm, (ChunkTreePointer_t&)chunkPtr);
            case ChunkKind_ChunkArrayTree:
                return ChunkAlgorithmRunner<ChunkAlgorithm_t, ChunkArrayTreePointer_t>::TryRunWithRouting(routing, algorithm, (ChunkArrayTreePointer_t&)chunkPtr);
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
                ChunkAlgorithmRunner<ChunkAlgorithm_t, ChunkArrayPointer_t>::RunWithRouting(routing, algorithm, *(IdentifiableChunkArrayPointer_t&)chunkPtr);
                break;
            case ChunkKind_ChunkTree:
                ChunkAlgorithmRunner<ChunkAlgorithm_t, ChunkTreePointer_t>::RunWithRouting(routing, algorithm, (ChunkTreePointer_t&)chunkPtr);
                break;
            case ChunkKind_ChunkArrayTree:
                ChunkAlgorithmRunner<ChunkAlgorithm_t, ChunkArrayTreePointer_t>::RunWithRouting(routing, algorithm, (ChunkArrayTreePointer_t&)chunkPtr);
                break;
            }
        }
    };

    // IdentifiableTreePointerT
    template< typename TChunkAlgorithm, typename TChunkType>
    struct IdentifiableChunkAlgorithmRunner<TChunkAlgorithm, IdentifiableTreePointerT<TChunkType>>
    {
    public:
        using ChunkAlgorithm_t = TChunkAlgorithm;
        using ChunkType_t = TChunkType;

        using Size_t = typename TChunkType::Size_t;

        using IdentifiableTreePointer_t = IdentifiableTreePointerT<ChunkType_t>;



        using ChunkPointer_t = ChunkPointerT<ChunkType_t>;
        using ChunkArrayPointer_t = ChunkArrayPointerT<ChunkPointer_t>;
        using IdentifiableChunkPointer_t = IdentifiableChunkPointerT<ChunkType_t>;
        using IdentifiableChunkArrayPointer_t = IdentifiableChunkArrayPointerT<ChunkType_t>;
        using ChunkTreePointer_t = ChunkTreePointerT<ChunkType_t>;
        using ChunkArrayTreePointer_t = ChunkArrayTreePointerT<ChunkPointer_t>;


        static bool TryRun(ChunkAlgorithm_t& algorithm, IdentifiableTreePointer_t& chunkPtr)
        {
            switch (chunkPtr.Kind)
            {
            case ChunkKind_Chunk:
            case ChunkKind_ChunkArray:
                checkNoEntry();
                return false;
            case ChunkKind_ChunkTree:
                return ChunkAlgorithmRunner<ChunkAlgorithm_t, ChunkTreePointer_t>::TryRun(algorithm, (ChunkTreePointer_t&)chunkPtr);
            case ChunkKind_ChunkArrayTree:
                return ChunkAlgorithmRunner<ChunkAlgorithm_t, ChunkArrayTreePointer_t>::TryRun(algorithm, (ChunkArrayTreePointer_t&)chunkPtr);
            }
            return true;
        }

        template<typename TRouting>
        static bool TryRunWithRouting(const TRouting& routing, ChunkAlgorithm_t& algorithm, IdentifiableTreePointer_t& chunkPtr)
        {
            switch (chunkPtr.Kind)
            {
            case ChunkKind_Chunk:
            case ChunkKind_ChunkArray:
                checkNoEntry();
                return false;
            case ChunkKind_ChunkTree:
                return ChunkAlgorithmRunner<ChunkAlgorithm_t, ChunkTreePointer_t>::TryRunWithRouting(routing, algorithm, (ChunkTreePointer_t&)chunkPtr);
            case ChunkKind_ChunkArrayTree:
                return ChunkAlgorithmRunner<ChunkAlgorithm_t, ChunkArrayTreePointer_t>::TryRunWithRouting(routing, algorithm, (ChunkArrayTreePointer_t&)chunkPtr);
            }
            return true;
        }

        template<typename TRouting>
        static void RunWithRouting(const TRouting& routing, ChunkAlgorithm_t& algorithm, IdentifiableTreePointer_t& chunkPtr)
        {
            switch (chunkPtr.Kind)
            {
            case ChunkKind_Chunk:
            case ChunkKind_ChunkArray:
                checkNoEntry();
                return;
            case ChunkKind_ChunkTree:
                return ChunkAlgorithmRunner<ChunkAlgorithm_t, ChunkTreePointer_t>::RunWithRouting(routing, algorithm, (ChunkTreePointer_t&)chunkPtr);
            case ChunkKind_ChunkArrayTree:
                return ChunkAlgorithmRunner<ChunkAlgorithm_t, ChunkArrayTreePointer_t>::RunWithRouting(routing, algorithm, (ChunkArrayTreePointer_t&)chunkPtr);
            }
            return true;
        }
    };

    // IdentifiableChunkPointerT
    template< typename TChunkAlgorithm, typename TChunkType>
    struct IdentifiableChunkAlgorithmRunner<TChunkAlgorithm, IdentifiableChunkPointerT<TChunkType>>
    {
    public:
        using ChunkAlgorithm_t = TChunkAlgorithm;
        using ChunkType_t = TChunkType;

        using Size_t = typename TChunkType::Size_t;

        using IdentifiableChunkPointer_t = IdentifiableChunkPointerT<ChunkType_t>;

        using ChunkPointer_t = ChunkPointerT<ChunkType_t>;
        using ChunkArrayPointer_t = ChunkArrayPointerT<ChunkPointer_t>;
        using IdentifiableChunkPointer_t = IdentifiableChunkPointerT<ChunkType_t>;
        using IdentifiableChunkArrayPointer_t = IdentifiableChunkArrayPointerT<ChunkType_t>;


        static bool TryRun(ChunkAlgorithm_t& algorithm, IdentifiableChunkPointer_t& chunkPtr)
        {
            switch (chunkPtr.Kind)
            {
            case ChunkKind_Chunk:
                return ChunkAlgorithmRunner<ChunkAlgorithm_t, ChunkPointer_t>::TryRun(algorithm, *(IdentifiableChunkPointer_t&)chunkPtr);
            case ChunkKind_ChunkArray:
                return ChunkAlgorithmRunner<ChunkAlgorithm_t, ChunkArrayPointer_t>::TryRun(algorithm, *(IdentifiableChunkArrayPointer_t&)chunkPtr);
            case ChunkKind_ChunkTree:
            case ChunkKind_ChunkArrayTree:
                checkNoEntry();
                return false;
            }
            return true;
        }

        template<typename TRouting>
        static bool TryRunWithRouting(const TRouting& routing, ChunkAlgorithm_t& algorithm, IdentifiableChunkPointer_t& chunkPtr)
        {
            switch (chunkPtr.Kind)
            {
            case ChunkKind_Chunk:
                return ChunkAlgorithmRunner<ChunkAlgorithm_t, ChunkPointer_t>::TryRunWithRouting(routing, algorithm, *(IdentifiableChunkPointer_t&)chunkPtr);
            case ChunkKind_ChunkArray:
                return ChunkAlgorithmRunner<ChunkAlgorithm_t, ChunkArrayPointer_t>::TryRunWithRouting(routing, algorithm, *(IdentifiableChunkArrayPointer_t&)chunkPtr);
            case ChunkKind_ChunkTree:
            case ChunkKind_ChunkArrayTree:
                checkNoEntry();
                return false;
            }
            return true;
        }

        template<typename TRouting>
        static void RunWithRouting(const TRouting& routing, ChunkAlgorithm_t& algorithm, IdentifiableChunkPointer_t& chunkPtr)
        {
            switch (chunkPtr.Kind)
            {
            case ChunkKind_Chunk:
                return ChunkAlgorithmRunner<ChunkAlgorithm_t, ChunkPointer_t>::RunWithRouting(routing, algorithm, *(IdentifiableChunkPointer_t&)chunkPtr);
            case ChunkKind_ChunkArray:
                return ChunkAlgorithmRunner<ChunkAlgorithm_t, ChunkArrayPointer_t>::RunWithRouting(routing, algorithm, *(IdentifiableChunkArrayPointer_t&)chunkPtr);
            case ChunkKind_ChunkTree:
            case ChunkKind_ChunkArrayTree:
                checkNoEntry();
                return;
            }
        }
    };

    // IdentifiableChunkArrayPointerT
    template< typename TChunkAlgorithm, typename TChunkPointerElement>
    struct IdentifiableChunkAlgorithmRunner<TChunkAlgorithm, IdentifiableChunkArrayPointerT<TChunkPointerElement>>
        : public IdentifiableChunkAlgorithmRunner<TChunkAlgorithm, IdentifiableChunkPointerT<typename TChunkPointerElement::Chunk_t>>
    {
    };

    // ChunkTreePointerT
    template< typename TChunkAlgorithm, typename TChunkType>
    struct IdentifiableChunkAlgorithmRunner<TChunkAlgorithm, ChunkTreePointerT<TChunkType>>
        : public IdentifiableChunkAlgorithmRunner<TChunkAlgorithm, IdentifiableTreePointerT<TChunkType>>
    {
    };

    // ChunkArrayTreePointerT
    template< typename TChunkAlgorithm, typename TChunkPointerElement>
    struct IdentifiableChunkAlgorithmRunner<TChunkAlgorithm, ChunkArrayTreePointerT<TChunkPointerElement>>
        : public IdentifiableChunkAlgorithmRunner<TChunkAlgorithm, IdentifiableTreePointerT<typename TChunkPointerElement::Chunk_t>>
    {
    };

    // ChunkPointerT
    template< typename TChunkAlgorithm, typename TChunkType>
    struct IdentifiableChunkAlgorithmRunner<TChunkAlgorithm, ChunkPointerT<TChunkType>>
        : public ChunkAlgorithmRunner<TChunkAlgorithm, ChunkPointerT<TChunkType>>
    {
    };

    // ChunkArrayPointerT
    template< typename TChunkAlgorithm, typename TChunkPointerElement>
    struct IdentifiableChunkAlgorithmRunner<TChunkAlgorithm, ChunkArrayPointerT<TChunkPointerElement>>
        : public ChunkAlgorithmRunner<TChunkAlgorithm, ChunkArrayPointerT<TChunkPointerElement>>
    {
    };

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
            return IdentifiableChunkAlgorithmRunner<ChunkAlgorithm_t, TChunkPointer>::TryRun(*Impl(), chunkPointer);
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
            return IdentifiableChunkAlgorithmRunner<ChunkAlgorithm_t, TChunkPointer>::TryRunWithRouting(routing, *Impl(), chunkPointer);
        }
        template<typename TRouting, typename TChunkPointer>
        void RunWithRouting(const TRouting& routing, TChunkPointer& chunkPointer)
        {
            return IdentifiableChunkAlgorithmRunner<ChunkAlgorithm_t, TChunkPointer>::RunWithRouting(routing, *Impl(), chunkPointer);
        }

    private:
        ChunkAlgorithm_t* Impl() { return (reinterpret_cast<ChunkAlgorithm_t*>(this)); }
    };

    template<typename TChunkPointer>
    struct SetAlgorithmChunkBase
    {
    public:
        using ChunkPointer_t = TChunkPointer;
        using ChunkType_t = typename ChunkPointer_t::ChunkType_t;
        using Size_t = typename ChunkPointer_t::Size_t;

    protected:
        ChunkPointer_t* ChunkPointer;

    public:
        SetAlgorithmChunkBase(ChunkPointer_t* chunkPointer)
            :ChunkPointer(chunkPointer)
        {
        }

        template<typename T>
        bool Component(T*& component)
        {
            auto& chunk = ChunkPointer->GetChunk();
            const auto& chunkType = chunk.GetChunkType();
            auto index = chunkType.GetComponentTypeIndexInChunk(&typeid(T));
            if (index < 0)
                return false;
            component = (T*)chunk.GetComponentData(index);
            return true;
        }

        bool ChunkIndex(Size_t& index)
        {
            index = 0;
            return true;
        }

        template<typename T>
        bool ParentComponent(T*& component)
        {
            return false;
        }

        bool ParentChunk(ChunkPointer_t*& parent)
        {
            parent = ChunkPointer->GetParentChunk();
            return parent != nullptr;
        }
        bool ChildrenChunk(ChunkPointer_t*& children)
        {
            children = ChunkPointer->GetFirstChildChunk();
            return children != nullptr;
        }
    };

    /// <summary>
    /// Will set the required component pointers on an algorithm from a given chunk.
    /// </summary>
    /// <typeparam name="TChunkPointer"></typeparam>
    template<typename TChunkPointer>
    struct SetAlgorithmChunk : public SetAlgorithmChunkBase<TChunkPointer>
    {
    public:
        using Base_t = SetAlgorithmChunkBase<TChunkPointer>;
        using ChunkPointer_t = TChunkPointer;
        using ChunkType_t = typename ChunkPointer_t::ChunkType_t;
        using Size_t = typename ChunkPointer_t::Size_t;


    public:
        SetAlgorithmChunk(ChunkPointer_t* chunkPointer)
            :Base_t(chunkPointer)
        {
        }
    };

    template<typename TChunkType>
    struct SetAlgorithmChunk<ChunkTreePointerT<TChunkType>> : public SetAlgorithmChunkBase<ChunkTreePointerT<TChunkType>>
    {
    public:
        using Base_t = SetAlgorithmChunkBase<ChunkTreePointerT<TChunkType>>;
        using ChunkType_t = TChunkType;
        using ChunkPointer_t = ChunkTreePointerT<ChunkType_t>;
        using Size_t = typename ChunkPointer_t::Size_t;

        SetAlgorithmChunk(ChunkPointer_t* chunkPointer)
            :Base_t(chunkPointer)
        {
        }

        template<typename T>
        bool ParentComponent(T*& component)
        {
            if (this->ChunkPointer->GetParentChunk() == nullptr)
                return false;
            auto& parentChunk = this->ChunkPointer->GetParentChunk()->GetChunk();
            const auto& chunkType = parentChunk.GetChunkType();
            auto index = chunkType.GetComponentTypeIndexInChunk(&typeid(T));
            if (index < 0)
                return false;
            component = (T*)parentChunk.GetComponentData(index);
            return true;
        }

        bool ParentChunk(ChunkPointer_t*& parent)
        {
            parent = this->ChunkPointer->GetParentChunk();
            return parent != nullptr;
        }
        bool ChildrenChunk(ChunkPointer_t*& children)
        {
            children = this->ChunkPointer->GetFirstChildChunk();
            return children != nullptr;
        }
    };

    template<typename TChunkPointerElement>
    struct SetAlgorithmChunk<ChunkArrayTreePointerT<TChunkPointerElement>> : public SetAlgorithmChunk<ChunkTreePointerT<typename TChunkPointerElement::ChunkType_t>>
    {
    public:
        using Base_t = SetAlgorithmChunk<ChunkTreePointerT<typename TChunkPointerElement::ChunkType_t>>;
        using ChunkPointerElement_t = TChunkPointerElement;
        using ChunkType_t = typename ChunkPointerElement_t::ChunkType_t;
        using ChunkPointer_t = ChunkArrayTreePointerT<TChunkPointerElement>;
        using Size_t = typename ChunkPointer_t::Size_t;

        SetAlgorithmChunk(ChunkPointer_t* chunkPointer)
            :Base_t(chunkPointer)
        {
        }

    };

    template<typename TChunkArray>
    struct OffsetAlgorithmNode
    {
    public:
        using ChunkArray_t = TChunkArray;
        using Size_t = typename TChunkArray::Size_t;

    protected:
        Size_t NodeOffset;
    public:
        OffsetAlgorithmNode(Size_t nodeOffset)
            : NodeOffset(nodeOffset)
        {
        }

        template<typename T>
        bool Component(T*& component)
        {
            switch (T::Owner)
            {
            case ComponentOwner_Chunk:
                ++component;
                break;
            case ComponentOwner_Node:
                component += NodeOffset;
                break;
            }
            return true;
        }

        template<typename T>
        bool ParentComponent(T*& component)
        {
            return true;
        }

        template<typename TChunk>
        bool ParentChunk(TChunk*& parent)
        {
            return true;
        }
        template<typename TChunk>
        bool ChildrenChunk(TChunk*& children)
        {
            return true;
        }
        bool ChunkIndex(Size_t& index)
        {
            ++index;
            return true;
        }
    };

    template<typename TChunkType>
    struct AlgorithmRequirementMatchForChunkType
    {
    public:
        typedef TChunkType TChunkType_t;

    protected:
        const TChunkType* ChunkType;

    public:
        AlgorithmRequirementMatchForChunkType(const TChunkType* chunkType)
            :ChunkType(chunkType)
        {
        }

        template<typename T>
        bool Component(T*& component)
        {
            auto index = ChunkType->GetComponentTypeIndexInChunk(&typeid(T));
            return index >= 0;
        }

        template<typename T>
        bool ParentComponent(T*& component)
        {
            return true;
        }

        template<typename TChunk>
        bool ParentChunk(TChunk*& parent)
        {
            return true;
        }

        template<typename TChunk>
        bool ChildrenChunk(TChunk*& children)
        {
            return true;
        }
    };
}