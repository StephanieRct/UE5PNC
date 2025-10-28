// MIT License
// Copyright (c) 2025 Stephanie Rancourt

#pragma once
#include "common.h"
#include "ChunkPointerInternal.h"

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
        static void ConstructAllChunkComponentsUnsafe(TChunk& chunk, const Size_t firstChunkIndex = 0, const Size_t chunkCount = 1)
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
                void* componentData = chunk->GetComponentData(index);
                componentType->ConstructDataUnsafe(componentData, firstChunkIndex, chunkCount);
            }
#else
            Size_t componentTypeCount = chunkStructure.DefaultConstructibleChunkIndex.size();
            for (Size_t i = 0; i < componentTypeCount; ++i)
            {
                Size_t index = chunkStructure.DefaultConstructibleChunkIndex[i];
                const ComponentType_t* componentType = chunkStructure.Components[index];
                void* componentData = chunk->GetComponentData(index);
                componentType->ConstructDataUnsafe(componentData, firstChunkIndex, chunkCount);
            }
#endif
        }

        // can be called before index at (firstNodeIndex + nodeCount) is included by the chunk NodeCount, 
        // making this function unsafe.
        template<typename TChunk>
        static void ConstructAllNodeComponentsUnsafe(TChunk& chunk, const Size_t firstNodeIndex, const Size_t nodeCount)
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
        static void DestructAllChunkComponentsUnsafe(TChunk& chunk, const Size_t firstChunkIndex = 0, const Size_t chunkCount = 1)
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
        static void DestructAllNodeComponentsUnsafe(TChunk& chunk, const Size_t firstNodeIndex, const Size_t nodeCount)
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
        static void MoveAllNodeComponentsForwardUnsafe(TChunk& chunkTo,   const Size_t firstNodeIndexTo, 
                                                       TChunk& chunkFrom, const Size_t firstNodeIndexFrom, 
                                                       const Size_t nodeCount)
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
                componentType->MoveDataForwardUnsafe(componentDataTo,   firstNodeIndexTo,
                                                     componentDataFrom, firstNodeIndexFrom, 
                                                     nodeCount);
            }
        }
        
        template<typename TChunk>
        static void MoveAllChunkComponentsForwardUnsafe(TChunk& chunkTo,   const Size_t firstChunkIndexTo, 
                                                        TChunk& chunkFrom, const Size_t firstChunkIndexFrom, 
                                                        const Size_t chunkCount)
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









        static void ConstructComponentUnsafe(const ComponentType_t& componentType, 
            void* const baseComponentData,const Size_t firstNodeIndex, const Size_t nodeCount,
            const Size_t firstChunkIndex = 0, const Size_t chunkCount = 1)
        {
            componentType.ConstructComponentUnsafe(baseComponentData, firstNodeIndex, nodeCount, firstChunkIndex, chunkCount);
        }

        static void DestructComponentUnsafe(const ComponentType_t& componentType, 
            void* const baseComponentData, const Size_t firstNodeIndex, const Size_t nodeCount,
            const Size_t firstChunkIndex = 0, const Size_t chunkCount = 1)
        {
            componentType.DestructComponentUnsafe(baseComponentData, firstNodeIndex, nodeCount, firstChunkIndex, chunkCount);
        }




        static void CopyComponentForwardUnsafe(const ComponentType_t& componentType,
                  void* const baseComponentDataTo,   const Size_t firstNodeIndexTo,
            const void* const baseComponentDataFrom, const Size_t firstNodeIndexFrom,
            const Size_t nodeCount)
        {
            CopyComponentForwardUnsafe(componentType, baseComponentDataTo, firstNodeIndexTo, 0, baseComponentDataFrom, firstNodeIndexFrom, 0, nodeCount, 1);
        }

        static void CopyComponentForwardUnsafe(const ComponentType_t& componentType,
                  void* const baseComponentDataTo,   const Size_t firstNodeIndexTo,   const Size_t firstChunkIndexTo,
            const void* const baseComponentDataFrom, const Size_t firstNodeIndexFrom, const Size_t firstChunkIndexFrom, 
            const Size_t nodeCount, const Size_t chunkCount)
        {
            componentType.CopyComponentForwardUnsafe(baseComponentDataTo  , firstNodeIndexTo  , firstChunkIndexTo,
                                                     baseComponentDataFrom, firstNodeIndexFrom, firstChunkIndexFrom,
                                                     nodeCount, chunkCount);
        }





        static void MoveComponentForwardUnsafe(const ComponentType_t& componentType,
            void* const baseComponentDataTo, void* const baseComponentDataFrom,
            const Size_t firstNodeIndexTo, const Size_t firstNodeIndexFrom, const Size_t nodeCount,
            const Size_t firstChunkIndexTo = 0, const Size_t firstChunkIndexFrom = 0, const Size_t chunkCount = 1)
        {
            componentType.CopyComponentForwardUnsafe(baseComponentDataTo, baseComponentDataFrom,
                firstNodeIndexTo, firstNodeIndexFrom,
                firstChunkIndexTo, firstChunkIndexFrom,
                nodeCount, chunkCount);
        }
        
        static void SwapComponentForwardUnsafe(const ComponentType_t& componentType,
            void* const baseComponentDataTo, void* const baseComponentDataFrom,
            const Size_t firstNodeIndexTo, const Size_t firstNodeIndexFrom, const Size_t nodeCount,
            const Size_t firstChunkIndexTo = 0, const Size_t firstChunkIndexFrom = 0, const Size_t chunkCount = 1)
        {
            componentType.CopyComponentForwardUnsafe(baseComponentDataTo, baseComponentDataFrom,
                firstNodeIndexTo, firstNodeIndexFrom,
                firstChunkIndexTo, firstChunkIndexFrom,
                nodeCount, chunkCount);
        }



        //template<typename TChunk>
        //static void CopyNodeComponentsUnsafe(const ComponentType_t& componentType,
        //    void* const baseComponentDataTo, void* const baseComponentDataFrom,
        //    const Size_t firstNodeIndexTo, const Size_t firstNodeIndexFrom, const Size_t nodeCount)
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






        template<typename TChunk>
        static void ConstructNode(TChunk& chunk,
            const Size_t firstNodeIndex, const Size_t nodeCount,
            const Size_t firstChunkIndex = 0, const Size_t chunkCount = 1)
        {
            pnc_assert(!chunk.IsNull());
            pnc_assert(firstNodeIndex >= 0);
            pnc_assert(nodeCount >= 0);
            pnc_assert(firstChunkIndex >= 0);
            pnc_assert(chunkCount >= 0);
            pnc_assert(firstNodeIndex < chunk.GetNodeCount());
            pnc_assert(firstNodeIndex + nodeCount <= chunk.GetNodeCount());
            pnc_assert(firstChunkIndex < chunk.GetChunkCount());
            pnc_assert(firstChunkIndex + chunkCount <= chunk.GetChunkCount());

            const ChunkStructure_t& chunkStructure = chunk.GetStructure();
            Size_t componentCount = chunkStructure.GetComponentCount();
            for (Size_t i = 0; i < componentCount; ++i)
            {
                const ComponentType_t* componentType = chunkStructure.Components[i];
                componentType->ConstructComponentUnsafe(chunk.GetComponentData(i), firstNodeIndex, nodeCount, firstChunkIndex, chunkCount);
            }
        }

        template<typename TChunk>
        static void DestructNode(TChunk& chunk,
            const Size_t firstNodeIndex, const Size_t nodeCount,
            const Size_t firstChunkIndex = 0, const Size_t chunkCount = 1)
        {
            pnc_assert(!chunk.IsNull());
            pnc_assert(firstNodeIndex >= 0);
            pnc_assert(nodeCount >= 0);
            pnc_assert(firstChunkIndex >= 0);
            pnc_assert(chunkCount >= 0);
            pnc_assert(firstNodeIndex < chunk.GetNodeCount());
            pnc_assert(firstNodeIndex + nodeCount <= chunk.GetNodeCount());
            pnc_assert(firstChunkIndex < chunk.GetChunkCount());
            pnc_assert(firstChunkIndex + chunkCount <= chunk.GetChunkCount());

            const ChunkStructure_t& chunkStructure = chunk.GetStructure();
            Size_t componentCount = chunkStructure.GetComponentCount();
            for (Size_t i = 0; i < componentCount; ++i)
            {
                const ComponentType_t* componentType = chunkStructure.Components[i];
                componentType->DestructComponentUnsafe(chunk.GetComponentData(i), firstNodeIndex, nodeCount, firstChunkIndex, chunkCount);
            }
        }

        /// <summary>
        /// Copy data between two chunks of the same ChunkStructure.
        /// Will return -1 if the any chunk is null or not the same ChunkStructure
        /// </summary>
        /// <param name="chunkTo">Chunk where to write the data to.</param>
        /// <param name="chunkFrom">Chunk where to read the data from.</param>
        /// <param name="firstNodeIndexTo"></param>
        /// <param name="firstNodeIndexFrom"></param>
        /// <param name="nodeCount"></param>
        /// <returns>Number of node data copied or -1 if failed.</returns>
        template<typename TChunkTo, typename TChunkFrom>
        static void CopyNodeForward(TChunkTo& chunkTo, const TChunkFrom& chunkFrom,
            const Size_t firstNodeIndexTo, const Size_t firstNodeIndexFrom, const Size_t nodeCount,
            const Size_t firstChunkIndexTo = 0, const Size_t firstChunkIndexFrom = 0, const Size_t chunkCount = 1)
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
            Size_t componentCount = chunkStructure.GetComponentCount();
            for (Size_t i = 0; i < componentCount; ++i)
            {
                const ComponentType_t* componentType = chunkStructure.Components[i];
                componentType->CopyComponentForwardUnsafe(chunkTo.GetComponentData(i), chunkFrom.GetComponentData(i),
                    firstNodeIndexTo, firstNodeIndexFrom, nodeCount,
                    firstChunkIndexTo, firstChunkIndexFrom, chunkCount);
            }
        }

        template<typename TChunkTo, typename TChunkFrom>
        static void MoveNodeForward(TChunkTo& chunkTo, const TChunkFrom& chunkFrom,
            const Size_t firstNodeIndexTo, const Size_t firstNodeIndexFrom, const Size_t nodeCount,
            const Size_t firstChunkIndexTo = 0, const Size_t firstChunkIndexFrom = 0, const Size_t chunkCount = 1)
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
            Size_t componentCount = chunkStructure.GetComponentCount();
            for (Size_t i = 0; i < componentCount; ++i)
            {
                const ComponentType_t* componentType = chunkStructure.Components[i];
                componentType->MoveComponentForwardUnsafe(chunkTo.GetComponentData(i), chunkFrom.GetComponentData(i),
                    firstNodeIndexTo, firstNodeIndexFrom, nodeCount,
                    firstChunkIndexTo, firstChunkIndexFrom, chunkCount);
            }
        }

        template<typename TChunkTo, typename TChunkFrom>
        static void SwapNodeForward(TChunkTo& chunkTo, const TChunkFrom& chunkFrom,
            const Size_t firstNodeIndexTo, const Size_t firstNodeIndexFrom, const Size_t nodeCount,
            const Size_t firstChunkIndexTo = 0, const Size_t firstChunkIndexFrom = 0, const Size_t chunkCount = 1)
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
            Size_t componentCount = chunkStructure.GetComponentCount();
            for (Size_t i = 0; i < componentCount; ++i)
            {
                const ComponentType_t* componentType = chunkStructure.Components[i];
                componentType->SwapComponentForwardUnsafe(chunkTo.GetComponentData(i), chunkFrom.GetComponentData(i),
                    firstNodeIndexTo, firstNodeIndexFrom, nodeCount,
                    firstChunkIndexTo, firstChunkIndexFrom, chunkCount);
            }
        }

        template<typename TChunkA, typename TChunkB>
        static bool IsSameStructure(const TChunkA& a, const TChunkB& b) { return a.GetStructure() == b.GetStructure(); }



        template<typename TChunkTo, typename TChunkFrom>
        static void CopyNodeComponentsForward(TChunkTo& chunkTo, const TChunkFrom& chunkFrom,
            const Size_t firstNodeIndexTo, const Size_t firstNodeIndexFrom, const Size_t nodeCount)
        {
            pnc_assert(!chunkTo.IsNull());
            pnc_assert(!chunkFrom.IsNull());
            pnc_assert(IsSameStructure(chunkTo, chunkFrom));
            pnc_assert(firstNodeIndexTo >= 0);
            pnc_assert(firstNodeIndexFrom >= 0);
            pnc_assert(nodeCount >= 0);
            pnc_assert(firstNodeIndexTo < chunkTo.GetNodeCount());
            pnc_assert(firstNodeIndexTo + nodeCount <= chunkTo.GetNodeCount());
            pnc_assert(firstNodeIndexFrom < chunkFrom.GetNodeCount());
            pnc_assert(firstNodeIndexFrom + nodeCount <= chunkFrom.GetNodeCount());

            const ChunkStructure_t& chunkStructure = chunkTo.GetStructure();
            Size_t componentCount = chunkStructure.NodeComponentIndex.size();
            for (Size_t i = 0; i < componentCount; ++i)
            {
                Size_t index = chunkStructure.NodeComponentIndex[i];
                void* componentDataTo = chunkTo.GetComponentData(index);
                void* componentDataFrom = chunkFrom.GetComponentData(index);
                chunkStructure.Components[index]->CopyComponentDataForwardUnsafe(componentDataTo, componentDataFrom, firstNodeIndexTo, firstNodeIndexFrom, nodeCount);
            }
        }
        template<typename TChunkTo, typename TChunkFrom>
        static void CopyChunkComponentsForward(TChunkTo& chunkTo, const TChunkFrom& chunkFrom,
            const Size_t firstChunkIndexTo, const Size_t firstChunkIndexFrom, const Size_t chunkCount)
        {
            pnc_assert(!chunkTo.IsNull());
            pnc_assert(!chunkFrom.IsNull());
            pnc_assert(IsSameStructure(chunkTo, chunkFrom));
            pnc_assert(firstChunkIndexTo >= 0);
            pnc_assert(firstChunkIndexFrom >= 0);
            pnc_assert(chunkCount >= 0);
            pnc_assert(firstChunkIndexTo < chunkTo.GetChunkCount());
            pnc_assert(firstChunkIndexTo + chunkCount <= chunkTo.GetChunkCount());
            pnc_assert(firstChunkIndexFrom < chunkFrom.GetChunkCount());
            pnc_assert(firstChunkIndexFrom + chunkCount <= chunkFrom.GetChunkCount());


            const ChunkStructure_t& chunkStructure = chunkTo.GetStructure();
            Size_t componentCount = chunkStructure.ChunkComponentIndex.size();
            for (Size_t i = 0; i < componentCount; ++i)
            {
                Size_t index = chunkStructure.ChunkComponentIndex[i];
                void* componentDataTo = chunkTo.GetComponentData(index);
                void* componentDataFrom = chunkFrom.GetComponentData(index);
                chunkStructure.Components[index]->CopyComponentDataForwardUnsafe(componentDataTo, componentDataFrom, firstChunkIndexTo, firstChunkIndexFrom, chunkCount);
            }
        }



        //static void DestructComponentUnsafe(const ComponentType_t& componentType, void* const componentData, const Size_t firstNodeIndex, const Size_t nodeCount)
        //{
        //    pnc_assert(componentData);
        //    pnc_assert(firstNodeIndex >= 0);
        //    pnc_assert(nodeCount >= 0);
        //    componentType.DestructNodesUnsafe(componentData, firstNodeIndex, nodeCount);
        //}
        //static void CopyComponentUnsafe(const ComponentType_t& componentType, void* const componentDataTo, void* const componentDataFrom, const Size_t firstNodeIndexTo, const Size_t firstNodeIndexFrom, const Size_t nodeCount)
        //{
        //    pnc_assert(componentDataTo);
        //    pnc_assert(componentDataFrom);
        //    pnc_assert(firstNodeIndexTo >= 0);
        //    pnc_assert(firstNodeIndexFrom >= 0);
        //    pnc_assert(nodeCount >= 0);
        //    componentType.CopyNodesUnsafe(componentDataTo, componentDataFrom, firstNodeIndexTo, firstNodeIndexFrom, nodeCount);
        //}
        //static void MoveComponentUnsafe(const ComponentType_t& componentType, void* const componentDataTo, void* const componentDataFrom, const Size_t firstNodeIndexTo, const Size_t firstNodeIndexFrom, const Size_t nodeCount)
        //{
        //    pnc_assert(componentDataTo);
        //    pnc_assert(componentDataFrom);
        //    pnc_assert(firstNodeIndexTo >= 0);
        //    pnc_assert(firstNodeIndexFrom >= 0);
        //    pnc_assert(nodeCount >= 0);
        //    componentType.MoveNodesUnsafe(componentDataTo, componentDataFrom, firstNodeIndexTo, firstNodeIndexFrom, nodeCount);
        //}
        //static void SwapComponentUnsafe(const ComponentType_t& componentType, void* const componentDataTo, void* const componentDataFrom, const Size_t firstNodeIndexTo, const Size_t firstNodeIndexFrom, const Size_t nodeCount)
        //{
        //    pnc_assert(componentDataTo);
        //    pnc_assert(componentDataFrom);
        //    pnc_assert(firstNodeIndexTo >= 0);
        //    pnc_assert(firstNodeIndexFrom >= 0);
        //    pnc_assert(nodeCount >= 0);
        //    componentType.SwapNodesUnsafe(componentDataTo, componentDataFrom, firstNodeIndexTo, firstNodeIndexFrom, nodeCount);
        //}

        //template<typename TChunk>
        //static void ConstructNodesUnsafe(TChunk& chunk, const Size_t firstNodeIndex, const Size_t nodeCount)
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
        //static void DestructNodesUnsafe(TChunk& chunk, const Size_t firstNodeIndex, const Size_t nodeCount)
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
