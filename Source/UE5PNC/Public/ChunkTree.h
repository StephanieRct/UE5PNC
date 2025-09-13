// MIT License
// Copyright (c) 2025 Stephanie Rancourt

#pragma once
#include "common.h"
#include "chunk.h"
#include "Components.h"

namespace PNC 
{
    template<typename TChunkType>
    struct IdentifiableTreePointerT : public IdentifiableChunkT<TChunkType>
    {
    public:
        using ChunkType_t = TChunkType;
        using Base_t = IdentifiableChunkT<TChunkType>;
        using Size_t = typename ChunkType_t::Size_t;

        using ChunkTreePointer_t = IdentifiableTreePointerT<TChunkType>;
        using CoChunkTree_t = CoChunkTreeT<ChunkTreePointer_t>;

    public:
        CoChunkTree_t Tree;
        ChunkTreePointer_t* GetParentChunk()const { return Tree.Parent; }
        ChunkTreePointer_t* GetFirstChildChunk()const { return Tree.FirstChild; }
        ChunkTreePointer_t* GetNextSiblingChunk()const { return Tree.NextSibling; }
        ChunkTreePointer_t* GetPreviousSiblingChunk()const { return Tree.PreviousSibling; }


        IdentifiableTreePointerT(ChunkKind kind)
            :Base_t(kind) 
        {
        }

        void MoveToFirstChild(ChunkTreePointer_t* child)
        {
            child->Extract();
            InsertFirstChild(child);
        }

        void MoveToLastChild(ChunkTreePointer_t* child)
        {
            child->Extract();
            InsertLastChild(child);
        }

        void MoveToPreviousSibling(ChunkTreePointer_t* sibling)
        {
            sibling->Extract();
            InsertPreviousSibling(sibling);
        }

        void MoveToNextSibling(ChunkTreePointer_t* sibling)
        {
            Tree.NextSibling->MoveToNextSibling(sibling);
        }

        void InsertFirstChild(ChunkTreePointer_t* child)
        {
            assert_pnc(child->Tree.IsExtracted());
            child->Tree.Parent = this;
            if (Tree.FirstChild == nullptr)
            {
                child->Tree.NextSibling = child;
                child->Tree.PreviousSibling = child;
                Tree.FirstChild = child;
            }
            else
            {
                Tree.FirstChild->InsertPreviousSibling(child);
                Tree.FirstChild = child;
            }
        }

        void InsertLastChild(ChunkTreePointer_t* child)
        {
            assert_pnc(child->Tree.IsExtracted());
            child->Tree.Parent = this;
            if (Tree.FirstChild == nullptr)
            {
                child->Tree.NextSibling = child;
                child->Tree.PreviousSibling = child;
                Tree.FirstChild = child;
            }
            else
            {
                Tree.FirstChild->InsertPreviousSibling(child);
            }
        }

        void InsertPreviousSibling(ChunkTreePointer_t* sibling)
        {
            assert_pnc(sibling->Tree.IsExtracted());
            auto last = Tree.PreviousSibling;
            last->Tree.NextSibling = sibling;
            sibling->Tree.PreviousSibling = last;
            sibling->Tree.NextSibling = this;
            Tree.PreviousSibling = sibling;
        }

        void InsertNextSibling(ChunkTreePointer_t* sibling)
        {
            Tree.NextSibling->InsertPreviousSibling(sibling);
        }

        void Extract()
        {
            Tree.PreviousSibling->Tree.NextSibling = Tree.NextSibling;
            Tree.NextSibling->Tree.PreviousSibling = Tree.PreviousSibling;
            Tree.PreviousSibling = nullptr;
            Tree.NextSibling = nullptr;
        }

    };

    template<typename TChunkType>
    struct ChunkTreePointerT : public IdentifiableTreePointerT<TChunkType>
    {
    public:
        using Base_t = IdentifiableTreePointerT<TChunkType>;
        using ChunkType_t = TChunkType;
        using Size_t = typename ChunkType_t::Size_t;
        using ChunkPointer_t = ChunkPointerT<ChunkType_t>;
        using Chunk_t = ChunkPointer_t;
        using ChunkInternal_t = ChunkPointerInternalT<TChunkType>;

    protected:
        ChunkInternal_t Chunk;

    public:
        ChunkTreePointerT()
            : Base_t(ChunkKind_ChunkTree)
        {
        }

        ChunkTreePointerT(const ChunkType_t* chunkType, Size_t nodeCount, void** componentData)
            : Base_t(ChunkKind_ChunkTree)
            , Chunk(chunkType, nodeCount, componentData)
        {
        }

