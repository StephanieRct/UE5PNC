// MIT License
// Copyright (c) 2025 Stephanie Rancourt

#pragma once
#include "common.h"

namespace PNC 
{
    struct NodeComponent 
    {
    public:
        static const ComponentOwner Owner = ComponentOwner_Node;

    };

    struct ChunkComponent
    {
    public:
        static const ComponentOwner Owner = ComponentOwner_Chunk;

    };

    /// <summary>
    /// Each node in the chunk has a parent at the given index in the same chunk
    /// except for root nodes who have a parent index of -1.
    /// </summary>
    template< typename TSize>
    struct CoParentInChunkT : public NodeComponent
    {
    public:
        typedef TSize Size_t;

    public:
        Size_t Index;
    };

    /// <summary>
    /// Each node has a parent node at a given index in the parent chunk 
    /// except for child node inside the same chunk
    /// </summary>
    template< typename TSize>
    struct CoParentOutsideChunkT : public NodeComponent
    {
    public:
        typedef TSize Size_t;

    public:
        Size_t Index;
    };

    /// <summary>
    /// The whole chunk has a single parent (or ancestor) node in the parent chunk.
    /// </summary>
    template< typename TSize>
    struct CoSingleParentOutsideChunkT : public ChunkComponent
    {
    public:
        typedef TSize Size_t;

    public:
        Size_t Index;
    };

    /// <summary>
    /// Each node has a parent node either in the same chunk (positive index) or in
    /// the parent chunk (negative index)
    /// </summary>
    /// <typeparam name="TSize">Must be a signed integer type.</typeparam>
    template< typename TSize>
    struct CoParentInOrOutsideIndexT : public NodeComponent
    {
    public:
        typedef TSize Size_t;

    public:
        Size_t Index;
        bool IsInSameChunk()const { return Index >= 0; }
        bool IsInParentChunk()const { return Index < 0; }
        Size_t GetIndexInParentChunk()const 
        { 
            assert_pnc(IsInParentChunk());
            return -Index - 1; 
        }
        Size_t GetIndexInSameChunk()const 
        {
            assert_pnc(IsInSameChunk());
            return Index; 
        }
        void SetIndexInParentChunk(Size_t index)
        {
            Index = -index - 1;
        }
        void SetIndexInSameChunk(Size_t index)
        {
            Index = index;
        }
    };

    template< typename TSize>
    struct CoChildrenInChunkT : public NodeComponent
    {
    public:
        typedef TSize Size_t;

    public:
        Size_t FirstIndex;
        Size_t Count;
    };

    template< typename TChunk>
    struct CoChunkTreeT : public ChunkComponent
    {
    public:
        using CoChunkTree_t = CoChunkTreeT<TChunk>;
        using Chunk_t = TChunk;

    public:
        Chunk_t* Parent = nullptr;
        Chunk_t* FirstChild = nullptr;
        /// <summary>
        /// Circular linked list of previous siblings.
        /// Will point to itself if single child.
        /// Will be null only when the ChunkTree is extracted (i.e. not part of a tree)
        /// </summary>
        Chunk_t* PreviousSibling = nullptr;

        /// <summary>
        /// Circular linked list of next siblings.
        /// Will point to itself if single child.
        /// Will be null only when the ChunkTree is extracted (i.e. not part of a tree)
        /// </summary>
        Chunk_t* NextSibling = nullptr;

        bool HasParent()const { return Parent != nullptr; }
        bool HasChildren()const { return FirstChild != nullptr; }
        bool IsExtracted()const { return NextSibling == nullptr; }
    };
}