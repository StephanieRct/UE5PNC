// MIT License
// Copyright (c) 2025 Stephanie Rancourt

#pragma once
#include "common.h"
#include "Node.h"

namespace PNC
{
    template<typename TChunkPointer, typename TBase>
    struct DChunkPointer : public TBase
    {
    public:
        using Base_t = TBase;
        using Self_t = DChunkPointer<TChunkPointer, TBase>;
        using typename Base_t::Size_t;
        using typename Base_t::ComponentType_t;
        using typename Base_t::ChunkStructure_t;
        using ChunkPointer_t = TChunkPointer;

        using Node_t = typename ChunkPointer_t::Node_t;

        using ChunkPointerInternal_t = typename ChunkPointer_t::ChunkPointerInternal_t;

    protected:
        ChunkPointer_t Chunk;

    public:
        using Base_t::Base_t;

        /// <summary>
        /// Create a VoidNull Container.
        /// </summary>
        DChunkPointer() = default;

        /// <summary>
        /// Create a StructNull Container
        /// </summary>
        DChunkPointer(const StructurePtr<ChunkStructure_t>& chunkStructure)
            : Base_t()
            , Chunk(chunkStructure)
        {
        }

        /// <summary>
        /// Create a StructData from a ComponentDataArray with a NodeCount
        /// </summary>
        DChunkPointer(const StructurePtr<ChunkStructure_t>& chunkStructure, const NodeCountT<Size_t> nodeCount, const PropComponentDataArray& componentDataArray)
            : Base_t()
            , Chunk(chunkStructure, nodeCount, componentDataArray)
        {
        }

    protected:
        DChunkPointer(const StructurePtr<ChunkStructure_t>& chunkStructure, const NodeCountT<Size_t> nodeCount)
            : Base_t()
            , Chunk(chunkStructure, nodeCount)
        {
        }

    protected:
        template<typename TProps>
        DChunkPointer(const DPropsTag& tag, const TProps& props)
            : Base_t(tag, props)
            , Chunk(tag, props)
        {
        }

    public:
        DChunkPointer(Self_t&& o) = default;
        Self_t& operator=(Self_t&& o) = default;
        DChunkPointer(const Self_t& o) = default;
        Self_t& operator=(const Self_t& o) = default;

    public:
        bool IsVoid()const { return Chunk.IsVoid(); }
        bool IsStruct()const { return Chunk.IsStruct(); }
        bool IsNull()const { return Chunk.IsNull(); }
        bool IsData()const { return Chunk.IsData(); }
        bool IsVoidNull()const { return Chunk.IsVoidNull(); }
        bool IsVoidData()const { return Chunk.IsVoidData(); }
        bool IsStructNull()const { return Chunk.IsStructNull(); }
        bool IsStructData()const { return Chunk.IsStructData(); }
        const ChunkStructure_t& GetStructure()const { return Chunk.GetStructure(); }
        NodeCountT<Size_t> GetNodeCount()const { return Chunk.GetNodeCount(); }
        NodeCapacityT<Size_t> GetNodeCapacity()const { return Chunk.GetNodeCapacity(); }
        ChunkCountT<Size_t> GetChunkCount()const { return Chunk.GetChunkCount(); }
        ChunkCapacityT<Size_t> GetChunkCapacity()const { return Chunk.GetChunkCapacity(); }
        void* GetComponentData(const Size_t componentTypeIndexInChunk) { return Chunk.GetComponentData(componentTypeIndexInChunk); }
        const void* GetComponentData(const Size_t componentTypeIndexInChunk)const { return Chunk.GetComponentData(componentTypeIndexInChunk); }
        ChunkPointer_t& GetChunk() { return Chunk; }
        const ChunkPointer_t& GetChunk()const { return Chunk; }

        static bool IsSameStructure(const Self_t& a, const Self_t& b) { return ChunkPointer_t::IsSameStructure(a.Chunk, b.Chunk); }
        static bool IsSameData(const Self_t& a, const Self_t& b) { return ChunkPointer_t::IsSameData(a.Chunk, b.Chunk); }


