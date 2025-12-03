// MIT License
// Copyright (c) 2025 Stephanie Rancourt

#pragma once
#include "common.h"

#include "Containers.h"
#include "KContainers.h"
//#include "KChunkTreePointer.h"
//#include "KChunkPointer.h"
//#include "KChunkArrayPointer.h"
//#include "KChunkArrayTreePointer.h"

namespace PNC
{

    template<typename TBase>
    const typename DKind<TBase>::ChunkStructure_t& DKind<TBase>::GetStructure()const
    {
        switch (this->Kind)
        {
        case ContainerKind::Chunk:
        case ContainerKind::ChunkArray:
            return reinterpret_cast<const KChunkPointerT<ChunkStructure_t>*>(this)->GetStructure();
        case ContainerKind::ChunkTree:
        case ContainerKind::ChunkArrayTree:
            return reinterpret_cast<const KChunkTreePointerT<ChunkStructure_t>*>(this)->GetStructure();
        }
        pnc_assert_no_entry_return(*(ChunkStructure_t*)nullptr);
    }

    template<typename TBase>
    const typename DKind<TBase>::ChunkPointer_t& DKind<TBase>::GetChunk()const
    {
        switch (this->Kind)
        {
        case ContainerKind::Chunk:
        case ContainerKind::ChunkArray:
            return reinterpret_cast<ChunkPointer_t&>(reinterpret_cast<KChunkPointerT<ChunkStructure_t>*>(this)->GetChunk());
        case ContainerKind::ChunkTree:
        case ContainerKind::ChunkArrayTree:
            return reinterpret_cast<ArrayPointer_t&>(reinterpret_cast<KChunkTreePointerT<ChunkStructure_t>*>(this)->GetChunk());
        }
        pnc_assert_no_entry_return(*(ChunkPointer_t*)nullptr);
    }

    template<typename TBase>
    typename DKind<TBase>::ChunkPointer_t& DKind<TBase>::GetChunk()
    {
        switch (this->Kind)
        {
        case ContainerKind::Chunk:
        case ContainerKind::ChunkArray:
            return reinterpret_cast<ChunkPointer_t&>(reinterpret_cast<KChunkPointerT<ChunkStructure_t>*>(this)->GetChunk());
        case ContainerKind::ChunkTree:
        case ContainerKind::ChunkArrayTree:
            return reinterpret_cast<ArrayPointer_t&>(reinterpret_cast<KChunkTreePointerT<ChunkStructure_t>*>(this)->GetChunk());
        }
        pnc_assert_no_entry_return(*(ChunkPointer_t*)nullptr);
    }

    template<typename TBase>
    const typename DKind<TBase>::ArrayPointer_t& DKind<TBase>::GetChunkArray()const
    {
        switch (this->Kind)
        {
        case ContainerKind::Chunk:
            pnc_assert_no_entry_return(*(ArrayPointer_t*)nullptr);
        case ContainerKind::ChunkTree:
            pnc_assert_no_entry_return(*(ArrayPointer_t*)nullptr);
        case ContainerKind::ChunkArray:
            return reinterpret_cast<ArrayPointer_t&>(reinterpret_cast<KChunkArrayPointerT<ChunkStructure_t, ChunkPointerElement_t>*>(this)->GetChunk());
        case ContainerKind::ChunkArrayTree:
            return reinterpret_cast<ArrayPointer_t&>(reinterpret_cast<KChunkArrayTreePointerT<ChunkStructure_t, ChunkPointerElement_t>*>(this)->GetChunk());
        }
        pnc_assert_no_entry_return(*(ArrayPointer_t*)nullptr);
    }

    template<typename TBase>
    typename DKind<TBase>::ArrayPointer_t& DKind<TBase>::GetChunkArray()
    {
        switch (this->Kind)
        {
        case ContainerKind::Chunk:
            pnc_assert_no_entry_return(*(ArrayPointer_t*)nullptr);
        case ContainerKind::ChunkTree:
            pnc_assert_no_entry_return(*(ArrayPointer_t*)nullptr);
        case ContainerKind::ChunkArray:
            return reinterpret_cast<ArrayPointer_t&>(reinterpret_cast<KChunkArrayPointerT<ChunkStructure_t, ChunkPointerElement_t>*>(this)->GetChunk());
        case ContainerKind::ChunkArrayTree:
            return reinterpret_cast<ArrayPointer_t&>(reinterpret_cast<KChunkArrayTreePointerT<ChunkStructure_t, ChunkPointerElement_t>*>(this)->GetChunk());
        }
        pnc_assert_no_entry_return(*(ArrayPointer_t*)nullptr);
    }
}
