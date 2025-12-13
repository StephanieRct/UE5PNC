// MIT License
// Copyright (c) 2025 Stephanie Rancourt

#pragma once
#include "common.h"

namespace NiT
{
    template<typename TChunkStructure>
    struct NodeT
    {
    public:
        using Self_t = NodeT<TChunkStructure>;
        using ChunkStructure_t = TChunkStructure;
        using Size_t = typename ChunkStructure_t::Size_t;
        using ComponentType_t = typename ChunkStructure_t::ComponentType_t;

    public:
        
        /// <summary>
        /// Construct only the ChunkComponents in a container.
        /// Notes:
        ///     It can be called before index at (firstChunkIndex + chunkCount) is included by 
        ///     the container ChunkCount, making this function unsafe.
        ///
        /// </summary>
        /// <typeparam name="TContainer"></typeparam>
        /// <param name="container"></param>
        /// <param name="firstChunkIndex"></param>
        /// <param name="chunkCount"></param>
        template<typename TContainer>
        static void ConstructAllChunkComponentsUnsafe(TContainer& container, 
                                                      const             Size_t firstChunkIndex, 
                                                      const ChunkCountT<Size_t> chunkCount)
        {
            ni_assert(!container.IsNull());
            ni_assert(firstChunkIndex >= 0);
            ni_assert(chunkCount >= 0);

            if (chunkCount == 0) return;

            const ChunkStructure_t& chunkStructure = container.GetStructure();
#if defined(NI_MEMORY_NODE_CONSTRUCTZERO_TRIVIAL) || defined(NI_MEMORY_NODE_CONSTRUCTZERO)
            Size_t componentTypeCount = chunkStructure.ChunkComponentIndex.size();
            for (Size_t i = 0; i < componentTypeCount; ++i)
            {
                Size_t index = chunkStructure.ChunkComponentIndex[i];
                const ComponentType_t* componentType = chunkStructure.Components[index];
                void* componentData = container.GetComponentData(index);
                componentType->ConstructDataUnsafe(componentData, firstChunkIndex, chunkCount);
            }
#else
            Size_t componentTypeCount = chunkStructure.DefaultConstructibleChunkIndex.size();
            for (Size_t i = 0; i < componentTypeCount; ++i)
            {
                Size_t index = chunkStructure.DefaultConstructibleChunkIndex[i];
                const ComponentType_t* componentType = chunkStructure.Components[index];
                void* componentData = container.GetComponentData(index);
                componentType->ConstructDataUnsafe(componentData, firstChunkIndex, chunkCount);
            }
#endif
        }

        /// <summary>
        /// Construct only the NodeComponents in a container.
        /// Notes:
        ///     It can be called before index at (firstNodeIndex + nodeCount) is included by 
        ///     the container NodeCount, making this function unsafe.
        ///
        /// </summary>
        template<typename TContainer>
        static void ConstructAllNodeComponentsUnsafe(TContainer& container, 
                                                     const            Size_t firstNodeIndex, 
                                                     const NodeCountT<Size_t> nodeCount)
        {
            ni_assert(!container.IsNull());
            ni_assert(firstNodeIndex >= 0);
            ni_assert(nodeCount >= 0);

            if (nodeCount == 0) return;

            const ChunkStructure_t& chunkStructure = container.GetStructure();
#if defined(NI_MEMORY_NODE_CONSTRUCTZERO_TRIVIAL) || defined(NI_MEMORY_NODE_CONSTRUCTZERO)
            Size_t componentTypeCount = chunkStructure.NodeComponentIndex.size();
            for (Size_t i = 0; i < componentTypeCount; ++i)
            {
                Size_t index = chunkStructure.NodeComponentIndex[i];
                const ComponentType_t* componentType = chunkStructure.Components[index];
                void* componentData = container.GetComponentData(index);
                componentType->ConstructDataUnsafe(componentData, firstNodeIndex, nodeCount);
            }
#else
            Size_t componentTypeCount = chunkStructure.DefaultConstructibleNodeIndex.size();
            for (Size_t i = 0; i < componentTypeCount; ++i)
            {
                Size_t index = chunkStructure.DefaultConstructibleNodeIndex[i];
                const ComponentType_t* componentType = chunkStructure.Components[index];
                void* componentData = container.GetComponentData(index);
                componentType->ConstructDataUnsafe(componentData, firstNodeIndex, nodeCount);
            }
#endif
        }

