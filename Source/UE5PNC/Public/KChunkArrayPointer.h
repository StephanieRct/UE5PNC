// MIT License
// Copyright (c) 2025 Stephanie Rancourt

#pragma once
#include "common.h"
#include "KChunkPointer.h"
//#include "ChunkArrayPointer.h"

namespace PNC
{
    template<typename TChunkStructure, typename TChunkPointerElement>
    using KChunkArrayPointerT = DArrayPointerT<ChunkArrayExtensionT<TChunkStructure, TChunkPointerElement>,
                                KChunkPointerT<TChunkStructure> >;
    //// TODO REMOVE
    ///// <summary>
    ///// A KChunkArrayPointer is a KindPointer pointing to an array of ChunkPointers whose Component data are adjacent in memory.
    ///// A KChunkArrayPointer is itself a KChunkPointer pointing to the beginning of the Component data, 
    ///// effectively pointing to the first Chunk in the array.
    ///// Memory layout of this struct must be:
    /////     Kind            : ChunkKind
    /////     Structure       : ChunkStructure
    /////     ComponentData   : void**
    /////     NodeCount       : Size_t
    /////     Array.Chunks    : ChunkPointerElement_t
    /////     Array.ChunkCount: Size_t
    ///// </summary>
    ///// <typeparam name="TChunkStructure">Structure of the Chunk's Component data.</typeparam>
    ///// <typeparam name="TChunkPointerElement">Structure of the Chunk pointer in the array.</typeparam>
    //template<typename TChunkStructure, typename TChunkPointerElement>
    //struct KChunkArrayPointerT : public KChunkPointerT<TChunkStructure>
    //{
    //public:
    //    using Base_t = KChunkPointerT<TChunkStructure>;
    //    using Self_t = KChunkArrayPointerT<TChunkStructure, TChunkPointerElement>;
    //    using ChunkStructure_t = TChunkStructure;
    //    using ChunkPointerElement_t = TChunkPointerElement;
    //    using Size_t = typename ChunkStructure_t::Size_t;
    //    using Chunk_t = ChunkArrayPointerT<ChunkStructure_t, ChunkPointerElement_t>; // The type this pointer is pointing to

    //    using ChunkArrayPointer_t = Chunk_t;

    //    /// <summary>
    //    /// Use the same ArrayExtension_t as ChunkArrayPointerT so both keep the same memory layout.
    //    /// </summary>
    //    using ArrayExtension_t = ChunkArrayPointer_t::ArrayExtension_t;

    //    using ChunkPointerInternal_t = ChunkArrayPointer_t::ChunkPointerInternal_t;
    //    using ChunkPointerElementInternal_t = typename ChunkPointerElement_t::ChunkPointerInternal_t;

    //protected:
    //    /// <summary>
    //    /// Contains pointer to the array of Chunks
    //    /// </summary>
    //    ArrayExtension_t Array;

    //public:
    //    /// <summary>
    //    /// Create a Null ChunkArrayPointer without ChunkStructure.
    //    /// IsNull() will evaluate to true.
    //    /// </summary>
    //    KChunkArrayPointerT()
    //    {
    //    }

    //    /// <summary>
    //    /// Contructs from its member data fields.
    //    /// </summary>
    //    /// <param name="chunkStructure">Structure of the Chunk's Component data.</param>
    //    /// <param name="componentData">Points to an array of Component data pointers created according to the ChunkStructure.</param>
    //    /// <param name="chunks">Points to an array of ChunkPointerElement_t the size of chunkCount or more.</param>
    //    /// <param name="chunkCount">Number of Chunks in the Array.</param>
    //    /// <param name="totalNodeCount">The total number of Nodes used by all Chunks in the array.</param>
    //    KChunkArrayPointerT(const ChunkStructure_t* chunkStructure, void** componentData, ChunkPointerElement_t* chunks, Size_t chunkCount, Size_t totalNodeCount)
    //        : Base_t(chunkStructure, totalNodeCount, componentData)
    //        , Array(chunks, chunkCount)
    //    {
    //    }

    //protected:
    //    KChunkArrayPointerT(const ChunkStructure_t* chunkStructure, void** componentData, ChunkPointerElement_t* chunks, Size_t chunkCount, Size_t totalNodeCount, ChunkKind kind)
    //        : Base_t(chunkStructure, totalNodeCount, componentData, kind)
    //        , Array(chunks, chunkCount)
    //    {
    //    }

    //public:
    //    Size_t GetChunkCount()const { return Array.ChunkCount; }

    //    const ChunkPointerElement_t& operator[](Size_t index)const { return Array.Chunks[index]; }
    //    ChunkPointerElement_t& operator[](Size_t index) { return Array.Chunks[index]; }
    //    const Chunk_t& GetChunk(Size_t index)const { return Array.Chunks[index]; }
    //    Chunk_t& GetChunk(Size_t index) { return Array.Chunks[index]; }
    //    const Chunk_t& operator*()const { return GetChunk(); }
    //    Chunk_t& operator*() { return GetChunk(); }
    //    const Chunk_t* operator->()const { return &GetChunk(); }
    //    Chunk_t* operator->() { return &GetChunk(); }
    //    const Chunk_t& GetChunk()const { return (const Chunk_t&)Base_t::GetChunk(); }
    //    Chunk_t& GetChunk() { return (Chunk_t&)Base_t::GetChunk(); }

    //    static ChunkPointerInternal_t& GetInternalChunk(Self_t& chunkPointer) { return ChunkArrayPointer_t::GetInternalChunk(chunkPointer.GetChunk()); }
    //    static ChunkPointerElementInternal_t& GetInternalChunkElement(Self_t& chunkPointer, const Size_t index) { return chunkPointer.GetInternalChunkElement(index); }
    ////protected:
    ////    ChunkPointerInternal_t& GetInternalChunk() { return reinterpret_cast<ChunkPointerInternal_t&>(GetChunk()); }
    ////    ChunkPointerElementInternal_t& GetInternalChunkElement(const Size_t index) { return reinterpret_cast<ChunkPointerElementInternal_t&>(Array.Chunks[index]); }
    //};
}
