// MIT License
// Copyright (c) 2025 Stephanie Rancourt

#pragma once
#include "common.h"
#include "Node.h"

namespace NiT
{
    /// <summary>
    /// A ChunkPointerInternal points to a chunk's component data memory within a range of nodes.
    /// Used to get read-write access to a ChunkPointer's data fields.
    /// </summary>
    /// <typeparam name="TChunkStructure">Structure of the chunk's component data.</typeparam>
    template<typename TChunkStructure>
    struct ChunkPointerInternalT
    {
    public:
        using Self_t = ChunkPointerInternalT<TChunkStructure>;
        using ChunkStructure_t = TChunkStructure;
        using Size_t = typename ChunkStructure_t::Size_t;
        using ComponentType_t = typename ChunkStructure_t::ComponentType_t;
        using Node_t = NodeT<TChunkStructure>;
        using ChunkPointerInternal_t = Self_t;

    public:
        /// <summary>
        /// ChunkStructure used to create the structure of what ComponentDataArray points to.
        /// </summary>
        const ChunkStructure_t* Structure;

        /// <summary>
        /// Points to an array of component data pointers.
        /// </summary>
        void** ComponentDataArray;

        /// <summary>
        /// Number of nodes (and not in bytes) all component data array contains
        /// </summary>
        NodeCountT<Size_t> NodeCount;

    public:
        /// <summary>
        /// Create a VoidNull Chunk.
        /// </summary>
        ChunkPointerInternalT()
            : Structure(nullptr)
            , ComponentDataArray(nullptr)
            , NodeCount(0)
        {
        }

        template<typename TProps>
        ChunkPointerInternalT(const DPropsTag& tag, const TProps& props)
            : Structure(props.GetStructurePtr())
            , ComponentDataArray(props.GetComponentDataArray())
            , NodeCount(props.GetNodeCount())
        {
        }

        /// <summary>
        /// Create a StructNull Chunk.
        /// </summary>
        ChunkPointerInternalT(const ChunkStructure_t* const chunkStructure)
            : Structure(chunkStructure)
            , ComponentDataArray(nullptr)
            , NodeCount(0)
        {
        }

        /// <summary>
        /// Create a StructNull Chunk with a node count.
        /// </summary>
        ChunkPointerInternalT(const ChunkStructure_t* const chunkStructure, const NodeCountT<Size_t> nodeCount)
            : Structure(chunkStructure)
            , ComponentDataArray(nullptr)
            , NodeCount(nodeCount)
        {
        }

        /// <summary>
        /// Create a VoidData Chunk
        /// </summary>
        ChunkPointerInternalT(const NodeCountT<Size_t> nodeCount, void** const componentData)
            : Structure(nullptr)
            , ComponentDataArray(componentData)
            , NodeCount(nodeCount)
        {
        }

        /// <summary>
        /// Create a StructData Chunk
        /// </summary>
        /// <param name="chunkStructure">Structure of the Chunk's Component data.</param>
        /// <param name="nodeCount">Number of nodes are included by this pointer.</param>
        /// <param name="componentData">Points to an array of component data pointers created according to the chunk type.</param>
        ChunkPointerInternalT(const ChunkStructure_t* const chunkStructure, const NodeCountT<Size_t> nodeCount, void** const componentData)
            : Structure(chunkStructure)
            , ComponentDataArray(componentData)
            , NodeCount(nodeCount)
        {
        }

        ChunkPointerInternalT(Self_t&& o)
            : Structure(o.Structure)
            , ComponentDataArray(o.ComponentDataArray)
            , NodeCount(o.NodeCount)
        {
            o.Structure = nullptr;
            o.ComponentDataArray = nullptr;
            o.NodeCount = NodeCountT<Size_t>::V_0();
        }

