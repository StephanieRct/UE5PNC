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
    /// <typeparam name="TChunkStructure">Structure of the chunk's component data.</typeparam>
    template<typename TChunkStructure>
    struct ChunkPointerInternalT
    {
    public:
        using Self_t = ChunkPointerInternalT<TChunkStructure>;
        using ChunkStructure_t = TChunkStructure;
        using Size_t = typename ChunkStructure_t::Size_t;

    public:
        /// <summary>
        /// ChunkStructure used to create the structure of what ComponentData points to.
        /// </summary>
        const ChunkStructure_t* Structure;

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
        /// Create a null ChunkRefT without structure nor data.
        /// IsNull() will evaluate to true.
        /// </summary>
        ChunkPointerInternalT()
            : Structure(nullptr)
            , ComponentData(nullptr)
            , NodeCount(0)
        {
        }

        ChunkPointerInternalT(const ChunkStructure_t* chunkStructure, Size_t nodeCount, void** componentData)
            : Structure(chunkStructure)
            , ComponentData(componentData)
            , NodeCount(nodeCount)
        {
        }

        ChunkPointerInternalT(const ChunkStructure_t* chunkStructure, Size_t nodeCount)
            : Structure(chunkStructure)
            , ComponentData(nullptr)
            , NodeCount(nodeCount)
        {
        }

    public:
        /// <summary>
        /// Test if the chunk is null and has no structure nor component data.
        /// </summary>
        /// <returns></returns>
        bool IsNull()const { return this->Structure == nullptr; }

        /// <summary>
        /// Get the size of the chunk.
        /// The size is the number of valid nodes in the chunk that can be processed by algorithms.
        /// The size can grow up to the capacity without having to reallocate the component's memory
        /// </summary>
        /// <returns>The capacity of the chunk</returns>
        Size_t GetNodeCount()const { return this->NodeCount; }

        /// <summary>
        /// Get the ChunkStructure of this chunk
        /// </summary>
        /// <returns></returns>
        const ChunkStructure_t& GetChunkStructure()const { return *this->Structure; }
    };
}
