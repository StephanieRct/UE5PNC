// MIT License
// Copyright (c) 2025 Stephanie Rancourt

#pragma once
#include "common.h"
#include "KTreePointer.h"

namespace PNC
{
    /// <summary>
    /// A KChunkTreePointer is a KindPointer to a Chunk that is part of a Tree of Chunks.
    /// A KChunkTreePointer is non-copyable as it may be pointed to by other KTreePointers.
    /// </summary>
    /// <typeparam name="TChunkStructure">Structure of the Chunk's Component data.</typeparam>
    /// <summary>
    template<typename TChunkStructure>
    struct KChunkTreePointerT : public KTreePointerT<TChunkStructure>
    {
    public:
        using Base_t = KTreePointerT<TChunkStructure>;
        using Self_t = KChunkTreePointerT<TChunkStructure>;
        using ChunkStructure_t = TChunkStructure;
        using Size_t = typename ChunkStructure_t::Size_t;
        using Chunk_t = ChunkPointerT<ChunkStructure_t>; // Structure this pointer is pointing at
        using Node_t = typename Chunk_t::Node_t;
        using ChunkPointer_t = ChunkPointerT<ChunkStructure_t>;

        using ChunkPointerInternal_t = typename ChunkPointer_t::ChunkPointerInternal_t;
    protected:
        ChunkPointerInternal_t Chunk;

    public:
        /// <summary>
        /// Create a Null KChunkTreePointer without ChunkStructure
        /// </summary>
        KChunkTreePointerT()
            : Base_t(ChunkKind_ChunkTree)
        {
        }

        /// <summary>
        /// Contructs the Chunk from its member data fields.
        /// </summary>
        /// <param name="chunkStructure">Structure of the Chunk's Component data.</param>
        /// <param name="nodeCount">Number of Nodes that are included by this pointer.</param>
        /// <param name="componentData">Points to an array of Component data pointers created according to the ChunkStructure.</param>
        KChunkTreePointerT(const ChunkStructure_t* chunkStructure, const Size_t nodeCount, void** const  componentData)
            : Base_t(ChunkKind_ChunkTree)
            , Chunk(chunkStructure, nodeCount, componentData)
        {
        }

        KChunkTreePointerT(const KChunkTreePointerT&) = delete;
        KChunkTreePointerT(const KChunkTreePointerT&&) = delete;
        KChunkTreePointerT& operator=(const KChunkTreePointerT&) = delete;

    protected:
        KChunkTreePointerT(const ChunkStructure_t* chunkStructure, const Size_t nodeCount, void** const  componentData, const ChunkKind kind)
            : Base_t(kind)
            , Chunk(chunkStructure, nodeCount, componentData)
        {
        }

        KChunkTreePointerT(const ChunkStructure_t* chunkStructure, const Size_t nodeCount, const ChunkKind kind = ChunkKind_ChunkTree)
            : Base_t(kind)
            , Chunk(chunkStructure, nodeCount)
        {
        }


    public:
        bool IsVoid()const { return Chunk.IsVoid(); }
        bool IsStruct()const { return Chunk.IsStruct(); }
        bool IsNull()const { return Chunk.IsNull(); }
        bool IsData()const { return Chunk.IsData(); }
        bool IsVoidNull()const { return Chunk.IsVoidNull(); }
        bool IsVoidData()const { return Chunk.IsVoidData(); }
        bool IsStructNull()const { return Chunk.IsStructNull(); }
        bool IsStructData()const { return Chunk.IsStructData(); }

        /// <summary>
        /// Get the ChunkStructure of this chunk
        /// </summary>
        /// <returns></returns>
        const ChunkStructure_t& GetStructure()const { return Chunk.GetStructure(); }

        /// <summary>
        /// Get the size of the chunk.
        /// The size is the number of valid nodes in the chunk that can be processed by algorithms.
        /// The size can grow up to the capacity without having to reallocate the component's memory
        /// </summary>
        /// <returns>The capacity of the chunk</returns>
        Size_t GetNodeCount()const { return Chunk.GetNodeCount(); }

        Size_t GetChunkCount()const { return 1; }

        const Chunk_t& operator*()const { return (const Chunk_t&)Chunk; }
        Chunk_t& operator*() { return (Chunk_t&)Chunk; }
        const Chunk_t* operator->()const { return &(const Chunk_t&)Chunk; }
        Chunk_t* operator->() { return &(Chunk_t&)Chunk; }
        const Chunk_t& GetChunk()const { return (const Chunk_t&)Chunk; }
        Chunk_t& GetChunk() { return (Chunk_t&)Chunk; }

        void* GetComponentData(const Size_t componentTypeIndexInChunk) { return GetChunk().GetComponentData(componentTypeIndexInChunk); }
        void* GetComponentData(const Size_t componentTypeIndexInChunk)const { return GetChunk().GetComponentData(componentTypeIndexInChunk); }
        void* GetComponentData(const type_info* const componentType) { return GetChunk().GetComponentData(componentType); }
        void* GetComponentData(const type_info* const componentType)const { return GetChunk().GetComponentData(componentType); }
        template<typename TComponent>
        TComponent* GetComponentData() { return GetChunk().GetComponentData<TComponent>(); }
        template<typename TComponent>
        TComponent* GetComponentData()const { return GetChunk().GetComponentData<TComponent>(); }

        static bool IsSameStructure(const Self_t& a, const Self_t& b) { return a.Chunk.Structure == b.Chunk.Structure; }

        static ChunkPointerInternal_t& GetInternalChunk(Self_t& chunkPointer) { return chunkPointer.Chunk; }
    //protected:
    //    ChunkPointerInternal_t& GetInternalChunk() { return Chunk; }
    };
}
