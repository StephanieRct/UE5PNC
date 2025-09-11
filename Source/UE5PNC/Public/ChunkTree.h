// MIT License
// Copyright (c) 2025 Stephanie Rancourt

#pragma once
#include "common.h"
#include "chunk.h"
#include "Components.h"

namespace PNC 
{


    template<typename TChunkType>
    struct IdentifiableTreePointerT : IdentifiableChunk<TChunkType>
    {
    public:
        using ChunkType_t = TChunkType;
        using Base_t = IdentifiableChunk<TChunkType>;
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
    struct ChunkTreePointerT : public IdentifiableTreePointerT<TChunkType>, public ChunkPointerT<TChunkType>
    {
    public:
        using Base_t = IdentifiableTreePointerT<TChunkType>;
        using Base2_t = ChunkPointerT<TChunkType>;
        using ChunkType_t = TChunkType;
        using Size_t = typename TChunkType::Size_t;

        ChunkTreePointerT()
            :Base_t(ChunkKind_ChunkTree)
        {
        }
        ChunkTreePointerT(const ChunkType_t* chunkType, Size_t nodeCount, void** componentData)
            : Base_t(ChunkKind_ChunkTree)
            , Base2_t(chunkType, nodeCount, componentData)
        {
        }
    protected:

        ChunkTreePointerT(const ChunkType_t* chunkType, Size_t nodeCount)
            : Base_t(ChunkKind_ChunkTree)
            , Base2_t(chunkType, nodeCount)
        {
        }
    };

    template<typename TChunkType>
    struct ChunkTreeT : public ChunkAllocationT<ChunkTreePointerT<TChunkType>>//public ChunkTreePointerT<TChunkType>, public ChunkT<TChunkType>
    {
    public:
        using Base_t = ChunkAllocationT<ChunkTreePointerT<TChunkType>>;
        using ChunkType_t = TChunkType;
        using Chunk_t = ChunkT<ChunkType_t>;
        using Size_t = typename ChunkType_t::Size_t;


        ChunkTreeT(const ChunkType_t* chunkType, Size_t nodeCapacity, Size_t nodeCount = 0)
            : Base_t(chunkType, nodeCapacity, nodeCount)
        {
        }

    };


    template<typename TChunkPointerElement>
    struct ChunkArrayTreePointerT 
        : public IdentifiableTreePointerT<typename TChunkPointerElement::ChunkType_t>
        , public ChunkArrayPointerT<TChunkPointerElement>
    {
    public:
        using Base_t = IdentifiableTreePointerT<typename TChunkPointerElement::ChunkType_t>;
        using Base2_t = ChunkArrayPointerT<TChunkPointerElement>;
        using ChunkPointerElement_t = TChunkPointerElement;
        using ChunkType_t = typename TChunkPointerElement::ChunkType_t;
        using Size_t = typename TChunkPointerElement::Size_t;

        ChunkArrayTreePointerT()
            : Base_t(ChunkKind_ChunkArrayTree)
        {
        }

        ChunkArrayTreePointerT(const ChunkType_t* chunkType, void** componentData, ChunkPointerElement_t* chunks, Size_t chunkCount, Size_t capacityPerChunk)
            : Base_t(ChunkKind_ChunkArrayTree)
            , Base2_t(chunkType, componentData, chunks, chunkCount, capacityPerChunk)
        {

        }
    public:
        ChunkArrayTreePointerT(const ChunkType_t* chunkType, Size_t capacityPerChunk, Size_t chunkCount = 0)
            : Base_t(ChunkKind_ChunkArrayTree)
            , Base2_t(chunkType, capacityPerChunk, chunkCount)
        {
        }

    };

    template<typename TChunkPointerElement>
    struct ChunkArrayTreeT : public ChunkArrayAllocationT< ChunkArrayTreePointerT<TChunkPointerElement>>
    {
    public:
        using Base_t = ChunkArrayAllocationT<ChunkArrayTreePointerT<TChunkPointerElement>>;
        using ChunkPointerElement_t = TChunkPointerElement;
        using ChunkType_t = typename TChunkPointerElement::ChunkType_t;
        using ChunkArrayType_t = ChunkArrayT<TChunkPointerElement>;
        using Size_t = typename TChunkPointerElement::Size_t;


