// MIT License
// Copyright (c) 2025 Stephanie Rancourt

#pragma once
#include "common.h"

namespace PNC 
{
    enum ChunkKind 
    {
        ChunkKind_Chunk,
        ChunkKind_ChunkArray,

        ChunkKind_ChunkTree,
        ChunkKind_ChunkArrayTree,
    };


    template<typename TChunkType>
    struct ChunkPointerT// : public IdentifiableChunk<TChunkType>
    {
    public:
        using ChunkType_t = TChunkType;
        using Size_t = typename ChunkType_t::Size_t;
        using ChunkPointer_t = ChunkPointerT;

    protected:
        const ChunkType_t* Type;
        void** ComponentData;
        Size_t NodeCount;

    public:
        /// <summary>
        /// Get the size of the chunk.
        /// The size is the number of valid nodes in the chunk that can be processed by algorithms.
        /// The size can grow up to the capacity without having to reallocate the component's memory
        /// </summary>
        /// <returns>The capacity of the chunk</returns>
        Size_t GetNodeCount()const { return NodeCount; }

        /// <summary>
        /// Get the chunk type of this chunk
        /// </summary>
        /// <returns></returns>
        const ChunkType_t& GetChunkType()const { return *Type; }

        /// <summary>
        /// Test if the chunk is null and has no type nor component data.
        /// </summary>
        /// <returns></returns>
        bool IsNull()const { return Type == nullptr; }

        /// <summary>
        /// Create a null chunk without chunk type nor component data.
        /// </summary>
        /// <returns></returns>
        static ChunkPointerT Null() { return ChunkPointerT(); }

    public:
        /// <summary>
        /// Create a null ChunkRefT without chunk type.
        /// IsNull() will evaluate to true.
        /// </summary>
        ChunkPointerT()
            : Type(nullptr)
            , ComponentData(nullptr)
            , NodeCount(0)
        {
        }

        ChunkPointerT(const ChunkType_t* chunkType, Size_t nodeCount, void** componentData)
            : Type(chunkType)
            , ComponentData(componentData)
            , NodeCount(nodeCount)
        {
        }
    protected:

        ChunkPointerT(const ChunkType_t* chunkType, Size_t nodeCount)
            : Type(chunkType)
            , ComponentData(nullptr)
            , NodeCount(nodeCount)
        {
        }
    public:
        /// <summary>
        /// Get the pointer to a component's memory array using the component type index in the ChunkType ComponentTypeSet.
        /// For components with ComponentOwner_Node, the array will be at least the length of the size of the chunk.
        /// For components with ComponentOwner_Chunk, the array will be of length 1.
        /// This is the fastest way to access the chunk's component data.
        /// </summary>
        /// <param name="componentIndexInChunk">index in the ChunkType::Components ComponentTypeSet</param>
        /// <returns>Pointer to the component memory array</returns>
        void* GetComponentData(Size_t componentTypeIndexInChunk)
        {
            assert_pnc(!IsNull());
            return ComponentData[componentTypeIndexInChunk];
        }

        /// <summary>
        /// Get the const pointer to a component's memory array using the component type index in the ChunkType ComponentTypeSet.
        /// For components with ComponentOwner_Node, the array will be at least the length of the size of the chunk.
        /// For components with ComponentOwner_Chunk, the array will be of length 1.
        /// This is the fastest way to access the chunk's component data.
        /// </summary>
        /// <param name="componentIndexInChunk">index in the ChunkType::Components ComponentTypeSet</param>
        /// <returns>Const pointer to the component memory array</returns>
        const void* GetComponentData(Size_t componentIndexInChunk)const
        {
            assert_pnc(!IsNull());
            return ComponentData[componentIndexInChunk];
        }

        /// <summary>
        /// Get the pointer to a component's memory array using the component's type const type_info* from &typeid(ComponentTypename).
        /// For components with ComponentOwner_Node, the array will be at least the length of the size of the chunk.
        /// For components with ComponentOwner_Chunk, the array will be of length 1.
        /// This is a slow way to access the chunk's component data as it require to do a map lookup of the 
        /// component's const type_info* pointer to the component type index in the chunk's ChunkType ComponentTypeSet.
        /// </summary>
        /// <param name="componentType">const type_info* pointer obtained from &typeid(ComponentTypename)</param>
        /// <returns>Pointer to the component memory array</returns>
        void* GetComponentData(const type_info* componentType)
        {
            assert_pnc(!IsNull());
            auto index = Type->Components.GetComponentTypeIndexInChunk(componentType);
            if (index < 0)
                return nullptr;
            return GetComponentData(index);
        }

        /// <summary>
        /// Get the const pointer to a component's memory array using the component's type const type_info* from &typeid(ComponentTypename).
        /// For components with ComponentOwner_Node, the array will be at least the length of the size of the chunk.
        /// For components with ComponentOwner_Chunk, the array will be of length 1.
        /// This is a slow way to access the chunk's component data as it require to do a map lookup of the 
        /// component's const type_info* pointer to the component type index in the chunk's ChunkType ComponentTypeSet.
        /// </summary>
        /// <param name="componentType">const type_info* pointer obtained from &typeid(ComponentTypename)</param>
        /// <returns>Const pointer to the component memory array</returns>
        const void* GetComponentData(const type_info* componentType)const
        {
            assert_pnc(!IsNull());
            auto index = Type->Components.GetComponentTypeIndexInChunk(componentType);
            if (index < 0)
                return nullptr;
            return GetComponentData(index);
        }


