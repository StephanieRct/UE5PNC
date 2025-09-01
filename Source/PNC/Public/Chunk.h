// MIT License
// Copyright (c) 2025 Stephanie Rancourt

#pragma once
#include "common.h"

namespace PNC 
{
    /// <summary>
    /// Chunk of multiple nodes with the same component types (same set of component).
    /// </summary>
    /// <typeparam name="TChunkType">ChunkType type represent the structure of the chunk and what component types it has.</typeparam>
    /// <typeparam name="TSize">Size unit for indexing nodes in the chunk</typeparam>
    template<typename TChunkType, typename TSize>
    struct ChunkT 
    {
    public:
        typedef TChunkType ChunkType_t;
        typedef TSize Size_t;

    private:
        const ChunkType_t* Type;
        Size_t Capacity;
        Size_t Size;
        std::vector<void*> ComponentData;

    public:
        /// <summary>
        /// Get the capacity of the chunk.
        /// The capacity is the max size the chunk can be without reallocating the component's memory.
        /// Algorithms will only process nodes up to the chunk's size.
        /// </summary>
        /// <returns>The capacity of the chunk</returns>
        Size_t GetCapacity()const { return Capacity; }

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
        static ChunkT Null() { return ChunkT(); }

    public:
        /// <summary>
        /// Create a null chunk without chunk type.
        /// IsNull() will evaluate to true.
        /// Size() and Capacity() will evaluate to 0
        /// </summary>
        ChunkT()
            : Type(nullptr)
            , Capacity(0)
            , Size(0)
            , ComponentData() 
        {
        }

        /// <summary>
        /// Deep copy a chunk and its component data.
        /// The result chunk will have the same size and capacity as the original.
        /// </summary>
        /// <param name="o"></param>
        ChunkT(const ChunkT& o)
            : Type(o.Type)
            , Capacity(o.Capacity)
            , Size(o.Size)
            , ComponentData(o.ComponentData.size()) 
        {
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
            Type = o.Type;
            Capacity = o.Capacity;
            Size = o.Size;
            ComponentData.resize(o.ComponentData.size());
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
            :Type(chunkType)
            , Capacity(capacity)
            , Size(size)
            , ComponentData(chunkType->Components.GetSize()) 
        {
            AllocateData();
        }

        /// <summary>
        /// Deallocate data if not a null chunk
        /// </summary>
        ~ChunkT() 
        {
            DeallocateData();
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
            check(IsValid());
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
            check(IsValid());
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
            check(IsValid());
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
            check(IsValid());
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
            check(IsValid());
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
            check(IsValid());
            return (TComponent*)GetComponentData(&typeid(TComponent)); 
        }

        /// <summary>
        /// Copy data between two chunks of the same ChunkType.
        /// Will return -1 if the any chunk is null or not the same ChunkType
        /// </summary>
        /// <param name="destination">Chunk where to write the data to.</param>
        /// <param name="source">Chunk where to read the data from.</param>
        /// <returns>Number of node data copied or -1 if failed.</returns>
        static Size_t CopyData(ChunkT& destination, const ChunkT& source) {
            if (!destination.IsValid()
                || !source.IsValid()
                || destination.Type != source.Type)
                return (Size_t)-1;
            auto chunkType = destination.Type;
            auto componentCount = chunkType->Components.types.size();
            Size_t count = std::min(destination.GetCapacity(), source.Size);
            for (Size_t i = 0; i < componentCount; ++i) {
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
        static bool IsSameChunkType(const ChunkT& a, const ChunkT& b) { return a.Type == b.Type; }

    private:

        bool IsDataAllocated()const { return ComponentData.size() > 0 && ComponentData[0] != nullptr; }

        bool IsValid()const { return !IsNull() && IsDataAllocated(); }

        void AllocateData() 
        {
            check(!IsNull());
            check(!IsDataAllocated());
            auto componentCount = Type->Components.GetSize();
            std::vector<void*> componentsData(componentCount);
            for (size_t i = 0; i < componentCount; ++i) {
                auto componentTypeInfo = Type->Components[i];
                ComponentData[i] = componentTypeInfo->Allocate(Capacity);
            }
        }

        void AllocateDataCopy(const ChunkT& o) 
        {
            check (IsSameChunkType(*this, o));
            check (!IsDataAllocated());
            check (o.IsDataAllocated());
            auto componentCount = Type->Components.GetSize();
            for (size_t i = 0; i < componentCount; ++i) {
                auto componentTypeInfo = Type->Components[i];
                ComponentData[i] = componentTypeInfo->AllocateCopy(o.ComponentData[i], o.Size, o.Capacity);
            }
        }

        void DeallocateData() 
        {
            if (Type == nullptr)
                return;
            check(IsDataAllocated());
            auto componentCount = Type->Components.GetSize();
            for (size_t i = 0; i < componentCount; ++i) {
                auto componentTypeInfo = Type->Components[i];
                componentTypeInfo->Deallocate(ComponentData[i], Capacity);
            }
        }
    };

}