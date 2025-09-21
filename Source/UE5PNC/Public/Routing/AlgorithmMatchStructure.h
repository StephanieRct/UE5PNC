// MIT License
// Copyright (c) 2025 Stephanie Rancourt

#pragma once
#include "common.h"
#include "AlgorithmRequirementFulfiller.h"

namespace PNC::Routing
{
    template<typename TChunkStructure>
    struct AlgorithmMatchStructure : public AlgorithmRequirementFulfiller
    {
    public:
        using Base_t = AlgorithmRequirementFulfiller;
        using Self_t = AlgorithmMatchStructure<TChunkStructure>;
        using ChunkStructure_t = TChunkStructure;

    protected:
        const ChunkStructure_t* ChunkStructure;

    public:
        AlgorithmMatchStructure(const ChunkStructure_t* chunkStructure)
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