        /// <summary>
        /// Get the pointer to a component's memory array using the component's typename.
        /// For components with ComponentOwner_Node, the array will be at least the length of the size of the chunk.
        /// For components with ComponentOwner_Chunk, the array will be of length 1.
        /// This is a slow way to access the chunk's component data as it require to do a map lookup of the associated
        /// component typename const type_info* pointer to the component type index in the chunk's ChunkType ComponentTypeSet.
        /// </summary>
        /// <typeparam name="TComponent">Component typename for the desired component's memory array</typeparam>
        /// <returns>Pointer to the component memory array</returns>
        template<typename TComponent>
        TComponent* GetComponentData()
        {
            assert_pnc(!IsNull());
            return (TComponent*)GetComponentData(&typeid(TComponent));
        }

        /// <summary>
        /// Get the const pointer to a component's memory array using the component's typename.
        /// For components with ComponentOwner_Node, the array will be at least the length of the size of the chunk.
        /// For components with ComponentOwner_Chunk, the array will be of length 1.
        /// This is a slow way to access the chunk's component data as it require to do a map lookup of the associated
        /// component typename const type_info* pointer to the component type index in the chunk's ChunkType ComponentTypeSet.
        /// </summary>
        /// <typeparam name="TComponent">Component typename for the desired component's memory array</typeparam>
        /// <returns>Const Pointer to the component memory array</returns>
        template<typename TComponent>
        const TComponent* GetComponentData()const
        {
            assert_pnc(!IsNull());
            return (TComponent*)GetComponentData(&typeid(TComponent));
        }

        /// <summary>
        /// Copy data between two chunks of the same ChunkType.
        /// Will return -1 if the any chunk is null or not the same ChunkType
        /// </summary>
        /// <param name="destination">Chunk where to write the data to.</param>
        /// <param name="source">Chunk where to read the data from.</param>
        /// <returns>Number of node data copied or -1 if failed.</returns>
        static Size_t CopyData(ChunkPointerT& destination, const ChunkPointerT& source) {
            if (destination.IsNull()
                || source.IsNull()
                || destination.Type != source.Type)
                return (Size_t)-1;
            auto chunkType = destination.Type;
            auto componentCount = chunkType->Components.ComponentTypes.size();
            Size_t count = std::min(destination.GetNodeCount(), source.GetNodeCount());
            for (Size_t i = 0; i < componentCount; ++i)
            {
                auto componentTypeInfo = chunkType->Components[i];
                componentTypeInfo->Copy(destination.ComponentData[i], source.ComponentData[i], count);
            }
            destination.NodeCount = count;
            return count;
        }

        /// <summary>
        /// Test if 2 chunk have the same chunk type
        /// </summary>
        /// <param name="a"></param>
        /// <param name="b"></param>
        /// <returns></returns>
        static bool IsSameChunkType(const ChunkPointerT& a, const ChunkPointerT& b) { return a.Type == b.Type; }

    private:



        void AllocateComponentDataArray()
        {
            ComponentData = FMemory::Malloc(Type->Components.GetSize() * sizeof(void*), alignof(void*));
        }

        void DeallocateComponentDataArray()
        {
            FMemory::Free(ComponentData);
        }

        void CopyComponentDataArray(void** componentData)
        {
            auto componentCount = Type->Components.GetSize();
            for (int32 i = 0; i < componentCount; ++i)
            {
                ComponentData[i] = componentData[i];
            }
        }
    };


    template<typename TBase>
    struct ChunkAllocationT : public TBase
    {
    public:
        using Base_t = TBase;
        using ChunkType_t = typename TBase::ChunkType_t;
        using Size_t = typename TBase::Size_t;
        using Self_t = ChunkAllocationT<TBase>;

    private:
        Size_t NodeCapacity;

    public:
        /// <summary>
        /// Get the capacity of the chunk.
        /// The capacity is the max size the chunk can be without reallocating the component's memory.
        /// Algorithms will only process nodes up to the chunk's size.
        /// </summary>
        /// <returns>The capacity of the chunk</returns>
        Size_t GetNodeCapacity()const { return NodeCapacity; }

        /// <summary>
        /// Create a null chunk without chunk type nor component data.
        /// </summary>
        /// <returns></returns>
        static Self_t Null() { return Self_t(); }

    public:
        /// <summary>
        /// Create a null chunk without chunk type.
        /// IsNull() will evaluate to true.
        /// </summary>
        ChunkAllocationT()
            : Base_t()
            , NodeCapacity(0)
        {
        }

        /// <summary>
        /// Deep copy a chunk and its component data.
        /// The result chunk will have the same size and capacity as the original.
        /// </summary>
        /// <param name="o"></param>
        ChunkAllocationT(const Self_t& o)
            : Base_t(o)
            , NodeCapacity(o.NodeCapacity)
        {
            if (o.IsNull())
                return;
            AllocateComponentDataArray();
            AllocateDataCopy(o);
        }

