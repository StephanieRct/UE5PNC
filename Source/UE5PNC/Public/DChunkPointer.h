// MIT License
// Copyright (c) 2025 Stephanie Rancourt

#pragma once
#include "common.h"
#include "Node.h"

namespace PNC
{
    /// <summary>
    /// Adds a ChunkPointer member to the base container.
    /// Provides a user friendly way to get component data by types.
    /// Provides derived containers with methods to allocate/free/construct and destruct components
    /// </summary>
    template<typename TChunkPointer, typename TBase>
    struct DChunkPointer : public TBase
    {
    public:
        using Base_t = TBase;
        using Self_t = DChunkPointer<TChunkPointer, TBase>;
        using ChunkPointer_t = TChunkPointer;
        using typename Base_t::Size_t;
        using typename Base_t::ComponentType_t;
        using typename Base_t::ChunkStructure_t;
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
        
        const ChunkPointer_t& GetChunk()const { return Chunk; }
              ChunkPointer_t& GetChunk()      { return Chunk; }

    public:
        bool IsVoid      ()const { return Chunk.IsVoid(); }
        bool IsStruct    ()const { return Chunk.IsStruct(); }
        bool IsNull      ()const { return Chunk.IsNull(); }
        bool IsData      ()const { return Chunk.IsData(); }
        bool IsVoidNull  ()const { return Chunk.IsVoidNull(); }
        bool IsVoidData  ()const { return Chunk.IsVoidData(); }
        bool IsStructNull()const { return Chunk.IsStructNull(); }
        bool IsStructData()const { return Chunk.IsStructData(); }

        const ChunkStructure_t& GetStructure()const { return Chunk.GetStructure(); }

        NodeCountT    <Size_t> GetNodeCount    ()const { return Chunk.GetNodeCount(); }
        NodeCapacityT <Size_t> GetNodeCapacity ()const { return Chunk.GetNodeCapacity(); }
        ChunkCountT   <Size_t> GetChunkCount   ()const { return Chunk.GetChunkCount(); }
        ChunkCapacityT<Size_t> GetChunkCapacity()const { return Chunk.GetChunkCapacity(); }

        const void* GetComponentData(const Size_t componentTypeIndexInChunk)const { return Chunk.GetComponentData(componentTypeIndexInChunk); }
              void* GetComponentData(const Size_t componentTypeIndexInChunk)      { return Chunk.GetComponentData(componentTypeIndexInChunk); }

        static bool IsSameStructure(const Self_t& a, const Self_t& b) { return ChunkPointer_t::IsSameStructure(a.Chunk, b.Chunk); }
        static bool IsSameData     (const Self_t& a, const Self_t& b) { return ChunkPointer_t::IsSameData     (a.Chunk, b.Chunk); }

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

        static       ChunkPointerInternal_t& GetInternalChunk(      Self_t& a) { return ChunkPointer_t::GetInternalChunk(a.Chunk); }
        static const ChunkPointerInternal_t& GetInternalChunk(const Self_t& a) { return ChunkPointer_t::GetInternalChunk(a.Chunk); }

    protected:

        /// <summary>
        /// Set a container's ComponentDataArray to an allocated array of void* large enough to fit one per component in the container.
        /// This array is used to store the nodes and chunks component data pointer as void*.
        /// </summary>
        template<typename TContainer>
        static void AllocateComponentDataArray(TContainer& container)
        {
            pnc_assert(container.IsStruct());
            typename TContainer::ChunkPointerInternal_t& internalChunk = TContainer::GetInternalChunk(container);
            internalChunk.ComponentDataArray = (void**)pnc_alloc(internalChunk.Structure->Components.GetSize() * sizeof(void*), alignof(void*));
        }

