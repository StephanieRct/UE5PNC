// MIT License
// Copyright (c) 2025 Stephanie Rancourt

#pragma once
#include "common.h"
#include "Node.h"
#include "ChunkPointerInternal.h"

namespace PNC
{
    /// <summary>
    /// A ChunkPointer points to a Chunk's Component data memory within a range of Nodes.
    /// A ChunkPointer is also a Chunk by itself and provides access to the Component data.
    /// Copying a ChunkPointer does not copy the data.
    /// </summary>
    /// <typeparam name="TChunkStructure">Structure of the Chunk's Component data.</typeparam>
    template<typename TChunkStructure>
    struct ChunkPointerT : protected ChunkPointerInternalT<TChunkStructure>
    {
    public:
        using Base_t = ChunkPointerInternalT<TChunkStructure>;
        using Self_t = ChunkPointerT<TChunkStructure>;
        using ChunkStructure_t = TChunkStructure;
        using ComponentType_t = typename ChunkStructure_t::ComponentType_t;
        using Size_t = typename ChunkStructure_t::Size_t;
        using ChunkPointer_t = Self_t;
        using Node_t = NodeT<TChunkStructure>;

        /// <summary>
        /// Reinterprete_cast this object to this type to get read-write access to its private data fields
        /// Use with caution.
        /// </summary>
        using ChunkPointerInternal_t = ChunkPointerInternalT<TChunkStructure>;

        /// <summary>
        /// Chunk_t is the type a ChunkPointer points to
        /// </summary>
        using Chunk_t = ChunkPointerT;

    public:
        ChunkPointerT() = default;
        ChunkPointerT(const Self_t& o) = default;
        Self_t& operator=(const Self_t& o) = default;
        ChunkPointerT(Self_t&& o) = default;
        Self_t& operator=(Self_t&& o) = default;

        /// <summary>
        /// Contructs from its member data fields.
        /// </summary>
        /// <param name="chunkStructure">Structure of the Chunk's Component data.</param>
        /// <param name="nodeCount">Number of nodes are included by this pointer.</param>
        /// <param name="componentData">Points to an array of component data pointers created according to the chunk type.</param>
        ChunkPointerT(const ChunkStructure_t* const chunkStructure, const Size_t nodeCount, void** const componentData)
            : Base_t(chunkStructure, nodeCount, componentData)
        {
        }

    protected:
        ChunkPointerT(const ChunkStructure_t* const chunkStructure, const Size_t nodeCount)
            : Base_t(chunkStructure, nodeCount)
        {
        }

    public:
        using Base_t::IsVoid;
        using Base_t::IsStruct;
        using Base_t::IsNull;
        using Base_t::IsData;
        using Base_t::IsVoidNull;
        using Base_t::IsVoidData;
        using Base_t::IsStructNull;
        using Base_t::IsStructData;

        using Base_t::GetStructure;
        using Base_t::GetNodeCount;
        using Base_t::GetChunkCount;

        /// <summary>
        /// Create a null chunk without structure nor component data.
        /// </summary>
        /// <returns></returns>
        static Self_t Null() { return Self_t(); }

        const Chunk_t& operator*()const { return *this; }
        Chunk_t& operator*() { return *this; }
        const Chunk_t* operator->()const { return this; }
        Chunk_t* operator->() { return this; }
        const Chunk_t& GetChunk()const { return *this; }
        Chunk_t& GetChunk() { return *this; }

    public:
        /// <summary>
        /// Get the pointer to a component's memory array using the component type index in the ChunkStructure ComponentTypeSet.
        /// For components with ComponentOwner_Node, the array will be at least the length of the size of the chunk.
        /// For components with ComponentOwner_Chunk, the array will be of length 1.
        /// This is the fastest way to access the chunk's component data.
        /// </summary>
        /// <param name="componentIndexInChunk">index in the ChunkStructure::Components ComponentTypeSet</param>
        /// <returns>Pointer to the component memory array</returns>
        void* GetComponentData(const Size_t componentTypeIndexInChunk)
        {
            assert_pnc(!IsNull());
            return this->ComponentData[componentTypeIndexInChunk];
        }

