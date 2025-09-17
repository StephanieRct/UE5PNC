// MIT License
// Copyright (c) 2025 Stephanie Rancourt

#pragma once
#include "common.h"
#include "IdentifiableChunk.h"
#include "ChunkTreePointer.h"
#include "IdentifiableChunkPointer.h"
#include "IdentifiableChunkArrayPointer.h"
#include "ChunkArrayTreePointer.h"

namespace PNC
{
    template<typename TChunkType>
    const IdentifiableChunkT<TChunkType>::ChunkPointer_t& IdentifiableChunkT<TChunkType>::GetChunk()const
    {
        switch (this->Kind)
        {
        case ChunkKind_Chunk:
        case ChunkKind_ChunkArray:
            return reinterpret_cast<const IdentifiableChunkPointerT<TChunkType>*>(this)->GetChunk();
        case ChunkKind_ChunkTree:
        case ChunkKind_ChunkArrayTree:
            return reinterpret_cast<const ChunkTreePointerT<TChunkType>*>(this)->GetChunk();
        }
        checkNoEntry();
        return *(ChunkPointer_t*)nullptr;
    }

    template<typename TChunkType>
    IdentifiableChunkT<TChunkType>::ChunkPointer_t& IdentifiableChunkT<TChunkType>::GetChunk()
    {
        switch (this->Kind)
        {
        case ChunkKind_Chunk:
        case ChunkKind_ChunkArray:
            return reinterpret_cast<IdentifiableChunkPointerT<TChunkType>*>(this)->GetChunk();
        case ChunkKind_ChunkTree:
        case ChunkKind_ChunkArrayTree:
            return reinterpret_cast<ChunkTreePointerT<TChunkType>*>(this)->GetChunk();
        }
        checkNoEntry();
        return *(ChunkPointer_t*)nullptr;
    }

    template<typename TChunkType>
    const IdentifiableChunkT<TChunkType>::ChunkArrayPointer_t& IdentifiableChunkT<TChunkType>::GetChunkArray()const
    {
        switch (this->Kind)
        {
        case ChunkKind_Chunk:
            checkNoEntry();
            break;
        case ChunkKind_ChunkArray:
            return reinterpret_cast<const IdentifiableChunkArrayPointerT<ChunkPointer_t>*>(this)->GetChunk();
        case ChunkKind_ChunkTree:
            checkNoEntry();
            break;
        case ChunkKind_ChunkArrayTree:
            return reinterpret_cast<const ChunkTreePointerT<TChunkType>*>(this)->GetChunk();
        }
        checkNoEntry();
        return *(ChunkArrayPointer_t*)nullptr;
    }

    template<typename TChunkType>
    IdentifiableChunkT<TChunkType>::ChunkArrayPointer_t& IdentifiableChunkT<TChunkType>::GetChunkArray()
    {
        switch (this->Kind)
        {
        case ChunkKind_Chunk:
            checkNoEntry();
            break;
        case ChunkKind_ChunkArray:
            return reinterpret_cast<IdentifiableChunkArrayPointerT<ChunkPointer_t>*>(this)->GetChunk();
        case ChunkKind_ChunkTree:
            checkNoEntry();
            break;
        case ChunkKind_ChunkArrayTree:
            return reinterpret_cast<ChunkArrayTreePointerT<ChunkPointer_t>*>(this)->GetChunk();
        }
        checkNoEntry();
        return *(ChunkArrayPointer_t*)nullptr;
    }
}
