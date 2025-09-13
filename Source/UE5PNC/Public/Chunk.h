// MIT License
// Copyright (c) 2025 Stephanie Rancourt

#pragma once
#include "common.h"

namespace PNC 
{
    template<typename TChunkType>
    struct ChunkPointerInternalT
    {
    public:
        using ChunkType_t = TChunkType;
        using Size_t = typename ChunkType_t::Size_t;

    public:
        const ChunkType_t* Type;
        void** ComponentData;
        Size_t NodeCount;

    public:
        /// <summary>
        /// Create a null ChunkRefT without chunk type.
        /// IsNull() will evaluate to true.
        /// </summary>
        ChunkPointerInternalT()
            : Type(nullptr)
            , ComponentData(nullptr)
            , NodeCount(0)
        {
        }

        ChunkPointerInternalT(const ChunkType_t* chunkType, Size_t nodeCount, void** componentData)
            : Type(chunkType)
            , ComponentData(componentData)
            , NodeCount(nodeCount)
        {
        }

        ChunkPointerInternalT(const ChunkType_t* chunkType, Size_t nodeCount)
            : Type(chunkType)
            , ComponentData(nullptr)
            , NodeCount(nodeCount)
        {
        }

    public:
        bool IsNull()const { return this->Type == nullptr; }
        Size_t GetNodeCount()const { return this->NodeCount; }
        const ChunkType_t& GetChunkType()const { return *this->Type; }
    };

    template<typename TChunkType>
    struct ChunkPointerT : private ChunkPointerInternalT<TChunkType>
    {
    public:
        using Base_t = ChunkPointerInternalT<TChunkType>;
        using ChunkType_t = TChunkType;
        using Size_t = typename ChunkType_t::Size_t;
        using ChunkPointer_t = ChunkPointerT;
        using Chunk_t = ChunkPointerT;

    public:

        const Chunk_t& operator*()const { return *this; }
        Chunk_t& operator*() { return *this; }
        const Chunk_t* operator->()const { return this; }
        Chunk_t* operator->() { return this; }

        const Chunk_t& GetChunk()const { return *this; }
        Chunk_t& GetChunk() { return *this; }

        /// <summary>
        /// Get the size of the chunk.
        /// The size is the number of valid nodes in the chunk that can be processed by algorithms.
        /// The size can grow up to the capacity without having to reallocate the component's memory
        /// </summary>
        /// <returns>The capacity of the chunk</returns>
        Size_t GetNodeCount()const { return this->NodeCount; }

        /// <summary>
        /// Get the chunk type of this chunk
        /// </summary>
        /// <returns></returns>
        const ChunkType_t& GetChunkType()const { return *this->Type; }



        /// <summary>
        /// Test if the chunk is null and has no type nor component data.
        /// </summary>
        /// <returns></returns>
        bool IsNull()const { return this->Type == nullptr; }

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
            : Base_t()
        {
        }

        ChunkPointerT(const ChunkType_t* chunkType, Size_t nodeCount, void** componentData)
            : Base_t(chunkType, nodeCount, componentData)
        {
        }

    protected:
        ChunkPointerT(const ChunkType_t* chunkType, Size_t nodeCount)
            : Base_t(chunkType, nullptr, nodeCount)
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
            return this->ComponentData[componentTypeIndexInChunk];
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
            return this->ComponentData[componentIndexInChunk];
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
            auto index = this->Type->Components.GetComponentTypeIndexInChunk(componentType);
            if (index < 0)
                return nullptr;
            return this->GetComponentData(index);
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
            auto index = this->Type->Components.GetComponentTypeIndexInChunk(componentType);
            if (index < 0)
                return nullptr;
            return this->GetComponentData(index);
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
            return (TComponent*)this->GetComponentData(&typeid(TComponent));
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
            return (TComponent*)this->GetComponentData(&typeid(TComponent));
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
            this->ComponentData = FMemory::Malloc(this->Type->Components.GetSize() * sizeof(void*), alignof(void*));
        }

        void DeallocateComponentDataArray()
        {
            FMemory::Free(this->ComponentData);
        }

