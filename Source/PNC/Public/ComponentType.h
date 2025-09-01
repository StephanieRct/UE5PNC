// MIT License
// Copyright (c) 2025 Stephanie Rancourt

#pragma once
#include "common.h"

namespace PNC
{
    /// <summary>
    /// The owner of a component determine its multiplicity when allocating the component memory in a chunk
    /// </summary>
    enum ComponentOwner
    {
        /// <summary>
        /// Creates a component instance for each node in a chunk. 
        /// All instances are allocated sequentially in memory.
        /// </summary>
        ComponentOwner_Node = 0,

        /// <summary>
        /// Creates a single component instance for each chunk which is shared by all nodes in the chunk.
        /// </summary>
        ComponentOwner_Chunk = 1,

        ComponentOwner__Begin = 0,
        ComponentOwner__End = 2,
    };


    /// <summary>
    /// Provide a way to uniquely identify each component types, their owner and how to allocate component memory on demand.
    /// </summary>
    template< typename TSize = size_t >
    struct ComponentTypeT 
    {
    public:
        typedef TSize Size_t;

    public:
        const type_info* TypeInfo;
        Size_t Size;
        Size_t Align;
        ComponentOwner Owner;

        /// <summary>
        /// Create a ComponentType from the component's type_info.
        /// </summary>
        /// <param name="typeInfo">type_info of the component type. Must be a valid pointer.</param>
        /// <param name="size">Size of the component in bytes. Must be greater than 0.</param>
        /// <param name="align">Alignment of the component in bytes. Must be greater than 0.</param>
        /// <param name="owner">Owner of this component type.</param>
        ComponentTypeT(const type_info* typeInfo, Size_t size, Size_t align, ComponentOwner owner)
            : TypeInfo(typeInfo)
            , Size(size)
            , Align(align)
            , Owner(owner) 
        {
            check(typeInfo != nullptr);
            check(size > 0);
            check(align > 0);
            check(owner >= ComponentOwner__Begin && owner < ComponentOwner__End);
        }

        /// <summary>
        /// Create a ComponentType from a typename.
        /// </summary>
        /// <typeparam name="T">The component typename</typeparam>
        /// <param name="_nullptr">Should always be equal to (const T*)nullptr. Provides a way to specify the component typename argument.</param>
        /// <param name="aOwner">Owner for this component type.</param>
        template<typename T>
        ComponentTypeT(const T* _nullptr, ComponentOwner owner)
            : TypeInfo(&typeid(T))
            , Size(sizeof(T))
            , Align(alignof(T))
            , Owner(owner)
        {
            check(_nullptr == nullptr);
            check(owner >= ComponentOwner__Begin && owner < ComponentOwner__End);
        }

        /// <summary>
        /// Allocate enough memory to fit all component instance for the given capacity of a chunk.
        /// </summary>
        /// <param name="chunkCapacity">How many instances of the component is required to be allocated</param>
        /// <returns>Pointer to the allocated memory. Must be freed by calling Deallocate.</returns>
        void* Allocate(Size_t chunkCapacity)const
        {
            auto count = GetNodeDataIndex(chunkCapacity);
            return FMemory::Malloc(Size * count, Align);
        }

        /// <summary>
        /// Allocate enough memory to fit all component instance for the given capacity of a chunk and copy data into it.
        /// </summary>
        /// <param name="chunkCapacity">How many instances of the component is required to be allocated</param>
        /// <param name="chunkSize">How many instances of the component is required to be copied</param>
        /// <returns>Pointer to the allocated memory. Must be freed by calling Deallocate.</returns>
        void* AllocateCopy(void* from, Size_t chunkCapacity, Size_t chunkSize)const
        {
            auto chunkCapacityCount = GetNodeDataIndex(chunkCapacity);
            auto ptr = FMemory::Malloc(Size * chunkCapacityCount, Align);
            auto chunkSizeCount = GetNodeDataIndex(chunkCapacity);
            Copy(ptr, from, Size * chunkSizeCount);
            return ptr;
        }

        /// <summary>
        /// Deallocate previously allocated component memory.
        /// </summary>
        /// <param name="ptr">pointer from a previous call to Allocate or AllocateCopy</param>
        /// <param name="chunkCapacity"></param>
        void Deallocate(void* ptr, Size_t chunkCapacity)const
        {
            FMemory::Free(ptr);
        }

        /// <summary>
        /// Copy component data from one chunk of memory to another.
        /// </summary>
        /// <param name="to">destination memory</param>
        /// <param name="from">source memory</param>
        /// <param name="chunkSize">How many component instances to copy</param>
        void Copy(void* to, void* from, Size_t chunkSize)const
        {
            auto count = GetNodeDataIndex(chunkSize);
            memcpy_s(to, count * Size, from, count * Size);
        }

        /// <summary>
        /// Figure out the index into an array of this component type where a node's component instance is stored.
        /// </summary>
        /// <param name="nodeIndex"></param>
        /// <returns></returns>
        Size_t GetNodeDataIndex(Size_t nodeIndex)const {
            switch (Owner)
            {
            case ComponentOwner_Node:
                return nodeIndex;
            case ComponentOwner_Chunk:
                return 1;
            default:
                checkNoEntry();
                return -1;
            }
        }
    };


}