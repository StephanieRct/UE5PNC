// MIT License
// Copyright (c) 2025 Stephanie Rancourt

#pragma once
#include "common.h"

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
        typedef TChunkAlgorithm ChunkAlgorithm_t;

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
            if (chunk.IsNull())
                return false;
            if (!Impl()->Requirements(SetAlgorithmChunk<TChunk>(&chunk)))
                return false;
            Impl()->Execute(chunk.GetSize());
            return true;
        }

        /// <summary>
        /// Will execute the algorithm on a matching chunk.
        /// The chunk must not be null and must match the algorithm or it halt execution
        /// </summary>
        /// <typeparam name="TChunk"></typeparam>
        /// <param name="chunk"></param>
        template<typename TChunk>
        void Run(TChunk& chunk) 
        {
            check(!chunk.IsNull());
            if (!Impl()->Requirements(SetAlgorithmChunk<TChunk>(&chunk)))
            {
                checkf(false, TEXT("Could not run algorithm '%hs' on chunk '%hs'. The chunk failed the algorithm requirements."), typeid(ChunkAlgorithm_t).name(), typeid(TChunk).name());
            }
            Impl()->Execute(chunk.GetSize());
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
        typedef TChunk Chunk_t;

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