        /// <summary>
        /// Construct all components in a container.
        /// Notes:
        ///     It can be called before index at (firstNodeIndex + nodeCount) is included by 
        ///     the container NodeCount, making this function unsafe. 
        ///     Same for chunk index firstChunkIndex + chunkCount.
        /// </summary>
        template<typename TContainer>
        static void ConstructAllComponentsUnsafe(
                        TContainer& container, const Size_t firstNodeIndex,  const NodeCountT< Size_t> nodeCount,
                                               const Size_t firstChunkIndex, const ChunkCountT<Size_t> chunkCount)
        {
            ni_assert(!container.IsNull());
            ni_assert(firstNodeIndex >= 0);
            ni_assert(nodeCount >= 0);
            ni_assert(firstChunkIndex >= 0);
            ni_assert(chunkCount >= 0);

            const ChunkStructure_t& chunkStructure = container.GetStructure();
            Size_t componentCount = chunkStructure.GetComponentCount();
            for (Size_t i = 0; i < componentCount; ++i)
            {
                const ComponentType_t& componentType = chunkStructure.GetComponentType(i);
                componentType.ConstructComponentUnsafe(container.GetComponentData(i), firstNodeIndex, firstChunkIndex, nodeCount, chunkCount);
            }
        }
        /// <summary>
        /// Destruct all ChunkComponents
        /// </summary>
        template<typename TContainer>
        static void DestructAllChunkComponentsUnsafe(TContainer& container, 
                                                     const             Size_t firstChunkIndex, 
                                                     const ChunkCountT<Size_t> chunkCount)
        {
            ni_assert(!container.IsNull());
            ni_assert(firstChunkIndex >= 0);
            ni_assert(chunkCount >= 0);

            if (chunkCount == 0) return;

            const ChunkStructure_t& chunkStructure = container.GetStructure();
#if defined(NI_MEMORY_NODE_CONSTRUCTZERO_TRIVIAL) || defined(NI_MEMORY_NODE_CONSTRUCTZERO)
            Size_t componentTypeCount = chunkStructure.ChunkComponentIndex.size();
            for (Size_t i = 0; i < componentTypeCount; ++i)
            {
                Size_t index = chunkStructure.ChunkComponentIndex[i];
                const ComponentType_t* componentType = chunkStructure.Components[index];
                void* componentData = container.GetComponentData(index);
                componentType->DestructDataUnsafe(componentData, firstChunkIndex, chunkCount);
            }
#else
            Size_t componentTypeCount = chunkStructure.DestructibleChunkIndex.size();
            for (Size_t i = 0; i < componentTypeCount; ++i)
            {
                Size_t index = chunkStructure.DestructibleChunkIndex[i];
                const ComponentType_t* componentType = chunkStructure.Components[index];
                void* componentData = container.GetComponentData(index);
                componentType->DestructDataUnsafe(componentData, firstChunkIndex, chunkCount);
            }
#endif
        }

        /// <summary>
        /// Destruct all NodeComponents
        /// </summary>
        template<typename TContainer>
        static void DestructAllNodeComponentsUnsafe(TContainer& container, 
                                                    const            Size_t firstNodeIndex, 
                                                    const NodeCountT<Size_t> nodeCount)
        {
            ni_assert(!container.IsNull());
            ni_assert(firstNodeIndex >= 0);
            ni_assert(nodeCount >= 0);

            if (nodeCount == 0) return;

            const ChunkStructure_t& chunkStructure = container.GetStructure();
#if defined(NI_MEMORY_NODE_CONSTRUCTZERO_TRIVIAL) || defined(NI_MEMORY_NODE_CONSTRUCTZERO)
            Size_t componentTypeCount = chunkStructure.NodeComponentIndex.size();
            for (Size_t i = 0; i < componentTypeCount; ++i)
            {
                Size_t index = chunkStructure.NodeComponentIndex[i];
                const ComponentType_t* componentType = chunkStructure.Components[index];
                void* componentData = container.GetComponentData(index);
                componentType->DestructDataUnsafe(componentData, firstNodeIndex, nodeCount);
            }
#else
            Size_t componentTypeCount = chunkStructure.DestructibleNodeIndex.size();
            for (Size_t i = 0; i < componentTypeCount; ++i)
            {
                Size_t index = chunkStructure.DestructibleNodeIndex[i];
                const ComponentType_t* componentType = chunkStructure.Components[index];
                void* componentData = container.GetComponentData(index);
                componentType->DestructDataUnsafe(componentData, firstNodeIndex, nodeCount);
            }
#endif
        }