        void CopyComponentDataArray(void** componentData)
        {
            auto componentCount = this->Type->Components.GetSize();
            for (int32 i = 0; i < componentCount; ++i)
            {
                this->ComponentData[i] = componentData[i];
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
        using Internal_t = ChunkPointerInternalT<ChunkType_t>;

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
    private:
        Internal_t& GetInternalChunk() { return (Internal_t&)this->GetChunk(); }


        void AllocateData()
        {
            auto& chunk = GetInternalChunk();
            assert_pnc(!chunk.IsNull());
            auto componentCount = chunk.Type->Components.GetSize();
            for (Size_t i = 0; i < componentCount; ++i)
            {
                auto componentTypeInfo = chunk.Type->Components[i];
                chunk.ComponentData[i] = componentTypeInfo->Allocate(NodeCapacity);
            }
        }

        void AllocateDataCopy(const Self_t& o)
        {
            auto& chunk = GetInternalChunk();
            assert_pnc(!chunk.IsNull());
            assert_pnc(chunk.IsSameChunkType(chunk, o.GetChunk()));
            auto componentCount = chunk.Type->Components.GetSize();
            for (Size_t i = 0; i < componentCount; ++i)
            {
                auto componentTypeInfo = chunk.Type->Components[i];
                chunk.ComponentData[i] = componentTypeInfo->AllocateCopy(o.ComponentData[i], o.NodeCapacity, o.NodeCount);
            }
        }

        void DeallocateData()
        {
            auto& chunk = GetInternalChunk();
            if (chunk.Type == nullptr)
                return;
            auto componentCount = chunk.Type->Components.GetSize();
            for (Size_t i = 0; i < componentCount; ++i)
            {
                auto componentTypeInfo = chunk.Type->Components[i];
                componentTypeInfo->Deallocate(chunk.ComponentData[i], NodeCapacity);
            }
        }

        void AllocateComponentDataArray()
        {
            auto& chunk = GetInternalChunk();
            chunk.ComponentData = (void**)FMemory::Malloc(chunk.Type->Components.GetSize() * sizeof(void*), alignof(void*));
        }
        void DeallocateComponentDataArray()
        {
            auto& chunk = GetInternalChunk();
            if (chunk.Type == nullptr)
                return;
            FMemory::Free(chunk.ComponentData);
        }

        void CopyComponentDataArray(void** componentData)const
        {
            auto& chunk = GetInternalChunk();
            auto componentCount = chunk.Type->Components.GetSize();
            for (Size_t i = 0; i < componentCount; ++i)
            {
                chunk.ComponentData[i] = componentData[i];
            }
        }
    };

    /// <summary>
    /// Chunk of multiple nodes with the same component types (same set of component).
    /// </summary>
    /// <typeparam name="TChunkType">ChunkType type represent the structure of the chunk and what component types it has.</typeparam>
    template<typename TChunkType>
    struct ChunkT : public ChunkAllocationT<ChunkPointerT<TChunkType>>
    {
    public:
        using Base_t = ChunkPointerT<TChunkType>;
        using ChunkType_t = TChunkType;
        using Size_t = typename ChunkType_t::Size_t;
        using Chunk_t = ChunkT<ChunkType_t>;

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
    public:

        const Chunk_t& operator*()const { return *this; }
        Chunk_t& operator*() { return *this; }
        const Chunk_t* operator->()const { return this; }
        Chunk_t* operator->() { return this; }

    };

    template<typename TChunkPointerElement>
    struct ChunkArrayExtentionT 
    {
    public:
        using ChunkPointerElement_t = TChunkPointerElement;
        using ChunkType_t = typename TChunkPointerElement::ChunkType_t;
        using Size_t = typename TChunkPointerElement::Size_t;

    public:
        ChunkPointerElement_t* Chunks;
        Size_t ChunkCount;

    public:
        ChunkArrayExtentionT(Size_t chunkCount = 0)
            : Chunks(nullptr)
            , ChunkCount(chunkCount)
        {
        }

        ChunkArrayExtentionT(ChunkPointerElement_t* chunks, Size_t chunkCount)
            : Chunks(chunks)
            , ChunkCount(chunkCount)
        {
        }
    };

    template<typename TChunkPointerElement>
    struct ChunkArrayPointerInternalT : public ChunkPointerInternalT<typename TChunkPointerElement::ChunkType_t>
    {
    public:
        using Base_t = ChunkPointerInternalT<typename TChunkPointerElement::ChunkType_t>;
        using ChunkPointerElement_t = TChunkPointerElement;
        using ChunkType_t = typename TChunkPointerElement::ChunkType_t;
        using Size_t = typename ChunkType_t::Size_t;

    public:
        ChunkArrayExtentionT<ChunkPointerElement_t> Array;

        ChunkArrayPointerInternalT() = delete;
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
        //using ChunkPointer_t = ChunkArrayPointer_t;
        using ChunkArrayExtention_t = ChunkArrayExtentionT<TChunkPointerElement>;
        using Chunk_t = ChunkArrayPointer_t;

    protected:
        ChunkArrayExtention_t Array;

    public:
        ChunkArrayPointerT()
        {
        }

        //TODO change capacityPerChunk to totalNodeCount
        ChunkArrayPointerT(const ChunkType_t* chunkType, void** componentData, ChunkPointerElement_t* chunks, Size_t chunkCount, Size_t capacityPerChunk)
            : Base_t(chunkType, chunkCount* capacityPerChunk, componentData)
            , Array(chunks, chunkCount)
        {

        }

    protected:
        //TODO change capacityPerChunk to totalNodeCount
        ChunkArrayPointerT(const ChunkType_t* chunkType, Size_t capacityPerChunk, Size_t chunkCount = 0)
            : Base_t(chunkType, chunkCount * capacityPerChunk)
            , Array(chunkCount)
        {
        }

    public:
        Size_t GetChunkCount()const { return Array.ChunkCount; }

        const Chunk_t& operator*()const { return *this; }
        Chunk_t& operator*() { return *this; }
        const Chunk_t* operator->()const { return this; }
        Chunk_t* operator->() { return this; }

        const ChunkPointerElement_t& operator[](Size_t index)const { return Array.Chunks[index]; }
        ChunkPointerElement_t& operator[](Size_t index) { return Array.Chunks[index]; }

        const Chunk_t& GetChunk(Size_t index)const { return Array.Chunks[index]; }
        Chunk_t& GetChunk(Size_t index) { return Array.Chunks[index]; }

        const Chunk_t& GetChunk()const { return *this; }
        Chunk_t& GetChunk() { return *this; }

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
        using Internal_t = ChunkArrayPointerInternalT<ChunkPointerElement_t>;
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
        Internal_t& GetInternalChunk() { return (Internal_t&)this->GetChunk(); }
        void CopyChunkArray(const Self_t& o)
        {
            auto& chunk = GetInternalChunk();
            for (int i = 0; i < ChunkCapacity; ++i)
                chunk.Array.Chunks[i] = ChunkPointerElement_t(chunk.Type, o[i].GetNodeCount(), GetComponentDataForChunk(i));
        }
        void InitChunkArray(Size_t nodeCountPerChunk = 0)
        {
            auto& chunk = GetInternalChunk();
            if (chunk.IsNull())
                for (int i = 0; i < ChunkCapacity; ++i)
                    chunk.Array.Chunks[i] = ChunkPointerElement_t::Null();
            else
                for (int i = 0; i < ChunkCapacity; ++i)
                    chunk.Array.Chunks[i] = ChunkPointerElement_t(chunk.Type, nodeCountPerChunk, GetComponentDataForChunk(i));
        }

        void** GetComponentDataForChunk(Size_t chunkIndex)
        {
            auto& chunk = GetInternalChunk();
            return &chunk.ComponentData[chunkIndex * chunk.Type->Components.GetSize()];
        }

        void AllocateChunkArray()
        {
            auto& chunk = GetInternalChunk();
            chunk.Array.Chunks = (ChunkPointerElement_t*)FMemory::Malloc(ChunkCapacity * sizeof(ChunkPointerElement_t), alignof(ChunkPointerElement_t));
        }
        void DeallocateChunkArray()
        {
            FMemory::Free(GetInternalChunk().Array.Chunks);
        }
        void AllocateComponentDataArray()
        {
            auto& chunk = GetInternalChunk();
            chunk.ComponentData = (void**)FMemory::Malloc(ChunkCapacity * chunk.Type->Components.GetSize() * sizeof(void*), alignof(void*));
        }

        void DeallocateComponentDataArray()
        {
            auto& chunk = GetInternalChunk();
            FMemory::Free(chunk.ComponentData);
        }

        void AllocateData()
        {
            auto& chunk = GetInternalChunk();
            assert_pnc(!chunk.IsNull());
            auto componentCount = chunk.Type->Components.GetSize();
            auto nodeCapacityTotal = GetNodeCapacityTotal();
            for (size_t i = 0; i < componentCount; ++i)
            {
                auto componentTypeInfo = chunk.Type->Components[i];
                chunk.ComponentData[i] = componentTypeInfo->Allocate(nodeCapacityTotal, ChunkCapacity);
                for (size_t k = 1; k < ChunkCapacity; ++k)
                {
                    chunk.ComponentData[k * componentCount + i] = componentTypeInfo->Forward(chunk.ComponentData[i], NodeCapacityPerChunk);
                }
            }
        }

        void AllocateDataCopy(const Self_t& o)
        {
            auto& chunk = (Internal_t&)GetInternalChunk();
            assert_pnc(!chunk.IsNull());
            assert_pnc(chunk.IsSameChunkType(*this, o));
            auto componentCount = chunk.Type->Components.GetSize();
            auto nodeCapacityTotal = o.GetNodeCapacityTotal();
            for (size_t i = 0; i < componentCount; ++i)
            {
                auto componentTypeInfo = chunk.Type->Components[i];
                chunk.ComponentData[i] = componentTypeInfo->AllocateCopy(o.ComponentData[i], nodeCapacityTotal, o.GetChunkCount() * o.GetNodeCapacityPerChunk(), ChunkCapacity);
                for (size_t k = 1; k < ChunkCapacity; ++k)
                {
                    chunk.ComponentData[k * componentCount + i] = componentTypeInfo->Forward(chunk.ComponentData[i], NodeCapacityPerChunk);
                }
            }
        }
        void DeallocateData()
        {
            auto& chunk = GetInternalChunk();
            if (&chunk.Type == nullptr)
                return;
            auto componentCount = chunk.Type->Components.GetSize();
            auto nodeCapacityTotal = GetNodeCapacityTotal();
            for (size_t i = 0; i < componentCount; ++i)
            {
                auto componentTypeInfo = chunk.Type->Components[i];
                componentTypeInfo->Deallocate(chunk.ComponentData[i], nodeCapacityTotal, ChunkCapacity);
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
        using Chunk_t = ChunkArrayT<TChunkPointerElement>;

    public:
        const ChunkPointerElement_t& operator[](Size_t index)const { return Base_t::operator[](index); }
        ChunkPointerElement_t& operator[](Size_t index) { return Base_t::operator[](index); }

        ChunkArrayT() {}
        ChunkArrayT(const ChunkType_t* chunkType, Size_t nodeCapacityPerChunk, Size_t chunkCapacity, Size_t chunkCount = 0, Size_t nodeCountPerChunk = 0)
            : Base_t(chunkType, nodeCapacityPerChunk, chunkCapacity, chunkCount, nodeCountPerChunk)
        {
        }

        const Chunk_t& operator*()const { return *this; }
        Chunk_t& operator*() { return *this; }
        const Chunk_t* operator->()const { return this; }
        Chunk_t* operator->() { return this; }
    };

    enum ChunkKind
    {
        ChunkKind_Chunk,
        ChunkKind_ChunkArray,

        ChunkKind_ChunkTree,
        ChunkKind_ChunkArrayTree,
    };

    template<typename TChunkType>
    struct IdentifiableChunkT
    {
    public:
        using ChunkType_t = TChunkType;
        using Size_t = typename ChunkType_t::Size_t;
        using ChunkPointer_t = ChunkPointerT<TChunkType>;
        using ChunkArrayPointer_t = ChunkArrayPointerT<ChunkPointer_t>;
        using Chunk_t = ChunkPointer_t;
        using ChunkArray_t = ChunkArrayPointer_t;
    public:
        ChunkKind Kind;

    public:
        IdentifiableChunkT(ChunkKind kind)
            :Kind(kind)
        {
        }

        bool IsTree()const 
        {
            switch (Kind)
            {
            case PNC::ChunkKind_Chunk:
                return false;
            case PNC::ChunkKind_ChunkArray:
                return false;
            case PNC::ChunkKind_ChunkTree:
                return true;
            case PNC::ChunkKind_ChunkArrayTree:
                return true;
            }
        }

        bool isArray()const 
        {
            switch (Kind)
            {
            case PNC::ChunkKind_Chunk:
                return false;
            case PNC::ChunkKind_ChunkArray:
                return true;
            case PNC::ChunkKind_ChunkTree:
                return false;
            case PNC::ChunkKind_ChunkArrayTree:
                return true;
            }
        }

        const Chunk_t& operator*()const { return GetChunk(); }
        Chunk_t& operator*() { return GetChunk(); }
        const Chunk_t* operator->()const { return &GetChunk(); }
        Chunk_t* operator->() { return &GetChunk(); }

        const Chunk_t& GetChunk()const;
        Chunk_t& GetChunk();
        const ChunkArray_t& GetChunkArray()const;
        ChunkArray_t& GetChunkArray();

        //Size_t GetNodeCount()const { return GetChunk().GetNodeCount(); }
        //const ChunkType_t& GetChunkType()const { return GetChunk().GetChunkType(); }
        //bool IsNull()const { return GetChunk().IsNull(); }

        //operator ChunkPointer_t&() { return GetChunkPointer(); }
        //operator const ChunkPointer_t& ()const { return GetChunkPointer(); }
    };

    template<typename TChunkType>
    struct IdentifiableChunkPointerT : public IdentifiableChunkT<TChunkType>
    {
    public:
        using Base_t = IdentifiableChunkT<TChunkType>;
        using ChunkType_t = TChunkType;
        using Size_t = typename ChunkType_t::Size_t;
        using ChunkPointer_t = ChunkPointerT<TChunkType>;
        using Chunk_t = ChunkPointer_t;

    protected:
        ChunkPointer_t Chunk;

    public:
        IdentifiableChunkPointerT()
        {
        }

        IdentifiableChunkPointerT(const ChunkType_t* chunkType, Size_t nodeCount, void** componentData)
            : Base_t()
            , Chunk(chunkType, nodeCount, componentData)
        {
        }

    protected:
        IdentifiableChunkPointerT(const ChunkType_t* chunkType, Size_t nodeCount, void** componentData, ChunkKind kind)
            : Base_t(kind)
            , Chunk(chunkType, nodeCount, componentData)
        {
        }

        IdentifiableChunkPointerT(ChunkKind kind = ChunkKind_Chunk)
            : Base_t(kind)
            , Chunk()
        {
        }

    public:
        const Chunk_t& operator*()const { return Chunk; }
        Chunk_t& operator*() { return Chunk; }
        const Chunk_t* operator->()const { return &Chunk; }
        Chunk_t* operator->() { return &Chunk; }
        const Chunk_t& GetChunk()const { return Chunk; }
        Chunk_t& GetChunk() { return Chunk; }

        //const ChunkType_t& GetChunkType()const { return Chunk.GetChunkType(); }
    };

    template<typename TChunkPointerElement>
    struct IdentifiableChunkArrayPointerT : public IdentifiableChunkPointerT<typename TChunkPointerElement::ChunkType_t>
    {
    public:
        using Base_t = IdentifiableChunkPointerT<typename TChunkPointerElement::ChunkType_t>;
        using ChunkPointerElement_t = TChunkPointerElement;
        using ChunkType_t = typename TChunkPointerElement::ChunkType_t;
        using Size_t = typename ChunkType_t::Size_t;
        using ChunkArrayPointer_t = ChunkArrayPointerT<ChunkPointerElement_t>;
        using ChunkPointer_t = ChunkArrayPointer_t;
        using ChunkArrayExtention_t = ChunkArrayExtentionT<TChunkPointerElement>;
        using Chunk_t = ChunkArrayPointer_t;
        using ChunkArray_t = ChunkArrayPointer_t;

    protected:
        ChunkArrayExtention_t Array;

    public:
        IdentifiableChunkArrayPointerT()
        {
        }

        IdentifiableChunkArrayPointerT(const ChunkType_t* chunkType, void** componentData, ChunkPointerElement_t* chunks, Size_t chunkCount, Size_t capacityPerChunk)
            : Base_t(chunkType, chunkCount* capacityPerChunk, componentData)
            , Array(chunks, chunkCount)
        {
        }

    protected:
        IdentifiableChunkArrayPointerT(const ChunkType_t* chunkType, void** componentData, ChunkPointerElement_t* chunks, Size_t chunkCount, Size_t capacityPerChunk, ChunkKind kind)
            : Base_t(chunkType, chunkCount* capacityPerChunk, componentData, kind)
            , Array(chunks, chunkCount)
        {
        }

    public:
        const Chunk_t& operator*()const { return GetChunk(); }
        Chunk_t& operator*() { return GetChunk(); }
        const Chunk_t* operator->()const { return &GetChunk(); }
        Chunk_t* operator->() { return &GetChunk(); }
        const Chunk_t& GetChunk()const { return (const ChunkArrayPointer_t&)Base_t::GetChunk(); }
        Chunk_t& GetChunk() { return (ChunkArrayPointer_t&)Base_t::GetChunk(); }
    };

}