        const ChunkPointerElement_t& operator[](Size_t index)const { return Base_t::operator[](index); }
        ChunkPointerElement_t& operator[](Size_t index) { return Base_t::operator[](index); }

        ChunkArrayTreeT(const ChunkType_t* chunkType, Size_t nodeCapacityPerChunk, Size_t chunkCapacity, Size_t chunkCount = 0, Size_t nodeCountPerChunk = 0)
            : Base_t(chunkType, nodeCapacityPerChunk, chunkCapacity, chunkCount, nodeCountPerChunk)
        {
        }

    };






    //template<typename TChunkPointerBase>
    //struct ChunkTreePointerT : TChunkPointerBase
    //{
    //public:
    //    using ChunkPointerBase_t = TChunkPointerBase;
    //    using Base_t = ChunkPointerBase_t;
    //    using ChunkType_t = typename ChunkPointerBase_t::ChunkType_t;
    //    using Size_t = typename ChunkPointerBase_t::Size_t;

    //    using ChunkTreePointer_t = ChunkTreePointerT<ChunkPointerBase_t>;
    //    //using ChunkTree_t = ChunkTreeTT<TChunkBase>;
    //    using CoChunkTree_t = CoChunkTreeT<ChunkTreePointer_t>;

    //public:
    //    CoChunkTree_t Tree;
    //    ChunkTreePointer_t* GetParentChunk()const { return Tree.Parent; }
    //    ChunkTreePointer_t* GetFirstChildChunk()const { return Tree.FirstChild; }
    //    ChunkTreePointer_t* GetNextSiblingChunk()const { return Tree.NextSibling; }
    //    ChunkTreePointer_t* GetPreviousSiblingChunk()const { return Tree.PreviousSibling; }

    //    template <typename... TArgs>
    //    ChunkTreePointerT(TArgs&&... Args)
    //        :Base_t(Forward<TArgs>(Args)...)
    //    {
    //    }

    //    void MoveToFirstChild(ChunkTreePointer_t* child)
    //    {
    //        child->Extract();
    //        InsertFirstChild(child);
    //    }

    //    void MoveToLastChild(ChunkTreePointer_t* child)
    //    {
    //        child->Extract();
    //        InsertLastChild(child);
    //    }

    //    void MoveToPreviousSibling(ChunkTreePointer_t* sibling)
    //    {
    //        sibling->Extract();
    //        InsertPreviousSibling(sibling);
    //    }

    //    void MoveToNextSibling(ChunkTreePointer_t* sibling)
    //    {
    //        Tree.NextSibling->MoveToNextSibling(sibling);
    //    }

    //    void InsertFirstChild(ChunkTreePointer_t* child)
    //    {
    //        assert_pnc(child->Tree.IsExtracted());
    //        child->Tree.Parent = this;
    //        if (Tree.FirstChild == nullptr)
    //        {
    //            child->Tree.NextSibling = child;
    //            child->Tree.PreviousSibling = child;
    //            Tree.FirstChild = child;
    //        }
    //        else
    //        {
    //            Tree.FirstChild->InsertPreviousSibling(child);
    //            Tree.FirstChild = child;
    //        }
    //    }

    //    void InsertLastChild(ChunkTreePointer_t* child)
    //    {
    //        assert_pnc(child->Tree.IsExtracted());
    //        child->Tree.Parent = this;
    //        if (Tree.FirstChild == nullptr)
    //        {
    //            child->Tree.NextSibling = child;
    //            child->Tree.PreviousSibling = child;
    //            Tree.FirstChild = child;
    //        }
    //        else
    //        {
    //            Tree.FirstChild->InsertPreviousSibling(child);
    //        }
    //    }

    //    void InsertPreviousSibling(ChunkTreePointer_t* sibling)
    //    {
    //        assert_pnc(sibling->Tree.IsExtracted());
    //        auto last = Tree.PreviousSibling;
    //        last->Tree.NextSibling = sibling;
    //        sibling->Tree.PreviousSibling = last;
    //        sibling->Tree.NextSibling = Tree.FirstChild;
    //        Tree.PreviousSibling = sibling;
    //    }

    //    void InsertNextSibling(ChunkTreePointer_t* sibling)
    //    {
    //        Tree.NextSibling->InsertPreviousSibling(sibling);
    //    }

