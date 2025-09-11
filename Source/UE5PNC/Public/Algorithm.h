// MIT License
// Copyright (c) 2025 Stephanie Rancourt

#pragma once
#include "common.h"

namespace PNC 
{
    template< typename TChunkAlgorithm, typename TChunk>
    struct ChunkAlgorithmRunner
    {
    public:
        using ChunkAlgorithm_t = TChunkAlgorithm;
        using Chunk_t = TChunk;

        static bool TryRun(TChunkAlgorithm& algorithm, Chunk_t& chunk)
        {
            
            if (chunk.IsNull())
                return false;
            if (!algorithm.Requirements(SetAlgorithmChunk<Chunk_t>(&chunk)))
                return false;
            algorithm.Execute(chunk.GetNodeCount());
            return true;
        }

        template<typename TRouting>
        static void RunWithRouting(const TRouting& routing, ChunkAlgorithm_t& algorithm, Chunk_t& chunk)
        {
            assert_pnc(!chunk.IsNull());
            if (!routing.RouteAlgorithm(algorithm, chunk))
            {
                checkf(false, TEXT("Could not run algorithm '%hs' on chunk '%hs'. The chunk failed the algorithm requirements."), typeid(ChunkAlgorithm_t).name(), typeid(Chunk_t).name());
            }
            else
                algorithm.Execute(chunk.GetNodeCount());
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

        static bool TryRun(ChunkAlgorithm_t& algorithm, ChunkArrayPointer_t& chunkArray)
        {
            if (chunkArray.IsNull())
                return false;
            if (!algorithm.Requirements(SetAlgorithmChunk<ChunkArrayPointer_t>(&chunkArray)))
                return false;
            for (Size_t i = 0; i < chunkArray.GetChunkCount(); ++i)
            {
                auto& chunk = chunkArray[i];
                algorithm.Execute(chunk.GetNodeCount());
                if (!algorithm.Requirements(SetAlgorithmChunkArrayNext<ChunkArrayPointer_t>(&chunkArray)))
                    return false;
            }
            return true;
        }

        template<typename TRouting>
        static void RunWithRouting(const TRouting& routing, ChunkAlgorithm_t& algorithm, ChunkArrayPointer_t& chunkArray)
        {
            assert_pnc(!chunkArray.IsNull());
            if (!routing.RouteAlgorithm(algorithm, chunkArray))
            {
                checkf(false, TEXT("Could not run algorithm '%hs' on chunk '%hs'. The chunk failed the algorithm requirements."), typeid(ChunkAlgorithm_t).name(), typeid(ChunkArrayPointer_t).name());
            }
            else
            {

                for (Size_t i = 0; i < chunkArray.GetChunkCount(); ++i)
                {
                    auto& chunk = chunkArray[i];
                    algorithm.Execute(chunk.GetNodeCount());
                    if (!algorithm.Requirements(SetAlgorithmChunkArrayNext<ChunkArrayPointer_t>(&chunkArray)))
                        return false;
                }
            }
        }
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
        template<typename TChunk>
        bool TryRun(TChunk& chunk) 
        {
            return ChunkAlgorithmRunner<ChunkAlgorithm_t, TChunk>::TryRun(*Impl(), chunk);
        }
        template<typename TChunk>
        void TryRun(TChunk* chunk) = delete;

        /// <summary>
        /// Will execute the algorithm on a matching chunk.
        /// The chunk must not be null and must match the algorithm or it halt execution
        /// </summary>
        /// <typeparam name="TChunk"></typeparam>
        /// <param name="chunk"></param>
        template<typename TChunk>
        void Run(TChunk& chunk) 
        {
            if (!TryRun(chunk))
            {
                checkf(false, TEXT("Could not run algorithm '%hs' on chunk '%hs'. The chunk failed the algorithm requirements."), typeid(ChunkAlgorithm_t).name(), typeid(TChunk).name());
            }
        }
        template<typename TChunk>
        void Run(TChunk* chunk) = delete;


        template<typename TRouting, typename TChunk>
        void RunWithRouting(const TRouting& routing, TChunk& chunk)
        {
            return ChunkAlgorithmRunner<ChunkAlgorithm_t, TChunk>::RunWithRouting(routing, *Impl(), chunk);
        }

    private:
        ChunkAlgorithm_t* Impl() { return (reinterpret_cast<ChunkAlgorithm_t*>(this)); }
    };

