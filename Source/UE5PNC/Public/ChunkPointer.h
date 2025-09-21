// MIT License
// Copyright (c) 2025 Stephanie Rancourt

#pragma once
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
    struct ChunkPointerT : private ChunkPointerInternalT<TChunkStructure>
    {
    public:
        using Base_t = ChunkPointerInternalT<TChunkStructure>;
        using Self_t = ChunkPointerT<TChunkStructure>;
        using ChunkStructure_t = TChunkStructure;
        using Size_t = typename ChunkStructure_t::Size_t;

        /// <summary>
        /// Reinterprete_cast this object to this type to get read-write access to its private data fields
        /// Use with caution.
        /// </summary>
        using Internal_t = ChunkPointerInternalT<TChunkStructure>;

        /// <summary>
        /// Chunk_t is the type a ChunkPointer points to
        /// </summary>
        using Chunk_t = ChunkPointerT; 

    public:
        /// <summary>
        /// Create a null ChunkPointer without structure.
        /// IsNull() will evaluate to true.
        /// </summary>
        ChunkPointerT()
            : Base_t()
        {
        }

        /// <summary>
        /// Contructs from its member data fields.
        /// </summary>
        /// <param name="chunkStructure">Structure of the Chunk's Component data.</param>
        /// <param name="nodeCount">Number of nodes are included by this pointer.</param>
        /// <param name="componentData">Points to an array of component data pointers created according to the chunk type.</param>
        ChunkPointerT(const ChunkStructure_t* chunkStructure, Size_t nodeCount, void** componentData)
            : Base_t(chunkStructure, nodeCount, componentData)
        {
        }

    protected:
        ChunkPointerT(const ChunkStructure_t* chunkStructure, Size_t nodeCount)
            : Base_t(chunkStructure, nullptr, nodeCount)
        {
        }

    public:
        using Base_t::IsNull;
        using Base_t::GetNodeCount;
        using Base_t::GetChunkStructure;

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
        void* GetComponentData(Size_t componentTypeIndexInChunk)
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
        /// <param name="componentIndexInChunk">index in the ChunkStructure::Components ComponentTypeSet</param>
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
        /// component's const type_info* pointer to the component type index in the chunk's ChunkStructure ComponentTypeSet.
        /// </summary>
        /// <param name="componentType">const type_info* pointer obtained from &typeid(ComponentTypename)</param>
        /// <returns>Pointer to the component memory array</returns>
        void* GetComponentData(const type_info* componentType)
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
        const void* GetComponentData(const type_info* componentType)const
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
        /// Copy data between two chunks of the same ChunkStructure.
        /// Will return -1 if the any chunk is null or not the same ChunkStructure
        /// </summary>
        /// <param name="destination">Chunk where to write the data to.</param>
        /// <param name="source">Chunk where to read the data from.</param>
        /// <returns>Number of node data copied or -1 if failed.</returns>
        static Size_t CopyData(ChunkPointerT& destination, const ChunkPointerT& source) {
            if (destination.IsNull()
                || source.IsNull()
                || destination.Structure != source.Structure)
                return (Size_t)-1;
            auto chunkStructure = destination.Structure;
            auto componentCount = chunkStructure->Components.ComponentTypes.size();
            Size_t count = std::min(destination.GetNodeCount(), source.GetNodeCount());
            for (Size_t i = 0; i < componentCount; ++i)
            {
                auto componentTypeInfo = chunkStructure->Components[i];
                componentTypeInfo->Copy(destination.ComponentData[i], source.ComponentData[i], count);
            }
            destination.NodeCount = count;
            return count;
        }

        /// <summary>
        /// Test if 2 chunk have the same ChunkStructure
        /// </summary>
        /// <param name="a"></param>
        /// <param name="b"></param>
        /// <returns></returns>
        static bool IsSameChunkStructure(const ChunkPointerT& a, const ChunkPointerT& b) { return a.Structure == b.Structure; }
    };
}