        Self_t& operator=(Self_t&& o)
        {
            auto tmpStructure = o.Structure;
            auto tmpComponentData = o.ComponentDataArray;
            auto tmpNodeCount = o.NodeCount;
            o.Structure = nullptr;
            o.ComponentDataArray = nullptr;
            o.NodeCount = NodeCountT<Size_t>::V_0();
            Structure = tmpStructure;
            ComponentDataArray = tmpComponentData;
            NodeCount = tmpNodeCount;
            return *this;
        }

        ChunkPointerInternalT(const Self_t& o) = default;
        Self_t& operator=(const Self_t& o) = default;

#ifdef NI_MEMORYCLEANUP
        ~ChunkPointerInternalT()
        {
            ni_clean(Structure);
            ni_clean(ComponentDataArray);
            ni_clean(NodeCount);
        }
#endif
    public:
        bool IsVoid()const { return Structure == nullptr; }
        bool IsStruct()const { return Structure != nullptr; }
        bool IsNull()const { return ComponentDataArray == nullptr; }
        bool IsData()const { return ComponentDataArray != nullptr; }
        bool IsVoidNull()const { return Structure == nullptr && ComponentDataArray == nullptr; }
        bool IsVoidData()const { return Structure == nullptr && ComponentDataArray != nullptr; }
        bool IsStructNull()const { return Structure != nullptr && ComponentDataArray == nullptr; }
        bool IsStructData()const { return Structure != nullptr && ComponentDataArray != nullptr; }

        /// <summary>
        /// Test if 2 chunk have the same ChunkStructure
        /// </summary>
        /// <param name="a"></param>
        /// <param name="b"></param>
        /// <returns></returns>
        static bool IsSameStructure(const Self_t& a, const Self_t& b) { return a.Structure == b.Structure; }
        static bool IsSameData(const Self_t& a, const Self_t& b) { return a.ComponentDataArray == b.ComponentDataArray; }
        static ChunkPointerInternal_t& GetInternalChunk(Self_t& a) { return a; }
        static const ChunkPointerInternal_t& GetInternalChunk(const Self_t& a) { return a; }

        NodeCountT<    Size_t> GetNodeCount()    const { return NodeCount; }
        NodeCapacityT< Size_t> GetNodeCapacity() const { return PropCountToCapacity(NodeCount); }
        ChunkCountT<   Size_t> GetChunkCount()   const { return ChunkCountT<   Size_t>::V_1(); }
        ChunkCapacityT<Size_t> GetChunkCapacity()const { return ChunkCapacityT<Size_t>::V_1(); }
        
