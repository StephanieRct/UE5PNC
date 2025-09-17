// MIT License
// Copyright (c) 2025 Stephanie Rancourt

#pragma once
#include "common.h"

namespace PNC 
{
    /// <summary>
    /// Inherit of this struct to declare a Node Component.
    /// A Node Component will be instantiated for each Nodes created in a Chunk.
    /// </summary>
    struct NodeComponent 
    {
    public:
        static const ComponentOwner Owner = ComponentOwner_Node;
    };

    /// <summary>
    /// Inherit of this struct to declare a Chunk Component.
    /// A Chunk Component will be instantiated only once per Chunk and is shared
    /// among all Nodes in the Chunk.
    /// </summary>
    struct ChunkComponent
    {
    public:
        static const ComponentOwner Owner = ComponentOwner_Chunk;
    };

    /// <summary>
    /// Each node in the chunk has a parent at the given index in the same chunk
    /// except for root nodes which will have a parent Index of -1.
    /// </summary>
    template< typename TSize>
    struct CoParentInChunkT : public NodeComponent
    {
    public:
        using Self_t = CoParentInChunkT<TSize>;
        using Base_t = NodeComponent;
        using Size_t = TSize;

    public:
        /// <summary>
        /// Index of the parent inside the same chunk or -1 for root nodes.
        /// </summary>
        Size_t Index;
    };

    /// <summary>
    /// Each node has a parent node at a given index in the parent chunk 
    /// except for child node inside the same chunk which will have an outside parent Index of -1
    /// </summary>
    template< typename TSize>
    struct CoParentOutsideChunkT : public NodeComponent
    {
    public:
        using Self_t = CoParentOutsideChunkT<TSize>;
        using Base_t = NodeComponent;
        using Size_t = TSize;

    public:
        /// <summary>
        /// Index of the parent node in the parent chunk or -1.
        /// </summary>
        Size_t Index;
    };

    /// <summary>
    /// The whole chunk has a single parent (or ancestor) node in the parent chunk.
    /// </summary>
    template< typename TSize>
    struct CoSingleParentOutsideChunkT : public ChunkComponent
    {
    public:
        using Self_t = CoSingleParentOutsideChunkT<TSize>;
        using Base_t = ChunkComponent;
        using Size_t = TSize;

    public:
        /// <summary>
        /// Index of the parent node in the parent chunk for all root nodes in the chunk.
        /// </summary>
        Size_t Index;
    };

    /// <summary>
    /// Each node in the chunk has a number (0 or more) of sequential children in the same chunk.
    /// </summary>
    template< typename TSize>
    struct CoChildrenInChunkT : public NodeComponent
    {
    public:
        using Self_t = CoChildrenInChunkT<TSize>;
        using Base_t = NodeComponent;
        using Size_t = TSize;

    public:
        /// <summary>
        /// Index of the first child Node in the same Chunk or -1 if no children.
        /// </summary>
        Size_t FirstIndex;

        /// <summary>
        /// Number of adjacent child Nodes starting at FirstIndex.
        /// </summary>
        Size_t Count;
    };
}
