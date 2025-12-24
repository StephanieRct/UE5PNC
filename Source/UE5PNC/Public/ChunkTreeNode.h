// MIT License
// Copyright (c) 2025 Stephanie Rancourt

#pragma once
#include "common.h"

namespace NiT
{
    /// <summary>
    /// A Chunk part of a tree of Chunks
    /// </summary>
    /// <typeparam name="TContainer"></typeparam>
    template< typename TContainer>
    struct ChunkTreeNodeT
    {
    public:
        using Self_t = ChunkTreeNodeT<TContainer>;
        using Container_t = TContainer;

    public:
        /// <summary>
        /// Points to the parent ChunkPointer common with all siblings.
        /// Or null if a root Chunk.
        /// </summary>
        Container_t* Parent = nullptr;

        /// <summary>
        /// Points to the first child whoes siblings all have this parent in common.
        /// </summary>
        Container_t* FirstChild = nullptr;

        /// <summary>
        /// Circular linked list of previous siblings who share the same parent.
        /// Will point to itself if single child.
        /// Will be null only when it is extracted (i.e. not part of a tree)
        /// </summary>
        Container_t* PreviousSibling = nullptr;

        /// <summary>
        /// Circular linked list of next siblings who share the same parent.
        /// Will point to itself if single child.
        /// Will be null only when it is extracted (i.e. not part of a tree)
        /// </summary>
        Container_t* NextSibling = nullptr;

    public:
        ChunkTreeNodeT() = default;
#ifdef NI_MEMORYCLEANUP
        ~ChunkTreeNodeT()
        {
            ni_clean(Parent);
            ni_clean(FirstChild);
            ni_clean(PreviousSibling);
            ni_clean(NextSibling);
        }
#endif
        //ChunkTreeNodeT(Self_t&& o)
        //    : Parent(o.Parent)
        //    , FirstChild(o.FirstChild)
        //    , PreviousSibling(o.PreviousSibling)
        //    , NextSibling(o.NextSibling)
        //{
        //    o = Self_t();
        //}
        
        /// <summary>
        /// If it is not part of any tree.
        /// </summary>
        /// <returns></returns>
        bool IsExtracted()const { return NextSibling == nullptr; }

        bool IsRoot()const { return Parent == nullptr; }
        bool IsLeaf()const { return FirstChild == nullptr; }
        bool HasParent()const { return Parent != nullptr; }
        bool HasChildren()const { return FirstChild != nullptr; }
    };
}
