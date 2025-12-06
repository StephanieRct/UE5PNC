// MIT License
// Copyright (c) 2025 Stephanie Rancourt

#pragma once
#include "common.h"
#include "KContainers.h"

//#include "routing\AlgorithmRouter.h"
//#include "routing\AlgorithmCacheRouter.h"
#include "Algorithm.h"
#include "AlgorithmRunnerChunk.h"
#include "AlgorithmRunnerKindPointerSwitch.h"

namespace NiT
{
    //template<typename TA, typename TC, bool THasCunk, bool THasArray> 
    //struct AlgorithmRunnerSelector< TA, TC, THasCunk, THasArray, /*THasKind:*/true> : AlgorithmRunnerKindPointerSwitch<TA, TC> {};

}

namespace NiT::Routing
{
    template<typename TChunkStructure>
    struct SetAlgorithmChunk<KChunkTreePointerT<TChunkStructure>> : public SetAlgorithmChunkBase<KChunkTreePointerT<TChunkStructure>>
    {
    public:
        using Base_t = SetAlgorithmChunkBase<KChunkTreePointerT<TChunkStructure>>;
        using Self_t = SetAlgorithmChunk<KChunkTreePointerT<TChunkStructure>>;
        using ChunkStructure_t = TChunkStructure;
        using Size_t = typename ChunkStructure_t::Size_t;
        using Container_t = KChunkTreePointerT<ChunkStructure_t>;

        SetAlgorithmChunk(Container_t* container)
            :Base_t(container)
        {
        }

        template<typename T>
        bool ParentComponent(T*& component)
        {
            if (this->Container->GetParentChunk() == nullptr)
                return false;
            auto& parentChunk = this->Container->GetParentChunk()->GetChunk();
            const auto& chunkStructure = parentChunk.GetStructure();
            auto index = chunkStructure.GetComponentTypeIndexInChunk(&typeid(T));
            if (index < 0)
                return false;
            component = (T*)parentChunk.GetComponentData(index);
            return true;
        }

        bool ParentChunk(Container_t*& parent)
        {
            parent = this->Container->GetParentChunk();
            return parent != nullptr;
        }
        bool ChildrenChunk(Container_t*& children)
        {
            children = this->Container->GetFirstChildChunk();
            return children != nullptr;
        }
    };

    template<typename TChunkStructure, typename TChunkPointerElement>
    struct SetAlgorithmChunk<KArrayTreePointerT<TChunkStructure, TChunkPointerElement>> : public SetAlgorithmChunk<KChunkTreePointerT<TChunkStructure>>
    {
    public:
        using Base_t = SetAlgorithmChunk<KChunkTreePointerT<TChunkStructure>>;
        using Self_t = SetAlgorithmChunk<KArrayTreePointerT<TChunkStructure, TChunkPointerElement>>;
        using ChunkStructure_t = TChunkStructure;
        using ChunkPointerElement_t = TChunkPointerElement;
        using Size_t = typename TChunkStructure::Size_t;
        using Container_t = KArrayTreePointerT<ChunkStructure_t, ChunkPointerElement_t>;

        SetAlgorithmChunk(Container_t* container)
            :Base_t(container)
        {
        }

    };

}