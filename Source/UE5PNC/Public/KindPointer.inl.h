// MIT License
// Copyright (c) 2025 Stephanie Rancourt

#pragma once
#include "common.h"
#include "KindPointer.h"
#include "KChunkTreePointer.h"
#include "KChunkPointer.h"
#include "KChunkArrayPointer.h"
#include "KChunkArrayTreePointer.h"

namespace PNC
{
    template<typename TChunkStructure>
    const KindPointerT<TChunkStructure>::Chunk_t& KindPointerT<TChunkStructure>::GetChunk()const
    {
        switch (this->Kind)
        {
        case ChunkKind_Chunk:
        case ChunkKind_ChunkArray:
            return reinterpret_cast<const KChunkPointerT<TChunkStructure>*>(this)->GetChunk();
        case ChunkKind_ChunkTree:
        case ChunkKind_ChunkArrayTree:
            return reinterpret_cast<const KChunkTreePointerT<TChunkStructure>*>(this)->GetChunk();
        }
        checkNoEntry();
        return *(Chunk_t*)nullptr;
    }

    template<typename TChunkStructure>
    KindPointerT<TChunkStructure>::Chunk_t& KindPointerT<TChunkStructure>::GetChunk()
    {
        switch (this->Kind)
        {
        case ChunkKind_Chunk:
        case ChunkKind_ChunkArray:
            return reinterpret_cast<KChunkPointerT<TChunkStructure>*>(this)->GetChunk();
        case ChunkKind_ChunkTree:
        case ChunkKind_ChunkArrayTree:
            return reinterpret_cast<KChunkTreePointerT<TChunkStructure>*>(this)->GetChunk();
        }
        checkNoEntry();
        return *(Chunk_t*)nullptr;
    }

    template<typename TChunkStructure>
    const KindPointerT<TChunkStructure>::ChunkArray_t& KindPointerT<TChunkStructure>::GetChunkArray()const
    {
        switch (this->Kind)
        {
        case ChunkKind_Chunk:
            checkNoEntry();
            break;
        case ChunkKind_ChunkArray:
            return reinterpret_cast<const KChunkArrayPointerT<ChunkPointer_t>*>(this)->GetChunk();
        case ChunkKind_ChunkTree:
            checkNoEntry();
            break;
        case ChunkKind_ChunkArrayTree:
            return reinterpret_cast<const KChunkTreePointerT<TChunkStructure>*>(this)->GetChunk();
        }
        checkNoEntry();
        return *(ChunkArray_t*)nullptr;
    }

    template<typename TChunkStructure>
    KindPointerT<TChunkStructure>::ChunkArray_t& KindPointerT<TChunkStructure>::GetChunkArray()
    {
        switch (this->Kind)
        {
        case ChunkKind_Chunk:
            checkNoEntry();
            break;
        case ChunkKind_ChunkArray:
            return reinterpret_cast<KChunkArrayPointerT<ChunkPointer_t>*>(this)->GetChunk();
        case ChunkKind_ChunkTree:
            checkNoEntry();
            break;
        case ChunkKind_ChunkArrayTree:
            return reinterpret_cast<KChunkArrayTreePointerT<ChunkPointer_t>*>(this)->GetChunk();
        }
        checkNoEntry();
        return *(ChunkArray_t*)nullptr;
    }
}
