// MIT License
// Copyright (c) 2025 Stephanie Rancourt

#pragma once
#include "common.h"
#include "chunk.h"
#include "Components.h"

namespace PNC 
{
    template< typename TChunkType, typename TSize >
    struct ChunkTreeT : ChunkT<TChunkType, TSize>
    {
    public:
        using Base_t = ChunkT<TChunkType, TSize>;
        using ChunkType_t = TChunkType;
        using Size_t = TSize;
        using ChunkTree_t = ChunkTreeT<TChunkType, TSize>;
        using CoChunkTree_t = CoChunkTreeT<ChunkTree_t>;

    public:
        CoChunkTree_t Tree;
        ChunkTree_t* GetParentChunk()const { return Tree.Parent; }
        ChunkTree_t* GetFirstChildChunk()const { return Tree.FirstChild; }
        ChunkTree_t* GetNextSiblingChunk()const { return Tree.NextSibling; }
        ChunkTree_t* GetPreviousSiblingChunk()const { return Tree.PreviousSibling; }

        ChunkTreeT(const ChunkType_t* chunkType, Size_t capacity, Size_t size = 0)
            :ChunkT<TChunkType, TSize>(chunkType, capacity, size)
        {
        }

        void MoveToFirstChild(ChunkTree_t* child)
        {
            child->Extract();
            InsertFirstChild(child);
        }

        void MoveToLastChild(ChunkTree_t* child)
        {
            child->Extract();
            InsertLastChild(child);
        }

        void MoveToPreviousSibling(ChunkTree_t* sibling)
        {
            sibling->Extract();
            InsertPreviousSibling(sibling);
        }

        void MoveToNextSibling(ChunkTree_t* sibling)
        {
            Tree.NextSibling->MoveToNextSibling(sibling);
        }

        void InsertFirstChild(ChunkTree_t* child)
        {
            assert(child->Tree.IsExtracted());
            child->Tree.Parent = this;
            if (Tree.FirstChild == nullptr)
            {
                child->Tree.NextSibling = child;
                child->Tree.PreviousSibling = child;
                Tree.FirstChild = child;
            }
            else
            {
                Tree.FirstChild->InsertPreviousSibling(child);
                Tree.FirstChild = child;
            }
        }

        void InsertLastChild(ChunkTree_t* child)
        {
            assert(child->Tree.IsExtracted());
            child->Tree.Parent = this;
            if (Tree.FirstChild == nullptr)
            {
                child->Tree.NextSibling = child;
                child->Tree.PreviousSibling = child;
                Tree.FirstChild = child;
            }
            else
            {
                Tree.FirstChild->InsertPreviousSibling(child);
            }
        }

        void InsertPreviousSibling(ChunkTree_t* sibling)
        {
            assert(sibling->Tree.IsExtracted());
            auto last = Tree.PreviousSibling;
            last->Tree.NextSibling = sibling;
            sibling->Tree.PreviousSibling = last;
            sibling->Tree.NextSibling = Tree.FirstChild;
            Tree.PreviousSibling = sibling;
        }

        void InsertNextSibling(ChunkTree_t* sibling)
        {
            Tree.NextSibling->InsertPreviousSibling(sibling);
        }

        void Extract()
        {
            Tree.PreviousSibling->Tree.NextSibling = Tree.NextSibling;
            Tree.NextSibling->Tree.PreviousSibling = Tree.PreviousSibling;
            Tree.PreviousSibling = nullptr;
            Tree.NextSibling = nullptr;
        }
        
    };



    //template< typename TChunkBase, typename TChunkType, typename TSize >
    //struct ChunkArrayT : TChunkBase
    //{
    //public:
    //    using Base_t = TChunkBase;
    //    using ChunkType_t = TChunkType;
    //    using Size_t = TSize;
    //    using ChunkArray_t = ChunkArrayT<TChunkType, TSize>;


    //public:
    //    
    //    ChunkArrayT(const ChunkType_t* chunkType, Size_t chunkCount, Size_t totalCapacity, Size_t totalSize = 0)
    //        :ChunkT<TChunkType, TSize>(chunkType, capacity, size)
    //    {
    //    }

    //};
}