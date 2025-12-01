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
    const typename DKind<TBase>::Chunk_t& DKind<TBase>::GetChunk()const
    {
        switch (this->Kind)
        {
        case ChunkKind::Chunk:
        case ChunkKind::ChunkArray:
            return reinterpret_cast<Chunk_t&>(reinterpret_cast<KChunkPointerT<TBase>*>(this)->GetChunk());
        case ChunkKind::ChunkTree:
        case ChunkKind::ChunkArrayTree:
            return reinterpret_cast<ChunkArray_t&>(reinterpret_cast<KChunkTreePointerT<TBase>*>(this)->GetChunk());
        }
        pnc_assert_no_entry_return(*(Chunk_t*)nullptr);
    }

    template<typename TBase>
    typename DKind<TBase>::Chunk_t& DKind<TBase>::GetChunk()
    {
        switch (this->Kind)
        {
        case ChunkKind::Chunk:
        case ChunkKind::ChunkArray:
            return reinterpret_cast<Chunk_t&>(reinterpret_cast<KChunkPointerT<TBase>*>(this)->GetChunk());
        case ChunkKind::ChunkTree:
        case ChunkKind::ChunkArrayTree:
            return reinterpret_cast<ChunkArray_t&>(reinterpret_cast<KChunkTreePointerT<TBase>*>(this)->GetChunk());
        }
        pnc_assert_no_entry_return(*(Chunk_t*)nullptr);
    }

    template<typename TBase>
    const typename DKind<TBase>::ChunkArray_t& DKind<TBase>::GetChunkArray()const
    {
        switch (this->Kind)
        {
        case ChunkKind::Chunk:
            pnc_assert_no_entry_return(*(ChunkArray_t*)nullptr);
        case ChunkKind::ChunkTree:
            pnc_assert_no_entry_return(*(ChunkArray_t*)nullptr);
        case ChunkKind::ChunkArray:
            return reinterpret_cast<ChunkArray_t&>(reinterpret_cast<KChunkArrayPointerT<TBase, ChunkPointerElement_t>*>(this)->GetChunk());
        case ChunkKind::ChunkArrayTree:
            return reinterpret_cast<ChunkArray_t&>(reinterpret_cast<KChunkArrayTreePointerT<TBase, ChunkPointerElement_t>*>(this)->GetChunk());
        }
        pnc_assert_no_entry_return(*(ChunkArray_t*)nullptr);
    }

    template<typename TBase>
    typename DKind<TBase>::ChunkArray_t& DKind<TBase>::GetChunkArray()
    {
        switch (this->Kind)
        {
        case ChunkKind::Chunk:
            pnc_assert_no_entry_return(*(ChunkArray_t*)nullptr);
        case ChunkKind::ChunkTree:
            pnc_assert_no_entry_return(*(ChunkArray_t*)nullptr);
        case ChunkKind::ChunkArray:
            return reinterpret_cast<ChunkArray_t&>(reinterpret_cast<KChunkArrayPointerT<TBase, ChunkPointerElement_t>*>(this)->GetChunk());
        case ChunkKind::ChunkArrayTree:
            return reinterpret_cast<ChunkArray_t&>(reinterpret_cast<KChunkArrayTreePointerT<TBase, ChunkPointerElement_t>*>(this)->GetChunk());
        }
        pnc_assert_no_entry_return(*(ChunkArray_t*)nullptr);
    }
}
