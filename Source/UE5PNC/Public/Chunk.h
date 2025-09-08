// MIT License
// Copyright (c) 2025 Stephanie Rancourt

#pragma once
#include "common.h"

namespace PNC 
{

    template<typename TChunkType, typename TSize>
    struct ChunkPointerT
    {
    public:
        using ChunkType_t = TChunkType;
        using Size_t = TSize;
        using ChunkPointer_t = ChunkPointerT;

    protected:
        const ChunkType_t* Type;
        void** ComponentData;
        Size_t Size;

    public:
        /// <summary>
        /// Get the size of the chunk.
        /// The size is the number of valid nodes in the chunk that can be processed by algorithms.
        /// The size can grow up to the capacity without having to reallocate the component's memory
        /// </summary>
        /// <returns>The capacity of the chunk</returns>
        Size_t GetSize()const { return Size; }

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
        /// Size() and Capacity() will evaluate to 0
        /// </summary>
        ChunkPointerT()
            : Type(nullptr)
            , Size(0)
            , ComponentData(nullptr)
        {
        }

        ChunkPointerT(const ChunkType_t* chunkType, Size_t size, void** componentData)
            : Type(chunkType)
            , Size(size)
            , ComponentData(componentData)
        {
        }
    protected:

        ChunkPointerT(const ChunkType_t* chunkType, Size_t size)
            : Type(chunkType)
            , Size(size)
            , ComponentData(nullptr)
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
            assert(!IsNull());
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
            assert(!IsNull());
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
            assert(!IsNull());
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
            assert(!IsNull());
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
            assert(!IsNull());
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
            assert(!IsNull());
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
            Size_t count = std::min(destination.GetSize(), source.GetSize());
            for (Size_t i = 0; i < componentCount; ++i)
            {
                auto componentTypeInfo = chunkType->Components[i];
                componentTypeInfo->Copy(destination.ComponentData[i], source.ComponentData[i], count);
            }
            destination.Size = count;
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

    /// <summary>
    /// Chunk of multiple nodes with the same component types (same set of component).
    /// </summary>
    /// <typeparam name="TChunkType">ChunkType type represent the structure of the chunk and what component types it has.</typeparam>
    /// <typeparam name="TSize">Size unit for indexing nodes in the chunk</typeparam>
    template<typename TChunkType, typename TSize>
    struct ChunkT : public ChunkPointerT<TChunkType, TSize>
    {
    public:
        using Base_t = ChunkPointerT<TChunkType, TSize>;
        using ChunkType_t = TChunkType;
        using Size_t = TSize;
        using Chunk_t = ChunkT;

    private:
        Size_t Capacity;

    public:
        /// <summary>
        /// Get the capacity of the chunk.
        /// The capacity is the max size the chunk can be without reallocating the component's memory.
        /// Algorithms will only process nodes up to the chunk's size.
        /// </summary>
        /// <returns>The capacity of the chunk</returns>
        Size_t GetCapacity()const { return Capacity; }

        /// <summary>
        /// Create a null chunk without chunk type nor component data.
        /// </summary>
        /// <returns></returns>
        static Chunk_t Null() { return Chunk_t(); }

    public:
        /// <summary>
        /// Create a null chunk without chunk type.
        /// IsNull() will evaluate to true.
        /// Size() and Capacity() will evaluate to 0
        /// </summary>
        ChunkT()
            : Capacity(0)
        {
        }

        /// <summary>
        /// Deep copy a chunk and its component data.
        /// The result chunk will have the same size and capacity as the original.
        /// </summary>
        /// <param name="o"></param>
        ChunkT(const ChunkT& o)
            : Base_t(o)
            , Capacity(o.Capacity)
        {
            AllocateComponentDataArray();
            if(!o.IsNull())
                AllocateDataCopy(o);
        }

        /// <summary>
        /// Deep copy a chunk and its component data.
        /// It will deallocate any previous chunk data.
        /// The result chunk will have the same size and capacity as the original.
        /// </summary>
        /// <param name="o"></param>
        ChunkT& operator=(const ChunkT& o) 
        {
            if (this == &o)
                return *this;
            DeallocateData();
            DeallocateComponentDataArray();
            Base_t::operator=(o);
            Capacity = o.Capacity;
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
        /// <param name="capacity"></param>
        /// <param name="size"></param>
        ChunkT(const ChunkType_t* chunkType, Size_t capacity, Size_t size = 0)
            : Base_t(chunkType, size)
            , Capacity(capacity)
        {
            AllocateComponentDataArray();
            AllocateData();
        }

        /// <summary>
        /// Deallocate data if not a null chunk
        /// </summary>
        ~ChunkT() 
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
            assert(!this->IsNull());
            auto componentCount = this->Type->Components.GetSize();
            std::vector<void*> componentsData(componentCount);
            for (size_t i = 0; i < componentCount; ++i) 
            {
                auto componentTypeInfo = this->Type->Components[i];
                this->ComponentData[i] = componentTypeInfo->Allocate(Capacity);
            }
        }

        void AllocateDataCopy(const ChunkT& o) 
        {
            assert(!this->IsNull());
            assert(IsSameChunkType(*this, o));
            auto componentCount = this->Type->Components.GetSize();
            for (size_t i = 0; i < componentCount; ++i) 
            {
                auto componentTypeInfo = this->Type->Components[i];
                this->ComponentData[i] = componentTypeInfo->AllocateCopy(o.ComponentData[i], o.Capacity, o.Size);
            }
        }

        void DeallocateData() 
        {
            if (this->Type == nullptr)
                return;
            assert(IsDataAllocated());
            auto componentCount = this->Type->Components.GetSize();
            for (size_t i = 0; i < componentCount; ++i) 
            {
                auto componentTypeInfo = this->Type->Components[i];
                componentTypeInfo->Deallocate(this->ComponentData[i], Capacity);
            }
        }