        /// <summary>
        /// Destruct All components
        /// </summary>
        template<typename TContainer>
        static void DestructAllComponentsUnsafe(
                        TContainer& container, const            Size_t  firstNodeIndex, const             Size_t  firstChunkIndex,
                                               const NodeCountT<Size_t> nodeCount,      const ChunkCountT<Size_t> chunkCount)
        {
            ni_assert(!container.IsNull());
            ni_assert(firstNodeIndex >= 0);
            ni_assert(nodeCount >= 0);
            ni_assert(firstChunkIndex >= 0);
            ni_assert(chunkCount >= 0);

            const ChunkStructure_t& chunkStructure = container.GetStructure();
            Size_t componentCount = chunkStructure.GetComponentCount();
            for (Size_t i = 0; i < componentCount; ++i)
            {
                const ComponentType_t* componentType = chunkStructure.Components[i];
                componentType->DestructComponentUnsafe(container.GetComponentData(i), firstNodeIndex, firstChunkIndex, nodeCount, chunkCount);
            }
        }

        /// <summary>
        /// Move all NodeComponents between 2 containers.
        /// Notes:
        ///     containerTo and containerFrom CAN be the same but the range of nodes must not overlap forward.
        /// </summary>
        template<typename TContainer>
        static void MoveAllNodeComponentsForwardUnsafe(
                        TContainer& containerTo,   const            Size_t firstNodeIndexTo, 
                        TContainer& containerFrom, const            Size_t firstNodeIndexFrom, 
                                                   const NodeCountT<Size_t> nodeCount)
        { 
            ni_assert(!containerTo.IsNull());
            ni_assert(firstNodeIndexTo >= 0);
            ni_assert(!containerFrom.IsNull());
            ni_assert(firstNodeIndexFrom >= 0);
            ni_assert(nodeCount >= 0);
            ni_assert(IsSameStructure(containerTo, containerFrom));

            if (nodeCount == 0) return;

            const ChunkStructure_t& chunkStructure = containerTo.GetStructure();
            Size_t componentTypeCount = chunkStructure.NodeComponentIndex.size();
            for (Size_t i = 0; i < componentTypeCount; ++i)
            {
                Size_t index = chunkStructure.NodeComponentIndex[i];
                const ComponentType_t* componentType = chunkStructure.Components[index];
                void* componentDataTo =   containerTo.  GetComponentData(index);
                void* componentDataFrom = containerFrom.GetComponentData(index);
                componentType->MoveConstructDataForwardUnsafe(componentDataTo,   firstNodeIndexTo,
                                                              componentDataFrom, firstNodeIndexFrom, 
                                                              nodeCount);
            }
        }

        /// <summary>
        /// Move all ChunkComponents between 2 containers.
        /// Notes:
        ///     containerTo and containerFrom CAN be the same but the range of chunks must not overlap forward.
        /// </summary>
        template<typename TContainer>
        static void MoveAllChunkComponentsForwardUnsafe(
                        TContainer& containerTo,   const             Size_t firstChunkIndexTo, 
                        TContainer& containerFrom, const             Size_t firstChunkIndexFrom, 
                                                   const ChunkCountT<Size_t> chunkCount)
        {
            ni_assert(!containerTo.IsNull());
            ni_assert(firstChunkIndexTo >= 0);
            ni_assert(!containerFrom.IsNull());
            ni_assert(firstChunkIndexFrom >= 0);
            ni_assert(chunkCount >= 0);
            ni_assert(IsSameStructure(containerTo, containerFrom));

            if (chunkCount == 0) return;

            const ChunkStructure_t& chunkStructure = containerTo.GetStructure();
            Size_t componentTypeCount = chunkStructure.ChunkComponentIndex.size();
            for (Size_t i = 0; i < componentTypeCount; ++i)
            {
                Size_t index = chunkStructure.ChunkComponentIndex[i];
                const ComponentType_t* componentType = chunkStructure.Components[index];
                void* componentDataTo = containerTo.GetComponentData(index);
                void* componentDataFrom = containerFrom.GetComponentData(index);
                componentType->MoveDataForwardUnsafe(componentDataTo,   firstChunkIndexTo,
                                                     componentDataFrom, firstChunkIndexFrom, 
                                                     chunkCount);
            }
        }

