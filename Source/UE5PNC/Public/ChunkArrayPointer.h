// MIT License
// Copyright (c) 2025 Stephanie Rancourt

#pragma once
#include "common.h"
#include "ChunkArrayPointerInternal.h"

namespace PNC
{
    /// <summary>
    /// A ChunkArrayPointer points to an Array of ChunkPointers whose Component data is adjacent in memory.
    /// A ChunkArrayPointer is itself a ChunkPointer pointing to the beginning of the Component data, 
    /// effectively pointing to the first Chunk in the Array.
    /// </summary>
    /// <typeparam name="TChunkStructure">Structure of the Chunk's Component data.</typeparam>
    /// <typeparam name="TChunkPointerElement">Structure of the Chunk pointer in the Array. 
    ///     Must be a ChunkPointer (does not copy nodes when copied) rather than a Chunk (copy nodes when copied)
    /// </typeparam>
    template<typename TChunkStructure, typename TChunkPointerElement = ChunkPointerT<TChunkStructure>>
    struct ChunkArrayPointerT : public ChunkPointerT<TChunkStructure>
    {
    public:
        using Base_t = ChunkPointerT<TChunkStructure>;
        using Self_t = ChunkArrayPointerT<TChunkStructure, TChunkPointerElement>;
        using ChunkStructure_t = TChunkStructure;
        using ChunkPointerElement_t = TChunkPointerElement;
        using Size_t = typename TChunkStructure::Size_t;


        /// <summary>
        /// Chunk_t is the type a ChunkPointer points to
        /// </summary>
        using Chunk_t = Self_t;

        /// <summary>
        /// Use the same ChunkArrayExtension_t as ChunkArrayPointerInternalT so both keep the same memory layout.
        /// </summary>
        using ChunkArrayExtension_t = typename ChunkArrayPointerInternalT<ChunkStructure_t, TChunkPointerElement>::ChunkArrayExtension_t;

        /// <summary>
        /// Reinterprete_cast this object to this type to get read-write access to it's private data fields
        /// Use with caution.
        /// </summary>
        using ChunkPointerInternal_t = ChunkArrayPointerInternalT<ChunkStructure_t, TChunkPointerElement>;

        using ChunkPointerElementInternal_t = typename ChunkPointerElement_t::ChunkPointerInternal_t;

    protected:
        /// <summary>
        /// Contains pointer to the array of Chunks
        /// </summary>
        ChunkArrayExtension_t Array;

    public:
        ChunkArrayPointerT() = default;
        ChunkArrayPointerT(const Self_t& o) = default;
        Self_t& operator=(const Self_t& o) = default;
        ChunkArrayPointerT(Self_t&& o) = default;
        Self_t& operator=(Self_t&& o) = default;

        /// <summary>
        /// Contructs from its member data fields.
        /// </summary>
        /// <param name="chunkStructure">Structure of the Chunk's Component data.</param>
        /// <param name="componentData">Points to an array of Component data pointers created according to the ChunkStructure.</param>
        /// <param name="chunks">Points to an array of ChunkPointerElement_t the size of chunkCount or more.</param>
        /// <param name="chunkCount">Number of Chunks in the Array.</param>
        /// <param name="totalNodeCount">The total number of Nodes used by all Chunks in the Array.</param>
        ChunkArrayPointerT(const ChunkStructure_t* chunkStructure, void** componentData, ChunkPointerElement_t* chunks, Size_t chunkCount, Size_t totalNodeCount)
            : Base_t(chunkStructure, totalNodeCount, componentData)
            , Array(chunks, chunkCount)
        {

        }

    protected:
        ChunkArrayPointerT(const ChunkStructure_t* chunkStructure, Size_t totalNodeCount, Size_t chunkCount)
            : Base_t(chunkStructure, totalNodeCount)
            , Array(chunkCount)
        {
        }

    public:
        /// <summary>
        /// Number of elements in the array
        /// </summary>
        /// <returns></returns>
        Size_t GetChunkCount()const { return Array.ChunkCount; }

        // TODO Array indexing must return copies
        const ChunkPointerElement_t& operator[](Size_t index)const { return Array.Chunks[index]; }
        // TODO Array indexing must return copies
        ChunkPointerElement_t& operator[](Size_t index) { return Array.Chunks[index]; }
        // TODO Array indexing must return copies
        const ChunkPointerElement_t& GetChunk(Size_t index)const { return Array.Chunks[index]; }
        // TODO Array indexing must return copies
        ChunkPointerElement_t& GetChunk(Size_t index) { return Array.Chunks[index]; }
        const Chunk_t& operator*()const { return *this; }
        Chunk_t& operator*() { return *this; }
        const Chunk_t* operator->()const { return this; }
        Chunk_t* operator->() { return this; }
        const Chunk_t& GetChunk()const { return *this; }
        Chunk_t& GetChunk() { return *this; }

        static ChunkPointerInternal_t& GetInternalChunk(Self_t& chunkPointer) { return reinterpret_cast<ChunkPointerInternal_t&>(chunkPointer.GetChunk()); }
        static ChunkPointerElementInternal_t& GetInternalChunkElement(Self_t& chunkPointer, const Size_t index) { return reinterpret_cast<ChunkPointerElementInternal_t&>(chunkPointer.Array.Chunks[index]); }
    protected:
        //ChunkPointerInternal_t& GetInternalChunk() { return reinterpret_cast<ChunkPointerInternal_t&>(GetChunk()); }
        //ChunkPointerElementInternal_t& GetInternalChunkElement(const Size_t index) { return reinterpret_cast<ChunkPointerElementInternal_t&>(Array.Chunks[index]); }
        const ChunkArrayExtension_t& GetArrayExtension()const { return Array; }
        ChunkArrayExtension_t& GetArrayExtension() { return Array; }
    };
}
