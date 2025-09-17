// MIT License
// Copyright (c) 2025 Stephanie Rancourt

#pragma once
#include "common.h"

#include "..\KChunkTreePointer.h"
#include "..\KChunkArrayTreePointer.h"

namespace PNC
{
    template<typename TChunkPointer>
    struct SetAlgorithmChunkBase
    {
    public:
        using ChunkPointer_t = TChunkPointer;
        using ChunkType_t = typename ChunkPointer_t::ChunkType_t;
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
            const auto& chunkType = chunk.GetChunkType();
            auto index = chunkType.GetComponentTypeIndexInChunk(&typeid(T));
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
        using ChunkPointer_t = TChunkPointer;
        using ChunkType_t = typename ChunkPointer_t::ChunkType_t;
        using Size_t = typename ChunkPointer_t::Size_t;


    public:
        SetAlgorithmChunk(ChunkPointer_t* chunkPointer)
            :Base_t(chunkPointer)
        {
        }
    };

    template<typename TChunkType>
    struct SetAlgorithmChunk<KChunkTreePointerT<TChunkType>> : public SetAlgorithmChunkBase<KChunkTreePointerT<TChunkType>>
    {
    public:
        using Base_t = SetAlgorithmChunkBase<KChunkTreePointerT<TChunkType>>;
        using ChunkType_t = TChunkType;
        using Size_t = typename ChunkType_t::Size_t;
        using ChunkPointer_t = KChunkTreePointerT<ChunkType_t>;

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
            const auto& chunkType = parentChunk.GetChunkType();
            auto index = chunkType.GetComponentTypeIndexInChunk(&typeid(T));
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

    template<typename TChunkType, typename TChunkPointerElement>
    struct SetAlgorithmChunk<KChunkArrayTreePointerT<TChunkType, TChunkPointerElement>> : public SetAlgorithmChunk<KChunkTreePointerT<TChunkType>>
    {
    public:
        using Base_t = SetAlgorithmChunk<KChunkTreePointerT<TChunkType>>;
        using ChunkType_t = TChunkType;
        using ChunkPointerElement_t = TChunkPointerElement;
        using Size_t = typename TChunkType::Size_t;
        using ChunkPointer_t = KChunkArrayTreePointerT<ChunkType_t, ChunkPointerElement_t>;

        SetAlgorithmChunk(ChunkPointer_t* chunkPointer)
            :Base_t(chunkPointer)
        {
        }

    };

}