        /// <summary>
        /// Allocate and construct all components in a chunk.
        /// </summary>
        template<typename TContainer>
        static void AllocateConstructAllComponentsUnsafe(
                        TContainer& container, const               Size_t  firstNodeIndex, const                Size_t  firstChunkIndex,
                                               const NodeCapacityT<Size_t> nodeCapacity,   const ChunkCapacityT<Size_t> chunkCapacity,
                                               const NodeCountT   <Size_t> nodeCount,      const ChunkCountT   <Size_t> chunkCount)
        {
            ni_assert(!container.IsNull());
            ni_assert(firstNodeIndex >= 0);
            ni_assert(nodeCount >= 0);
            ni_assert(firstChunkIndex >= 0);
            ni_assert(chunkCount >= 0);

            void** const componentDataArrayTo = TContainer::GetInternalChunk(container).ComponentDataArray;

            if (nodeCount == 0)
            {
                // TODO: Construct only ChunkComponents
                //return;
            }
            const ChunkStructure_t& chunkStructure = container.GetStructure();
            Size_t componentCount = chunkStructure.GetComponentCount();
            for (Size_t i = 0; i < componentCount; ++i)
            {
                const ComponentType_t& componentType = chunkStructure.GetComponentType(i);
                componentDataArrayTo[i] = (void*)ni_alloc(componentType.GetSize(nodeCapacity, chunkCapacity), componentType.GetAlignment());
                componentType.ConstructComponentUnsafe(componentDataArrayTo[i], firstNodeIndex, firstChunkIndex, nodeCount, chunkCount);
            }
        }

        /// <summary>
        /// Allocate all components in the chunk and copy components' data from a different container with the same structure.
        /// </summary>
        template<typename TContainerTo, typename TContainerFrom>
        static void AllocateCopyConstructAllComponentsForwardUnsafe(
                              TContainerTo&   containerTo,   const               Size_t  firstNodeIndexTo,   const                Size_t  firstChunkIndexTo,
                        const TContainerFrom& containerFrom, const               Size_t  firstNodeIndexFrom, const                Size_t  firstChunkIndexFrom,
                                                             const NodeCapacityT<Size_t> nodeCapacity,       const ChunkCapacityT<Size_t> chunkCapacity,
                                                             const NodeCountT   <Size_t> nodeCount,          const ChunkCountT   <Size_t> chunkCount )
        {
            ni_assert(!containerTo.IsNull());
            ni_assert(!containerFrom.IsNull());
            ni_assert(IsSameStructure(containerTo, containerFrom));
            ni_assert(!IsSameData(containerTo, containerFrom));
            ni_assert(firstNodeIndexTo >= 0);
            ni_assert(firstNodeIndexFrom >= 0);
            ni_assert(nodeCount >= 0);
            ni_assert(firstChunkIndexTo >= 0);
            ni_assert(firstChunkIndexFrom >= 0);
            ni_assert(chunkCount >= 0);
            ni_assert(nodeCapacity >= nodeCount);
            ni_assert(chunkCapacity >= chunkCount);
            void** const componentDataArrayTo = TContainerTo::GetInternalChunk(containerTo).ComponentDataArray;
            const ChunkStructure_t& chunkStructure = containerTo.GetStructure();
            Size_t componentCount = chunkStructure.GetComponentCount();
            for (Size_t i = 0; i < componentCount; ++i)
            {
                const ComponentType_t& componentType = chunkStructure.GetComponentType(i);
                componentDataArrayTo[i] = (void*)ni_alloc(componentType.GetSize(nodeCapacity, chunkCapacity), componentType.GetAlignment());
                componentType.CopyConstructComponentForwardUnsafe(
                    componentDataArrayTo[i],           firstNodeIndexTo,   firstChunkIndexTo,
                    containerFrom.GetComponentData(i), firstNodeIndexFrom, firstChunkIndexFrom, 
                                                       nodeCount,          chunkCount);
            }
        }

