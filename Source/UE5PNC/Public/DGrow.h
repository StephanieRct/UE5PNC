// MIT License
// Copyright (c) 2025 Stephanie Rancourt

#pragma once
#include "common.h"

namespace PNC
{
    template<typename TBase>
    struct DGrow : public TBase
    {
    public:
        using Base_t = TBase;
        using Self_t = DGrow<TBase>;
        using typename Base_t::Size_t;
        using typename Base_t::ComponentType_t;
        using typename Base_t::ChunkStructure_t;
        using typename Base_t::Chunk_t;
        using typename Base_t::Node_t;
        using typename Base_t::ChunkPointerInternal_t;

    public:
        using Base_t::Base_t;
        using Base_t::GetNodeCapacity;
        using Base_t::GetChunkCapacity;
        using Base_t::AddNodes;
        PNC_USING_CHUNKPOINTER_INTERFACE();
        PNC_IMPLEMENT_CHUNKPOINTER_SELFPOINTER();


        Size_t AddNode() 
        { 
            return AddNodes(1); 
        }

        Size_t AddNodes(const Size_t count)
        {
            const NodeCountT<Size_t> nodeCount = GetNodeCount();
            const NodeCapacityT<Size_t> nodeCapacity = GetNodeCapacity();
            if (nodeCount + count > nodeCapacity)
            {
                const NodeCapacityT<Size_t> newNodeCapacity = std::max<Size_t>(nodeCapacity * 2, nodeCount + count);
                ReallocateMove(*this, *this, newNodeCapacity, GetChunkCapacity());
                this->SetNodeCapacity(newNodeCapacity);
            }
            return Base_t::AddNodes(count);
        }

    protected:
        using TBase::SetNodeCapacity;
        using TBase::ReallocateMove;
        
    };
}
