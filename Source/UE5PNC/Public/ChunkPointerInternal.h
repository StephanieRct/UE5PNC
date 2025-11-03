// MIT License
// Copyright (c) 2025 Stephanie Rancourt

#pragma once
#include "common.h"
#include <atomic>

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
        /// Create a VoidNull Chunk.
        /// </summary>
        ChunkPointerInternalT()
            : Structure(nullptr)
            , ComponentData(nullptr)
            , NodeCount(0)
        {
        }

        /// <summary>
        /// Create a StructNull Chunk.
        /// </summary>
        ChunkPointerInternalT(const ChunkStructure_t* chunkStructure)
            : Structure(chunkStructure)
            , ComponentData(nullptr)
            , NodeCount(0)
        {
        }

        /// <summary>
        /// Create a StructNull Chunk with a node count.
        /// </summary>
        ChunkPointerInternalT(const ChunkStructure_t* chunkStructure, const Size_t nodeCount)
            : Structure(chunkStructure)
            , ComponentData(nullptr)
            , NodeCount(nodeCount)
        {
        }

        /// <summary>
        /// Create a VoidData Chunk
        /// </summary>
        ChunkPointerInternalT(const Size_t nodeCount, void** const componentData)
            : Structure(nullptr)
            , ComponentData(componentData)
            , NodeCount(nodeCount)
        {
        }

        /// <summary>
        /// Create a StructData Chunk
        /// </summary>
        /// <param name="chunkStructure">Structure of the Chunk's Component data.</param>
        /// <param name="nodeCount">Number of nodes are included by this pointer.</param>
        /// <param name="componentData">Points to an array of component data pointers created according to the chunk type.</param>
        ChunkPointerInternalT(const ChunkStructure_t* chunkStructure, const Size_t nodeCount, void** const componentData)
            : Structure(chunkStructure)
            , ComponentData(componentData)
            , NodeCount(nodeCount)
        {
        }

        ChunkPointerInternalT(Self_t&& o)
            : Structure(o.Structure)
            , ComponentData(o.ComponentData)
            , NodeCount(o.NodeCount)
        {
            o.Structure = nullptr;
            o.ComponentData = nullptr;
            o.NodeCount = 0;
        }

        Self_t& operator=(Self_t&& o)
        {
            auto tmpStructure = o.Structure;
            auto tmpComponentData = o.ComponentData;
            auto tmpNodeCount = o.NodeCount;
            o.Structure = nullptr;
            o.ComponentData = nullptr;
            o.NodeCount = 0;
            Structure = tmpStructure;
            ComponentData = tmpComponentData;
            NodeCount = tmpNodeCount;
            return *this;
        }

        ChunkPointerInternalT(const Self_t& o) = default;
        Self_t& operator=(const Self_t& o) = default;

#ifdef PNC_MEMORYCLEANUP
        ~ChunkPointerInternalT()
        {
            pnc_clean(Structure);
            pnc_clean(ComponentData);
            pnc_clean(NodeCount);
        }
#endif
    public:
        bool IsVoid()const { return Structure == nullptr; }
        bool IsStruct()const { return Structure != nullptr; }
        bool IsNull()const { return ComponentData == nullptr; }
        bool IsData()const { return ComponentData != nullptr; }
        bool IsVoidNull()const { return Structure == nullptr && ComponentData == nullptr; }
        bool IsVoidData()const { return Structure == nullptr && ComponentData != nullptr; }
        bool IsStructNull()const { return Structure != nullptr && ComponentData == nullptr; }
        bool IsStructData()const { return Structure != nullptr && ComponentData != nullptr; }

        /// <summary>
        /// Get the size of the chunk.
        /// The size is the number of valid nodes in the chunk that can be processed by algorithms.
        /// The size can grow up to the capacity without having to reallocate the component's memory
        /// </summary>
        /// <returns>The capacity of the chunk</returns>
        Size_t GetNodeCount()const { return NodeCount; }

        Size_t GetChunkCount()const { return 1; }
        
        /// <summary>
        /// Get the ChunkStructure of this chunk
        /// </summary>
        /// <returns></returns>
        const ChunkStructure_t& GetStructure()const { return *this->Structure; }

        void* GetComponentData(const Size_t componentTypeIndexInChunk)
        {
            pnc_assert(!IsNull());
            return this->ComponentData[componentTypeIndexInChunk];
        }

    };
}