        /// <summary>
        /// Deep copy a chunk and its component data.
        /// It will deallocate any previous chunk data.
        /// The result chunk will have the same size and capacity as the original.
        /// </summary>
        /// <param name="o"></param>
        Self_t& operator=(const Self_t& o)
        {
            if (this == &o)
                return *this;
            DeallocateData();
            DeallocateComponentDataArray();
            Base_t::operator=(o);
            NodeCapacity = o.NodeCapacity;
            if (o.IsNull())
                return *this;
            AllocateComponentDataArray();
            AllocateDataCopy(o);
            return *this;
        }

        /// <summary>
        /// Create a chunk of a given chunk type and allocate the component's memory
        /// The components' memory can fit as many instances of each component as the chunk's capacity.
        /// Any computation perform in this chunk will only process node within the chunk's size and not it's capacity.
        /// </summary>
        /// <param name="chunkType"></param>
        /// <param name="nodeCapacity"></param>
        /// <param name="nodeCount"></param>
        ChunkAllocationT(const ChunkType_t* chunkType, Size_t nodeCapacity, Size_t nodeCount = 0)
            : Base_t(chunkType, nodeCount)
            , NodeCapacity(nodeCapacity)
        {
            AllocateComponentDataArray();
            AllocateData();
        }

        /// <summary>
        /// Deallocate data if not a null chunk
        /// </summary>
        ~ChunkAllocationT()
        {
            DeallocateData();
            DeallocateComponentDataArray();
        }

    public:
        //Chunk_t SubChunk(Size_t first, Size_t capacity, Size_t size)const
        //{
        //    assert(capacity >= size);
        //    assert(first + capacity < Capacity);
        //    Chunk_t subChunk;
        //    subChunk.Type = Type;
        //    subChunk.Capacity = capacity;
        //    subChunk.Size = size;
        //    subChunk.ComponentData = ComponentData;
        //    subChunk.IsSubChunk = true;
        //    for (int i = 0; i < subChunk.ComponentData.size(); ++i)
        //    {
        //        subChunk.ComponentData[i] = Type->Components[i]->SubChunk(ComponentData[i], first);
        //    }
        //    return subChunk;
        //dd

    private:


        void AllocateData()
        {
            assert_pnc(!this->IsNull());
            auto componentCount = this->Type->Components.GetSize();
            for (Size_t i = 0; i < componentCount; ++i)
            {
                auto componentTypeInfo = this->Type->Components[i];
                this->ComponentData[i] = componentTypeInfo->Allocate(NodeCapacity);
            }
        }

        void AllocateDataCopy(const Self_t& o)
        {
            assert_pnc(!this->IsNull());
            assert_pnc(this->IsSameChunkType(*this, o));
            auto componentCount = this->Type->Components.GetSize();
            for (Size_t i = 0; i < componentCount; ++i)
            {
                auto componentTypeInfo = this->Type->Components[i];
                this->ComponentData[i] = componentTypeInfo->AllocateCopy(o.ComponentData[i], o.NodeCapacity, o.NodeCount);
            }
        }

        void DeallocateData()
        {
            if (this->Type == nullptr)
                return;
            auto componentCount = this->Type->Components.GetSize();
            for (Size_t i = 0; i < componentCount; ++i)
            {
                auto componentTypeInfo = this->Type->Components[i];
                componentTypeInfo->Deallocate(this->ComponentData[i], NodeCapacity);
            }
        }

        void AllocateComponentDataArray()
        {
            this->ComponentData = (void**)FMemory::Malloc(this->Type->Components.GetSize() * sizeof(void*), alignof(void*));
        }
        void DeallocateComponentDataArray()
        {
            if (this->Type == nullptr)
                return;
            FMemory::Free(this->ComponentData);
        }

        void CopyComponentDataArray(void** componentData)const
        {
            auto componentCount = this->Type->Components.GetSize();
            for (Size_t i = 0; i < componentCount; ++i)
            {
                this->ComponentData[i] = componentData[i];
            }
        }
    };


    /// <summary>
    /// Chunk of multiple nodes with the same component types (same set of component).
    /// </summary>
    /// <typeparam name="TChunkType">ChunkType type represent the structure of the chunk and what component types it has.</typeparam>
    /// <typeparam name="TSize">Size unit for indexing nodes in the chunk</typeparam>
    template<typename TChunkType>
    struct ChunkT : public ChunkAllocationT<ChunkPointerT<TChunkType>>
    {
    public:
        using Base_t = ChunkPointerT<TChunkType>;
        using ChunkType_t = TChunkType;
        using Size_t = typename ChunkType_t::Size_t;
        using Chunk_t = ChunkT;

    public:
        /// <summary>
        /// Create a null chunk without chunk type nor component data.
        /// </summary>
        /// <returns></returns>
        static Chunk_t Null() { return Chunk_t(); }

    public:
        /// <summary>
        /// Create a null chunk without chunk type.
        /// IsNull() will evaluate to true.
        /// </summary>
        ChunkT()
            : Base_t()
        {
        }

