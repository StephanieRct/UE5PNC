// MIT License
// Copyright (c) 2025 Stephanie Rancourt

#pragma once
#include "common.h"
#include "AlgorithmRequirementFulfiller.h"

namespace PNC::Routing
{
    template<typename TChunkStructure>
    struct AlgorithmRequirementMatchForChunkType : public AlgorithmRequirementFulfiller
    {
    public:
        using ChunkType_t = TChunkStructure;

    protected:
        const TChunkStructure* ChunkStructure;

    public:
        AlgorithmRequirementMatchForChunkType(const TChunkStructure* chunkStructure)
            :ChunkStructure(chunkStructure)
        {
        }

        template<typename T>
        bool Component(T*& component)
        {
            auto index = ChunkStructure->GetComponentTypeIndexInChunk(&typeid(T));
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