    /// <summary>
    /// Will set the required component pointers on an algorithm from a given chunk.
    /// </summary>
    /// <typeparam name="TChunk"></typeparam>
    template<typename TChunk>
    struct SetAlgorithmChunk
    {
    public:
        using Chunk_t = TChunk;
        using Size_t = typename Chunk_t::Size_t;

    protected:
        Chunk_t* Chunk;

    public:
        SetAlgorithmChunk(Chunk_t* chunk)
            :Chunk(chunk) 
        {
        }

        template<typename T>
        bool Component(T*& component) 
        {
            const auto& chunkType = Chunk->GetChunkType();
            auto index = chunkType.GetComponentTypeIndexInChunk(&typeid(T));
            if (index < 0)
                return false;
            component = (T*)Chunk->GetComponentData(index);
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

        bool ParentChunk(Chunk_t*& parent)
        {
            parent = Chunk->GetParentChunk();
            return parent != nullptr;
        }
        bool ChildrenChunk(Chunk_t*& children)
        {
            children = Chunk->GetFirstChildChunk();
            return children != nullptr;
        }
    };

    //template<typename TChunkPointer>
    //struct SetAlgorithmChunk<ChunkArrayT<TChunkPointer>>
    //    : public SetAlgorithmChunk<ChunkArrayPointerT<TChunkPointer>>
    //{
    //public:
    //    using Size_t = typename TChunkPointer::Size_t;
    //    using Chunk_t = ChunkArrayT<TChunkPointer>;

    //protected:
    //    Chunk_t* Chunk;

    //public:
    //    SetAlgorithmChunk(Chunk_t* chunk)
    //        :Chunk(chunk)
    //    {
    //    }

    //    bool ChunkIndex(Size_t& index)
    //    {
    //        index = 0;
    //        return true;
    //    }

    //};

    template<typename TChunkArray>
    struct SetAlgorithmChunkArrayNext
    {
    public:
        using ChunkArray_t = TChunkArray;
        using Size_t = typename TChunkArray::Size_t;

    protected:
        ChunkArray_t* ChunkArray;

    public:
        SetAlgorithmChunkArrayNext(ChunkArray_t* chunkArray)
            :ChunkArray(chunkArray)
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
                component += ChunkArray->GetNodeCapacityPerChunk();
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
    //template<typename TChunkPointer, typename TChunkType, typename TSize>
    //struct SetAlgorithmChunk<ChunkArrayPointerT<TChunkPointer, TChunkType, TSize>>
    //{
    //public:
    //    using Chunk_t = ChunkArrayPointerT<TChunkPointer, TChunkType, TSize>;

    //protected:
    //    Chunk_t* Chunk;

    //public:
    //    SetAlgorithmChunk(Chunk_t* chunkArray, Size_t chunkIndex)
    //        :Chunk(chunk)
    //    {
    //    }

    //    template<typename T>
    //    bool Component(T*& component)
    //    {
    //        const auto& chunkType = Chunk->GetChunkType();
    //        auto index = chunkType.GetComponentTypeIndexInChunk(&typeid(T));
    //        if (index < 0)
    //            return false;
    //        component = (T*)Chunk->GetComponentData(index);
    //        return true;
    //    }

    //    template<typename T>
    //    bool ParentComponent(T*& component)
    //    {
    //        auto parentChunk = Chunk->GetParentChunk();
    //        if (parentChunk == nullptr)
    //            return false;
    //        const auto& chunkType = parentChunk->GetChunkType();
    //        auto index = chunkType.GetComponentTypeIndexInChunk(&typeid(T));
    //        if (index < 0)
    //            return false;
    //        component = (T*)parentChunk->GetComponentData(index);
    //        return true;
    //    }

