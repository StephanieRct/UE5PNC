// MIT License
// Copyright (c) 2025 Stephanie Rancourt

#pragma once
#include "common.h"
#include "ChunkTreeNode.h"

namespace NiT
{
    /// <summary>
    /// DTreePointerT Adds the pointers needed to make the container part of a tree of this container type.
    /// </summary>
    /// <typeparam name="TChunkStructure">Structure of the Chunk's Component data.</typeparam>
    template<typename TBase>
    struct DTreePointer : public TBase
    {
    public:
        using Base_t = TBase;
        using Self_t = DTreePointer<TBase>;
        using Size_t = TBase::Size_t;
        using ChunkStructure_t = TBase::ChunkStructure_t;

    protected:
        using ChunkTreeNode_t = ChunkTreeNodeT<Self_t>;
        ChunkTreeNode_t Tree;

    protected:
        DTreePointer()
            :Base_t()
        {
        }
        template<typename TProps>
        DTreePointer(const DPropsTag& tag, const TProps& props)
            : Base_t(tag, props)
        {
        }

    public:
        DTreePointer(const Self_t& o)
            : Base_t(o)
            , Tree()
        {
        }
        DTreePointer(const Self_t&& o)
            : Base_t(o)
            , Tree(o.Tree)
        {
            if(Tree.PreviousSibling)
                Tree.PreviousSibling->Tree.NextSibling = this;
            if (Tree.NextSibling)
                Tree.NextSibling->Tree.PreviousSibling = this;
            if (Tree.Parent && Tree.Parent->Tree.FirstChild == &o)
                Tree.Parent->Tree.FirstChild = this;
            if (Tree.FirstChild)
                for (auto* c = Tree.FirstChild;;)
                {
                    c->Tree.Parent = this;
                    c = c->Tree.NextSibling;
                    if (c == Tree.FirstChild)
                        break;
                }
        }
        DTreePointer& operator=(const DTreePointer&) = delete;

    public:
        /// <summary>
        /// Get the parent KTreePointer common with all siblings.
        /// Or null if a root Chunk.
        /// </summary>
        Self_t* GetParentChunk()const { return Tree.Parent; }

        /// <summary>
        /// Get the first child whoes siblings all have this parent in common.
        /// </summary>
        Self_t* GetFirstChildChunk()const { return Tree.FirstChild; }

        /// <summary>
        /// Get the previous sibling from a circular linked list of siblings who share the same parent.
        /// Will point to itself if single child.
        /// Will be null only when it is extracted (i.e. not part of a tree)
        /// </summary>
        Self_t* GetPreviousSiblingChunk()const { return Tree.PreviousSibling; }

        /// <summary>
        /// Get the next sibling from a circular linked list of siblings who share the same parent.
        /// Will point to itself if single child.
        /// Will be null only when it is extracted (i.e. not part of a tree)
        /// </summary>
        Self_t* GetNextSiblingChunk()const { return Tree.NextSibling; }

        /// <summary>
        /// Extract this KTreePointerT from any Tree.
        /// </summary>
        void Extract()
        {
            Tree.PreviousSibling->Tree.NextSibling = Tree.NextSibling;
            Tree.NextSibling->Tree.PreviousSibling = Tree.PreviousSibling;
            Tree.PreviousSibling = nullptr;
            Tree.NextSibling = nullptr;
        }

        /// <summary>
        /// Extract the given KTreePointer and insert it as the first child of this KTreePointer.
        /// </summary>
        /// <param name="child">KTreePointer to move.</param>
        void MoveToFirstChild(Self_t* child)
        {
            child->Extract();
            InsertFirstChild(child);
        }

        /// <summary>
        /// Extract the given KTreePointer and insert it as the last child of this KTreePointer.
        /// </summary>
        /// <param name="child">KTreePointer to move.</param>
        void MoveToLastChild(Self_t* child)
        {
            child->Extract();
            InsertLastChild(child);
        }

        /// <summary>
        /// Extract the given KTreePointer and insert it as the previous sibling of this KTreePointer.
        /// </summary>
        /// <param name="sibling">KTreePointer to move.</param>
        void MoveToPreviousSibling(Self_t* sibling)
        {
            sibling->Extract();
            InsertPreviousSibling(sibling);
        }

        /// <summary>
        /// Extract the given KTreePointer and insert it as the next sibling of this KTreePointer.
        /// </summary>
        /// <param name="sibling">KTreePointer to move.</param>
        void MoveToNextSibling(Self_t* sibling)
        {
            Tree.NextSibling->MoveToNextSibling(sibling);
        }

        /// <summary>
        /// Insert the given KTreePointer as the first child of this KTreePointer.
        /// </summary>
        /// <param name="child">KTreePointer to move.</param>
        void InsertFirstChild(Self_t* child)
        {
            ni_assert(child->Tree.IsExtracted());
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

        /// <summary>
        /// Insert the given KTreePointer as the last child of this KTreePointer.
        /// </summary>
        /// <param name="child">KTreePointer to move.</param>
        void InsertLastChild(Self_t* child)
        {
            ni_assert(child->Tree.IsExtracted());
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

        /// <summary>
        /// Insert the given KTreePointer as the previous sibling of this KTreePointer.
        /// </summary>
        /// <param name="sibling">KTreePointer to move.</param>
        void InsertPreviousSibling(Self_t* sibling)
        {
            ni_assert(sibling->Tree.IsExtracted());
            auto last = Tree.PreviousSibling;
            last->Tree.NextSibling = sibling;
            sibling->Tree.PreviousSibling = last;
            sibling->Tree.NextSibling = this;
            Tree.PreviousSibling = sibling;
        }

        /// <summary>
        /// Insert the given KTreePointer as the next sibling of this KTreePointer.
        /// </summary>
        /// <param name="sibling">KTreePointer to move.</param>
        void InsertNextSibling(Self_t* sibling)
        {
            Tree.NextSibling->InsertPreviousSibling(sibling);
        }
    };
}