    protected:
        ChunkTreePointerT(const ChunkType_t* chunkType, Size_t nodeCount, void** componentData, ChunkKind kind)
            : Base_t(kind)
            , Chunk(chunkType, nodeCount, componentData)
        {
        }

        ChunkTreePointerT(const ChunkType_t* chunkType, Size_t nodeCount, ChunkKind kind = ChunkKind_ChunkTree)
            : Base_t(kind)
            , Chunk(chunkType, nodeCount)
        {
        }

    public:
        const Chunk_t& operator*()const { return (const Chunk_t&)Chunk; }
        Chunk_t& operator*() { return (Chunk_t&)Chunk; }
        const Chunk_t* operator->()const { return &(const Chunk_t&)Chunk; }
        Chunk_t* operator->() { return &(Chunk_t&)Chunk; }
        const Chunk_t& GetChunk()const { return (const Chunk_t&)Chunk; }
        Chunk_t& GetChunk() { return (Chunk_t&)Chunk; }

    };

    template<typename TChunkType>
    struct ChunkTreeT : public ChunkAllocationT<ChunkTreePointerT<TChunkType>>//public ChunkTreePointerT<TChunkType>, public ChunkT<TChunkType>
    {
    public:
        using Base_t = ChunkAllocationT<ChunkTreePointerT<TChunkType>>;
        using ChunkType_t = TChunkType;
        using Size_t = typename ChunkType_t::Size_t;
        using ChunkPointer_t = ChunkPointerT<ChunkType_t>;
        using Chunk_t = ChunkPointer_t;


        ChunkTreeT(const ChunkType_t* chunkType, Size_t nodeCapacity, Size_t nodeCount = 0)
            : Base_t(chunkType, nodeCapacity, nodeCount)
        {
        }

        const Chunk_t& operator*()const { return Base_t::operator*(); }
        Chunk_t& operator*() { return Base_t::operator*(); }
        const Chunk_t* operator->()const { return Base_t::operator->(); }
        Chunk_t* operator->() { return Base_t::operator->(); }
    };

    template<typename TChunkPointerElement>
    struct ChunkArrayTreePointerT : public ChunkTreePointerT<typename TChunkPointerElement::ChunkType_t>
    {
    public:
        using Base_t = ChunkTreePointerT<typename TChunkPointerElement::ChunkType_t>;
        using ChunkPointerElement_t = TChunkPointerElement;
        using ChunkType_t = typename ChunkPointerElement_t::ChunkType_t;
        using Size_t = typename ChunkPointerElement_t::Size_t;
        using ChunkArrayPointer_t = ChunkArrayPointerT<ChunkPointerElement_t>;
        using ChunkPointer_t = ChunkArrayPointer_t;
        using Chunk_t = ChunkArrayPointer_t;

    protected:
        using ChunkArrayExtention_t = ChunkArrayExtentionT<ChunkPointerElement_t>;
        ChunkArrayExtention_t Array;

    public:
        ChunkArrayTreePointerT()
            : Base_t(ChunkKind_ChunkArrayTree)
        {
        }

        ChunkArrayTreePointerT(const ChunkType_t* chunkType, void** componentData, ChunkPointerElement_t* chunks, Size_t chunkCount, Size_t capacityPerChunk)
            : Base_t(chunkType, componentData, capacityPerChunk* chunkCount, ChunkKind_ChunkArrayTree)
            , Array(chunks, chunkCount)
        {
        }

    protected:
        ChunkArrayTreePointerT(const ChunkType_t* chunkType, void** componentData, ChunkPointerElement_t* chunks, Size_t chunkCount, Size_t capacityPerChunk, ChunkKind kind)
            : Base_t(chunkType, componentData, capacityPerChunk* chunkCount, kind)
            , Array(chunks, chunkCount)
        {
        }

        ChunkArrayTreePointerT(const ChunkType_t* chunkType, Size_t capacityPerChunk, Size_t chunkCount = 0, ChunkKind kind = ChunkKind_ChunkArrayTree)
            : Base_t(chunkType, capacityPerChunk * chunkCount, kind)
            , Array(chunkCount)
        {
        }

    public:
        const Chunk_t& operator*()const { return GetChunk(); }
        Chunk_t& operator*() { return GetChunk(); }
        const Chunk_t* operator->()const { return &GetChunk(); }
        Chunk_t* operator->() { return &GetChunk(); }
        const Chunk_t& GetChunk()const { return (const ChunkArrayPointer_t&) Base_t::GetChunk(); }
        Chunk_t& GetChunk() { return (ChunkArrayPointer_t&) Base_t::GetChunk(); }
    };