        /// <summary>
        /// Free a container's ComponentDataArray
        /// </summary>
        template<typename TContainer>
        static void FreeComponentDataArray(TContainer& container)
        {
            pnc_assert(container.IsStruct());
            typename TContainer::ChunkPointerInternal_t& internalChunk = TContainer::GetInternalChunk(container);
            pnc_free_clean(internalChunk.ComponentDataArray, internalChunk.Structure->Components.GetSize() * sizeof(void*), alignof(void*));
        }

        /// <summary>
        /// Allocate and contruct all components in the chunk for props.GetNodeCount() nodes
        /// Notes:
        ///     container must be StructData.
        ///     Does not set the NodeCapacity nor NodeCount on container. 
        ///     Will Set the chunk's ComponentDataArray values to the newly allocated component data.
        /// </summary>
        template<typename TContainer, typename TProps>
        static void AllocateConstruct(TContainer& container, const TProps& props)
        {
            pnc_assert(container.IsStructData());
            const auto nodeCount = props.GetNodeCount();
            Node_t::AllocateConstructAllComponentsUnsafe(
                        container, 0/*:firstNodeIndex*/,           0/*:firstChunkIndex*/, 
                                   nodeCount,                      PropChunkCount   <Size_t>(1), 
                                   PropCountToCapacity(nodeCount), PropChunkCapacity<Size_t>(1));
        }

        /// <summary>
        /// Allocate all components in the chunk and copy components' data from a different container with the same structure.
        /// Notes:
        ///     containerTo and containerFrom must be StructData of the same structure.
        ///     Does not set the NodeCapacity nor NodeCount on container.
        ///     Will Set the chunk's ComponentDataArray values to the newly allocated component data.
        ///     containerTo and containerFrom CANNOT be the same container
        /// </summary>
        template<typename TContainer>
        static void AllocateCopy(TContainer& containerTo, const TContainer& containerFrom, const NodeCountT<Size_t> nodeCount)
        {
            pnc_assert(containerTo.IsStructData());
            pnc_assert(containerFrom.IsStructData());
            pnc_assert(!IsSameData(containerTo, containerFrom));
            Node_t::AllocateCopyConstructAllComponentsForwardUnsafe(
                        containerTo,   0/*:firstNodeIndexTo*/,         0/*:firstChunkIndexTo*/, 
                        containerFrom, 0/*:firstNodeIndexFrom*/,       0/*:firstChunkIndexFrom*/,
                                       nodeCount,                      PropChunkCount   <Size_t>(1),
                                       PropCountToCapacity(nodeCount), PropChunkCapacity<Size_t>(1));
        }
        template<typename TContainer>
        static void AllocateCopy(TContainer& containerTo, const TContainer& containerFrom)
        {
            AllocateCopy(containerTo, containerFrom, containerFrom.GetNodeCount());
        }

        /// <summary>
        /// Reallocate all components and copy their data from a different container with the same structure
        /// Notes:
        ///     containerToReallocate and containerFrom must be StructData of the same structure.
        ///     Will destruct all components's data in containerToReallocate.
        ///     Will free the components's memory in containerToReallocate if they are not the same size as the new memory.
        ///     Will reuse the components' memory in containerToReallocate if they are the same size as the new memory.
        ///     Does not set NodeCapacity nor NodeCount on containerToReallocate.
        ///     containerToReallocate CANNOT be the same as containerFrom
        ///     copyNodeCount must be less or equal to containerFrom.GetNodeCount()
        /// </summary>
        template<typename TContainer>
        static void ReallocateCopy(TContainer& containerToReallocate, const TContainer& containerFrom, const NodeCountT<Size_t> copyNodeCount)
        {
            pnc_assert(containerToReallocate.IsStructData());
            pnc_assert(containerFrom.IsStructData());
            pnc_assert(copyNodeCount <= containerFrom.GetNodeCount());
            pnc_assert(!IsSameData(containerToReallocate, containerFrom));
            Node_t::ReallocateCopyAllComponentsForwardUnsafe(
                        containerToReallocate, 0/*:firstNodeIndexTo*/,             0/*:firstChunkIndexTo*/,
                        containerFrom,         0/*:firstNodeIndexFrom*/,           0/*:firstChunkIndexFrom*/,
                                               copyNodeCount,                      ChunkCountT   <Size_t>(1),
                                               PropCountToCapacity(copyNodeCount), ChunkCapacityT<Size_t>(1));
        }
        template<typename TContainer>
        static void ReallocateCopy(TContainer& containerToReallocate, const TContainer& containerFrom)
        {
            return ReallocateCopy(containerToReallocate, containerFrom, containerFrom.GetNodeCount());
        }
        
