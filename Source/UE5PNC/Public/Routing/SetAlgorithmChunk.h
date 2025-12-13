// MIT License
// Copyright (c) 2025 Stephanie Rancourt

#pragma once
#include "common.h"

#include "AlgorithmRequirementFulfiller.h"

namespace NiT::Routing
{
    template<typename TContainer>
    struct SetAlgorithmChunkBase : public AlgorithmRequirementFulfiller
    {
    public:
        using Base_t = AlgorithmRequirementFulfiller;
        using Self_t = SetAlgorithmChunkBase< TContainer>;
        using Container_t = TContainer;
        using ChunkStructure_t = typename Container_t::ChunkStructure_t;
        using Size_t = typename Container_t::Size_t;

    protected:
        Container_t* Container;

    public:
        SetAlgorithmChunkBase(Container_t* container)
            :Container(container)
        {
        }

        template<typename T>
        bool Component(T*& component)
        {
            auto& chunk = Container->GetChunk();
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

        bool ParentChunk(Container_t*& parent)
        {
            parent = Container->GetParentChunk();
            return parent != nullptr;
        }
        bool ChildrenChunk(Container_t*& children)
        {
            children = Container->GetFirstChildChunk();
            return children != nullptr;
        }
    };

    /// <summary>
    /// Will set the required component pointers on an algorithm from a given chunk.
    /// </summary>
    /// <typeparam name="TChunkPointer"></typeparam>
    template<typename TContainer>
    struct SetAlgorithmChunk : public SetAlgorithmChunkBase<TContainer>
    {
    public:
        using Base_t = SetAlgorithmChunkBase<TContainer>;
        using Self_t = SetAlgorithmChunk<TContainer>;
        using Container_t = TContainer;
        using ChunkStructure_t = typename Container_t::ChunkStructure_t;
        using Size_t = typename Container_t::Size_t;


    public:
        SetAlgorithmChunk(Container_t* container)
            :Base_t(container)
        {
        }
    };
}

