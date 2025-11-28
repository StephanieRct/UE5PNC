// MIT License
// Copyright (c) 2025 Stephanie Rancourt

#pragma once
#include "common.h"
#include "KContainers.h"

//#include "routing\AlgorithmRouter.h"
//#include "routing\AlgorithmCacheRouter.h"
#include "Algorithm.h"
#include "AlgorithmRunnerChunk.h"

namespace PNC
{
}

namespace PNC::Routing
{
    template<typename TChunkStructure>
    struct SetAlgorithmChunk<KChunkTreePointerT<TChunkStructure>> : public SetAlgorithmChunkBase<KChunkTreePointerT<TChunkStructure>>
    {
    public:
        using Base_t = SetAlgorithmChunkBase<KChunkTreePointerT<TChunkStructure>>;
        using Self_t = SetAlgorithmChunk<KChunkTreePointerT<TChunkStructure>>;
        using ChunkStructure_t = TChunkStructure;
        using Size_t = typename ChunkStructure_t::Size_t;
        using ChunkPointer_t = KChunkTreePointerT<ChunkStructure_t>;

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
            const auto& chunkStructure = parentChunk.GetStructure();
            auto index = chunkStructure.GetComponentTypeIndexInChunk(&typeid(T));
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

    template<typename TChunkStructure, typename TChunkPointerElement>
    struct SetAlgorithmChunk<KChunkArrayTreePointerT<TChunkStructure, TChunkPointerElement>> : public SetAlgorithmChunk<KChunkTreePointerT<TChunkStructure>>
    {
    public:
        using Base_t = SetAlgorithmChunk<KChunkTreePointerT<TChunkStructure>>;
        using Self_t = SetAlgorithmChunk<KChunkArrayTreePointerT<TChunkStructure, TChunkPointerElement>>;
        using ChunkStructure_t = TChunkStructure;
        using ChunkPointerElement_t = TChunkPointerElement;
        using Size_t = typename TChunkStructure::Size_t;
        using ChunkPointer_t = KChunkArrayTreePointerT<ChunkStructure_t, ChunkPointerElement_t>;

        SetAlgorithmChunk(ChunkPointer_t* chunkPointer)
            :Base_t(chunkPointer)
        {
        }

    };

}