        void AllocateComponentDataArray()
        {
            this->ComponentData = (void**)FMemory::Malloc(this->Type->Components.GetSize() * sizeof(void*), alignof(void*));
        }
        void DeallocateComponentDataArray()
        {
            FMemory::Free(this->ComponentData);
        }

        void CopyComponentDataArray(void** componentData)const
        {
            auto componentCount = this->Type->Components.GetSize();
            for (int32 i = 0; i < componentCount; ++i)
            {
                this->ComponentData[i] = componentData[i];
            }
        }
    };


    template<typename TChunkPointer, typename TChunkType, typename TSize>
    struct ChunkArrayPointerT : public ChunkPointerT<TChunkType, TSize>
    {
    public:
        using Base_t = ChunkPointerT<TChunkType, TSize>;
        using ChunkArrayPointer_t = ChunkArrayPointerT;
        using ChunkPointer_t = TChunkPointer;
        using ChunkType_t = TChunkType;
        using Size_t = TSize;

    protected:
        ChunkPointer_t** Chunks;
        Size_t ChunkCount;
        //Size_t CapacityPerChunk;
        //Size_t SizePerChunk;

        Size_t GetChunkCount()const { return ChunkCount; }
    public:
        //const ChunkType_t* chunkType, Size_t size, void** componentData
        ChunkArrayPointerT(const ChunkType_t* chunkType, void** componentData, ChunkPointer_t** chunks, Size_t chunkCount, Size_t capacityPerChunk)
            : Base_t(chunkType, chunkCount* capacityPerChunk, componentData)
            , Chunks(chunks)
            , ChunkCount(chunkCount)
            //, CapacityPerChunk(capacityPerChunk)
            //, SizePerChunk(sizePerChunk)
        {

        }
    protected:

        ChunkArrayPointerT(const ChunkType_t* chunkType, Size_t capacityPerChunk, Size_t chunkCount = 0)
            : Base_t(chunkType, chunkCount* capacityPerChunk)
            , Chunks(nullptr)
            , ChunkCount(chunkCount)
            //, CapacityPerChunk(capacityPerChunk)
            //, SizePerChunk(sizePerChunk)
        {
        }
    public:
        const ChunkPointer_t& operator[](Size_t index)const
        {
            return *Chunks[index];
        }


    };

    template<typename TChunkPointer, typename TChunkType, typename TSize>
    struct ChunkArrayT : public ChunkArrayPointerT<TChunkPointer, TChunkType, TSize>
    {
    public:
        using Base_t = ChunkArrayPointerT<TChunkPointer, TChunkType, TSize>;
        //using ChunkArrayPointer_t = ChunkArrayPointerT;
        using ChunkPointer_t = TChunkPointer;
        //using ChunkPointer_t = ChunkPointerT<TChunkType, TSize>;
        using ChunkType_t = TChunkType;
        using Size_t = TSize;
        using ChunkArray_t = ChunkArrayT<TChunkPointer, TChunkType, TSize>;

    protected:
        //Max total nodes
        Size_t Capacity;
        //Max chunks
        Size_t ChunkCapacity;
        Size_t GetCapacityPerChunk()const { return Capacity / ChunkCapacity; }

        ChunkArrayT(const ChunkType_t* chunkType, Size_t capacityPerChunk, Size_t chunkCapacity, Size_t chunkCount = 0)
            : Base_t(chunkType, capacityPerChunk, chunkCount)
            , Capacity(chunkCapacity * capacityPerChunk)
            , ChunkCapacity(chunkCapacity)
        {
            AllocateComponentDataArray();
            AllocateData();
            for (int i = 0; i < chunkCount; ++i)
            {
                this->Chunks[i] = ChunkPointer_t(chunkType, 0, GetComponentDataForChunk(i));
            }
        }

        void** GetComponentDataForChunk(Size_t chunkIndex)
        {
            return &this->ComponentData[chunkIndex * GetCapacityPerChunk()];
            
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
            assert(!this->IsNull());
            auto componentCount = this->Type->Components.GetSize();
            auto capacityPerChunk = GetCapacityPerChunk();
            std::vector<void*> componentsData(componentCount);
            for (size_t i = 0; i < componentCount; ++i)
            {
                auto componentTypeInfo = this->Type->Components[i];
                this->ComponentData[i] = componentTypeInfo->Allocate(Capacity, ChunkCapacity);
                for (size_t k = 1; k < ChunkCapacity; ++k)
                {
                    this->ComponentData[k * componentCount + i] = componentTypeInfo->Forward(this->ComponentData[i], capacityPerChunk);
                }
            }
        }

        void AllocateDataCopy(const ChunkArray_t& o)
        {
            assert(!this->IsNull());
            assert(IsSameChunkType(*this, o));
            auto componentCount = this->Type->Components.GetSize();
            for (size_t i = 0; i < componentCount; ++i)
            {
                auto componentTypeInfo = this->Type->Components[i];
                this->ComponentData[i] = componentTypeInfo->AllocateCopy(o.ComponentData[i], o.Capacity, o.Size, ChunkCapacity);
            }
        }

        void DeallocateData()
        {
            if (this->Type == nullptr)
                return;
            assert(IsDataAllocated());
            auto componentCount = this->Type->Components.GetSize();
            for (size_t i = 0; i < componentCount; ++i)
            {
                auto componentTypeInfo = this->Type->Components[i];
                componentTypeInfo->Deallocate(this->ComponentData[i], Capacity, ChunkCapacity);
            }
        }

    };

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