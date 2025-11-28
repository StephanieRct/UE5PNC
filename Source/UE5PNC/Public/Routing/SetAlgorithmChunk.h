// MIT License
// Copyright (c) 2025 Stephanie Rancourt

#pragma once
#include "common.h"

#include "AlgorithmRequirementFulfiller.h"

namespace PNC::Routing
{
    template<typename TChunkPointer>
    struct SetAlgorithmChunkBase : public AlgorithmRequirementFulfiller
    {
    public:
        using Base_t = AlgorithmRequirementFulfiller;
        using Self_t = SetAlgorithmChunkBase< TChunkPointer>;
        using ChunkPointer_t = TChunkPointer;
        using ChunkStructure_t = typename ChunkPointer_t::ChunkStructure_t;
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
            const auto& chunkStructure = chunk.GetStructure();
            auto index = chunkStructure.GetComponentTypeIndexInChunk(&typeid(T));
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
        using Self_t = SetAlgorithmChunk<TChunkPointer>;
        using ChunkPointer_t = TChunkPointer;
        using ChunkStructure_t = typename ChunkPointer_t::ChunkStructure_t;
        using Size_t = typename ChunkPointer_t::Size_t;


    public:
        SetAlgorithmChunk(ChunkPointer_t* chunkPointer)
            :Base_t(chunkPointer)
        {
        }
    };
}