        /// <summary>
        /// Get the ChunkStructure of this chunk
        /// </summary>
        /// <returns></returns>
        const ChunkStructure_t& GetStructure()const { return *this->Structure; }

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
            ni_assert(!IsNull());
            return this->ComponentDataArray[componentTypeIndexInChunk];
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
            ni_assert(!IsNull());
            return this->ComponentDataArray[componentTypeIndexInChunk];
        }

    };

    template<typename TChunkStructure>
    struct ChunkPointerProtectedT : protected ChunkPointerInternalT<TChunkStructure>
    {
    public:

        using Base_t = ChunkPointerInternalT<TChunkStructure>;
        using Self_t = ChunkPointerProtectedT<TChunkStructure>;
        using typename Base_t::ChunkStructure_t;
        using typename Base_t::Size_t;
        using typename Base_t::ComponentType_t;
        using typename Base_t::Node_t;
        using ChunkPointerInternal_t = typename Base_t::ChunkPointerInternal_t;

        using Base_t::Base_t;

        using Base_t::IsVoid;
        using Base_t::IsStruct;
        using Base_t::IsNull;
        using Base_t::IsData;
        using Base_t::IsVoidNull;
        using Base_t::IsVoidData;
        using Base_t::IsStructNull;
        using Base_t::IsStructData;
        using Base_t::IsSameStructure;
        using Base_t::IsSameData;
        using Base_t::GetInternalChunk;
        using Base_t::GetStructure;
        using Base_t::GetNodeCount;
        using Base_t::GetNodeCapacity;
        using Base_t::GetChunkCount;
        using Base_t::GetChunkCapacity;
        using Base_t::GetComponentData;

        static bool IsSameStructure(const Self_t& a, const Self_t& b) { return Base_t::IsSameStructure(a, b); }
        static bool IsSameData(const Self_t& a, const Self_t& b) { return Base_t::IsSameData(a, b); }
        static ChunkPointerInternal_t& GetInternalChunk(Self_t& a) { return Base_t::GetInternalChunk(a); }
        static const ChunkPointerInternal_t& GetInternalChunk(const Self_t& a) { return Base_t::GetInternalChunk(a); }

        /// <summary>
        /// Get the pointer to a component's memory array using the component's type const type_info* from &typeid(ComponentTypename).
        /// For components with ComponentOwner_Node, the array will be at least the length of the size of the container.
        /// For components with ComponentOwner_Chunk, the array will be of length 1.
        /// This is a slow way to access the container's component data as it require to do a map lookup of the 
        /// component's const type_info* pointer to the component type index in the container's ChunkStructure ComponentTypeSet.
        /// </summary>
        /// <param name="componentType">const type_info* pointer obtained from &typeid(ComponentTypename)</param>
        /// <returns>Pointer to the component memory array</returns>
        void* GetComponentData(const type_info* const componentType)
        {
            ni_assert(!IsNull());
            auto index = GetStructure().Components.GetComponentTypeIndexInChunk(componentType);
            if (index < 0)
                return nullptr;
            return GetComponentData(index);
        }

        /// <summary>
        /// Get the const pointer to a component's memory array using the component's type const type_info* from &typeid(ComponentTypename).
        /// For components with ComponentOwner_Node, the array will be at least the length of the size of the container.
        /// For components with ComponentOwner_Chunk, the array will be of length 1.
        /// This is a slow way to access the container's component data as it require to do a map lookup of the 
        /// component's const type_info* pointer to the component type index in the container's ChunkStructure ComponentTypeSet.
        /// </summary>
        /// <param name="componentType">const type_info* pointer obtained from &typeid(ComponentTypename)</param>
        /// <returns>Const pointer to the component memory array</returns>
        const void* GetComponentData(const type_info* const componentType)const
        {
            ni_assert(!IsNull());
            auto index = GetStructure().Components.GetComponentTypeIndexInChunk(componentType);
            if (index < 0)
                return nullptr;
            return GetComponentData(index);
        }


        /// <summary>
        /// Get the pointer to a component's memory array using the component's typename.
        /// For components with ComponentOwner_Node, the array will be at least the length of the size of the container.
        /// For components with ComponentOwner_Chunk, the array will be of length 1.
        /// This is a slow way to access the container's component data as it require to do a map lookup of the associated
        /// component typename const type_info* pointer to the component type index in the container's ChunkStructure ComponentTypeSet.
        /// </summary>
        /// <typeparam name="TComponent">Component typename for the desired component's memory array</typeparam>
        /// <returns>Pointer to the component memory array</returns>
        template<typename TComponent>
        TComponent* GetComponentData()
        {
            ni_assert(!IsNull());
            return (TComponent*)GetComponentData(&typeid(TComponent));
        }

        /// <summary>
        /// Get the const pointer to a component's memory array using the component's typename.
        /// For components with ComponentOwner_Node, the array will be at least the length of the size of the container.
        /// For components with ComponentOwner_Chunk, the array will be of length 1.
        /// This is a slow way to access the container's component data as it require to do a map lookup of the associated
        /// component typename const type_info* pointer to the component type index in the container's ChunkStructure ComponentTypeSet.
        /// </summary>
        /// <typeparam name="TComponent">Component typename for the desired component's memory array</typeparam>
        /// <returns>Const Pointer to the component memory array</returns>
        template<typename TComponent>
        const TComponent* GetComponentData()const
        {
            ni_assert(!IsNull());
            return (TComponent*)GetComponentData(&typeid(TComponent));
        }

    };
}