    public:
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
            pnc_assert(!IsNull());
            auto index = GetStructure().Components.GetComponentTypeIndexInChunk(componentType);
            if (index < 0)
                return nullptr;
            return Chunk.GetComponentData(index);
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
            pnc_assert(!IsNull());
            auto index = GetStructure().Components.GetComponentTypeIndexInChunk(componentType);
            if (index < 0)
                return nullptr;
            return Chunk.GetComponentData(index);
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
            pnc_assert(!IsNull());
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
            pnc_assert(!IsNull());
            return (TComponent*)GetComponentData(&typeid(TComponent));
        }


        static ChunkPointerInternal_t& GetInternalChunk(Self_t& a) { return ChunkPointer_t::GetInternalChunk(a.Chunk); }
        static const ChunkPointerInternal_t& GetInternalChunk(const Self_t& a) { return ChunkPointer_t::GetInternalChunk(a.Chunk); }

    protected:

        /// <summary>
        /// Notes:
        ///     Called by derived structs
        /// </summary>
        template<typename TContainer>
        static void AllocateComponentDataArray(TContainer& container)
        {
            pnc_assert(container.IsStruct());
            typename TContainer::ChunkPointerInternal_t& internalChunk = TContainer::GetInternalChunk(container);
            internalChunk.ComponentData = (void**)pnc_alloc(internalChunk.Structure->Components.GetSize() * sizeof(void*), alignof(void*));
        }

        /// <summary>
        /// Notes:
        ///     Called by derived structs
        /// </summary>
        template<typename TContainer>
        static void FreeComponentDataArray(TContainer& container)
        {
            pnc_assert(container.IsStruct());
            typename TContainer::ChunkPointerInternal_t& internalChunk = TContainer::GetInternalChunk(container);
            pnc_free_clean(internalChunk.ComponentData, internalChunk.Structure->Components.GetSize() * sizeof(void*), alignof(void*));
        }

        /// <summary>
        /// Notes:
        ///     Called by derived structs
        ///     Does not set the NodeCapacity nor NodeCount on container. It only allocate, construct and set componentData
        /// </summary>
        template<typename TContainer, typename TProps>
        static void AllocateConstruct(TContainer& container, const TProps& props)
        {
            const auto nodeCount = props.GetNodeCount();
            Node_t::AllocateConstructAllComponentsUnsafe(container, 0, 0, nodeCount, 1, PropCountToCapacity(nodeCount), 1);
        }

        /// <summary>
        /// Notes:
        ///     Called by derived structs
        ///     Does not set the NodeCapacity nor NodeCount on container. It only allocate, construct and set componentData
        ///     container and containerFrom cannot be the same container
        /// </summary>
        template<typename TContainer>
        static void AllocateCopy(TContainer& container, const TContainer& containerFrom, const NodeCountT<Size_t> nodeCount)
        {
            pnc_assert(!IsSameData(container, containerFrom));
            Node_t::AllocateCopyConstructAllComponentsForwardUnsafe(container, 0, 0, containerFrom, 0, 0, 
                nodeCount,                        ChunkCountT<Size_t>(1), 
                PropCountToCapacity(nodeCount), ChunkCapacityT<Size_t>(1));

        }

        /// <summary>
        /// Notes:
        ///     Called by derived structs
        ///     Does not set the NodeCapacity nor NodeCount on container. It only allocate, construct and set componentData
        /// </summary>
        template<typename TContainer>
        static void AllocateCopy(TContainer& container, const TContainer& containerFrom)
        {
            AllocateCopy(container, containerFrom, containerFrom.GetNodeCount());
        }

