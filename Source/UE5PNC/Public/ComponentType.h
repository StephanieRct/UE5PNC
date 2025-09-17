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
    template< typename TSize>
    struct ComponentTypeT 
    {
    public:
        using Self_t = ComponentTypeT<TSize>;
        using Size_t = TSize;

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
            assert_pnc(TypeInfo != nullptr);
            assert_pnc(Size > 0);
            assert_pnc(Align > 0);
            assert_pnc(Owner >= ComponentOwner__Begin && Owner < ComponentOwner__End);
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
            assert_pnc(_nullptr == nullptr);
            assert_pnc(owner >= ComponentOwner__Begin && owner < ComponentOwner__End);
        }

        /// <summary>
        /// Allocate enough memory to fit all component instance for the given capacity of a chunk.
        /// </summary>
        /// <param name="nodeCapacity">How many instances of the component is required to be allocated</param>
        /// <param name="chunkCapacity">How many sub-chunks in the array of data</param>
        /// <returns>Pointer to the allocated memory. Must be freed by calling Deallocate.</returns>
        void* Allocate(Size_t nodeCapacity, Size_t chunkCapacity = 1)const
        {
            auto count = GetNodeDataIndex(nodeCapacity, chunkCapacity) ;
            return FMemory::Malloc(Size * count, Align);
        }

        /// <summary>
        /// Allocate enough memory to fit all component instance for the given capacity of a chunk and copy data into it.
        /// </summary>
        /// <param name="nodeCapacity">How many instances of the component is required to be allocated</param>
        /// <param name="nodeCount">How many instances of the component is required to be copied</param>
        /// <param name="chunkCapacity">How many sub-chunks in the array of data</param>
        /// <returns>Pointer to the allocated memory. Must be freed by calling Deallocate.</returns>
        void* AllocateCopy(void* from, Size_t nodeCapacity, Size_t nodeCount, Size_t chunkCapacity = 1)const
        {
            auto chunkCapacityCount = GetNodeDataIndex(nodeCapacity, chunkCapacity);
            auto ptr = FMemory::Malloc(Size * chunkCapacityCount, Align);
            auto chunkSizeCount = GetNodeDataIndex(nodeCount, chunkCapacity);
            Copy(ptr, from, Size * chunkSizeCount);
            return ptr;
        }

        /// <summary>
        /// Deallocate previously allocated component memory.
        /// </summary>
        /// <param name="ptr">pointer from a previous call to Allocate or AllocateCopy</param>
        /// <param name="nodeCapacity"></param>
        /// <param name="chunkCapacity">How many sub-chunks in the array of data</param>
        void Deallocate(void* ptr, Size_t nodeCapacity, Size_t chunkCapacity = 1)const
        {
            FMemory::Free(ptr);
        }

        /// <summary>
        /// Copy component data from one chunk of memory to another.
        /// </summary>
        /// <param name="to">destination memory</param>
        /// <param name="from">source memory</param>
        /// <param name="nodeCount">How many component instances to copy</param>
        void Copy(void* to, void* from, Size_t nodeCount, Size_t chunkCapacity = 1)const
        {
            auto count = GetNodeDataIndex(nodeCount, chunkCapacity);
            memcpy_s(to, count * Size, from, count * Size);
        }

        void* SubChunk(void* ptr, Size_t count)const
        {
            switch (Owner)
            {
            case ComponentOwner_Node:
                return ptr + count * Size;
            case ComponentOwner_Chunk:
                return ptr + Size;
            default:
                checkNoEntry();
                return -1;
            }
        }

        void* Forward(void* ptr, Size_t count)const
        {
            return (uint8*)ptr + count * Size;
        }
        void* Backward(void* ptr, Size_t count)const
        {
            return (uint8*)ptr - count * Size;
        }
        /// <summary>
        /// Figure out the index into an array of this component type where a node's component instance is stored.
        /// </summary>
        /// <param name="nodeIndex"></param>
        /// <returns></returns>
        Size_t GetNodeDataIndex(Size_t nodeIndex, Size_t chunkIndex = 1)const
        {
            switch (Owner)
            {
            case ComponentOwner_Node:
                return nodeIndex;
            case ComponentOwner_Chunk:
                return chunkIndex;
            default:
                checkNoEntry();
                return -1;
            }
        }
    };
}