        /// <summary>
        /// Create a chunk of a given chunk type and allocate the component's memory
        /// The components' memory can fit as many instances of each component as the chunk's capacity.
        /// Any computation perform in this chunk will only process node within the chunk's size and not it's capacity.
        /// </summary>
        /// <param name="chunkType"></param>
        /// <param name="nodeCapacity"></param>
        /// <param name="nodeCount"></param>
        ChunkT(const ChunkType_t* chunkType, Size_t nodeCapacity, Size_t nodeCount = 0)
            : Base_t(chunkType, nodeCapacity, nodeCount)
        {
        }


    };



    template<typename TChunkPointerElement>
    struct ChunkArrayPointerT : public ChunkPointerT<typename TChunkPointerElement::ChunkType_t>
    {
    public:
        using ChunkType_t = typename TChunkPointerElement::ChunkType_t;
        using Size_t = typename TChunkPointerElement::Size_t;
        using ChunkArrayPointer_t = ChunkArrayPointerT<TChunkPointerElement>;
        using Base_t = ChunkPointerT<ChunkType_t>;
        using ChunkPointerElement_t = TChunkPointerElement;

    protected:
        ChunkPointerElement_t* Chunks;
        Size_t ChunkCount;

    public:

        Size_t GetChunkCount()const { return ChunkCount; }

        ChunkArrayPointerT(const ChunkType_t* chunkType, void** componentData, ChunkPointerElement_t* chunks, Size_t chunkCount, Size_t capacityPerChunk)
            : Base_t(chunkType, chunkCount* capacityPerChunk, componentData)
            , Chunks(chunks)
            , ChunkCount(chunkCount)
        {

        }
    protected:

        ChunkArrayPointerT(const ChunkType_t* chunkType, Size_t capacityPerChunk, Size_t chunkCount = 0)
            : Base_t(chunkType, chunkCount * capacityPerChunk)
            , Chunks(nullptr)
            , ChunkCount(chunkCount)
        {
        }
    public:
        const ChunkPointerElement_t& operator[](Size_t index)const
        {
            return Chunks[index];
        }
        ChunkPointerElement_t& operator[](Size_t index)
        {
            return Chunks[index];
        }


    };


    template<typename TBase>
    struct ChunkArrayAllocationT : public TBase
    {
        // TODO specialization when TChunkPointer is a ChunkArrayPointerT
    public:
        using Base_t = TBase;
        using ChunkType_t = typename TBase::ChunkType_t;
        using Size_t = typename TBase::Size_t;

        using ChunkPointerElement_t = typename TBase::ChunkPointerElement_t;
        using Self_t = ChunkArrayAllocationT<TBase>;
        //using ChunkArray_t = ChunkArrayT<TChunkPointerElement>;

    protected:
        Size_t NodeCapacityPerChunk;
        Size_t ChunkCapacity;

    public:
        Size_t GetNodeCapacityTotal()const { return NodeCapacityPerChunk * ChunkCapacity; }
        Size_t GetNodeCapacityPerChunk()const { return NodeCapacityPerChunk; }
        Size_t GetChunkCapacity()const { return ChunkCapacity; }
        const ChunkPointerElement_t& operator[](Size_t index)const { return Base_t::operator[](index); }
        ChunkPointerElement_t& operator[](Size_t index) { return Base_t::operator[](index); }

        ChunkArrayAllocationT()
            : NodeCapacityPerChunk(0)
            , ChunkCapacity(0)
        {
        }
        ChunkArrayAllocationT(const ChunkType_t* chunkType, Size_t nodeCapacityPerChunk, Size_t chunkCapacity, Size_t chunkCount = 0, Size_t nodeCountPerChunk = 0)
            : Base_t(chunkType, nodeCapacityPerChunk, chunkCount)
            , NodeCapacityPerChunk(nodeCapacityPerChunk)
            , ChunkCapacity(chunkCapacity)
        {
            AllocateComponentDataArray();
            AllocateData();
            AllocateChunkArray();
            InitChunkArray(nodeCountPerChunk);
        }

        ChunkArrayAllocationT(const ChunkArrayAllocationT& o)
            : Base_t(o)
            , NodeCapacityPerChunk(o.NodeCapacityPerChunk)
            , ChunkCapacity(o.ChunkCapacity)
        {
            if (o.IsNull())
                return;
            AllocateComponentDataArray();
            AllocateDataCopy(o);
            AllocateChunkArray();
            CopyChunkArray(o);
        }

        Self_t& operator=(const Self_t& o)
        {
            if (this == &o)
                return *this;
            DeallocateData();
            DeallocateComponentDataArray();
            Base_t::operator=(o);
            NodeCapacityPerChunk = o.NodeCapacityPerChunk;
            if (o.IsNull())
                return;
            AllocateComponentDataArray();
            AllocateDataCopy(o);
            AllocateChunkArray();
            CopyChunkArray(o);
            return *this;
        }

        ~ChunkArrayAllocationT()
        {
            DeallocateChunkArray();
            DeallocateData();
            DeallocateComponentDataArray();
        }
    protected:

