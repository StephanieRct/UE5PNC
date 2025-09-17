// MIT License
// Copyright (c) 2025 Stephanie Rancourt

#pragma once
#include "common.h"

namespace PNC
{
    /// <summary>
    /// A Chunk part of a tree of Chunks
    /// </summary>
    /// <typeparam name="TChunkPointer"></typeparam>
    template< typename TChunkPointer>
    struct ChunkTreeNodeT
    {
    public:
        using Self_t = ChunkTreeNodeT<TChunkPointer>;
        using ChunkPointer_t = TChunkPointer;

    public:
        /// <summary>
        /// Points to the parent ChunkPointer common with all siblings.
        /// Or null if a root Chunk.
        /// </summary>
        ChunkPointer_t* Parent = nullptr;

        /// <summary>
        /// Points to the first child whoes siblings all have this parent in common.
        /// </summary>
        ChunkPointer_t* FirstChild = nullptr;

        /// <summary>
        /// Circular linked list of previous siblings who share the same parent.
        /// Will point to itself if single child.
        /// Will be null only when it is extracted (i.e. not part of a tree)
        /// </summary>
        ChunkPointer_t* PreviousSibling = nullptr;

        /// <summary>
        /// Circular linked list of next siblings who share the same parent.
        /// Will point to itself if single child.
        /// Will be null only when it is extracted (i.e. not part of a tree)
        /// </summary>
        ChunkPointer_t* NextSibling = nullptr;

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
