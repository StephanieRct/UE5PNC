// MIT License
// Copyright (c) 2025 Stephanie Rancourt

#pragma once
#include "common.h"

namespace PNC
{
    template<typename TChunkStructure>
    struct NodeT
    {
    public:
        using Self_t = NodeT<TChunkStructure>;
        using ChunkStructure_t = TChunkStructure;
        using ComponentType_t = typename ChunkStructure_t::ComponentType_t;
        using Size_t = typename ChunkStructure_t::Size_t;

    public:
        
        // can be called before index at (firstChunkIndex + chunkCount) is included by the container ChunkCount, 
        // making this function unsafe.
        template<typename TContainer>
        static void ConstructAllChunkComponentsUnsafe(TContainer& container, const Size_t firstChunkIndex = 0, const ChunkCountT<Size_t> chunkCount = 1)
        {
            pnc_assert(!container.IsNull());
            pnc_assert(firstChunkIndex >= 0);
            pnc_assert(chunkCount >= 0);

            if (chunkCount == 0) return;

            const ChunkStructure_t& chunkStructure = container.GetStructure();
#if defined(PNC_MEMORY_NODE_CONSTRUCTZERO_TRIVIAL) || defined(PNC_MEMORY_NODE_CONSTRUCTZERO)
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

        // can be called before index at (firstNodeIndex + nodeCount) is included by the container NodeCount, 
        // making this function unsafe.
        template<typename TContainer>
        static void ConstructAllNodeComponentsUnsafe(TContainer& container, const Size_t firstNodeIndex, const NodeCountT<Size_t> nodeCount)
        {
            pnc_assert(!container.IsNull());
            pnc_assert(firstNodeIndex >= 0);
            pnc_assert(nodeCount >= 0);

            if (nodeCount == 0) return;

            const ChunkStructure_t& chunkStructure = container.GetStructure();
#if defined(PNC_MEMORY_NODE_CONSTRUCTZERO_TRIVIAL) || defined(PNC_MEMORY_NODE_CONSTRUCTZERO)
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

        template<typename TContainer>
        static void ConstructAllComponentsUnsafe(TContainer& container,
                                                 const Size_t firstNodeIndex,      const NodeCountT<Size_t> nodeCount,
                                                 const Size_t firstChunkIndex = 0, const ChunkCountT<Size_t> chunkCount = 1)
        {
            pnc_assert(!container.IsNull());
            pnc_assert(firstNodeIndex >= 0);
            pnc_assert(nodeCount >= 0);
            pnc_assert(firstChunkIndex >= 0);
            pnc_assert(chunkCount >= 0);

            const ChunkStructure_t& chunkStructure = container.GetStructure();
            Size_t componentCount = chunkStructure.GetComponentCount();
            for (Size_t i = 0; i < componentCount; ++i)
            {
                const ComponentType_t& componentType = chunkStructure.GetComponentType(i);
                componentType.ConstructComponentUnsafe(container.GetComponentData(i), firstNodeIndex, firstChunkIndex, nodeCount, chunkCount);
            }
        }

        template<typename TContainer>
        static void DestructAllChunkComponentsUnsafe(TContainer& container, const Size_t firstChunkIndex = 0, const ChunkCountT<Size_t> chunkCount = 1)
        {
            pnc_assert(!container.IsNull());
            pnc_assert(firstChunkIndex >= 0);
            pnc_assert(chunkCount >= 0);

            if (chunkCount == 0) return;

            const ChunkStructure_t& chunkStructure = container.GetStructure();
#if defined(PNC_MEMORY_NODE_CONSTRUCTZERO_TRIVIAL) || defined(PNC_MEMORY_NODE_CONSTRUCTZERO)
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

        template<typename TContainer>
        static void DestructAllNodeComponentsUnsafe(TContainer& container, const Size_t firstNodeIndex, const NodeCountT<Size_t> nodeCount)
        {
            pnc_assert(!container.IsNull());
            pnc_assert(firstNodeIndex >= 0);
            pnc_assert(nodeCount >= 0);

            if (nodeCount == 0) return;

            const ChunkStructure_t& chunkStructure = container.GetStructure();
#if defined(PNC_MEMORY_NODE_CONSTRUCTZERO_TRIVIAL) || defined(PNC_MEMORY_NODE_CONSTRUCTZERO)
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

        template<typename TContainer>
        static void DestructAllComponentsUnsafe(TContainer& container, const Size_t firstNodeIndex, const Size_t firstChunkIndex,
                                                               const NodeCountT<Size_t> nodeCount,      const ChunkCountT<Size_t> chunkCount)
        {
            pnc_assert(!container.IsNull());
            pnc_assert(firstNodeIndex >= 0);
            pnc_assert(nodeCount >= 0);
            pnc_assert(firstChunkIndex >= 0);
            pnc_assert(chunkCount >= 0);

            const ChunkStructure_t& chunkStructure = container.GetStructure();
            Size_t componentCount = chunkStructure.GetComponentCount();
            for (Size_t i = 0; i < componentCount; ++i)
            {
                const ComponentType_t* componentType = chunkStructure.Components[i];
                componentType->DestructComponentUnsafe(container.GetComponentData(i), firstNodeIndex, firstChunkIndex, nodeCount, chunkCount);
            }
        }

        template<typename TContainer>
        static void MoveAllNodeComponentsForwardUnsafe(TContainer& containerTo,   const Size_t firstNodeIndexTo, 
                                                       TContainer& containerFrom, const Size_t firstNodeIndexFrom, 
                                                       const NodeCountT<Size_t> nodeCount)
        {
            pnc_assert(!containerTo.IsNull());
            pnc_assert(firstNodeIndexTo >= 0);
            pnc_assert(!containerFrom.IsNull());
            pnc_assert(firstNodeIndexFrom >= 0);
            pnc_assert(nodeCount >= 0);
            pnc_assert(IsSameStructure(containerTo, containerFrom));

            if (nodeCount == 0) return;

            const ChunkStructure_t& chunkStructure = containerTo.GetStructure();
            Size_t componentTypeCount = chunkStructure.NodeComponentIndex.size();
            for (Size_t i = 0; i < componentTypeCount; ++i)
            {
                Size_t index = chunkStructure.NodeComponentIndex[i];
                const ComponentType_t* componentType = chunkStructure.Components[index];
                void* componentDataTo = containerTo.GetComponentData(index);
                void* componentDataFrom = containerFrom.GetComponentData(index);
                componentType->MoveConstructDataForwardUnsafe(componentDataTo,   firstNodeIndexTo,
                                                              componentDataFrom, firstNodeIndexFrom, 
                                                              nodeCount);
            }
        }
        
        template<typename TContainer>
        static void MoveAllChunkComponentsForwardUnsafe(TContainer& containerTo,   const Size_t firstChunkIndexTo, 
                                                        TContainer& containerFrom, const Size_t firstChunkIndexFrom, 
                                                        const ChunkCountT<Size_t> chunkCount)
        {
            pnc_assert(!containerTo.IsNull());
            pnc_assert(firstChunkIndexTo >= 0);
            pnc_assert(!containerFrom.IsNull());
            pnc_assert(firstChunkIndexFrom >= 0);
            pnc_assert(chunkCount >= 0);
            pnc_assert(IsSameStructure(containerTo, containerFrom));

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








        
        template<typename TContainerTo, typename TContainerFrom>
        static void CopyConstructNodeComponentsForwardUnsafe(TContainerTo& containerTo,     const Size_t firstNodeIndexTo,
                                                       const TContainerFrom& containerFrom, const Size_t firstNodeIndexFrom, 
                                                       const NodeCountT<Size_t> nodeCount)
        {
            pnc_assert(!containerTo.IsNull());
            pnc_assert(!containerFrom.IsNull());
            pnc_assert(IsSameStructure(containerTo, containerFrom));
            pnc_assert(firstNodeIndexTo >= 0);
            pnc_assert(firstNodeIndexFrom >= 0);
            pnc_assert(nodeCount >= 0);
            //pnc_assert(firstNodeIndexTo < containerTo.GetNodeCount());
            //pnc_assert(firstNodeIndexTo + nodeCount <= containerTo.GetNodeCount());
            //pnc_assert(firstNodeIndexFrom < containerFrom.GetNodeCount());
            //pnc_assert(firstNodeIndexFrom + nodeCount <= containerFrom.GetNodeCount());

            const ChunkStructure_t& chunkStructure = containerTo.GetStructure();
            Size_t componentCount = chunkStructure.NodeComponentIndex.size();
            for (Size_t i = 0; i < componentCount; ++i)
            {
                Size_t index = chunkStructure.NodeComponentIndex[i];
                void* componentDataTo = containerTo.GetComponentData(index);
                void* componentDataFrom = containerFrom.GetComponentData(index);
                chunkStructure.Components[index]->CopyConstructDataForwardUnsafe(
                    componentDataTo,   firstNodeIndexTo, 
                    componentDataFrom, firstNodeIndexFrom, nodeCount);
            }
        }

        template<typename TContainerTo, typename TContainerFrom>
        static void CopyConstructChunkComponentsForwardUnsafe(TContainerTo& containerTo,     const Size_t firstChunkIndexTo,
                                                        const TContainerFrom& containerFrom, const Size_t firstChunkIndexFrom, 
                                                        const ChunkCountT<Size_t> chunkCount)
        {
            pnc_assert(!containerTo.IsNull());
            pnc_assert(!containerFrom.IsNull());
            pnc_assert(IsSameStructure(containerTo, containerFrom));
            pnc_assert(firstChunkIndexTo >= 0);
            pnc_assert(firstChunkIndexFrom >= 0);
            pnc_assert(chunkCount >= 0);
            //pnc_assert(firstChunkIndexTo < containerTo.GetChunkCount());
            //pnc_assert(firstChunkIndexTo + chunkCount <= containerTo.GetChunkCount());
            //pnc_assert(firstChunkIndexFrom < containerFrom.GetChunkCount());
            //pnc_assert(firstChunkIndexFrom + chunkCount <= containerFrom.GetChunkCount());


            const ChunkStructure_t& chunkStructure = containerTo.GetStructure();
            Size_t componentCount = chunkStructure.ChunkComponentIndex.size();
            for (Size_t i = 0; i < componentCount; ++i)
            {
                Size_t index = chunkStructure.ChunkComponentIndex[i];
                void* componentDataTo = containerTo.GetComponentData(index);
                void* componentDataFrom = containerFrom.GetComponentData(index);
                chunkStructure.Components[index]->CopyConstructDataForwardUnsafe(
                    componentDataTo,   firstChunkIndexTo, 
                    componentDataFrom, firstChunkIndexFrom, chunkCount);
            }
        }

        template<typename TContainerTo, typename TContainerFrom>
        static void CopyConstructAllComponentsForwardUnsafe(TContainerTo& containerTo,     const Size_t firstNodeIndexTo,   const Size_t firstChunkIndexTo,
                                                      const TContainerFrom& containerFrom, const Size_t firstNodeIndexFrom, const Size_t firstChunkIndexFrom,
                                                      const NodeCountT<Size_t> nodeCount, const ChunkCountT<Size_t> chunkCount)
        {
            pnc_assert(!containerTo.IsNull());
            pnc_assert(!containerFrom.IsNull());
            pnc_assert(IsSameStructure(containerTo, containerFrom));
            pnc_assert(firstNodeIndexTo >= 0);
            pnc_assert(firstNodeIndexFrom >= 0);
            pnc_assert(nodeCount >= 0);
            pnc_assert(firstChunkIndexTo >= 0);
            pnc_assert(firstChunkIndexFrom >= 0);
            pnc_assert(chunkCount >= 0);
            //pnc_assert(firstNodeIndexTo < containerTo.GetNodeCount());
            //pnc_assert(firstNodeIndexTo + nodeCount <= containerTo.GetNodeCount());
            //pnc_assert(firstChunkIndexTo < containerTo.GetChunkCount());
            //pnc_assert(firstChunkIndexTo + chunkCount <= containerTo.GetChunkCount());
            //pnc_assert(firstNodeIndexFrom < containerFrom.GetNodeCount());
            //pnc_assert(firstNodeIndexFrom + nodeCount <= containerFrom.GetNodeCount());
            //pnc_assert(firstChunkIndexFrom < containerFrom.GetChunkCount());
            //pnc_assert(firstChunkIndexFrom + chunkCount <= containerFrom.GetChunkCount());

            const ChunkStructure_t& chunkStructure = containerTo.GetStructure();
            Size_t componentCount = chunkStructure.GetComponentCount();
            for (Size_t i = 0; i < componentCount; ++i)
            {
                chunkStructure.Components[i]->CopyConstructComponentForwardUnsafe(
                    containerTo.GetComponentData(i),   firstNodeIndexTo,   firstChunkIndexTo,
                    containerFrom.GetComponentData(i), firstNodeIndexFrom, firstChunkIndexFrom, 
                    nodeCount, chunkCount);
            }
        }

        











        
        template<typename TContainer>
        static void AllocateConstructAllComponentsUnsafe(TContainer& container,
                                                    const Size_t firstNodeIndex, const Size_t firstChunkIndex,
                                                    const NodeCountT<Size_t> nodeCount,      const ChunkCountT<Size_t> chunkCount,
                                                    const NodeCapacityT<Size_t> nodeCapacity,   const ChunkCapacityT<Size_t> chunkCapacity)
        {
            pnc_assert(!container.IsNull());
            pnc_assert(firstNodeIndex >= 0);
            pnc_assert(nodeCount >= 0);
            pnc_assert(firstChunkIndex >= 0);
            pnc_assert(chunkCount >= 0);

            void** const componentDataArrayTo = TContainer::GetInternalChunk(container).ComponentData;

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
                componentDataArrayTo[i] = (void*)pnc_alloc(componentType.GetSize(nodeCapacity, chunkCapacity), componentType.GetAlignment());
                componentType.ConstructComponentUnsafe(componentDataArrayTo[i], firstNodeIndex, firstChunkIndex, nodeCount, chunkCount);
            }
        }



        template<typename TContainerTo, typename TContainerFrom>
        static void AllocateCopyConstructAllComponentsForwardUnsafe(TContainerTo&   containerTo,   const Size_t firstNodeIndexTo,   const Size_t firstChunkIndexTo,
                                                              const TContainerFrom& containerFrom, const Size_t firstNodeIndexFrom, const Size_t firstChunkIndexFrom,
                                                              const NodeCountT<Size_t> nodeCount,    const ChunkCountT<Size_t> chunkCount, 
                                                              const NodeCapacityT<Size_t> nodeCapacity, const ChunkCapacityT<Size_t> chunkCapacity)
        {
            pnc_assert(!containerTo.IsNull());
            pnc_assert(!containerFrom.IsNull());
            pnc_assert(IsSameStructure(containerTo, containerFrom));
            pnc_assert(!IsSameData(containerTo, containerFrom));
            pnc_assert(firstNodeIndexTo >= 0);
            pnc_assert(firstNodeIndexFrom >= 0);
            pnc_assert(nodeCount >= 0);
            pnc_assert(firstChunkIndexTo >= 0);
            pnc_assert(firstChunkIndexFrom >= 0);
            pnc_assert(chunkCount >= 0);
            pnc_assert(nodeCapacity >= nodeCount);
            pnc_assert(chunkCapacity >= chunkCount);

            void** const componentDataArrayTo = TContainerTo::GetInternalChunk(containerTo).ComponentData;

            const ChunkStructure_t& chunkStructure = containerTo.GetStructure();
            Size_t componentCount = chunkStructure.GetComponentCount();
            for (Size_t i = 0; i < componentCount; ++i)
            {
                const ComponentType_t& componentType = chunkStructure.GetComponentType(i);
                componentDataArrayTo[i] = (void*)pnc_alloc(componentType.GetSize(nodeCapacity, chunkCapacity), componentType.GetAlignment());
                componentType.CopyConstructComponentForwardUnsafe(
                    componentDataArrayTo[i],       firstNodeIndexTo,   firstChunkIndexTo,
                    containerFrom.GetComponentData(i), firstNodeIndexFrom, firstChunkIndexFrom, 
                    nodeCount, chunkCount);
            }
        }


        /// <summary>
        /// Reallocate containerToReallocate to fit the nodes copied from containerFrom
        /// Notes:
        ///     Will destruct and free nodes from containerToReallocate.
        ///     Does not set NodeCapacity / NodeCount / ChunkCapacity / ChunkCount on containerToReallocate.
        ///     containerToReallocate CANNOT be the same data as containerFrom
        /// </summary>
        template<typename TContainerTo, typename TContainerFrom>
        static void ReallocateCopyConstructAllComponentsForwardUnsafe(TContainerTo&   containerToReallocate, const Size_t firstNodeIndexTo,   const Size_t firstChunkIndexTo,
                                                                const TContainerFrom& containerFrom,         const Size_t firstNodeIndexFrom, const Size_t firstChunkIndexFrom,
                                                                const NodeCountT<Size_t> nodeCountToCopy,    const ChunkCountT<Size_t> chunkCountToCopy, 
                                                                const NodeCapacityT<Size_t> nodeCapacity, const ChunkCapacityT<Size_t> chunkCapacity)
        {
            pnc_assert(!containerToReallocate.IsNull());
            pnc_assert(!containerFrom.IsNull());
            pnc_assert(IsSameStructure(containerToReallocate, containerFrom));
            pnc_assert(!IsSameData(containerToReallocate, containerFrom));
            pnc_assert(firstNodeIndexTo >= 0);
            pnc_assert(firstNodeIndexFrom >= 0);
            pnc_assert(nodeCountToCopy >= 0);
            pnc_assert(firstChunkIndexTo >= 0);
            pnc_assert(firstChunkIndexFrom >= 0);
            pnc_assert(chunkCountToCopy >= 0);

            pnc_assert(firstNodeIndexFrom + nodeCountToCopy  <= containerFrom.GetNodeCount());
            pnc_assert(firstNodeIndexTo   + nodeCountToCopy  <= nodeCapacity);
            pnc_assert(firstChunkIndexTo  + chunkCountToCopy <= chunkCapacity);

            const auto nodeCountTo       = containerToReallocate.GetNodeCount();
            const auto nodeCapacityTo    = containerToReallocate.GetNodeCapacity();
            const auto chunkCountTo      = containerToReallocate.GetChunkCount();
            const auto chunkCapacityTo   = containerToReallocate.GetChunkCapacity();
            const auto chunkCapacityFrom = containerToReallocate.GetChunkCapacity();
            void** const componentDataArrayTo = TContainerTo::GetInternalChunk(containerToReallocate).ComponentData;
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
                    void* const dataNew = (void*)pnc_alloc(allocSize, componentType.GetAlignment());
                    componentType.CopyConstructComponentForwardUnsafe(
                                    dataNew,  firstNodeIndexTo,   firstChunkIndexTo,
                                    dataFrom, firstNodeIndexFrom, firstChunkIndexFrom,
                                              nodeCountToCopy,    chunkCountToCopy);
                    componentType.DestructComponentUnsafe(componentDataArrayTo[i], 
                                                          /*firstNodeIndex:*/0, /*firstChunkIndex:*/0, 
                                                          nodeCountTo,          chunkCountTo);
                    pnc_free_dirty(componentDataArrayTo[i], componentType.GetSize(nodeCapacityTo), componentType.GetAlignment());
                    componentDataArrayTo[i] = dataNew;
                }
            }
        }


        template<typename TContainerTo, typename TContainerFrom>
        static void MoveAllComponentsForward(TContainerTo& containerTo, const TContainerFrom& containerFrom,
            const Size_t firstNodeIndexTo, const Size_t firstNodeIndexFrom, const NodeCountT<Size_t> nodeCount,
            const Size_t firstChunkIndexTo = 0, const Size_t firstChunkIndexFrom = 0, const ChunkCountT<Size_t> chunkCount = 1)
        {
            pnc_assert(!containerTo.IsNull());
            pnc_assert(!containerFrom.IsNull());
            pnc_assert(IsSameStructure(containerTo, containerFrom));
            pnc_assert(firstNodeIndexTo >= 0);
            pnc_assert(firstNodeIndexFrom >= 0);
            pnc_assert(nodeCount >= 0);
            pnc_assert(firstChunkIndexTo >= 0);
            pnc_assert(firstChunkIndexFrom >= 0);
            pnc_assert(chunkCount >= 0);
            pnc_assert(firstNodeIndexTo < containerTo.GetNodeCount());
            pnc_assert(firstNodeIndexTo + nodeCount <= containerTo.GetNodeCount());
            pnc_assert(firstChunkIndexTo < containerTo.GetChunkCount());
            pnc_assert(firstChunkIndexTo + chunkCount <= containerTo.GetChunkCount());
            pnc_assert(firstNodeIndexFrom < containerFrom.GetNodeCount());
            pnc_assert(firstNodeIndexFrom + nodeCount <= containerFrom.GetNodeCount());
            pnc_assert(firstChunkIndexFrom < containerFrom.GetChunkCount());
            pnc_assert(firstChunkIndexFrom + chunkCount <= containerFrom.GetChunkCount());

            const ChunkStructure_t& chunkStructure = containerTo.GetStructure();
            const auto componentCount = chunkStructure.GetComponentCount();
            for (Size_t i = 0; i < componentCount; ++i)
            {
                const ComponentType_t* componentType = chunkStructure.Components[i];
                componentType->MoveComponentForwardUnsafe(containerTo.GetComponentData(i), containerFrom.GetComponentData(i),
                    firstNodeIndexTo, firstNodeIndexFrom, nodeCount,
                    firstChunkIndexTo, firstChunkIndexFrom, chunkCount);
            }
        }


        template<typename TChunkA, typename TChunkB>
        static bool IsSameStructure(const TChunkA& a, const TChunkB& b) { return a.GetStructure() == b.GetStructure(); }
        template<typename TChunkA, typename TChunkB>
        static bool IsSameData(const TChunkA& a, const TChunkB& b) { return TChunkA::IsSameData(a,b); }
        

    };
}
