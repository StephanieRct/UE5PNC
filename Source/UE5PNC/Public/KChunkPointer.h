// MIT License
// Copyright (c) 2025 Stephanie Rancourt

#pragma once
#include "common.h"
#include "KindPointer.h"

namespace PNC
{
    /// <summary>
    /// A KChunkPointer is a KindPointer to a Chunk's Component data memory within a range of Nodes.
    /// A KChunkPointer is also a Chunk by itself and provides access to the Component data.
    /// Copying a KChunkPointer does not copy the data.
    /// </summary>
    /// <typeparam name="TChunkStructure">Structure of the Chunk's Component data.</typeparam>
    /// <summary>
    template<typename TChunkStructure>
    struct KChunkPointerT : public KindPointerT<TChunkStructure>
    {
    public:
        using Base_t = KindPointerT<TChunkStructure>;
        using Self_t = KChunkPointerT<TChunkStructure>;
        using ChunkStructure_t = TChunkStructure;
        using Size_t = typename ChunkStructure_t::Size_t;
        using ChunkPointer_t = ChunkPointerT<TChunkStructure>;
        using Chunk_t = ChunkPointer_t;
        using Node_t = NodeT<TChunkStructure>;

        using ChunkPointerInternal_t = typename ChunkPointer_t::ChunkPointerInternal_t;
    protected:
        ChunkPointerInternal_t Chunk;

    public:
        /// <summary>
        /// Create a Null KChunkPointer without ChunkStructure
        /// </summary>
        KChunkPointerT()
        {
        }

        /// <summary>
        /// Contructs from its member data fields.
        /// </summary>
        /// <param name="chunkStructure">Structure of the Chunk's Component data.</param>
        /// <param name="nodeCount">Number of Nodes that are included by this pointer.</param>
        /// <param name="componentData">Points to an array of Component data pointers created according to the ChunkStructure.</param>
        KChunkPointerT(const ChunkStructure_t* chunkStructure, Size_t nodeCount, void** componentData)
            : Base_t()
            , Chunk(chunkStructure, nodeCount, componentData)
        {
        }

    protected:
        KChunkPointerT(const ChunkStructure_t* chunkStructure, Size_t nodeCount, void** componentData, ChunkKind kind)
            : Base_t(kind)
            , Chunk(chunkStructure, nodeCount, componentData)
        {
        }

        KChunkPointerT(ChunkKind kind = ChunkKind_Chunk)
            : Base_t(kind)
            , Chunk()
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

        const ChunkStructure_t& GetStructure()const { return *Chunk.GetStructure(); }
        Size_t GetNodeCount()const { return Chunk.GetNodeCount(); }
        Size_t GetChunkCount()const { return 1; }
        void* GetComponentData(const Size_t componentTypeIndexInChunk) { return GetChunk().GetComponentData(componentTypeIndexInChunk); }
        void* GetComponentData(const Size_t componentTypeIndexInChunk)const { return GetChunk().GetComponentData(componentTypeIndexInChunk); }
        void* GetComponentData(const type_info* const componentType) { return GetChunk().GetComponentData(componentType); }
        void* GetComponentData(const type_info* const componentType)const { return GetChunk().GetComponentData(componentType); }
        template<typename TComponent>
        TComponent* GetComponentData() { return GetChunk().GetComponentData<TComponent>(); }
        template<typename TComponent>
        TComponent* GetComponentData()const { return GetChunk().GetComponentData<TComponent>(); }

        const Chunk_t& operator*()const { return (const Chunk_t&)Chunk; }
        Chunk_t& operator*() { return (Chunk_t&)Chunk; }
        const Chunk_t* operator->()const { return &(const Chunk_t&)Chunk; }
        Chunk_t* operator->() { return &(Chunk_t&)Chunk; }
        const Chunk_t& GetChunk()const { return (const Chunk_t&)Chunk; }
        Chunk_t& GetChunk() { return (Chunk_t&)Chunk; }


        static bool IsSameStructure(const Self_t& a, const Self_t& b) { return a.Chunk.Structure == b.Chunk.Structure; }
        static ChunkPointerInternal_t& GetInternalChunk(Self_t& chunkPointer) { return reinterpret_cast<ChunkPointerInternal_t&>(chunkPointer.GetChunk()); }
    //protected:
    //    ChunkPointerInternal_t& GetInternalChunk() { return reinterpret_cast<ChunkPointerInternal_t&>(GetChunk()); }
    };
}
