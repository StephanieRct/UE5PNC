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
    template<typename TChunkStructure>
    const typename KindPointerT<TChunkStructure>::Chunk_t& KindPointerT<TChunkStructure>::GetChunk()const
    {
        switch (this->Kind)
        {
        case ChunkKind::Chunk:
        case ChunkKind::ChunkArray:
            return reinterpret_cast<const KChunkPointerT<TChunkStructure>*>(this)->GetChunk();
        case ChunkKind::ChunkTree:
        case ChunkKind::ChunkArrayTree:
            return reinterpret_cast<const KChunkTreePointerT<TChunkStructure>*>(this)->GetChunk();
        }
        pnc_assert_no_entry_return(*(Chunk_t*)nullptr);
    }

    template<typename TChunkStructure>
    typename KindPointerT<TChunkStructure>::Chunk_t& KindPointerT<TChunkStructure>::GetChunk()
    {
        switch (this->Kind)
        {
        case ChunkKind::Chunk:
        case ChunkKind::ChunkArray:
            return reinterpret_cast<KChunkPointerT<TChunkStructure>*>(this)->GetChunk();
        case ChunkKind::ChunkTree:
        case ChunkKind::ChunkArrayTree:
            return reinterpret_cast<KChunkTreePointerT<TChunkStructure>*>(this)->GetChunk();
        }
        pnc_assert_no_entry_return(*(Chunk_t*)nullptr);
    }

    template<typename TChunkStructure>
    const typename KindPointerT<TChunkStructure>::ChunkArray_t& KindPointerT<TChunkStructure>::GetChunkArray()const
    {
        switch (this->Kind)
        {
        case ChunkKind::Chunk:
            pnc_assert_no_entry_return(*(ChunkArray_t*)nullptr);
        case ChunkKind::ChunkArray:
            return reinterpret_cast<const KChunkArrayPointerT<ChunkPointer_t>*>(this)->GetChunk();
        case ChunkKind::ChunkTree:
            pnc_assert_no_entry_return(*(ChunkArray_t*)nullptr);
        case ChunkKind::ChunkArrayTree:
            return reinterpret_cast<const KChunkTreePointerT<TChunkStructure>*>(this)->GetChunk();
        }
        pnc_assert_no_entry_return(*(ChunkArray_t*)nullptr);
    }

    template<typename TChunkStructure>
    typename KindPointerT<TChunkStructure>::ChunkArray_t& KindPointerT<TChunkStructure>::GetChunkArray()
    {
        switch (this->Kind)
        {
        case ChunkKind::Chunk:
            pnc_assert_no_entry_return(*(ChunkArray_t*)nullptr);
        case ChunkKind::ChunkArray:
            return reinterpret_cast<KChunkArrayPointerT<ChunkPointer_t>*>(this)->GetChunk();
        case ChunkKind::ChunkTree:
            pnc_assert_no_entry_return(*(ChunkArray_t*)nullptr);
        case ChunkKind::ChunkArrayTree:
            return reinterpret_cast<KChunkArrayTreePointerT<ChunkPointer_t>*>(this)->GetChunk();
        }
        pnc_assert_no_entry_return(*(ChunkArray_t*)nullptr);
    }
}