        /// <summary>
        /// Notes:
        ///     Called by derived structs
        ///     Will destruct and free data from chunkTo.
        ///     Does not set NodeCapacity nor NodeCount on chunkTo.
        ///     chunkTo cannot be the same as containerFrom
        ///     newNodeCapacity must be greater or equal to containerFrom.GetNodeCapacity()
        ///     if newNodeCount > containerFrom.GetNodeCount(), nodes after containerFrom.GetNodeCount() will not be constructed, caller is responsible for those nodes
        /// </summary>
        template<typename TContainer>
        static void ReallocateCopy(TContainer& containerToReallocate, const TContainer& containerFrom, const NodeCountT<Size_t> newNodeCount)
        {
            pnc_assert(newNodeCount >= containerFrom.GetNodeCount());
            pnc_assert(!IsSameData(containerToReallocate, containerFrom));
            Node_t::ReallocateCopyAllComponentsForwardUnsafe(containerToReallocate, 0/*:firstNodeIndexTo*/,            0/*:firstChunkIndexTo*/,
                                                             containerFrom,         0/*:firstNodeIndexFrom*/,          0/*:firstChunkIndexFrom*/,
                                                                                    newNodeCount,                      ChunkCountT<Size_t>(1),
                                                                                    PropCountToCapacity(newNodeCount), ChunkCapacityT<Size_t>(1));
        }
        template<typename TContainer>
        static void ReallocateCopy(TContainer& containerToReallocate, const TContainer& containerFrom)
        {
            return ReallocateCopy(containerToReallocate, containerFrom, containerFrom.GetNodeCount());
        }
        
        /// <summary>
        /// Notes:
        ///     Called by derived structs
        ///     Will destruct and free (or reuse) data from containerToReallocate.
        ///     Does not set NodeCapacity nor NodeCount on containerToReallocate.
        ///     containerToReallocate and containerFrom can be the same
        ///     newNodeCapacity must be greater or equal to containerFrom.GetNodeCapacity()
        ///     if newNodeCount > containerFrom.GetNodeCount(), nodes after containerFrom.GetNodeCount() will not be constructed, caller is responsible for those nodes.
        /// </summary>
        template<typename TContainer>
        static void ReallocateMove(TContainer& containerToReallocate, TContainer& containerFrom, const NodeCountT<Size_t> newNodeCount)
        {
            pnc_assert(!containerToReallocate.IsNull());
            pnc_assert(!containerFrom.IsNull());
            pnc_assert(IsSameStructure(containerToReallocate, containerFrom));
            pnc_assert(newNodeCount >= containerFrom.GetNodeCount());
            
            Node_t::ReallocateMoveAllComponentsForwardUnsafe(containerToReallocate, 0/*:firstNodeIndexTo*/,            0/*:firstChunkIndexTo*/,
                                                             containerFrom,         0/*:firstNodeIndexFrom*/,          0/*:firstChunkIndexFrom*/,
                                                                                    newNodeCount,                      ChunkCountT<Size_t>(1),
                                                                                    PropCountToCapacity(newNodeCount), ChunkCapacityT<Size_t>(1));
        }
        template<typename TContainer>
        static void ReallocateMove(TContainer& containerToReallocate, TContainer& containerFrom)
        {
            return ReallocateMove(containerToReallocate, containerFrom, containerFrom.GetNodeCount());
        }

        /// <summary>
        /// Notes:
        ///     Called by derived structs
        /// </summary>
        template<typename TContainer>
        static void FreeDestruct(TContainer& container, const NodeCapacityT<Size_t> nodeCapacity,   const NodeCountT<Size_t> nodeCount,  
                                                        const ChunkCapacityT<Size_t> chunkCapacity, const ChunkCountT<Size_t> chunkCount)
        {
            void** const componentDataArrayTo = TContainer::GetInternalChunk(container).ComponentData;
            const ChunkStructure_t& structure = container.GetStructure();
            auto componentCount = structure.GetComponentCount();
            for (Size_t i = 0; i < componentCount; ++i)
            {
                const ComponentType_t& componentType = structure.GetComponentType(i);
                componentType.DestructComponentUnsafe(/*base:*/componentDataArrayTo[i], 
                                                      /*firstNodexIndex:*/0,         /*firstChunkIndex:*/0,
                                                      /*nodeCount:      */nodeCount, /*chunkCount:     */chunkCount);
                pnc_free_clean(componentDataArrayTo[i], componentType.GetSize(nodeCapacity, chunkCapacity), componentType.GetAlignment());
            }
        }

        /// <summary>
        /// Notes:
        ///     Called by derived structs
        /// </summary>
        template<typename TContainer>
        static void FreeDestruct(TContainer& container)
        {
            const auto nodeCount = container.GetNodeCount();
            FreeDestruct(container, PropCountToCapacity(nodeCount), nodeCount, ChunkCapacityT<Size_t>(1), ChunkCountT<Size_t>(1));
        }
    };
}
