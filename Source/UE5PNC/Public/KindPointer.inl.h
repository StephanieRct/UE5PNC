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
    template<typename TChunkType>
    const KindPointerT<TChunkType>::Chunk_t& KindPointerT<TChunkType>::GetChunk()const
    {
        switch (this->Kind)
        {
        case ChunkKind_Chunk:
        case ChunkKind_ChunkArray:
            return reinterpret_cast<const KChunkPointerT<TChunkType>*>(this)->GetChunk();
        case ChunkKind_ChunkTree:
        case ChunkKind_ChunkArrayTree:
            return reinterpret_cast<const KChunkTreePointerT<TChunkType>*>(this)->GetChunk();
        }
        checkNoEntry();
        return *(Chunk_t*)nullptr;
    }

    template<typename TChunkType>
    KindPointerT<TChunkType>::Chunk_t& KindPointerT<TChunkType>::GetChunk()
    {
        switch (this->Kind)
        {
        case ChunkKind_Chunk:
        case ChunkKind_ChunkArray:
            return reinterpret_cast<KChunkPointerT<TChunkType>*>(this)->GetChunk();
        case ChunkKind_ChunkTree:
        case ChunkKind_ChunkArrayTree:
            return reinterpret_cast<KChunkTreePointerT<TChunkType>*>(this)->GetChunk();
        }
        checkNoEntry();
        return *(Chunk_t*)nullptr;
    }

    template<typename TChunkType>
    const KindPointerT<TChunkType>::ChunkArray_t& KindPointerT<TChunkType>::GetChunkArray()const
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
            return reinterpret_cast<const KChunkTreePointerT<TChunkType>*>(this)->GetChunk();
        }
        checkNoEntry();
        return *(ChunkArray_t*)nullptr;
    }

    template<typename TChunkType>
    KindPointerT<TChunkType>::ChunkArray_t& KindPointerT<TChunkType>::GetChunkArray()
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