        /// <summary>
        /// Get the const pointer to a component's memory array using the component type index in the ChunkStructure ComponentTypeSet.
        /// For components with ComponentOwner_Node, the array will be at least the length of the size of the chunk.
        /// For components with ComponentOwner_Chunk, the array will be of length 1.
        /// This is the fastest way to access the chunk's component data.
        /// </summary>
        /// <param name="componentTypeIndexInChunk">index in the ChunkStructure::Components ComponentTypeSet</param>
        /// <returns>Const pointer to the component memory array</returns>
        const void* GetComponentData(const Size_t componentTypeIndexInChunk)const
        {
            assert_pnc(!IsNull());
            return this->ComponentData[componentTypeIndexInChunk];
        }

        /// <summary>
        /// Get the pointer to a component's memory array using the component's type const type_info* from &typeid(ComponentTypename).
        /// For components with ComponentOwner_Node, the array will be at least the length of the size of the chunk.
        /// For components with ComponentOwner_Chunk, the array will be of length 1.
        /// This is a slow way to access the chunk's component data as it require to do a map lookup of the 
        /// component's const type_info* pointer to the component type index in the chunk's ChunkStructure ComponentTypeSet.
        /// </summary>
        /// <param name="componentType">const type_info* pointer obtained from &typeid(ComponentTypename)</param>
        /// <returns>Pointer to the component memory array</returns>
        void* GetComponentData(const type_info* const componentType)
        {
            assert_pnc(!IsNull());
            auto index = this->Structure->Components.GetComponentTypeIndexInChunk(componentType);
            if (index < 0)
                return nullptr;
            return this->GetComponentData(index);
        }

        /// <summary>
        /// Get the const pointer to a component's memory array using the component's type const type_info* from &typeid(ComponentTypename).
        /// For components with ComponentOwner_Node, the array will be at least the length of the size of the chunk.
        /// For components with ComponentOwner_Chunk, the array will be of length 1.
        /// This is a slow way to access the chunk's component data as it require to do a map lookup of the 
        /// component's const type_info* pointer to the component type index in the chunk's ChunkStructure ComponentTypeSet.
        /// </summary>
        /// <param name="componentType">const type_info* pointer obtained from &typeid(ComponentTypename)</param>
        /// <returns>Const pointer to the component memory array</returns>
        const void* GetComponentData(const type_info* const componentType)const
        {
            assert_pnc(!IsNull());
            auto index = this->Structure->Components.GetComponentTypeIndexInChunk(componentType);
            if (index < 0)
                return nullptr;
            return this->GetComponentData(index);
        }


        /// <summary>
        /// Get the pointer to a component's memory array using the component's typename.
        /// For components with ComponentOwner_Node, the array will be at least the length of the size of the chunk.
        /// For components with ComponentOwner_Chunk, the array will be of length 1.
        /// This is a slow way to access the chunk's component data as it require to do a map lookup of the associated
        /// component typename const type_info* pointer to the component type index in the chunk's ChunkStructure ComponentTypeSet.
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
        /// component typename const type_info* pointer to the component type index in the chunk's ChunkStructure ComponentTypeSet.
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
        /// Test if 2 chunk have the same ChunkStructure
        /// </summary>
        /// <param name="a"></param>
        /// <param name="b"></param>
        /// <returns></returns>
        static bool IsSameStructure(const Self_t& a, const Self_t& b) { return a.Structure == b.Structure; }

        static ChunkPointerInternal_t& GetInternalChunk(Self_t& chunkPointer) { return reinterpret_cast<ChunkPointerInternal_t&>(chunkPointer.GetChunk()); }
    //protected:
    //    ChunkPointerInternal_t& GetInternalChunk() { return reinterpret_cast<ChunkPointerInternal_t&>(GetChunk()); }

    };
}
