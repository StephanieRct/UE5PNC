// MIT License
// Copyright (c) 2025 Stephanie Rancourt

#pragma once
#include "ChunkPointer.h"

namespace PNC
{
    template<typename TChunkPointer, typename TBase>
    struct DChunkPointer : public TBase
    {
    public:
        using Base_t = TBase;
        using Self_t = DChunkPointer<TChunkPointer, TBase>;
        using ChunkPointer_t = TChunkPointer;
        using Chunk_t = ChunkPointer_t;
        using ChunkStructure_t = typename ChunkPointer_t::ChunkStructure_t;
        using Size_t = typename ChunkPointer_t::Size_t;
        using ChunkPointerInternal_t = typename ChunkPointer_t::ChunkPointerInternal_t;

    protected:
        ChunkPointer_t Chunk;
    public:
        DChunkPointer()
        {}

        DChunkPointer(const ChunkStructure_t* chunkStructure, Size_t nodeCount, void** componentData)
            : Base_t()
            , Chunk(chunkStructure, nodeCount, componentData)
        {}
        template<class... TBaseCTorArgumentTypes>
        DChunkPointer(const ChunkStructure_t* chunkStructure, Size_t nodeCount, void** componentData, TBaseCTorArgumentTypes&&... args)
            : Base_t(args...)
            , Chunk(chunkStructure, nodeCount, componentData)
        {}

        DChunkPointer(const ChunkStructure_t* chunkStructure, Size_t nodeCount)
            : Base_t()
            , Chunk(chunkStructure, nodeCount)
        {}
        template<class... TBaseCTorArgumentTypes>
        DChunkPointer(const ChunkStructure_t* chunkStructure, Size_t nodeCount, TBaseCTorArgumentTypes&&... args)
            : Base_t(args...)
            , Chunk(chunkStructure, nodeCount)
        {}

        template<class... TBaseCTorArgumentTypes>
        DChunkPointer(TBaseCTorArgumentTypes&&... args)
            : Base_t(args...)
        {}

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
        Size_t GetChunkCount()const { return Chunk.GetChunkCount(); }
        void* GetComponentData(const Size_t componentTypeIndexInChunk) { return Chunk.GetComponentData(componentTypeIndexInChunk); }
        void* GetComponentData(const Size_t componentTypeIndexInChunk)const { return Chunk.GetComponentData(componentTypeIndexInChunk); }
        void* GetComponentData(const type_info* const componentType) { return Chunk.GetComponentData(componentType); }
        void* GetComponentData(const type_info* const componentType)const { return Chunk.GetComponentData(componentType); }
        template<typename TComponent>
        TComponent* GetComponentData() { return Chunk.GetComponentData<TComponent>(); }
        template<typename TComponent>
        TComponent* GetComponentData()const { return Chunk.GetComponentData<TComponent>(); }
        const Chunk_t& operator*()const { return (const Chunk_t&)Chunk; }
        Chunk_t& operator*() { return (Chunk_t&)Chunk; }
        const Chunk_t* operator->()const { return &(const Chunk_t&)Chunk; }
        Chunk_t* operator->() { return &(Chunk_t&)Chunk; }
        const Chunk_t& GetChunk()const { return (const Chunk_t&)Chunk; }
        Chunk_t& GetChunk() { return (Chunk_t&)Chunk; }
        static bool IsSameStructure(const Self_t& a, const Self_t& b) { return a.Chunk.Structure == b.Chunk.Structure; }
        static ChunkPointerInternal_t& GetInternalChunk(Self_t& chunkPointer) { return reinterpret_cast<ChunkPointerInternal_t&>(chunkPointer.Chunk); }
    protected:
        template<typename TChunk>
        static void AllocateDataCopy(TChunk& chunkToOverwrite, const TChunk& chunkFrom, const Size_t newNodeCapacity) 
            { return ChunkPointer_t::AllocateDataCopy(chunkToOverwrite, chunkFrom, newNodeCapacity); }
        template<typename TChunk>
        static void AllocateDataCopy(TChunk& chunkToOverwrite, const TChunk& chunkFrom)
            { AllocateDataCopy(chunkToOverwrite, chunkFrom, chunkFrom.GetNodeCapacity()); }
        template<typename TChunk>
        static void ReallocateDataCopy(TChunk& chunkTo, const TChunk& chunkFrom, const Size_t newNodeCapacity)
            { return ChunkPointer_t::ReallocateDataCopy(chunkTo, chunkFrom, newNodeCapacity); }
        template<typename TChunk>
        static void ReallocateDataCopy(TChunk& chunkTo, const TChunk& chunkFrom)
            { return ReallocateDataCopy(chunkTo, chunkFrom, chunkFrom.GetNodeCapacity()); }
        template<typename TChunk>
        static void ReallocateDataMove(TChunk& chunkToReallocate, TChunk& chunkFrom, const Size_t newNodeCapacity)
            { return ChunkPointer_t::ReallocateDataMove(chunkToReallocate, chunkFrom, newNodeCapacity); }
        template<typename TChunk>
        static void ReallocateDataMove(TChunk& chunkToReallocate, TChunk& chunkFrom)
            { return ReallocateDataMove(chunkToReallocate, chunkFrom, chunkFrom.GetNodeCapacity()); }
        template<typename TChunk>
        static void AllocateAndConstructData(TChunk& chunkToOverwrite, const Size_t nodeCapacity, const Size_t nodeCount)
            { return ChunkPointer_t::AllocateAndConstructData(chunkToOverwrite, nodeCapacity, nodeCount); }
        template<typename TChunk>
        static void AllocateDataArray(TChunk& chunk){ return ChunkPointer_t::AllocateDataArray(chunk); }
        template<typename TChunk>
        static void DestructAndFreeData(TChunk& chunk){ return ChunkPointer_t::DestructAndFreeData(chunk); }
        template<typename TChunk>
        static void Destroy(TChunk& chunk){ return ChunkPointer_t::Destroy(chunk); }
        template<typename TChunk>
        static void FreeDataArray(TChunk& chunk){ return ChunkPointer_t::FreeDataArray(chunk); }
    };

}


