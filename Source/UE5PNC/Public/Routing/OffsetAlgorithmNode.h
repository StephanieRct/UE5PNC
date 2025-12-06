// MIT License
// Copyright (c) 2025 Stephanie Rancourt

#pragma once
#include "common.h"
#include "AlgorithmRequirementFulfiller.h"

namespace NiT::Routing
{
    template<typename TChunkArray>
    struct OffsetAlgorithmNode : public AlgorithmRequirementFulfiller
    {
    public:
        using Base_t = AlgorithmRequirementFulfiller;
        using Self_t = OffsetAlgorithmNode<TChunkArray>;
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
            case ComponentOwner::Chunk:
                ++component;
                break;
            case ComponentOwner::Node:
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

}