        void CopyChunkArray(const Self_t& o)
        {
            for (int i = 0; i < ChunkCapacity; ++i)
                this->Chunks[i] = ChunkPointerElement_t(this->Type, o[i].GetNodeCount(), GetComponentDataForChunk(i));
        }
        void InitChunkArray(Size_t nodeCountPerChunk = 0)
        {
            if (this->IsNull())
                for (int i = 0; i < ChunkCapacity; ++i)
                    this->Chunks[i] = ChunkPointerElement_t::Null();
            else
                for (int i = 0; i < ChunkCapacity; ++i)
                    this->Chunks[i] = ChunkPointerElement_t(this->Type, nodeCountPerChunk, GetComponentDataForChunk(i));
        }

        void** GetComponentDataForChunk(Size_t chunkIndex)
        {
            return &this->ComponentData[chunkIndex * this->Type->Components.GetSize()];
        }

        void AllocateChunkArray()
        {
            this->Chunks = (ChunkPointerElement_t*)FMemory::Malloc(this->ChunkCapacity * sizeof(ChunkPointerElement_t), alignof(ChunkPointerElement_t));
        }
        void DeallocateChunkArray()
        {
            FMemory::Free(this->Chunks);
        }
        void AllocateComponentDataArray()
        {
            this->ComponentData = (void**)FMemory::Malloc(this->ChunkCapacity * this->Type->Components.GetSize() * sizeof(void*), alignof(void*));
        }

        void DeallocateComponentDataArray()
        {
            FMemory::Free(this->ComponentData);
        }

        void AllocateData()
        {
            assert_pnc(!this->IsNull());
            auto componentCount = this->Type->Components.GetSize();
            auto nodeCapacityTotal = GetNodeCapacityTotal();
            for (size_t i = 0; i < componentCount; ++i)
            {
                auto componentTypeInfo = this->Type->Components[i];
                this->ComponentData[i] = componentTypeInfo->Allocate(nodeCapacityTotal, ChunkCapacity);
                for (size_t k = 1; k < ChunkCapacity; ++k)
                {
                    this->ComponentData[k * componentCount + i] = componentTypeInfo->Forward(this->ComponentData[i], NodeCapacityPerChunk);
                }
            }
        }

        void AllocateDataCopy(const Self_t& o)
        {
            assert_pnc(!this->IsNull());
            assert_pnc(this->IsSameChunkType(*this, o));
            auto componentCount = this->Type->Components.GetSize();
            auto nodeCapacityTotal = o.GetNodeCapacityTotal();
            for (size_t i = 0; i < componentCount; ++i)
            {
                auto componentTypeInfo = this->Type->Components[i];
                this->ComponentData[i] = componentTypeInfo->AllocateCopy(o.ComponentData[i], nodeCapacityTotal, o.GetChunkCount() * o.GetNodeCapacityPerChunk(), ChunkCapacity);
                for (size_t k = 1; k < ChunkCapacity; ++k)
                {
                    this->ComponentData[k * componentCount + i] = componentTypeInfo->Forward(this->ComponentData[i], NodeCapacityPerChunk);
                }
            }
        }

        void DeallocateData()
        {
            if (this->Type == nullptr)
                return;
            auto componentCount = this->Type->Components.GetSize();
            auto nodeCapacityTotal = GetNodeCapacityTotal();
            for (size_t i = 0; i < componentCount; ++i)
            {
                auto componentTypeInfo = this->Type->Components[i];
                componentTypeInfo->Deallocate(this->ComponentData[i], nodeCapacityTotal, ChunkCapacity);
            }
        }

    };


    template<typename TChunkPointerElement>
    struct ChunkArrayT : public ChunkArrayAllocationT<ChunkArrayPointerT<TChunkPointerElement>>
    {
        // TODO specialization when TChunkPointer is a ChunkArrayPointerT
    public:
        using Base_t = ChunkArrayAllocationT<ChunkArrayPointerT<TChunkPointerElement>>;
        using ChunkPointerElement_t = TChunkPointerElement;
        using ChunkType_t = typename TChunkPointerElement::ChunkType_t;
        using Size_t = typename TChunkPointerElement::Size_t;
        using ChunkArray_t = ChunkArrayT<TChunkPointerElement>;

    public:
        const ChunkPointerElement_t& operator[](Size_t index)const { return Base_t::operator[](index); }
        ChunkPointerElement_t& operator[](Size_t index) { return Base_t::operator[](index); }

        ChunkArrayT() {}
        ChunkArrayT(const ChunkType_t* chunkType, Size_t nodeCapacityPerChunk, Size_t chunkCapacity, Size_t chunkCount = 0, Size_t nodeCountPerChunk = 0)
            : Base_t(chunkType, nodeCapacityPerChunk, chunkCapacity, chunkCount, nodeCountPerChunk)
        {
        }

    };








    template<typename TChunkType>
    struct IdentifiableChunk
    {
    public:
        using ChunkType_t = TChunkType;
        using Size_t = typename ChunkType_t::Size_t;
        ChunkKind Kind;
        IdentifiableChunk(ChunkKind kind)
            :Kind(kind)
        {
        }
    };
    template<typename TChunkType>
    struct IdentifiableChunkPointerT : public IdentifiableChunk<TChunkType>, public ChunkPointerT<TChunkType>
    {
    public:
        using base_t = IdentifiableChunk<TChunkType>;
        using base2_t = ChunkPointerT<TChunkType>;
        using ChunkType_t = TChunkType;
        using Size_t = typename ChunkType_t::Size_t;
        using ChunkPointer_t = ChunkPointerT;

