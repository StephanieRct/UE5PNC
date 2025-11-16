// MIT License
// Copyright (c) 2025 Stephanie Rancourt

#pragma once
#include "common.h"

namespace PNC
{
    template<typename TBase>
    struct DGrowingChunkT : public TBase
    {
    public:
        using Base_t = TBase;
        using Self_t = DGrowingChunkT<TBase>;
        using typename Base_t::ChunkStructure_t;
        using typename Base_t::Chunk_t;
        using typename Base_t::Size_t;
        using typename Base_t::Node_t;
        using ComponentType_t = typename ChunkStructure_t::ComponentType_t;
        using ChunkPointerInternal_t = ChunkPointerT<ChunkStructure_t>::ChunkPointerInternal_t;

    public:
        using Base_t::Base_t;
        using Base_t::GetNodeCapacity;
        using Base_t::AddNodes;
        PNC_USING_CHUNKPOINTER_INTERFACE();
        PNC_IMPLEMENT_CHUNKPOINTER_SELFPOINTER();

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
