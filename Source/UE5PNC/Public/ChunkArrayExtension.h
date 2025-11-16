// MIT License
// Copyright (c) 2025 Stephanie Rancourt

#pragma once
#include "common.h"

namespace PNC
{
    /// <summary>
    /// Extend a ChunkPointer to include the required data field to point to an array of chunks.
    /// </summary>
    /// <typeparam name="TChunkStructure">Structure of the chunk's component data.</typeparam>
    /// <typeparam name="TChunkPointerElement">Structure of the chunk pointer in the array.</typeparam>
    template<typename TChunkStructure, typename TChunkPointerElement>
    struct ChunkArrayExtensionT
    {
    public:
        using Self = ChunkArrayExtensionT<TChunkStructure, TChunkPointerElement>;
        using ChunkStructure_t = TChunkStructure;
        using ChunkPointerElement_t = TChunkPointerElement;
        using Size_t = typename ChunkStructure_t::Size_t;
        using ChunkPointerInternal_t = ChunkPointerInternalT<TChunkStructure>;
        using ChunkPointerElementInternal_t = typename ChunkPointerElement_t::ChunkPointerInternal_t;

    public:
        /// <summary>
        /// Points to an array of ChunkPointerElement_t with at least ChunkCount elements
        /// </summary>
        ChunkPointerElement_t* Chunks;

        /// <summary>
        /// Number of chunk in the Chunks array
        /// </summary>
        Size_t ChunkCount;

    public:
        /// <summary>
        /// Create a null pointer.
        /// </summary>
        /// <param name="chunkCount"></param>
        ChunkArrayExtensionT(Size_t chunkCount = 0)
            : Chunks(nullptr)
            , ChunkCount(chunkCount)
        {
        }

        /// <summary>
        /// Contructs from its member data fields.
        /// </summary>
        /// <param name="chunks">Array of ChunkPointerElement_t with at least chunkCount elements.</param>
        /// <param name="chunkCount">Number of chunk in chunks array</param>
        ChunkArrayExtensionT(ChunkPointerElement_t* chunks, Size_t chunkCount)
            : Chunks(chunks)
            , ChunkCount(chunkCount)
        {
        }
#ifdef PNC_MEMORYCLEANUP
        ~ChunkArrayExtensionT()
        {
            pnc_clean(Chunks);
            pnc_clean(ChunkCount);
        }
#endif

    public:
        Size_t GetChunkCount()const { return ChunkCount; }
        const ChunkPointerElement_t GetChunk(const ChunkPointerInternal_t& chunkArray, Size_t index)const { return Chunks[index]; }
        ChunkPointerElement_t GetChunk(ChunkPointerInternal_t& chunkArray, Size_t index) { return Chunks[index]; }

        const ChunkPointerElementInternal_t& GetInternalChunk(const ChunkPointerInternal_t& chunkArray, Size_t index)const { return ChunkPointerElement_t::GetInternalChunk(Chunks[index]); }
        ChunkPointerElementInternal_t& GetInternalChunk(ChunkPointerInternal_t& chunkArray, Size_t index){ return ChunkPointerElement_t::GetInternalChunk(Chunks[index]); }
    };


    // TODO consider an alternative ChunkArrayExtensionT with these data fields:
    //  Size_t Multiplicity;
    //  Size_t FirstNodePerChunk; //Element(x).FirstNode == FirstNodePerChunk * x + FirstNodes[x%Multiplicity]
    //  Size_t NodeCountPerChunk; //Element(x).NodeCount == NodeCountPerChunk * x + NodeCounts[x%Multiplicity]
    //  Size_t* FirstNodes;
    //  Size_t* NodeCounts;
    //  NodeRange<Size_t>* Ranges;
    // 
    //  template<typename TSize>
    //  struct NodeRange
    //  {
    //      using Size_t = TSize;
    //      Size_t First;
    //      Size_t Count;
    //  };
}