    public:
        /// <summary>
        /// Create a null ChunkRefT without chunk type.
        /// IsNull() will evaluate to true.
        /// </summary>
        IdentifiableChunkPointerT(ChunkKind kind = ChunkKind_Chunk)
            : base_t(kind)
            , base2_t()
        {
        }

        IdentifiableChunkPointerT(const ChunkType_t* chunkType, Size_t nodeCount, void** componentData, ChunkKind kind = ChunkKind_ChunkTree)
            : base_t(kind)
            , base2_t(chunkType, nodeCount, componentData)
        {
        }
    };





    ///// <summary>
    ///// Chunk of multiple nodes with the same component types (same set of component).
    ///// </summary>
    ///// <typeparam name="TChunkType">ChunkType type represent the structure of the chunk and what component types it has.</typeparam>
    ///// <typeparam name="TSize">Size unit for indexing nodes in the chunk</typeparam>
    //template<typename TChunkType>
    //struct ChunkT : public ChunkPointerT<TChunkType>
    //{
    //public:
    //    using Base_t = ChunkPointerT<TChunkType>;
    //    using ChunkType_t = TChunkType;
    //    using Size_t = typename ChunkType_t::Size_t;
    //    using Chunk_t = ChunkT;

    //private:
    //    Size_t NodeCapacity;

    //public:
    //    /// <summary>
    //    /// Get the capacity of the chunk.
    //    /// The capacity is the max size the chunk can be without reallocating the component's memory.
    //    /// Algorithms will only process nodes up to the chunk's size.
    //    /// </summary>
    //    /// <returns>The capacity of the chunk</returns>
    //    Size_t GetNodeCapacity()const { return NodeCapacity; }

    //    /// <summary>
    //    /// Create a null chunk without chunk type nor component data.
    //    /// </summary>
    //    /// <returns></returns>
    //    static Chunk_t Null() { return Chunk_t(); }

    //public:
    //    /// <summary>
    //    /// Create a null chunk without chunk type.
    //    /// IsNull() will evaluate to true.
    //    /// </summary>
    //    ChunkT()
    //        : Base_t()
    //        , NodeCapacity(0)
    //    {
    //    }

    //    /// <summary>
    //    /// Deep copy a chunk and its component data.
    //    /// The result chunk will have the same size and capacity as the original.
    //    /// </summary>
    //    /// <param name="o"></param>
    //    ChunkT(const ChunkT& o)
    //        : Base_t(o)
    //        , NodeCapacity(o.NodeCapacity)
    //    {
    //        if (o.IsNull())
    //            return;
    //        AllocateComponentDataArray();
    //        AllocateDataCopy(o);
    //    }

    //    /// <summary>
    //    /// Deep copy a chunk and its component data.
    //    /// It will deallocate any previous chunk data.
    //    /// The result chunk will have the same size and capacity as the original.
    //    /// </summary>
    //    /// <param name="o"></param>
    //    ChunkT& operator=(const ChunkT& o) 
    //    {
    //        if (this == &o)
    //            return *this;
    //        DeallocateData();
    //        DeallocateComponentDataArray();
    //        Base_t::operator=(o);
    //        NodeCapacity = o.NodeCapacity;
    //        if (o.IsNull())
    //            return *this;
    //        AllocateComponentDataArray();
    //        AllocateDataCopy(o);
    //        return *this;
    //    }

    //    /// <summary>
    //    /// Create a chunk of a given chunk type and allocate the component's memory
    //    /// The components' memory can fit as many instances of each component as the chunk's capacity.
    //    /// Any computation perform in this chunk will only process node within the chunk's size and not it's capacity.
    //    /// </summary>
    //    /// <param name="chunkType"></param>
    //    /// <param name="capacity"></param>
    //    /// <param name="size"></param>
    //    ChunkT(const ChunkType_t* chunkType, Size_t capacity, Size_t size = 0)
    //        : Base_t(chunkType, size)
    //        , NodeCapacity(capacity)
    //    {
    //        AllocateComponentDataArray();
    //        AllocateData();
    //    }

    //    /// <summary>
    //    /// Deallocate data if not a null chunk
    //    /// </summary>
    //    ~ChunkT() 
    //    {
    //        DeallocateData();
    //        DeallocateComponentDataArray();
    //    }

    //public:
    //    //Chunk_t SubChunk(Size_t first, Size_t capacity, Size_t size)const
    //    //{
    //    //    assert(capacity >= size);
    //    //    assert(first + capacity < Capacity);
    //    //    Chunk_t subChunk;
    //    //    subChunk.Type = Type;
    //    //    subChunk.Capacity = capacity;
    //    //    subChunk.Size = size;
    //    //    subChunk.ComponentData = ComponentData;
    //    //    subChunk.IsSubChunk = true;
    //    //    for (int i = 0; i < subChunk.ComponentData.size(); ++i)
    //    //    {
    //    //        subChunk.ComponentData[i] = Type->Components[i]->SubChunk(ComponentData[i], first);
    //    //    }
    //    //    return subChunk;
    //    //dd

