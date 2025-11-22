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

        using typename Base_t::Size_t;
        using ComponentType_t = typename TChunkPointer::ComponentType_t;
        using ChunkStructure_t = typename TChunkPointer::ChunkStructure_t;
        using Node_t = typename TChunkPointer::Node_t;
        using ChunkPointerInternal_t = typename TChunkPointer::ChunkPointerInternal_t;
        using ChunkPointer_t = TChunkPointer;
        using Chunk_t = typename TChunkPointer::Chunk_t;

    protected:
        ChunkPointer_t Chunk;

    public:
        DChunkPointer()
        {}

        DChunkPointer(const ChunkStructure_t* const chunkStructure, const Size_t nodeCount, void** componentData)
            : Base_t()
            , Chunk(chunkStructure, nodeCount, componentData)
        {}
        template<class... TBaseCTorArgumentTypes>
        DChunkPointer(const ChunkStructure_t* const chunkStructure, const Size_t nodeCount, void** const componentData, TBaseCTorArgumentTypes&&... args)
            : Base_t(args...)
            , Chunk(chunkStructure, nodeCount, componentData)
        {}

        template<class... TBaseCTorArgumentTypes>
        DChunkPointer(TBaseCTorArgumentTypes&&... args, const ChunkStructure_t* const chunkStructure, const Size_t nodeCount, void** const componentData)
            : Base_t(args...)
            , Chunk(chunkStructure, nodeCount, componentData)
        {
        }
        template<class... TBaseCTorArgumentTypesBefore, class... TBaseCTorArgumentTypesAfter>
        DChunkPointer(TBaseCTorArgumentTypesBefore&&... argsBefore, const ChunkStructure_t* const chunkStructure, const Size_t nodeCount, void** const componentData, TBaseCTorArgumentTypesAfter&&... argsAfter)
            : Base_t(argsBefore..., argsAfter...)
            , Chunk(chunkStructure, nodeCount, componentData)
        {
        }

        DChunkPointer(const ChunkStructure_t* const chunkStructure, const Size_t nodeCount)
            : Base_t()
            , Chunk(chunkStructure, nodeCount)
        {}
        template<class... TBaseCTorArgumentTypes>
        DChunkPointer(const ChunkStructure_t*const chunkStructure, const Size_t nodeCount, TBaseCTorArgumentTypes&&... args)
            : Base_t(args...)
            , Chunk(chunkStructure, nodeCount)
        {}
        template<class... TBaseCTorArgumentTypes>
        DChunkPointer(TBaseCTorArgumentTypes&&... args, const ChunkStructure_t* const chunkStructure, const Size_t nodeCount)
            : Base_t(args...)
            , Chunk(chunkStructure, nodeCount)
        {
        }
        template<class... TBaseCTorArgumentTypesBefore, class... TBaseCTorArgumentTypesAfter>
        DChunkPointer(TBaseCTorArgumentTypesBefore&&... argsBefore, const ChunkStructure_t* const chunkStructure, const Size_t nodeCount, TBaseCTorArgumentTypesAfter&&... argsAfter)
            : Base_t(argsBefore..., argsAfter...)
            , Chunk(chunkStructure, nodeCount)
        {
        }



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
        const ChunkStructure_t& GetStructure()const { return Chunk.GetStructure(); }
        Size_t GetNodeCount()const { return Chunk.GetNodeCount(); }
        Size_t GetNodeCapacity()const { return Chunk.GetNodeCapacity(); }
        Size_t GetChunkCount()const { return Chunk.GetChunkCount(); }
        Size_t GetChunkCapacity()const { return Chunk.GetChunkCapacity(); }
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
        static bool IsSameStructure(const Self_t& a, const Self_t& b) { return ChunkPointer_t::IsSameStructure(a.Chunk, b.Chunk); }
        static bool IsSameData(const Self_t& a, const Self_t& b) { return ChunkPointer_t::IsSameData(a.Chunk, b.Chunk); }
        static ChunkPointerInternal_t& GetInternalChunk(Self_t& chunkPointer) { return reinterpret_cast<ChunkPointerInternal_t&>(chunkPointer.Chunk); }
    protected:
        template<typename TChunk>
        static void AllocateComponentDataArray(TChunk& chunk)
            { return ChunkPointer_t::AllocateComponentDataArray(chunk); }
        template<typename TChunk>
        static void FreeComponentDataArray(TChunk& chunk)
            { return ChunkPointer_t::FreeComponentDataArray(chunk); }
        template<typename TChunk>
        static void AllocateConstruct(TChunk& chunk, const Size_t nodeCapacity, const Size_t chunkCapacity)
            { return ChunkPointer_t::AllocateConstruct(chunk, nodeCapacity, chunkCapacity); }
        template<typename TChunk>
        static void AllocateConstruct(TChunk& chunk)
            { return ChunkPointer_t::AllocateConstruct(chunk); }
        template<typename TChunk>
        static void AllocateCopy(TChunk& chunkToOverwrite, const TChunk& chunkFrom, const Size_t nodeCapacity, const Size_t chunkCapacity)
            { return ChunkPointer_t::AllocateCopy(chunkToOverwrite, chunkFrom, nodeCapacity, chunkCapacity); }
        template<typename TChunk>
        static void AllocateCopy(TChunk& chunkToOverwrite, const TChunk& chunkFrom)
            { AllocateCopy(chunkToOverwrite, chunkFrom, chunkFrom.GetNodeCapacity()); }
        template<typename TChunk>
        static void ReallocateCopy(TChunk& chunkTo, const TChunk& chunkFrom, const Size_t newNodeCapacity, const Size_t newChunkCapacity)
            { return ChunkPointer_t::ReallocateCopy(chunkTo, chunkFrom, newNodeCapacity, newChunkCapacity); }
        template<typename TChunk>
        static void ReallocateCopy(TChunk& chunkTo, const TChunk& chunkFrom)
            { return ReallocateCopy(chunkTo, chunkFrom, chunkFrom.GetNodeCapacity()); }
        template<typename TChunk>
        static void ReallocateMove(TChunk& chunkToReallocate, TChunk& chunkFrom, const Size_t newNodeCapacity, const Size_t newChunkCapacity)
            { return ChunkPointer_t::ReallocateMove(chunkToReallocate, chunkFrom, newNodeCapacity, newChunkCapacity); }
        template<typename TChunk>
        static void ReallocateMove(TChunk& chunkToReallocate, TChunk& chunkFrom)
            { return ReallocateMove(chunkToReallocate, chunkFrom, chunkFrom.GetNodeCapacity()); }
        template<typename TChunk>
        static void FreeDestruct(TChunk& chunk)
            { return ChunkPointer_t::FreeDestruct(chunk); }
    };

}