    template<typename TChunkPointerElement>
    struct ChunkArrayTreeT : public ChunkArrayAllocationT< ChunkArrayTreePointerT<TChunkPointerElement>>
    {
    public:
        using Base_t = ChunkArrayAllocationT<ChunkArrayTreePointerT<TChunkPointerElement>>;
        using ChunkPointerElement_t = TChunkPointerElement;
        using ChunkType_t = typename ChunkPointerElement_t::ChunkType_t;
        using Size_t = typename ChunkPointerElement_t::Size_t;
        using ChunkArrayPointer_t = ChunkArrayPointerT<ChunkPointerElement_t>;
        using Chunk_t = ChunkArrayPointer_t;

    public:
        ChunkArrayTreeT(const ChunkType_t* chunkType, Size_t nodeCapacityPerChunk, Size_t chunkCapacity, Size_t chunkCount = 0, Size_t nodeCountPerChunk = 0)
            : Base_t(chunkType, nodeCapacityPerChunk, chunkCapacity, chunkCount, nodeCountPerChunk)
        {
        }

        const ChunkPointerElement_t& operator[](Size_t index)const { return Base_t::operator[](index); }
        ChunkPointerElement_t& operator[](Size_t index) { return Base_t::operator[](index); }
        const Chunk_t& operator*()const { return this->GetChunk(); }
        Chunk_t& operator*() { return this->GetChunk(); }
        const Chunk_t* operator->()const { return &this->GetChunk(); }
        Chunk_t* operator->() { return &this->GetChunk(); }
    };

    template<typename TChunkType>
    const IdentifiableChunkT<TChunkType>::ChunkPointer_t& IdentifiableChunkT<TChunkType>::GetChunk()const
    {
        switch (this->Kind)
        {
        case ChunkKind_Chunk:
        case ChunkKind_ChunkArray:
            return reinterpret_cast<const IdentifiableChunkPointerT<TChunkType>*>(this)->GetChunk();
        case ChunkKind_ChunkTree:
        case ChunkKind_ChunkArrayTree:
            return reinterpret_cast<const ChunkTreePointerT<TChunkType>*>(this)->GetChunk();
        }
        checkNoEntry();
        return *(ChunkPointer_t*)nullptr;
    }

    template<typename TChunkType>
    IdentifiableChunkT<TChunkType>::ChunkPointer_t& IdentifiableChunkT<TChunkType>::GetChunk()
    {
        switch (this->Kind)
        {
        case ChunkKind_Chunk:
        case ChunkKind_ChunkArray:
            return reinterpret_cast<IdentifiableChunkPointerT<TChunkType>*>(this)->GetChunk();
        case ChunkKind_ChunkTree:
        case ChunkKind_ChunkArrayTree:
            return reinterpret_cast<ChunkTreePointerT<TChunkType>*>(this)->GetChunk();
        }
        checkNoEntry();
        return *(ChunkPointer_t*)nullptr;
    }

    template<typename TChunkType>
    const IdentifiableChunkT<TChunkType>::ChunkArrayPointer_t& IdentifiableChunkT<TChunkType>::GetChunkArray()const
    {
        switch (this->Kind)
        {
        case ChunkKind_Chunk:
            checkNoEntry();
            break;
        case ChunkKind_ChunkArray:
            return reinterpret_cast<const IdentifiableChunkArrayPointerT<ChunkPointer_t>*>(this)->GetChunk();
        case ChunkKind_ChunkTree:
            checkNoEntry();
            break;
        case ChunkKind_ChunkArrayTree:
            return reinterpret_cast<const ChunkTreePointerT<TChunkType>*>(this)->GetChunk();
        }
        checkNoEntry();
        return *(ChunkArrayPointer_t*)nullptr;
    }

    template<typename TChunkType>
    IdentifiableChunkT<TChunkType>::ChunkArrayPointer_t& IdentifiableChunkT<TChunkType>::GetChunkArray()
    {
        switch (this->Kind)
        {
        case ChunkKind_Chunk:
            checkNoEntry();
            break;
        case ChunkKind_ChunkArray:
            return reinterpret_cast<IdentifiableChunkArrayPointerT<ChunkPointer_t>*>(this)->GetChunk();
        case ChunkKind_ChunkTree:
            checkNoEntry();
            break;
        case ChunkKind_ChunkArrayTree:
            return reinterpret_cast<ChunkArrayTreePointerT<ChunkPointer_t>*>(this)->GetChunk();
        }
        checkNoEntry();
        return *(ChunkArrayPointer_t*)nullptr;
    }    
}