        /// <summary>
        /// Notes:
        ///     containerToReallocate and containerFrom must be StructData of the same structure.
        ///     Will free the components's memory in containerToReallocate if they are not the same size as the new memory.
        ///     Will reuse the components' memory in containerToReallocate if they are the same size as the new memory.
        ///     Does not set NodeCapacity nor NodeCount on containerToReallocate.
        ///     containerToReallocate and containerFrom CAN be the same.
        /// </summary>
        template<typename TContainer>
        static void ReallocateMove(TContainer& containerToReallocate, TContainer& containerFrom, const NodeCountT<Size_t> moveNodeCount)
        {
            pnc_assert(containerToReallocate.IsStructData());
            pnc_assert(containerFrom.IsStructData());
            pnc_assert(IsSameStructure(containerToReallocate, containerFrom));
            pnc_assert(moveNodeCount <= containerFrom.GetNodeCount());
            
            Node_t::ReallocateMoveAllComponentsForwardUnsafe(
                        containerToReallocate, 0/*:firstNodeIndexTo*/,             0/*:firstChunkIndexTo*/,
                        containerFrom,         0/*:firstNodeIndexFrom*/,           0/*:firstChunkIndexFrom*/,
                                               moveNodeCount,                      ChunkCountT   <Size_t>(1),
                                               PropCountToCapacity(moveNodeCount), ChunkCapacityT<Size_t>(1));
        }
        template<typename TContainer>
        static void ReallocateMove(TContainer& containerToReallocate, TContainer& containerFrom)
        {
            return ReallocateMove(containerToReallocate, containerFrom, containerFrom.GetNodeCount());
        }

        /// <summary>
        /// Destruct NodeComponents of nodes in [0, nodeCount[ and ChunkComponents from [0, chunkCount[ and free their memory.
        /// </summary>
        template<typename TContainer>
        static void FreeDestruct(TContainer& container, const NodeCapacityT <Size_t> nodeCapacity,  const NodeCountT <Size_t> nodeCount,  
                                                        const ChunkCapacityT<Size_t> chunkCapacity, const ChunkCountT<Size_t> chunkCount)
        {
            void** const componentDataArrayTo = TContainer::GetInternalChunk(container).ComponentDataArray;
            const ChunkStructure_t& structure = container.GetStructure();
            auto componentCount = structure.GetComponentCount();
            for (Size_t i = 0; i < componentCount; ++i)
            {
                const ComponentType_t& componentType = structure.GetComponentType(i);
                componentType.DestructComponentUnsafe(
                                  componentDataArrayTo[i], 0/*:firstNodexIndex*/, 0/*:firstChunkIndex*/,
                                                           nodeCount,             chunkCount);
                pnc_free_clean(componentDataArrayTo[i], componentType.GetSize(nodeCapacity, chunkCapacity), componentType.GetAlignment());
            }
        }

        /// <summary>
        /// Destruct all components and free their memory.
        /// </summary>
        template<typename TContainer>
        static void FreeDestruct(TContainer& container)
        {
            const auto nodeCount = container.GetNodeCount();
            FreeDestruct(container, PropCountToCapacity(nodeCount), nodeCount, ChunkCapacityT<Size_t>(1), ChunkCountT<Size_t>(1));
        }
    };
}
