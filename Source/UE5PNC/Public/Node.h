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
        
        // can be called before index at (firstChunkIndex + chunkCount) is included by the chunk ChunkCount, 
        // making this function unsafe.
        template<typename TChunk>
        static void ConstructAllChunkComponentsUnsafe(TChunk& chunk, const Size_t firstChunkIndex = 0, const ChunkCountT<Size_t> chunkCount = 1)
        {
            pnc_assert(!chunk.IsNull());
            pnc_assert(firstChunkIndex >= 0);
            pnc_assert(chunkCount >= 0);

            if (chunkCount == 0) return;

            const ChunkStructure_t& chunkStructure = chunk.GetStructure();
#if defined(PNC_MEMORY_NODE_CONSTRUCTZERO_TRIVIAL) || defined(PNC_MEMORY_NODE_CONSTRUCTZERO)
            Size_t componentTypeCount = chunkStructure.ChunkComponentIndex.size();
            for (Size_t i = 0; i < componentTypeCount; ++i)
            {
                Size_t index = chunkStructure.ChunkComponentIndex[i];
                const ComponentType_t* componentType = chunkStructure.Components[index];
                void* componentData = chunk.GetComponentData(index);
                componentType->ConstructDataUnsafe(componentData, firstChunkIndex, chunkCount);
            }
#else
            Size_t componentTypeCount = chunkStructure.DefaultConstructibleChunkIndex.size();
            for (Size_t i = 0; i < componentTypeCount; ++i)
            {
                Size_t index = chunkStructure.DefaultConstructibleChunkIndex[i];
                const ComponentType_t* componentType = chunkStructure.Components[index];
                void* componentData = chunk.GetComponentData(index);
                componentType->ConstructDataUnsafe(componentData, firstChunkIndex, chunkCount);
            }
#endif
        }

        // can be called before index at (firstNodeIndex + nodeCount) is included by the chunk NodeCount, 
        // making this function unsafe.
        template<typename TChunk>
        static void ConstructAllNodeComponentsUnsafe(TChunk& chunk, const Size_t firstNodeIndex, const NodeCountT<Size_t> nodeCount)
        {
            pnc_assert(!chunk.IsNull());
            pnc_assert(firstNodeIndex >= 0);
            pnc_assert(nodeCount >= 0);

            if (nodeCount == 0) return;

            const ChunkStructure_t& chunkStructure = chunk.GetStructure();
#if defined(PNC_MEMORY_NODE_CONSTRUCTZERO_TRIVIAL) || defined(PNC_MEMORY_NODE_CONSTRUCTZERO)
            Size_t componentTypeCount = chunkStructure.NodeComponentIndex.size();
            for (Size_t i = 0; i < componentTypeCount; ++i)
            {
                Size_t index = chunkStructure.NodeComponentIndex[i];
                const ComponentType_t* componentType = chunkStructure.Components[index];
                void* componentData = chunk.GetComponentData(index);
                componentType->ConstructDataUnsafe(componentData, firstNodeIndex, nodeCount);
            }
#else
            Size_t componentTypeCount = chunkStructure.DefaultConstructibleNodeIndex.size();
            for (Size_t i = 0; i < componentTypeCount; ++i)
            {
                Size_t index = chunkStructure.DefaultConstructibleNodeIndex[i];
                const ComponentType_t* componentType = chunkStructure.Components[index];
                void* componentData = chunk.GetComponentData(index);
                componentType->ConstructDataUnsafe(componentData, firstNodeIndex, nodeCount);
            }
#endif
        }

        template<typename TChunk>
        static void ConstructAllComponentsUnsafe(TChunk& chunk,
                                                 const Size_t firstNodeIndex,      const NodeCountT<Size_t> nodeCount,
                                                 const Size_t firstChunkIndex = 0, const ChunkCountT<Size_t> chunkCount = 1)
        {
            pnc_assert(!chunk.IsNull());
            pnc_assert(firstNodeIndex >= 0);
            pnc_assert(nodeCount >= 0);
            pnc_assert(firstChunkIndex >= 0);
            pnc_assert(chunkCount >= 0);

            const ChunkStructure_t& chunkStructure = chunk.GetStructure();
            Size_t componentCount = chunkStructure.GetComponentCount();
            for (Size_t i = 0; i < componentCount; ++i)
            {
                const ComponentType_t& componentType = chunkStructure.GetComponentType(i);
                componentType.ConstructComponentUnsafe(chunk.GetComponentData(i), firstNodeIndex, firstChunkIndex, nodeCount, chunkCount);
            }
        }

        template<typename TChunk>
        static void DestructAllChunkComponentsUnsafe(TChunk& chunk, const Size_t firstChunkIndex = 0, const ChunkCountT<Size_t> chunkCount = 1)
        {
            pnc_assert(!chunk.IsNull());
            pnc_assert(firstChunkIndex >= 0);
            pnc_assert(chunkCount >= 0);

            if (chunkCount == 0) return;

            const ChunkStructure_t& chunkStructure = chunk.GetStructure();
#if defined(PNC_MEMORY_NODE_CONSTRUCTZERO_TRIVIAL) || defined(PNC_MEMORY_NODE_CONSTRUCTZERO)
            Size_t componentTypeCount = chunkStructure.ChunkComponentIndex.size();
            for (Size_t i = 0; i < componentTypeCount; ++i)
            {
                Size_t index = chunkStructure.ChunkComponentIndex[i];
                const ComponentType_t* componentType = chunkStructure.Components[index];
                void* componentData = chunk.GetComponentData(index);
                componentType->DestructDataUnsafe(componentData, firstChunkIndex, chunkCount);
            }
#else
            Size_t componentTypeCount = chunkStructure.DestructibleChunkIndex.size();
            for (Size_t i = 0; i < componentTypeCount; ++i)
            {
                Size_t index = chunkStructure.DestructibleChunkIndex[i];
                const ComponentType_t* componentType = chunkStructure.Components[index];
                void* componentData = chunk.GetComponentData(index);
                componentType->DestructDataUnsafe(componentData, firstChunkIndex, chunkCount);
            }
#endif
        }

        template<typename TChunk>
        static void DestructAllNodeComponentsUnsafe(TChunk& chunk, const Size_t firstNodeIndex, const NodeCountT<Size_t> nodeCount)
        {
            pnc_assert(!chunk.IsNull());
            pnc_assert(firstNodeIndex >= 0);
            pnc_assert(nodeCount >= 0);

            if (nodeCount == 0) return;

            const ChunkStructure_t& chunkStructure = chunk.GetStructure();
#if defined(PNC_MEMORY_NODE_CONSTRUCTZERO_TRIVIAL) || defined(PNC_MEMORY_NODE_CONSTRUCTZERO)
            Size_t componentTypeCount = chunkStructure.NodeComponentIndex.size();
            for (Size_t i = 0; i < componentTypeCount; ++i)
            {
                Size_t index = chunkStructure.NodeComponentIndex[i];
                const ComponentType_t* componentType = chunkStructure.Components[index];
                void* componentData = chunk.GetComponentData(index);
                componentType->DestructDataUnsafe(componentData, firstNodeIndex, nodeCount);
            }
#else
            Size_t componentTypeCount = chunkStructure.DestructibleNodeIndex.size();
            for (Size_t i = 0; i < componentTypeCount; ++i)
            {
                Size_t index = chunkStructure.DestructibleNodeIndex[i];
                const ComponentType_t* componentType = chunkStructure.Components[index];
                void* componentData = chunk.GetComponentData(index);
                componentType->DestructDataUnsafe(componentData, firstNodeIndex, nodeCount);
            }
#endif
        }

        template<typename TChunk>
        static void DestructAllComponentsUnsafe(TChunk& chunk, const Size_t firstNodeIndex, const Size_t firstChunkIndex,
                                                               const NodeCountT<Size_t> nodeCount,      const ChunkCountT<Size_t> chunkCount)
        {
            pnc_assert(!chunk.IsNull());
            pnc_assert(firstNodeIndex >= 0);
            pnc_assert(nodeCount >= 0);
            pnc_assert(firstChunkIndex >= 0);
            pnc_assert(chunkCount >= 0);

            const ChunkStructure_t& chunkStructure = chunk.GetStructure();
            Size_t componentCount = chunkStructure.GetComponentCount();
            for (Size_t i = 0; i < componentCount; ++i)
            {
                const ComponentType_t* componentType = chunkStructure.Components[i];
                componentType->DestructComponentUnsafe(chunk.GetComponentData(i), firstNodeIndex, firstChunkIndex, nodeCount, chunkCount);
            }
        }

        template<typename TChunk>
        static void MoveAllNodeComponentsForwardUnsafe(TChunk& chunkTo,   const Size_t firstNodeIndexTo, 
                                                       TChunk& chunkFrom, const Size_t firstNodeIndexFrom, 
                                                       const NodeCountT<Size_t> nodeCount)
        {
            pnc_assert(!chunkTo.IsNull());
            pnc_assert(firstNodeIndexTo >= 0);
            pnc_assert(!chunkFrom.IsNull());
            pnc_assert(firstNodeIndexFrom >= 0);
            pnc_assert(nodeCount >= 0);
            pnc_assert(IsSameStructure(chunkTo, chunkFrom));

            if (nodeCount == 0) return;

            const ChunkStructure_t& chunkStructure = chunkTo.GetStructure();
            Size_t componentTypeCount = chunkStructure.NodeComponentIndex.size();
            for (Size_t i = 0; i < componentTypeCount; ++i)
            {
                Size_t index = chunkStructure.NodeComponentIndex[i];
                const ComponentType_t* componentType = chunkStructure.Components[index];
                void* componentDataTo = chunkTo.GetComponentData(index);
                void* componentDataFrom = chunkFrom.GetComponentData(index);
                componentType->MoveConstructDataForwardUnsafe(componentDataTo,   firstNodeIndexTo,
                                                              componentDataFrom, firstNodeIndexFrom, 
                                                              nodeCount);
            }
        }
        
        template<typename TChunk>
        static void MoveAllChunkComponentsForwardUnsafe(TChunk& chunkTo,   const Size_t firstChunkIndexTo, 
                                                        TChunk& chunkFrom, const Size_t firstChunkIndexFrom, 
                                                        const ChunkCountT<Size_t> chunkCount)
        {
            pnc_assert(!chunkTo.IsNull());
            pnc_assert(firstChunkIndexTo >= 0);
            pnc_assert(!chunkFrom.IsNull());
            pnc_assert(firstChunkIndexFrom >= 0);
            pnc_assert(chunkCount >= 0);
            pnc_assert(IsSameStructure(chunkTo, chunkFrom));

            if (chunkCount == 0) return;

            const ChunkStructure_t& chunkStructure = chunkTo.GetStructure();
            Size_t componentTypeCount = chunkStructure.ChunkComponentIndex.size();
            for (Size_t i = 0; i < componentTypeCount; ++i)
            {
                Size_t index = chunkStructure.ChunkComponentIndex[i];
                const ComponentType_t* componentType = chunkStructure.Components[index];
                void* componentDataTo = chunkTo.GetComponentData(index);
                void* componentDataFrom = chunkFrom.GetComponentData(index);
                componentType->MoveDataForwardUnsafe(componentDataTo,   firstChunkIndexTo,
                                                     componentDataFrom, firstChunkIndexFrom, 
                                                     chunkCount);
            }
        }








        
        template<typename TChunkTo, typename TChunkFrom>
        static void CopyConstructNodeComponentsForwardUnsafe(TChunkTo& chunkTo,     const Size_t firstNodeIndexTo,
                                                       const TChunkFrom& chunkFrom, const Size_t firstNodeIndexFrom, 
                                                       const NodeCountT<Size_t> nodeCount)
        {
            pnc_assert(!chunkTo.IsNull());
            pnc_assert(!chunkFrom.IsNull());
            pnc_assert(IsSameStructure(chunkTo, chunkFrom));
            pnc_assert(firstNodeIndexTo >= 0);
            pnc_assert(firstNodeIndexFrom >= 0);
            pnc_assert(nodeCount >= 0);
            //pnc_assert(firstNodeIndexTo < chunkTo.GetNodeCount());
            //pnc_assert(firstNodeIndexTo + nodeCount <= chunkTo.GetNodeCount());
            //pnc_assert(firstNodeIndexFrom < chunkFrom.GetNodeCount());
            //pnc_assert(firstNodeIndexFrom + nodeCount <= chunkFrom.GetNodeCount());

            const ChunkStructure_t& chunkStructure = chunkTo.GetStructure();
            Size_t componentCount = chunkStructure.NodeComponentIndex.size();
            for (Size_t i = 0; i < componentCount; ++i)
            {
                Size_t index = chunkStructure.NodeComponentIndex[i];
                void* componentDataTo = chunkTo.GetComponentData(index);
                void* componentDataFrom = chunkFrom.GetComponentData(index);
                chunkStructure.Components[index]->CopyConstructDataForwardUnsafe(
                    componentDataTo,   firstNodeIndexTo, 
                    componentDataFrom, firstNodeIndexFrom, nodeCount);
            }
        }

        template<typename TChunkTo, typename TChunkFrom>
        static void CopyConstructChunkComponentsForwardUnsafe(TChunkTo& chunkTo,     const Size_t firstChunkIndexTo,
                                                        const TChunkFrom& chunkFrom, const Size_t firstChunkIndexFrom, 
                                                        const ChunkCountT<Size_t> chunkCount)
        {
            pnc_assert(!chunkTo.IsNull());
            pnc_assert(!chunkFrom.IsNull());
            pnc_assert(IsSameStructure(chunkTo, chunkFrom));
            pnc_assert(firstChunkIndexTo >= 0);
            pnc_assert(firstChunkIndexFrom >= 0);
            pnc_assert(chunkCount >= 0);
            //pnc_assert(firstChunkIndexTo < chunkTo.GetChunkCount());
            //pnc_assert(firstChunkIndexTo + chunkCount <= chunkTo.GetChunkCount());
            //pnc_assert(firstChunkIndexFrom < chunkFrom.GetChunkCount());
            //pnc_assert(firstChunkIndexFrom + chunkCount <= chunkFrom.GetChunkCount());


            const ChunkStructure_t& chunkStructure = chunkTo.GetStructure();
            Size_t componentCount = chunkStructure.ChunkComponentIndex.size();
            for (Size_t i = 0; i < componentCount; ++i)
            {
                Size_t index = chunkStructure.ChunkComponentIndex[i];
                void* componentDataTo = chunkTo.GetComponentData(index);
                void* componentDataFrom = chunkFrom.GetComponentData(index);
                chunkStructure.Components[index]->CopyConstructDataForwardUnsafe(
                    componentDataTo,   firstChunkIndexTo, 
                    componentDataFrom, firstChunkIndexFrom, chunkCount);
            }
        }

        template<typename TChunkTo, typename TChunkFrom>
        static void CopyConstructAllComponentsForwardUnsafe(TChunkTo& chunkTo,     const Size_t firstNodeIndexTo,   const Size_t firstChunkIndexTo,
                                                      const TChunkFrom& chunkFrom, const Size_t firstNodeIndexFrom, const Size_t firstChunkIndexFrom,
                                                      const NodeCountT<Size_t> nodeCount, const ChunkCountT<Size_t> chunkCount)
        {
            pnc_assert(!chunkTo.IsNull());
            pnc_assert(!chunkFrom.IsNull());
            pnc_assert(IsSameStructure(chunkTo, chunkFrom));
            pnc_assert(firstNodeIndexTo >= 0);
            pnc_assert(firstNodeIndexFrom >= 0);
            pnc_assert(nodeCount >= 0);
            pnc_assert(firstChunkIndexTo >= 0);
            pnc_assert(firstChunkIndexFrom >= 0);
            pnc_assert(chunkCount >= 0);
            //pnc_assert(firstNodeIndexTo < chunkTo.GetNodeCount());
            //pnc_assert(firstNodeIndexTo + nodeCount <= chunkTo.GetNodeCount());
            //pnc_assert(firstChunkIndexTo < chunkTo.GetChunkCount());
            //pnc_assert(firstChunkIndexTo + chunkCount <= chunkTo.GetChunkCount());
            //pnc_assert(firstNodeIndexFrom < chunkFrom.GetNodeCount());
            //pnc_assert(firstNodeIndexFrom + nodeCount <= chunkFrom.GetNodeCount());
            //pnc_assert(firstChunkIndexFrom < chunkFrom.GetChunkCount());
            //pnc_assert(firstChunkIndexFrom + chunkCount <= chunkFrom.GetChunkCount());

            const ChunkStructure_t& chunkStructure = chunkTo.GetStructure();
            Size_t componentCount = chunkStructure.GetComponentCount();
            for (Size_t i = 0; i < componentCount; ++i)
            {
                chunkStructure.Components[i]->CopyConstructComponentForwardUnsafe(
                    chunkTo.GetComponentData(i),   firstNodeIndexTo,   firstChunkIndexTo,
                    chunkFrom.GetComponentData(i), firstNodeIndexFrom, firstChunkIndexFrom, 
                    nodeCount, chunkCount);
            }
        }

        











        
        template<typename TChunk>
        static void AllocateConstructAllComponentsUnsafe(TChunk& chunk,
                                                    const Size_t firstNodeIndex, const Size_t firstChunkIndex,
                                                    const NodeCountT<Size_t> nodeCount,      const ChunkCountT<Size_t> chunkCount,
                                                    const NodeCapacityT<Size_t> nodeCapacity,   const ChunkCapacityT<Size_t> chunkCapacity)
        {
            pnc_assert(!chunk.IsNull());
            pnc_assert(firstNodeIndex >= 0);
            pnc_assert(nodeCount >= 0);
            pnc_assert(firstChunkIndex >= 0);
            pnc_assert(chunkCount >= 0);

            void** const componentDataArrayTo = TChunk::GetInternalChunk(chunk).ComponentData;

            if (nodeCount == 0)
            {
                // TODO: Construct only ChunkComponents
                //return;
            }
            const ChunkStructure_t& chunkStructure = chunk.GetStructure();
            Size_t componentCount = chunkStructure.GetComponentCount();
            for (Size_t i = 0; i < componentCount; ++i)
            {
                const ComponentType_t& componentType = chunkStructure.GetComponentType(i);
                componentDataArrayTo[i] = (void*)pnc_alloc(componentType.GetSize(nodeCapacity, chunkCapacity), componentType.GetAlignment());
                componentType.ConstructComponentUnsafe(componentDataArrayTo[i], firstNodeIndex, firstChunkIndex, nodeCount, chunkCount);
            }
        }



        template<typename TChunkTo, typename TChunkFrom>
        static void AllocateCopyConstructAllComponentsForwardUnsafe(TChunkTo&   chunkTo,   const Size_t firstNodeIndexTo,   const Size_t firstChunkIndexTo,
                                                              const TChunkFrom& chunkFrom, const Size_t firstNodeIndexFrom, const Size_t firstChunkIndexFrom,
                                                              const NodeCountT<Size_t> nodeCount,    const ChunkCountT<Size_t> chunkCount, 
                                                              const NodeCapacityT<Size_t> nodeCapacity, const ChunkCapacityT<Size_t> chunkCapacity)
        {
            pnc_assert(!chunkTo.IsNull());
            pnc_assert(!chunkFrom.IsNull());
            pnc_assert(IsSameStructure(chunkTo, chunkFrom));
            pnc_assert(!IsSameData(chunkTo, chunkFrom));
            pnc_assert(firstNodeIndexTo >= 0);
            pnc_assert(firstNodeIndexFrom >= 0);
            pnc_assert(nodeCount >= 0);
            pnc_assert(firstChunkIndexTo >= 0);
            pnc_assert(firstChunkIndexFrom >= 0);
            pnc_assert(chunkCount >= 0);
            pnc_assert(nodeCapacity >= nodeCount);
            pnc_assert(chunkCapacity >= chunkCount);

            void** const componentDataArrayTo = TChunkTo::GetInternalChunk(chunkTo).ComponentData;

            const ChunkStructure_t& chunkStructure = chunkTo.GetStructure();
            Size_t componentCount = chunkStructure.GetComponentCount();
            for (Size_t i = 0; i < componentCount; ++i)
            {
                const ComponentType_t& componentType = chunkStructure.GetComponentType(i);
                componentDataArrayTo[i] = (void*)pnc_alloc(componentType.GetSize(nodeCapacity, chunkCapacity), componentType.GetAlignment());
                componentType.CopyConstructComponentForwardUnsafe(
                    componentDataArrayTo[i],       firstNodeIndexTo,   firstChunkIndexTo,
                    chunkFrom.GetComponentData(i), firstNodeIndexFrom, firstChunkIndexFrom, 
                    nodeCount, chunkCount);
            }
        }


        /// <summary>
        /// Notes:
        ///     Will destruct and free data from chunkTo.
        ///     Does not set NodeCapacity / NodeCount / ChunkCapacity / ChunkCount on chunkTo.
        ///     chunkTo cannot be the same data as chunkFrom
        /// </summary>
        template<typename TChunkTo, typename TChunkFrom>
        static void ReallocateCopyConstructAllComponentsForwardUnsafe(TChunkTo&   chunkToReallocate, const Size_t firstNodeIndexTo,   const Size_t firstChunkIndexTo,
                                                                const TChunkFrom& chunkFrom,         const Size_t firstNodeIndexFrom, const Size_t firstChunkIndexFrom,
                                                                const NodeCountT<Size_t> nodeCount,    const ChunkCountT<Size_t> chunkCount, 
                                                                const NodeCapacityT<Size_t> nodeCapacity, const ChunkCapacityT<Size_t> chunkCapacity)
        {
            pnc_assert(!chunkToReallocate.IsNull());
            pnc_assert(!chunkFrom.IsNull());
            pnc_assert(IsSameStructure(chunkToReallocate, chunkFrom));
            pnc_assert(!IsSameData(chunkToReallocate, chunkFrom));
            pnc_assert(firstNodeIndexTo >= 0);
            pnc_assert(firstNodeIndexFrom >= 0);
            pnc_assert(nodeCount >= 0);
            pnc_assert(firstChunkIndexTo >= 0);
            pnc_assert(firstChunkIndexFrom >= 0);
            pnc_assert(chunkCount >= 0);
            pnc_assert(nodeCapacity >= nodeCount);
            pnc_assert(chunkCapacity >= chunkCount);

            const auto nodeCountTo       = chunkToReallocate.GetNodeCount();
            const auto nodeCapacityTo    = chunkToReallocate.GetNodeCapacity();
            const auto chunkCountTo      = chunkToReallocate.GetChunkCount();
            const auto chunkCapacityTo   = chunkToReallocate.GetChunkCapacity();
            const auto chunkCapacityFrom = chunkToReallocate.GetChunkCapacity();
            void** const componentDataArrayTo = TChunkTo::GetInternalChunk(chunkToReallocate).ComponentData;
            const ChunkStructure_t& structure = chunkToReallocate.GetStructure();
            const auto componentCount = structure.GetComponentCount();
            for (Size_t i = 0; i < componentCount; ++i)
            {
                const void* const dataFrom = chunkFrom.GetComponentData(i);
                const ComponentType_t& componentType = structure.GetComponentType(i);
                const Size_t allocSize = componentType.GetSize(nodeCapacity, chunkCapacity);
                const Size_t freeSize  = componentType.GetSize(nodeCapacityTo, chunkCapacityTo);
                if(allocSize == freeSize)
                    componentType.CopyAssignComponentForwardUnsafe(componentDataArrayTo[i], 0, 0,
                                                                   dataFrom,                0, 0, 
                                                                   nodeCount, chunkCount);
                else
                {
                    void* const dataNew = (void*)pnc_alloc(allocSize, componentType.GetAlignment());
                    componentType.CopyConstructComponentForwardUnsafe(dataNew,  0, 0,
                                                                      dataFrom, 0, 0,
                                                                      nodeCount, chunkCount);
                    componentType.DestructComponentUnsafe(componentDataArrayTo[i], 0, 0, nodeCountTo, chunkCountTo);
                    pnc_free_dirty(componentDataArrayTo[i], componentType.GetSize(nodeCapacityTo), componentType.GetAlignment());
                    componentDataArrayTo[i] = dataNew;
                }

            }
            //
            //void** const componentDataArrayTo = TChunkTo::GetInternalChunk(chunkTo).ComponentData;
            //
            //const ChunkStructure_t& chunkStructure = chunkTo.GetStructure();
            //Size_t componentCount = chunkStructure.GetComponentCount();
            //for (Size_t i = 0; i < componentCount; ++i)
            //{
            //    const ComponentType_t& componentType = chunkStructure.GetComponentType(i);
            //    componentDataArrayTo[i] = (void*)pnc_alloc(componentType.GetSize(nodeCapacity, chunkCapacity), componentType.GetAlignment());
            //    componentType.CopyConstructComponentForwardUnsafe(
            //        componentDataArrayTo[i],       firstNodeIndexTo,   firstChunkIndexTo,
            //        chunkFrom.GetComponentData(i), firstNodeIndexFrom, firstChunkIndexFrom, 
            //        nodeCount, chunkCount);
            //}
        }


        //static void CopyComponentForwardUnsafe(const ComponentType_t& componentType,
        //          void* const baseComponentDataTo,   const Size_t firstNodeIndexTo,
        //    const void* const baseComponentDataFrom, const Size_t firstNodeIndexFrom,
        //    const NodeCountT<Size_t> nodeCount)
        //{
        //    CopyComponentForwardUnsafe(componentType, baseComponentDataTo, firstNodeIndexTo, 0, baseComponentDataFrom, firstNodeIndexFrom, 0, nodeCount, 1);
        //}

        //static void CopyComponentForwardUnsafe(const ComponentType_t& componentType,
        //          void* const baseComponentDataTo,   const Size_t firstNodeIndexTo,   const Size_t firstChunkIndexTo,
        //    const void* const baseComponentDataFrom, const Size_t firstNodeIndexFrom, const Size_t firstChunkIndexFrom, 
        //    const NodeCountT<Size_t> nodeCount, const ChunkCountT<Size_t> chunkCount)
        //{
        //    componentType.CopyComponentForwardUnsafe(baseComponentDataTo  , firstNodeIndexTo  , firstChunkIndexTo,
        //                                             baseComponentDataFrom, firstNodeIndexFrom, firstChunkIndexFrom,
        //                                             nodeCount, chunkCount);
        //}





        //static void MoveComponentForwardUnsafe(const ComponentType_t& componentType,
        //    void* const baseComponentDataTo, void* const baseComponentDataFrom,
        //    const Size_t firstNodeIndexTo, const Size_t firstNodeIndexFrom, const NodeCountT<Size_t> nodeCount,
        //    const Size_t firstChunkIndexTo = 0, const Size_t firstChunkIndexFrom = 0, const ChunkCountT<Size_t> chunkCount = 1)
        //{
        //    componentType.CopyComponentForwardUnsafe(baseComponentDataTo, baseComponentDataFrom,
        //        firstNodeIndexTo, firstNodeIndexFrom,
        //        firstChunkIndexTo, firstChunkIndexFrom,
        //        nodeCount, chunkCount);
        //}
        //



        //static void SwapComponentForwardUnsafe(const ComponentType_t& componentType,
        //    void* const baseComponentDataTo, void* const baseComponentDataFrom,
        //    const Size_t firstNodeIndexTo, const Size_t firstNodeIndexFrom, const NodeCountT<Size_t> nodeCount,
        //    const Size_t firstChunkIndexTo = 0, const Size_t firstChunkIndexFrom = 0, const ChunkCountT<Size_t> chunkCount = 1)
        //{
        //    componentType.CopyComponentForwardUnsafe(baseComponentDataTo, baseComponentDataFrom,
        //        firstNodeIndexTo, firstNodeIndexFrom,
        //        firstChunkIndexTo, firstChunkIndexFrom,
        //        nodeCount, chunkCount);
        //}



        //template<typename TChunk>
        //static void CopyNodeComponentsUnsafe(const ComponentType_t& componentType,
        //    void* const baseComponentDataTo, void* const baseComponentDataFrom,
        //    const Size_t firstNodeIndexTo, const Size_t firstNodeIndexFrom, const NodeCountT<Size_t> nodeCount)
        //{
        //    //pnc_assert(!chunk.IsNull());
        //    //pnc_assert(firstNodeIndex >= 0);
        //    //pnc_assert(nodeCount >= 0);
        //    //
        //    //const ChunkStructure_t& chunkStructure = chunk.GetStructure();
        //    //Size_t countDefaultConstructible = chunkStructure.DefaultConstructibleNodeIndex.size();
        //    //for (Size_t i = 0; i < countDefaultConstructible; ++i)
        //    //{
        //    //    Size_t index = chunkStructure.DefaultConstructibleNodeIndex[i];
        //    //    void* componentData = chunk->GetComponentData(index);
        //    //    chunkStructure.Components[index]->ConstructComponentDataUnsafe(componentData, firstNodeIndex, nodeCount);
        //    //}


        //    //componentType.CopyComponentUnsafe(baseComponentDataTo, baseComponentDataFrom,
        //    //    firstNodeIndexTo, firstNodeIndexFrom,
        //    //    firstChunkIndexTo, firstChunkIndexFrom,
        //    //    nodeCount, chunkCount);
        //}


















         
         

        template<typename TChunkTo, typename TChunkFrom>
        static void MoveAllComponentsForward(TChunkTo& chunkTo, const TChunkFrom& chunkFrom,
            const Size_t firstNodeIndexTo, const Size_t firstNodeIndexFrom, const NodeCountT<Size_t> nodeCount,
            const Size_t firstChunkIndexTo = 0, const Size_t firstChunkIndexFrom = 0, const ChunkCountT<Size_t> chunkCount = 1)
        {
            pnc_assert(!chunkTo.IsNull());
            pnc_assert(!chunkFrom.IsNull());
            pnc_assert(IsSameStructure(chunkTo, chunkFrom));
            pnc_assert(firstNodeIndexTo >= 0);
            pnc_assert(firstNodeIndexFrom >= 0);
            pnc_assert(nodeCount >= 0);
            pnc_assert(firstChunkIndexTo >= 0);
            pnc_assert(firstChunkIndexFrom >= 0);
            pnc_assert(chunkCount >= 0);
            pnc_assert(firstNodeIndexTo < chunkTo.GetNodeCount());
            pnc_assert(firstNodeIndexTo + nodeCount <= chunkTo.GetNodeCount());
            pnc_assert(firstChunkIndexTo < chunkTo.GetChunkCount());
            pnc_assert(firstChunkIndexTo + chunkCount <= chunkTo.GetChunkCount());
            pnc_assert(firstNodeIndexFrom < chunkFrom.GetNodeCount());
            pnc_assert(firstNodeIndexFrom + nodeCount <= chunkFrom.GetNodeCount());
            pnc_assert(firstChunkIndexFrom < chunkFrom.GetChunkCount());
            pnc_assert(firstChunkIndexFrom + chunkCount <= chunkFrom.GetChunkCount());

            const ChunkStructure_t& chunkStructure = chunkTo.GetStructure();
            const auto componentCount = chunkStructure.GetComponentCount();
            for (Size_t i = 0; i < componentCount; ++i)
            {
                const ComponentType_t* componentType = chunkStructure.Components[i];
                componentType->MoveComponentForwardUnsafe(chunkTo.GetComponentData(i), chunkFrom.GetComponentData(i),
                    firstNodeIndexTo, firstNodeIndexFrom, nodeCount,
                    firstChunkIndexTo, firstChunkIndexFrom, chunkCount);
            }
        }



        //template<typename TChunkTo, typename TChunkFrom>
        //static void SwapNodeForward(TChunkTo& chunkTo, const TChunkFrom& chunkFrom,
        //    const Size_t firstNodeIndexTo, const Size_t firstNodeIndexFrom, const NodeCountT<Size_t> nodeCount,
        //    const Size_t firstChunkIndexTo = 0, const Size_t firstChunkIndexFrom = 0, const ChunkCountT<Size_t> chunkCount = 1)
        //{
        //    pnc_assert(!chunkTo.IsNull());
        //    pnc_assert(!chunkFrom.IsNull());
        //    pnc_assert(IsSameStructure(chunkTo, chunkFrom));
        //    pnc_assert(firstNodeIndexTo >= 0);
        //    pnc_assert(firstNodeIndexFrom >= 0);
        //    pnc_assert(nodeCount >= 0);
        //    pnc_assert(firstChunkIndexTo >= 0);
        //    pnc_assert(firstChunkIndexFrom >= 0);
        //    pnc_assert(chunkCount >= 0);
        //    pnc_assert(firstNodeIndexTo < chunkTo.GetNodeCount());
        //    pnc_assert(firstNodeIndexTo + nodeCount <= chunkTo.GetNodeCount());
        //    pnc_assert(firstChunkIndexTo < chunkTo.GetChunkCount());
        //    pnc_assert(firstChunkIndexTo + chunkCount <= chunkTo.GetChunkCount());
        //    pnc_assert(firstNodeIndexFrom < chunkFrom.GetNodeCount());
        //    pnc_assert(firstNodeIndexFrom + nodeCount <= chunkFrom.GetNodeCount());
        //    pnc_assert(firstChunkIndexFrom < chunkFrom.GetChunkCount());
        //    pnc_assert(firstChunkIndexFrom + chunkCount <= chunkFrom.GetChunkCount());

        //    const ChunkStructure_t& chunkStructure = chunkTo.GetStructure();
        //    Size_t componentCount = chunkStructure.GetComponentCount();
        //    for (Size_t i = 0; i < componentCount; ++i)
        //    {
        //        const ComponentType_t* componentType = chunkStructure.Components[i];
        //        componentType->SwapComponentForwardUnsafe(chunkTo.GetComponentData(i), chunkFrom.GetComponentData(i),
        //            firstNodeIndexTo, firstNodeIndexFrom, nodeCount,
        //            firstChunkIndexTo, firstChunkIndexFrom, chunkCount);
        //    }
        //}

        template<typename TChunkA, typename TChunkB>
        static bool IsSameStructure(const TChunkA& a, const TChunkB& b) { return a.GetStructure() == b.GetStructure(); }
        template<typename TChunkA, typename TChunkB>
        static bool IsSameData(const TChunkA& a, const TChunkB& b) { return TChunkA::IsSameData(a,b); }
        



        //static void DestructComponentUnsafe(const ComponentType_t& componentType, void* const componentData, const Size_t firstNodeIndex, const NodeCountT<Size_t> nodeCount)
        //{
        //    pnc_assert(componentData);
        //    pnc_assert(firstNodeIndex >= 0);
        //    pnc_assert(nodeCount >= 0);
        //    componentType.DestructNodesUnsafe(componentData, firstNodeIndex, nodeCount);
        //}
        //static void CopyComponentUnsafe(const ComponentType_t& componentType, void* const componentDataTo, void* const componentDataFrom, const Size_t firstNodeIndexTo, const Size_t firstNodeIndexFrom, const NodeCountT<Size_t> nodeCount)
        //{
        //    pnc_assert(componentDataTo);
        //    pnc_assert(componentDataFrom);
        //    pnc_assert(firstNodeIndexTo >= 0);
        //    pnc_assert(firstNodeIndexFrom >= 0);
        //    pnc_assert(nodeCount >= 0);
        //    componentType.CopyNodesUnsafe(componentDataTo, componentDataFrom, firstNodeIndexTo, firstNodeIndexFrom, nodeCount);
        //}
        //static void MoveComponentUnsafe(const ComponentType_t& componentType, void* const componentDataTo, void* const componentDataFrom, const Size_t firstNodeIndexTo, const Size_t firstNodeIndexFrom, const NodeCountT<Size_t> nodeCount)
        //{
        //    pnc_assert(componentDataTo);
        //    pnc_assert(componentDataFrom);
        //    pnc_assert(firstNodeIndexTo >= 0);
        //    pnc_assert(firstNodeIndexFrom >= 0);
        //    pnc_assert(nodeCount >= 0);
        //    componentType.MoveNodesUnsafe(componentDataTo, componentDataFrom, firstNodeIndexTo, firstNodeIndexFrom, nodeCount);
        //}
        //static void SwapComponentUnsafe(const ComponentType_t& componentType, void* const componentDataTo, void* const componentDataFrom, const Size_t firstNodeIndexTo, const Size_t firstNodeIndexFrom, const NodeCountT<Size_t> nodeCount)
        //{
        //    pnc_assert(componentDataTo);
        //    pnc_assert(componentDataFrom);
        //    pnc_assert(firstNodeIndexTo >= 0);
        //    pnc_assert(firstNodeIndexFrom >= 0);
        //    pnc_assert(nodeCount >= 0);
        //    componentType.SwapNodesUnsafe(componentDataTo, componentDataFrom, firstNodeIndexTo, firstNodeIndexFrom, nodeCount);
        //}

        //template<typename TChunk>
        //static void ConstructNodesUnsafe(TChunk& chunk, const Size_t firstNodeIndex, const NodeCountT<Size_t> nodeCount)
        //{
        //    pnc_assert(!chunk.IsNull());
        //    pnc_assert(firstNodeIndex >= 0);
        //    pnc_assert(nodeCount >= 0);

        //    const ChunkStructure_t& chunkStructure = chunk.GetStructure();
        //    Size_t countDefaultConstructible = chunkStructure.DefaultConstructibleIndex.size();
        //    for (Size_t i = 0; i < countDefaultConstructible; ++i)
        //    {
        //        Size_t index = chunkStructure.DefaultConstructibleIndex[i];
        //        void* componentData = chunk->GetComponentData(index);
        //        chunkStructure.Components[index]->ConstructNodesUnsafe(componentData, firstNodeIndex, nodeCount);
        //    }
        //}

        //template<typename TChunk>
        //static void DestructNodesUnsafe(TChunk& chunk, const Size_t firstNodeIndex, const NodeCountT<Size_t> nodeCount)
        //{
        //    pnc_assert(!chunk.IsNull());
        //    pnc_assert(firstNodeIndex >= 0);
        //    pnc_assert(nodeCount >= 0);

        //    const ChunkStructure_t& chunkStructure = chunk.GetStructure();
        //    Size_t countDestructible = chunkStructure.DestructibleIndex.size();
        //    for (Size_t i = 0; i < countDestructible; ++i)
        //    {
        //        Size_t index = chunkStructure.DestructibleIndex[i];
        //        void* componentData = chunk->GetComponentData(index);
        //        chunkStructure.Components[index]->DestructNodesUnsafe(componentData, firstNodeIndex, nodeCount);
        //    }
        //}

    };
}