        /// <summary>
        /// Reallocate containerToReallocate to fit the nodes copied from containerFrom
        /// Notes:
        ///     containerToReallocate CANNOT be the same data as containerFrom
        ///     Will destruct and free nodes from containerToReallocate.
        ///     Does not set NodeCapacity / NodeCount / ChunkCapacity / ChunkCount on containerToReallocate.
        /// </summary>
        template<typename TContainerTo, typename TContainerFrom>
        static void ReallocateCopyAllComponentsForwardUnsafe(
                              TContainerTo&   containerToReallocate, const               Size_t firstNodeIndexTo,   const                Size_t firstChunkIndexTo,
                        const TContainerFrom& containerFrom,         const               Size_t firstNodeIndexFrom, const                Size_t firstChunkIndexFrom, 
                                                                     const NodeCapacityT<Size_t> nodeCapacity,      const ChunkCapacityT<Size_t> chunkCapacity,
                                                                     const NodeCountT   <Size_t> nodeCountToCopy,   const ChunkCountT   <Size_t> chunkCountToCopy)
        {
            ni_assert(!containerToReallocate.IsNull());
            ni_assert(!containerFrom.IsNull());
            ni_assert(IsSameStructure(containerToReallocate, containerFrom));
            ni_assert(!IsSameData(containerToReallocate, containerFrom));
            ni_assert(firstNodeIndexTo >= 0);
            ni_assert(firstNodeIndexFrom >= 0);
            ni_assert(nodeCountToCopy >= 0);
            ni_assert(firstChunkIndexTo >= 0);
            ni_assert(firstChunkIndexFrom >= 0);
            ni_assert(chunkCountToCopy >= 0);
            ni_assert(firstNodeIndexFrom + nodeCountToCopy  <= containerFrom.GetNodeCount());
            ni_assert(firstNodeIndexTo   + nodeCountToCopy  <= nodeCapacity);
            ni_assert(firstChunkIndexTo  + chunkCountToCopy <= chunkCapacity);

            const auto nodeCountTo       = containerToReallocate.GetNodeCount();
            const auto nodeCapacityTo    = containerToReallocate.GetNodeCapacity();
            const auto chunkCountTo      = containerToReallocate.GetChunkCount();
            const auto chunkCapacityTo   = containerToReallocate.GetChunkCapacity();
            const auto chunkCapacityFrom = containerToReallocate.GetChunkCapacity();
            void** const componentDataArrayTo = TContainerTo::GetInternalChunk(containerToReallocate).ComponentDataArray;
            const ChunkStructure_t& structure = containerToReallocate.GetStructure();
            const auto componentCount = structure.GetComponentCount();
            for (Size_t i = 0; i < componentCount; ++i)
            {
                const void* const dataFrom = containerFrom.GetComponentData(i);
                const ComponentType_t& componentType = structure.GetComponentType(i);
                const Size_t allocSize = componentType.GetSize(nodeCapacity,   chunkCapacity);
                const Size_t freeSize  = componentType.GetSize(nodeCapacityTo, chunkCapacityTo);
                if(allocSize == freeSize)
                    componentType.CopyAssignComponentForwardUnsafe(
                                    componentDataArrayTo[i], firstNodeIndexTo,   firstChunkIndexTo,
                                    dataFrom,                firstNodeIndexFrom, firstChunkIndexFrom,
                                                             nodeCountToCopy,    chunkCountToCopy);
                else
                {
                    void* const dataNew = (void*)ni_alloc(allocSize, componentType.GetAlignment());
                    componentType.CopyConstructComponentForwardUnsafe(
                                    dataNew,  firstNodeIndexTo,   firstChunkIndexTo,
                                    dataFrom, firstNodeIndexFrom, firstChunkIndexFrom,
                                              nodeCountToCopy,    chunkCountToCopy);
                    componentType.DestructComponentUnsafe(componentDataArrayTo[i], 
                                                          0/*:firstNodeIndex*/, 0/*:firstChunkIndex*/,
                                                          nodeCountTo,          chunkCountTo);
                    ni_free_dirty(componentDataArrayTo[i], componentType.GetSize(nodeCapacityTo), componentType.GetAlignment());
                    componentDataArrayTo[i] = dataNew;
                }
            }
        }
        
