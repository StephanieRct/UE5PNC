// MIT License
// Copyright (c) 2025 Stephanie Rancourt

#pragma once
#include "common.h"

namespace NiT
{
    /// <summary>
    /// Will reallocate with a greater NodeCapacity when adding nodes passed the current NodeCapacity.
    /// It will not shrink NodeCapacity when nodes are removed.
    /// </summary>
    /// <typeparam name="TBase"></typeparam>
    template<typename TBase>
    struct DBunchPointer : public TBase
    {
    public:
        using Base_t = TBase;
        using Self_t = DBunchPointer<TBase>;
        using typename Base_t::Size_t;
        using typename Base_t::ComponentType_t;
        using typename Base_t::ChunkStructure_t;
        using typename Base_t::Node_t;
        using typename Base_t::ChunkPointerInternal_t;

    public:
        using Base_t::Base_t;
        using Base_t::GetNodeCount;
        using Base_t::GetNodeCapacity;
        using Base_t::GetChunkCapacity;
        using Base_t::AddNodes;

        /// <summary>
        /// Add a single node at NodeCount and return the index.
        /// It will reallocate with a greater NodeCapacity if NodeCount == NodeCapacity.
        /// </summary>
        /// <returns></returns>
        Size_t AddNode() 
        { 
            return AddNodes(1); 
        }
        /// <summary>
        /// Add a multiple sequential nodes at NodeCount and return the index of the first node.
        /// It will reallocate with a greater NodeCapacity if NodeCount + count >= NodeCapacity.
        /// </summary>
        Size_t AddNodes(const Size_t count)
        {
            const auto nodeCount =    GetNodeCount();
            const auto nodeCapacity = GetNodeCapacity();
            if (nodeCount + count > nodeCapacity)
            {
                const NodeCapacityT<Size_t> newNodeCapacity = std::max(nodeCapacity * 2, PropCountToCapacity(nodeCount + count));
                ReallocateMove(*this, *this, newNodeCapacity, GetChunkCapacity());
                this->SetNodeCapacity(newNodeCapacity);
            }
            return Base_t::AddNodes(count);
        }

        // TODO void ShrinkToFit()
    protected:
        using TBase::SetNodeCapacity;
        using TBase::ReallocateMove;
    };
}
