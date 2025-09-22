// MIT License
// Copyright (c) 2025 Stephanie Rancourt

#pragma once
#include "common.h"

#include "..\KChunkTreePointer.h"
#include "..\KChunkArrayTreePointer.h"
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
            const auto& chunkStructure = chunk.GetChunkStructure();
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
            const auto& chunkStructure = parentChunk.GetChunkStructure();
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