        /// <summary>
        /// Reallocate containerToReallocate to fit the nodes moved from containerFrom
        /// Notes:
        ///     containerToReallocate and containerFrom must have the same structure
        ///     containerToReallocate CAN be the same as containerFrom 
        ///     Will destruct and free (or reuse) data from containerToReallocate.
        ///     Does not set NodeCapacity nor NodeCount on containerToReallocate.
        ///     nodeCountToMove must be less or equal to nodeCapacity.
        ///     chunkCountToMove must be less or equal to chunkCapacity.
        ///     firstNodeIndexFrom + nodeCountToMove must be less or equal to containerFrom.GetNodeCount()
        /// </summary>
        template<typename TContainerTo, typename TContainerFrom>
        static void ReallocateMoveAllComponentsForwardUnsafe(
                        TContainerTo&   containerToReallocate, const               Size_t  firstNodeIndexTo,   const                Size_t firstChunkIndexTo,
                        TContainerFrom& containerFrom,         const               Size_t  firstNodeIndexFrom, const                Size_t firstChunkIndexFrom,                                                              
                                                               const NodeCapacityT<Size_t> nodeCapacity,       const ChunkCapacityT<Size_t> chunkCapacity,
                                                               const NodeCountT   <Size_t> nodeCountToMove,    const ChunkCountT   <Size_t> chunkCountToMove)
        {
            ni_assert(!containerToReallocate.IsNull());
            ni_assert(!containerFrom.IsNull());
            ni_assert(IsSameStructure(containerToReallocate, containerFrom));
            ni_assert(firstNodeIndexTo >= 0);
            ni_assert(firstNodeIndexFrom >= 0);
            ni_assert(nodeCountToMove >= 0);
            ni_assert(firstChunkIndexTo >= 0);
            ni_assert(firstChunkIndexFrom >= 0);
            ni_assert(chunkCountToMove >= 0);
            ni_assert(firstNodeIndexFrom + nodeCountToMove  <= containerFrom.GetNodeCount());
            ni_assert(firstNodeIndexTo   + nodeCountToMove  <= nodeCapacity);
            ni_assert(firstChunkIndexTo  + chunkCountToMove <= chunkCapacity);

            const auto nodeCountTo       = containerToReallocate.GetNodeCount();
            const auto nodeCapacityTo    = containerToReallocate.GetNodeCapacity();
            const auto chunkCountTo      = containerToReallocate.GetChunkCount();
            const auto chunkCapacityTo   = containerToReallocate.GetChunkCapacity();
            const auto chunkCapacityFrom = containerToReallocate.GetChunkCapacity();
            void** const componentDataArrayTo = TContainerTo::GetInternalChunk(containerToReallocate).ComponentDataArray;
            const ChunkStructure_t& structure = containerToReallocate.GetStructure();
            const auto componentCount = structure.GetComponentCount();
            for (Size_t i = 0; i < componentCount; ++i)
            {
                void* const dataFrom = containerFrom.GetComponentData(i);
                const ComponentType_t& componentType = structure.GetComponentType(i);
                const Size_t allocSize = componentType.GetSize(nodeCapacity,   chunkCapacity);
                const Size_t freeSize  = componentType.GetSize(nodeCapacityTo, chunkCapacityTo);
                if(allocSize == freeSize) // Resuse the same buffer if it has the same size as the new buffer size
                {
                    void* const dataTo = componentDataArrayTo[i];
                    if(dataTo != dataFrom) // don't move if containerToReallocate and containerFrom are the same containers
                        componentType.MoveAssignComponentForwardUnsafe(
                                          dataTo,   firstNodeIndexTo,   firstChunkIndexTo,
                                          dataFrom, firstNodeIndexFrom, firstChunkIndexFrom,
                                                    nodeCountToMove,    chunkCountToMove);
                }
                else
                {
                    void* const dataNew = (void*)ni_alloc(allocSize, componentType.GetAlignment());
                    componentType.MoveConstructComponentForwardUnsafe(
                                      dataNew,  firstNodeIndexTo,   firstChunkIndexTo,
                                      dataFrom, firstNodeIndexFrom, firstChunkIndexFrom,
                                                nodeCountToMove,    chunkCountToMove);
                    componentType.DestructComponentUnsafe(
                                      componentDataArrayTo[i], 0/*:firstNodeIndex*/, 0/*:firstChunkIndex*/,
                                                               nodeCountTo,          chunkCountTo);
                    ni_free_dirty(componentDataArrayTo[i], componentType.GetSize(nodeCapacityTo), componentType.GetAlignment());
                    componentDataArrayTo[i] = dataNew;
                }
            }
        }

        template<typename TContainerA, typename TContainerB>
        static bool IsSameStructure(const TContainerA& a, const TContainerB& b) { return a.GetStructure() == b.GetStructure(); }
        template<typename TContainerA, typename TContainerB>
        static bool IsSameData(const TContainerA& a, const TContainerB& b) { return TContainerA::IsSameData(a,b); }
    };
}