    //    void Extract()
    //    {
    //        Tree.PreviousSibling->Tree.NextSibling = Tree.NextSibling;
    //        Tree.NextSibling->Tree.PreviousSibling = Tree.PreviousSibling;
    //        Tree.PreviousSibling = nullptr;
    //        Tree.NextSibling = nullptr;
    //    }

    //};



    //template< typename TChunkBase, typename TChunkTreePointer>
    //struct ChunkTreeTT : TChunkBase
    //{
    //public:
    //    using Base_t = TChunkBase;
    //    using ChunkType_t = typename Base_t::ChunkType_t;
    //    using Size_t = typename Base_t::Size_t;
    //    using ChunkTreePointer_t = TChunkTreePointer;
    //    //using ChunkTree_t = ChunkTreeTT<TChunkBase>;
    //    using CoChunkTree_t = CoChunkTreeT<ChunkTreePointer_t>;

    //public:
    //    CoChunkTree_t Tree;
    //    ChunkTreePointer_t* GetParentChunk()const { return Tree.Parent; }
    //    ChunkTreePointer_t* GetFirstChildChunk()const { return Tree.FirstChild; }
    //    ChunkTreePointer_t* GetNextSiblingChunk()const { return Tree.NextSibling; }
    //    ChunkTreePointer_t* GetPreviousSiblingChunk()const { return Tree.PreviousSibling; }

    //    template <typename... TArgs>
    //    ChunkTreeTT(TArgs&&... Args)
    //        :Base_t(Forward<TArgs>(Args)...)
    //    {
    //    }

    //    void MoveToFirstChild(ChunkTreePointer_t* child)
    //    {
    //        child->Extract();
    //        InsertFirstChild(child);
    //    }

    //    void MoveToLastChild(ChunkTreePointer_t* child)
    //    {
    //        child->Extract();
    //        InsertLastChild(child);
    //    }

    //    void MoveToPreviousSibling(ChunkTreePointer_t* sibling)
    //    {
    //        sibling->Extract();
    //        InsertPreviousSibling(sibling);
    //    }

    //    void MoveToNextSibling(ChunkTreePointer_t* sibling)
    //    {
    //        Tree.NextSibling->MoveToNextSibling(sibling);
    //    }

    //    void InsertFirstChild(ChunkTreePointer_t* child)
    //    {
    //        assert_pnc(child->Tree.IsExtracted());
    //        child->Tree.Parent = (ChunkTreePointer_t*)this;
    //        if (Tree.FirstChild == nullptr)
    //        {
    //            child->Tree.NextSibling = child;
    //            child->Tree.PreviousSibling = child;
    //            Tree.FirstChild = child;
    //        }
    //        else
    //        {
    //            Tree.FirstChild->InsertPreviousSibling(child);
    //            Tree.FirstChild = child;
    //        }
    //    }

    //    void InsertLastChild(ChunkTreePointer_t* child)
    //    {
    //        assert_pnc(child->Tree.IsExtracted());
    //        child->Tree.Parent = (ChunkTreePointer_t*)this;
    //        if (Tree.FirstChild == nullptr)
    //        {
    //            child->Tree.NextSibling = child;
    //            child->Tree.PreviousSibling = child;
    //            Tree.FirstChild = child;
    //        }
    //        else
    //        {
    //            Tree.FirstChild->InsertPreviousSibling(child);
    //        }
    //    }

    //    void InsertPreviousSibling(ChunkTreePointer_t* sibling)
    //    {
    //        assert_pnc(sibling->Tree.IsExtracted());
    //        auto last = Tree.PreviousSibling;
    //        last->Tree.NextSibling = sibling;
    //        sibling->Tree.PreviousSibling = last;
    //        sibling->Tree.NextSibling = Tree.FirstChild;
    //        Tree.PreviousSibling = sibling;
    //    }

    //    void InsertNextSibling(ChunkTreePointer_t* sibling)
    //    {
    //        Tree.NextSibling->InsertPreviousSibling(sibling);
    //    }

    //    void Extract()
    //    {
    //        Tree.PreviousSibling->Tree.NextSibling = Tree.NextSibling;
    //        Tree.NextSibling->Tree.PreviousSibling = Tree.PreviousSibling;
    //        Tree.PreviousSibling = nullptr;
    //        Tree.NextSibling = nullptr;
    //    }

    //};