    //private:


    //    void AllocateData() 
    //    {
    //        assert_pnc(!this->IsNull());
    //        auto componentCount = this->Type->Components.GetSize();
    //        for (size_t i = 0; i < componentCount; ++i) 
    //        {
    //            auto componentTypeInfo = this->Type->Components[i];
    //            this->ComponentData[i] = componentTypeInfo->Allocate(NodeCapacity);
    //        }
    //    }

    //    void AllocateDataCopy(const ChunkT& o) 
    //    {
    //        assert_pnc(!this->IsNull());
    //        assert_pnc(this->IsSameChunkType(*this, o));
    //        auto componentCount = this->Type->Components.GetSize();
    //        for (size_t i = 0; i < componentCount; ++i) 
    //        {
    //            auto componentTypeInfo = this->Type->Components[i];
    //            this->ComponentData[i] = componentTypeInfo->AllocateCopy(o.ComponentData[i], o.NodeCapacity, o.NodeCount);
    //        }
    //    }

    //    void DeallocateData() 
    //    {
    //        if (this->Type == nullptr)
    //            return;
    //        auto componentCount = this->Type->Components.GetSize();
    //        for (size_t i = 0; i < componentCount; ++i) 
    //        {
    //            auto componentTypeInfo = this->Type->Components[i];
    //            componentTypeInfo->Deallocate(this->ComponentData[i], NodeCapacity);
    //        }
    //    }

    //    void AllocateComponentDataArray()
    //    {
    //        this->ComponentData = (void**)FMemory::Malloc(this->Type->Components.GetSize() * sizeof(void*), alignof(void*));
    //    }
    //    void DeallocateComponentDataArray()
    //    {
    //        if (this->Type == nullptr)
    //            return;
    //        FMemory::Free(this->ComponentData);
    //    }