    //    bool ParentChunk(Chunk_t*& parent)
    //    {
    //        parent = Chunk->GetParentChunk();
    //    }
    //    bool ChildrenChunk(Chunk_t*& children)
    //    {
    //        children = Chunk->GetFirstChildChunk();
    //    }
    //};


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




    //template<typename TChunkTree>
    //struct SetAlgorithmChunkTree {
    //public:
    //    TChunkTree* chunk;
    //    SetAlgorithmChunkTree(TChunkTree* aChunk)
    //        :chunk(aChunk) {

    //    }
    //    template<typename T>
    //    bool NodeComponent(T*& aComponent) {
    //        auto chunkType = chunk->getChunkType();
    //        auto index = chunkType.components.getComponentTypeIndexInChunk(&typeid(T));
    //        if (index < 0)
    //            return false;
    //        aComponent = (T*)chunk->getComponentData(index);
    //        return true;
    //    }
    //    template<typename T>
    //    bool ParentComponent(T*& aComponent) {
    //        //auto chunkType = chunk->getChunkType();
    //        //auto index = chunkType.components.getComponentTypeIndexInChunk(&typeid(T));
    //        //if (index < 0)
    //        //    return false;
    //        //aComponent = (T*)chunk->getComponentData(index);
    //        return true;
    //    }
    //    template<typename T>
    //    bool ChildrenComponent(T*& aComponent) {
    //        //auto chunkType = chunk->getChunkType();
    //        //auto index = chunkType.components.getComponentTypeIndexInChunk(&typeid(T));
    //        //if (index < 0)
    //        //    return false;
    //        //aComponent = (T*)chunk->getComponentData(index);
    //        return true;
    //    }
    //};
    ////template<typename TChildrenRequirements>
    ////struct ChildrenWithRequirementsIterator {
    ////public:
    ////    ChunkTree* firstChunk;
    ////    ChunkTree* currentChunk;
    ////    TChildrenRequirements currentChildChunk;
    ////    unsigned int cycleCount = 0;
    ////    ChildrenWithRequirementsIterator(ChunkTree* first, const TChildrenRequirements& currentChildChunk) {}
    ////    bool operator==(const ChildrenWithRequirementsIterator& o)const { return currentChunk == o.currentChunk; }
    ////    bool operator!=(const ChildrenWithRequirementsIterator& o)const { return currentChunk != o.currentChunk; }
    ////    const ChildrenWithRequirementsIterator& operator++() {
    ////        do {
    ////            currentChunk = currentChunk->nextSibling;
    ////            if (currentChunk == firstChunk) {
    ////                ++cycleCount;
    ////                break;
    ////            }
    ////        } while (currentChildChunk.requirements(SetAlgorithmChunk<TChunk>(currentChunk)))
    ////            return *this;
    ////    }
    ////    const ChildrenWithRequirementsIterator& operator--() {
    ////        do {
    ////            currentChunk = currentChunk->previousSibling;
    ////            if (currentChunk == firstChunk) {
    ////                ++cycleCount;
    ////                break;
    ////            }
    ////        } while (currentChildChunk.requirements(SetAlgorithmChunk<TChunk>(currentChunk)));
    ////        return *this;
    ////    }
    ////    ChildrenWithRequirementsIterator operator++(int) {
    ////        ChildrenWithRequirementsIterator o(*this);
    ////        operator++();
    ////        return o;
    ////    }
    ////    ChildrenWithRequirementsIterator operator--(int) {
    ////        ChildrenWithRequirementsIterator o(*this);
    ////        operator--();
    ////        return o;
    ////    }
    ////};

    //template< typename TRequirements >
    //struct ParentRequirements {
    //};

    //template< typename TRequirements >
    //struct ChildrenRequirements {
    //public:
    //    ChunkTree* first;
    //    //ChildrenWithRequirementsIterator<TRequirements> begin()const {
    //    //    return ChildrenWithRequirementsIterator(first, *(TRequirements*)this);
    //    //}
    //};

}