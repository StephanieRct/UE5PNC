// MIT License
// Copyright (c) 2025 Stephanie Rancourt

#pragma once
#include "common.h"

namespace PNC
{
    /// <summary>
    /// A ChunkPointerInternal points to a chunk's component data memory within a range of nodes.
    /// Used to get read-write access to a ChunkPointer's data fields.
    /// </summary>
    /// <typeparam name="TChunkType">Structure of the chunk's component data.</typeparam>
    template<typename TChunkType>
    struct ChunkPointerInternalT
    {
    public:
        using Self_t = ChunkPointerInternalT<TChunkType>;
        using ChunkType_t = TChunkType;
        using Size_t = typename ChunkType_t::Size_t;

    public:
        /// <summary>
        /// ChunkType used to create the structure of what ComponentData points to.
        /// </summary>
        const ChunkType_t* Type;

        /// <summary>
        /// Points to an array of component data pointers.
        /// </summary>
        void** ComponentData;

        /// <summary>
        /// Number of nodes (and not in bytes) all component data array contains
        /// </summary>
        Size_t NodeCount;

    public:
        /// <summary>
        /// Create a null ChunkRefT without chunk type.
        /// IsNull() will evaluate to true.
        /// </summary>
        ChunkPointerInternalT()
            : Type(nullptr)
            , ComponentData(nullptr)
            , NodeCount(0)
        {
        }

        ChunkPointerInternalT(const ChunkType_t* chunkType, Size_t nodeCount, void** componentData)
            : Type(chunkType)
            , ComponentData(componentData)
            , NodeCount(nodeCount)
        {
        }

        ChunkPointerInternalT(const ChunkType_t* chunkType, Size_t nodeCount)
            : Type(chunkType)
            , ComponentData(nullptr)
            , NodeCount(nodeCount)
        {
        }

    public:
        /// <summary>
        /// Test if the chunk is null and has no type nor component data.
        /// </summary>
        /// <returns></returns>
        bool IsNull()const { return this->Type == nullptr; }

        /// <summary>
        /// Get the size of the chunk.
        /// The size is the number of valid nodes in the chunk that can be processed by algorithms.
        /// The size can grow up to the capacity without having to reallocate the component's memory
        /// </summary>
        /// <returns>The capacity of the chunk</returns>
        Size_t GetNodeCount()const { return this->NodeCount; }

        /// <summary>
        /// Get the chunk type of this chunk
        /// </summary>
        /// <returns></returns>
        const ChunkType_t& GetChunkType()const { return *this->Type; }
    };
}
