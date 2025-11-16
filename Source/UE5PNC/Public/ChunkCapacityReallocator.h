// MIT License
// Copyright (c) 2025 Stephanie Rancourt

#pragma once
#include "common.h"

namespace PNC
{
    template<typename TBase>
    struct ChunkCapacityReallocatorT : public TBase
    {
    public:
        using Base_t = TBase;
        using Self_t = ChunkCapacityReallocatorT<TBase>;
        using typename TBase::ChunkStructure_t;
        using typename TBase::Size_t;
        using typename Base_t::Node_t;
        using ComponentType_t = typename ChunkStructure_t::ComponentType_t;
        using ChunkPointerInternal_t = ChunkPointerT<ChunkStructure_t>::ChunkPointerInternal_t;

        using Base_t::GetNodeCount;
        using Base_t::GetNodeCapacity;
        using Base_t::AddNodes;
        using Base_t::IsSameStructure;
        using Base_t::GetInternalChunk;
    public:
        using Base_t::Base_t;

        Size_t AddNode() 
        { 
            return AddNodes(1); 
        }

        Size_t AddNodes(const Size_t count)
        {
            const Size_t nodeCount = GetNodeCount();
            const Size_t nodeCapacity = GetNodeCapacity();
            if (nodeCount + count > nodeCapacity)
            {
                const Size_t newCapacity = std::max(nodeCapacity * 2, nodeCount + count);
                ReallocateDataMove(*this, *this, newCapacity);
                this->SetNodeCapacity(newCapacity);
            }
            return Base_t::AddNodes(count);
        }

    protected:
        using TBase::SetNodeCapacity;
        using TBase::ReallocateDataMove;
        
    };
}
