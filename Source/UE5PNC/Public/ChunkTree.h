// MIT License
// Copyright (c) 2025 Stephanie Rancourt

#pragma once
#include "common.h"
#include "chunk.h"

namespace PNC 
{
    template< typename TChunkType, typename TSize >
    struct ChunkTreeT : ChunkT<TChunkType, TSize>
    {
    public:
        typedef ChunkT<TChunkType, TSize> Base_t;
        typedef TChunkType ChunkType_t;
        typedef TSize size_t;
    public:
        // up a level
        // can be null
        ChunkTreeT* parent;

        // down a level
        // can be null
        ChunkTreeT* firstChild;

        // circular linked siblings backward
        // cannot be null
        ChunkTreeT* previousSibling;

        // circular linked siblings forward
        // cannot be null
        ChunkTreeT* nextSibling;


        ChunkTreeT()
            :Base_t()
            , parent(nullptr)
            , firstChild(nullptr)
            , previousSibling(nullptr)
            , nextSibling(nullptr) {
        }
        ChunkTreeT(const ChunkTreeT& o)
            : Base_t(o)
            , parent(nullptr)
            , firstChild(nullptr)
            , previousSibling(nullptr)
            , nextSibling(nullptr) {
        }
        ChunkTreeT& operator=(const ChunkTreeT& o) {
            Base_t::operator =(o);
            parent = nullptr;
            firstChild = nullptr;
            previousSibling = nullptr;
            nextSibling = nullptr;
            return *this;
        }
        ChunkTreeT(const ChunkType_t* chunkType, size_t capacity, size_t size = 0)
            :Base_t(chunkType, capacity, size)
            , parent(nullptr)
            , firstChild(nullptr)
            , previousSibling(nullptr)
            , nextSibling(nullptr) {
        }
        ~ChunkTreeT() {
        }


        void AddChild(ChunkTreeT* child) {
            child->Extract();
            child->parent = this;
            if (firstChild == nullptr) {
                child->nextSibling = child;
                child->previousSibling = child;
                firstChild = child;
            }
            else {
                auto& last = firstChild->previousSibling;
                last->nextSibling = child;
                child->previousSibling = last;
                child->nextSibling = firstChild;
                last = child;
            }

        }
        void AddSibling(ChunkTreeT* child) {

        }
        void Extract() {

        }
    };

}