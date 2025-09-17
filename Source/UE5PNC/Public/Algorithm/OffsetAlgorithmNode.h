// MIT License
// Copyright (c) 2025 Stephanie Rancourt

#pragma once
#include "common.h"

namespace PNC
{
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

}
