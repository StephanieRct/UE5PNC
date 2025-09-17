// MIT License
// Copyright (c) 2025 Stephanie Rancourt

#pragma once
#include "common.h"
#include "Components.h"
#include "IdentifiableChunk.h"

namespace PNC
{
    template<typename TChunkType>
    struct IdentifiableTreePointerT : public IdentifiableChunkT<TChunkType>
    {
    public:
        using ChunkType_t = TChunkType;
        using Base_t = IdentifiableChunkT<TChunkType>;
        using Size_t = typename ChunkType_t::Size_t;

        using ChunkTreePointer_t = IdentifiableTreePointerT<TChunkType>;
        using CoChunkTree_t = CoChunkTreeT<ChunkTreePointer_t>;

    public:
        CoChunkTree_t Tree;
        ChunkTreePointer_t* GetParentChunk()const { return Tree.Parent; }
        ChunkTreePointer_t* GetFirstChildChunk()const { return Tree.FirstChild; }
        ChunkTreePointer_t* GetNextSiblingChunk()const { return Tree.NextSibling; }
        ChunkTreePointer_t* GetPreviousSiblingChunk()const { return Tree.PreviousSibling; }


        IdentifiableTreePointerT(ChunkKind kind)
            :Base_t(kind)
        {
        }

        void MoveToFirstChild(ChunkTreePointer_t* child)
        {
            child->Extract();
            InsertFirstChild(child);
        }

        void MoveToLastChild(ChunkTreePointer_t* child)
        {
            child->Extract();
            InsertLastChild(child);
        }

        void MoveToPreviousSibling(ChunkTreePointer_t* sibling)
        {
            sibling->Extract();
            InsertPreviousSibling(sibling);
        }

        void MoveToNextSibling(ChunkTreePointer_t* sibling)
        {
            Tree.NextSibling->MoveToNextSibling(sibling);
        }

        void InsertFirstChild(ChunkTreePointer_t* child)
        {
            assert_pnc(child->Tree.IsExtracted());
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

        void InsertLastChild(ChunkTreePointer_t* child)
        {
            assert_pnc(child->Tree.IsExtracted());
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

        void InsertPreviousSibling(ChunkTreePointer_t* sibling)
        {
            assert_pnc(sibling->Tree.IsExtracted());
            auto last = Tree.PreviousSibling;
            last->Tree.NextSibling = sibling;
            sibling->Tree.PreviousSibling = last;
            sibling->Tree.NextSibling = this;
            Tree.PreviousSibling = sibling;
        }

        void InsertNextSibling(ChunkTreePointer_t* sibling)
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
}