    //template< typename TChunkType, typename TSize >
    //struct ChunkTreeT : ChunkT<TChunkType>
    //{
    //public:
    //    using Base_t = ChunkT<TChunkType>;
    //    using ChunkType_t = TChunkType;
    //    using Size_t = TSize;
    //    using ChunkTree_t = ChunkTreeT<TChunkType, TSize>;
    //    using CoChunkTree_t = CoChunkTreeT<ChunkTree_t>;

    //public:
    //    CoChunkTree_t Tree;
    //    ChunkTree_t* GetParentChunk()const { return Tree.Parent; }
    //    ChunkTree_t* GetFirstChildChunk()const { return Tree.FirstChild; }
    //    ChunkTree_t* GetNextSiblingChunk()const { return Tree.NextSibling; }
    //    ChunkTree_t* GetPreviousSiblingChunk()const { return Tree.PreviousSibling; }

    //    ChunkTreeT(const ChunkType_t* chunkType, Size_t capacity, Size_t size = 0)
    //        :ChunkT<TChunkType>(chunkType, capacity, size)
    //    {
    //    }

    //    void MoveToFirstChild(ChunkTree_t* child)
    //    {
    //        child->Extract();
    //        InsertFirstChild(child);
    //    }

    //    void MoveToLastChild(ChunkTree_t* child)
    //    {
    //        child->Extract();
    //        InsertLastChild(child);
    //    }

    //    void MoveToPreviousSibling(ChunkTree_t* sibling)
    //    {
    //        sibling->Extract();
    //        InsertPreviousSibling(sibling);
    //    }

    //    void MoveToNextSibling(ChunkTree_t* sibling)
    //    {
    //        Tree.NextSibling->MoveToNextSibling(sibling);
    //    }

    //    void InsertFirstChild(ChunkTree_t* child)
    //    {
    //        assert(child->Tree.IsExtracted());
    //        child->Tree.Parent = this;
    //        if (Tree.FirstChild == nullptr)
    //        {
    //            child->Tree.NextSibling = child;
    //            child->Tree.PreviousSibling = child;
    //            Tree.FirstChild = child;
    //        }
    //        else
    //        {
    //            Tree.FirstChild->InsertPreviousSibling(child);
    //            Tree.FirstChild = child;
    //        }
    //    }

    //    void InsertLastChild(ChunkTree_t* child)
    //    {
    //        assert(child->Tree.IsExtracted());
    //        child->Tree.Parent = this;
    //        if (Tree.FirstChild == nullptr)
    //        {
    //            child->Tree.NextSibling = child;
    //            child->Tree.PreviousSibling = child;
    //            Tree.FirstChild = child;
    //        }
    //        else
    //        {
    //            Tree.FirstChild->InsertPreviousSibling(child);
    //        }
    //    }

    //    void InsertPreviousSibling(ChunkTree_t* sibling)
    //    {
    //        assert(sibling->Tree.IsExtracted());
    //        auto last = Tree.PreviousSibling;
    //        last->Tree.NextSibling = sibling;
    //        sibling->Tree.PreviousSibling = last;
    //        sibling->Tree.NextSibling = Tree.FirstChild;
    //        Tree.PreviousSibling = sibling;
    //    }

    //    void InsertNextSibling(ChunkTree_t* sibling)
    //    {
    //        Tree.NextSibling->InsertPreviousSibling(sibling);
    //    }

    //    void Extract()
    //    {
    //        Tree.PreviousSibling->Tree.NextSibling = Tree.NextSibling;
    //        Tree.NextSibling->Tree.PreviousSibling = Tree.PreviousSibling;
    //        Tree.PreviousSibling = nullptr;
    //        Tree.NextSibling = nullptr;
    //    }
    //    
    //};



    //template< typename TChunkBase, typename TChunkType, typename TSize >
    //struct ChunkArrayT : TChunkBase
    //{
    //public:
    //    using Base_t = TChunkBase;
    //    using ChunkType_t = TChunkType;
    //    using Size_t = TSize;
    //    using ChunkArray_t = ChunkArrayT<TChunkType, TSize>;


    //public:
    //    
    //    ChunkArrayT(const ChunkType_t* chunkType, Size_t chunkCount, Size_t totalCapacity, Size_t totalSize = 0)
    //        :ChunkT<TChunkType, TSize>(chunkType, capacity, size)
    //    {
    //    }

    //};
}