    //    void CopyComponentDataArray(void** componentData)const
    //    {
    //        auto componentCount = this->Type->Components.GetSize();
    //        for (int32 i = 0; i < componentCount; ++i)
    //        {
    //            this->ComponentData[i] = componentData[i];
    //        }
    //    }
    //};
//
//
//template<typename TChunkPointerElement>
//struct ChunkArrayT : public ChunkArrayPointerT<TChunkPointerElement>
//{
//    // TODO specialization when TChunkPointer is a ChunkArrayPointerT
//public:
//    using ChunkPointerElement_t = TChunkPointerElement;
//    using ChunkType_t = typename TChunkPointerElement::ChunkType_t;
//    using Size_t = typename TChunkPointerElement::Size_t;
//    using Base_t = ChunkArrayPointerT<TChunkPointerElement>;
//    //using ChunkArrayPointer_t = ChunkArrayPointerT;
//    //using ChunkPointer_t = ChunkPointerT<TChunkType>;
//    using ChunkArray_t = ChunkArrayT<TChunkPointerElement>;
//
//protected:
//    Size_t NodeCapacityPerChunk;
//    Size_t ChunkCapacity;
//
//public:
//    Size_t GetNodeCapacityTotal()const { return NodeCapacityPerChunk * ChunkCapacity; }
//    Size_t GetNodeCapacityPerChunk()const { return NodeCapacityPerChunk; }
//    Size_t GetChunkCapacity()const { return ChunkCapacity; }
//    const ChunkPointerElement_t& operator[](Size_t index)const { return Base_t::operator[](index); }
//    ChunkPointerElement_t& operator[](Size_t index) { return Base_t::operator[](index); }
//
//    ChunkArrayT(const ChunkType_t* chunkType, Size_t nodeCapacityPerChunk, Size_t chunkCapacity, Size_t chunkCount = 0, Size_t nodeCountPerChunk = 0)
//        : Base_t(chunkType, nodeCapacityPerChunk, chunkCount)
//        , NodeCapacityPerChunk(nodeCapacityPerChunk)
//        , ChunkCapacity(chunkCapacity)
//    {
//        AllocateComponentDataArray();
//        AllocateData();
//        AllocateChunkArray();
//        InitChunkArray(nodeCountPerChunk);
//    }
//
//    ChunkArrayT(const ChunkArrayT& o)
//        : Base_t(o)
//        , NodeCapacityPerChunk(o.NodeCapacityPerChunk)
//        , ChunkCapacity(o.ChunkCapacity)
//    {
//        if (o.IsNull())
//            return;
//        AllocateComponentDataArray();
//        AllocateDataCopy(o);
//        AllocateChunkArray();
//        CopyChunkArray(o);
//    }
//
//    ChunkArrayT& operator=(const ChunkArrayT& o)
//    {
//        if (this == &o)
//            return *this;
//        DeallocateData();
//        DeallocateComponentDataArray();
//        Base_t::operator=(o);
//        NodeCapacityPerChunk = o.NodeCapacityPerChunk;
//        if (o.IsNull())
//            return;
//        AllocateComponentDataArray();
//        AllocateDataCopy(o);
//        AllocateChunkArray();
//        CopyChunkArray(o);
//        return *this;
//    }
//
//    ~ChunkArrayT()
//    {
//        DeallocateChunkArray();
//        DeallocateData();
//        DeallocateComponentDataArray();
//    }
//protected:
//
//    void CopyChunkArray(const ChunkArrayT& o)
//    {
//        for (int i = 0; i < ChunkCapacity; ++i)
//            this->Chunks[i] = ChunkPointerElement_t(this->Type, o[i].GetNodeCount(), GetComponentDataForChunk(i));
//    }
//    void InitChunkArray(Size_t nodeCountPerChunk = 0)
//    {
//        if (this->IsNull())
//            for (int i = 0; i < ChunkCapacity; ++i)
//                this->Chunks[i] = ChunkPointerElement_t::Null();
//        else
//            for (int i = 0; i < ChunkCapacity; ++i)
//                this->Chunks[i] = ChunkPointerElement_t(this->Type, nodeCountPerChunk, GetComponentDataForChunk(i));
//    }
//
//    void** GetComponentDataForChunk(Size_t chunkIndex)
//    {
//        return &this->ComponentData[chunkIndex * this->Type->Components.GetSize()];
//    }
//
//    void AllocateChunkArray()
//    {
//        this->Chunks = (ChunkPointerElement_t*)FMemory::Malloc(this->ChunkCapacity * sizeof(ChunkPointerElement_t), alignof(ChunkPointerElement_t));
//    }
//    void DeallocateChunkArray()
//    {
//        FMemory::Free(this->Chunks);
//    }
//    void AllocateComponentDataArray()
//    {
//        this->ComponentData = (void**)FMemory::Malloc(this->ChunkCapacity * this->Type->Components.GetSize() * sizeof(void*), alignof(void*));
//    }
//
//    void DeallocateComponentDataArray()
//    {
//        FMemory::Free(this->ComponentData);
//    }
//
//    void AllocateData()
//    {
//        assert_pnc(!this->IsNull());
//        auto componentCount = this->Type->Components.GetSize();
//        auto nodeCapacityTotal = GetNodeCapacityTotal();
//        for (size_t i = 0; i < componentCount; ++i)
//        {
//            auto componentTypeInfo = this->Type->Components[i];
//            this->ComponentData[i] = componentTypeInfo->Allocate(nodeCapacityTotal, ChunkCapacity);
//            for (size_t k = 1; k < ChunkCapacity; ++k)
//            {
//                this->ComponentData[k * componentCount + i] = componentTypeInfo->Forward(this->ComponentData[i], NodeCapacityPerChunk);
//            }
//        }
//    }
//
//    void AllocateDataCopy(const ChunkArray_t& o)
//    {
//        assert_pnc(!this->IsNull());
//        assert_pnc(IsSameChunkType(*this, o));
//        auto componentCount = this->Type->Components.GetSize();
//        auto nodeCapacityTotal = o.GetNodeCapacityTotal();
//        for (size_t i = 0; i < componentCount; ++i)
//        {
//            auto componentTypeInfo = this->Type->Components[i];
//            this->ComponentData[i] = componentTypeInfo->AllocateCopy(o.ComponentData[i], nodeCapacityTotal, o.GetChunkCount() * o.GetNodeCapacityPerChunk(), ChunkCapacity);
//            for (size_t k = 1; k < ChunkCapacity; ++k)
//            {
//                this->ComponentData[k * componentCount + i] = componentTypeInfo->Forward(this->ComponentData[i], NodeCapacityPerChunk);
//            }
//        }
//    }
//
//    void DeallocateData()
//    {
//        if (this->Type == nullptr)
//            return;
//        auto componentCount = this->Type->Components.GetSize();
//        auto nodeCapacityTotal = GetNodeCapacityTotal();
//        for (size_t i = 0; i < componentCount; ++i)
//        {
//            auto componentTypeInfo = this->Type->Components[i];
//            componentTypeInfo->Deallocate(this->ComponentData[i], nodeCapacityTotal, ChunkCapacity);
//        }
//    }
//
//};
//

    //template<typename TChunk, typename TChunkType, typename TSize>
    //struct MultiChunk 
    //{
    //public:
    //    using Chunk_t = TChunk;
    //    using ChunkType_t = TChunkType;
    //    using Size_t = TSize;

    //    std::vector<Chunk_t> Chunks;

    //    MultiChunk(const ChunkType_t* chunkType, Size_t capacityChunks, Size_t capacityNodes)
    //        : Chunks(capacityChunks)
    //    {

    //    }

    //    Chunk_t AddChunk(Size_t capacity, Size_t size = 0)
    //    {

    //    }


    //protected:

    //    //bool IsDataAllocated()const { return ComponentData.size() > 0 && ComponentData[0] != nullptr; }

    //    //bool IsValid()const { return !IsNull() && IsDataAllocated(); }
    //    //void AllocateData()
    //    //{
    //    //    check(!IsNull());
    //    //    check(!IsDataAllocated());
    //    //    auto componentCount = Type->Components.GetSize();
    //    //    std::vector<void*> componentsData(componentCount);
    //    //    for (size_t i = 0; i < componentCount; ++i)
    //    //    {
    //    //        auto componentTypeInfo = Type->Components[i];
    //    //        ComponentData[i] = componentTypeInfo->Allocate(Capacity);
    //    //    }
    //    //}
    //};
}