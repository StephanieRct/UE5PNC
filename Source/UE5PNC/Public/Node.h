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


        static void ConstructComponentUnsafe(const ComponentType_t& componentType, void* const baseComponentData,
            const Size_t firstNodeIndex, const Size_t nodeCount,
            const Size_t firstChunkIndex = 0, const Size_t chunkCount = 1)
        {
            componentType.ConstructComponentUnsafe(baseComponentData, firstNodeIndex, nodeCount, firstChunkIndex, chunkCount);
        }

        static void DestructComponentUnsafe(const ComponentType_t& componentType, void* const baseComponentData,
            const Size_t firstNodeIndex, const Size_t nodeCount,
            const Size_t firstChunkIndex = 0, const Size_t chunkCount = 1)
        {
            componentType.DestructComponentUnsafe(baseComponentData, firstNodeIndex, nodeCount, firstChunkIndex, chunkCount);
        }

        static void CopyComponentUnsafe(const ComponentType_t& componentType,
            void* const baseComponentDataTo, void* const baseComponentDataFrom,
            const Size_t firstNodeIndexTo, const Size_t firstNodeIndexFrom, const Size_t nodeCount,
            const Size_t firstChunkIndexTo = 0, const Size_t firstChunkIndexFrom = 0, const Size_t chunkCount = 1)
        {
            componentType.CopyComponentUnsafe(baseComponentDataTo, baseComponentDataFrom,
                firstNodeIndexTo, firstNodeIndexFrom,
                firstChunkIndexTo, firstChunkIndexFrom,
                nodeCount, chunkCount);
        }

        static void MoveComponentUnsafe(const ComponentType_t& componentType,
            void* const baseComponentDataTo, void* const baseComponentDataFrom,
            const Size_t firstNodeIndexTo, const Size_t firstNodeIndexFrom, const Size_t nodeCount,
            const Size_t firstChunkIndexTo = 0, const Size_t firstChunkIndexFrom = 0, const Size_t chunkCount = 1)
        {
            componentType.CopyComponentUnsafe(baseComponentDataTo, baseComponentDataFrom,
                firstNodeIndexTo, firstNodeIndexFrom,
                firstChunkIndexTo, firstChunkIndexFrom,
                nodeCount, chunkCount);
        }
        
        static void SwapComponentUnsafe(const ComponentType_t& componentType,
            void* const baseComponentDataTo, void* const baseComponentDataFrom,
            const Size_t firstNodeIndexTo, const Size_t firstNodeIndexFrom, const Size_t nodeCount,
            const Size_t firstChunkIndexTo = 0, const Size_t firstChunkIndexFrom = 0, const Size_t chunkCount = 1)
        {
            componentType.CopyComponentUnsafe(baseComponentDataTo, baseComponentDataFrom,
                firstNodeIndexTo, firstNodeIndexFrom,
                firstChunkIndexTo, firstChunkIndexFrom,
                nodeCount, chunkCount);
        }

        template<typename TChunk>
        static void ConstructChunkComponentsUnsafe(TChunk& chunk, const Size_t firstChunkIndex = 0, const Size_t chunkCount = 1)
        {
            assert_pnc(!chunk.IsNull());
            assert_pnc(firstChunkIndex >= 0);
            assert_pnc(chunkCount >= 0);

            const ChunkStructure_t& chunkStructure = chunk.GetStructure();
            Size_t countDefaultConstructible = chunkStructure.DefaultConstructibleChunkIndex.size();
            for (Size_t i = 0; i < countDefaultConstructible; ++i)
            {
                Size_t index = chunkStructure.DefaultConstructibleChunkIndex[i];
                void* componentData = chunk->GetComponentData(index);
                chunkStructure.Components[index]->ConstructComponentDataUnsafe(componentData, firstChunkIndex, chunkCount);
            }
        }

        template<typename TChunk>
        static void ConstructNodeComponentsUnsafe(TChunk& chunk, const Size_t firstNodeIndex, const Size_t nodeCount)
        {
            assert_pnc(!chunk.IsNull());
            assert_pnc(firstNodeIndex >= 0);
            assert_pnc(nodeCount >= 0);

            const ChunkStructure_t& chunkStructure = chunk.GetStructure();
            Size_t countDefaultConstructible = chunkStructure.DefaultConstructibleNodeIndex.size();
            for (Size_t i = 0; i < countDefaultConstructible; ++i)
            {
                Size_t index = chunkStructure.DefaultConstructibleNodeIndex[i];
                void* componentData = chunk->GetComponentData(index);
                chunkStructure.Components[index]->ConstructComponentDataUnsafe(componentData, firstNodeIndex, nodeCount);
            }
        }

        template<typename TChunk>
        static void DestructChunkComponentsUnsafe(TChunk& chunk, const Size_t firstChunkIndex = 0, const Size_t chunkCount = 1)
        {
            assert_pnc(!chunk.IsNull());
            assert_pnc(firstChunkIndex >= 0);
            assert_pnc(chunkCount >= 0);

            const ChunkStructure_t& chunkStructure = chunk.GetStructure();
            Size_t countDestructible = chunkStructure.DestructibleChunkIndex.size();
            for (Size_t i = 0; i < countDestructible; ++i)
            {
                Size_t index = chunkStructure.DestructibleChunkIndex[i];
                void* componentData = chunk->GetComponentData(index);
                chunkStructure.Components[index]->DestructComponentUnsafe(componentData, firstChunkIndex, chunkCount);
            }
        }

        template<typename TChunk>
        static void DestructNodeComponentsUnsafe(TChunk& chunk, const Size_t firstNodeIndex, const Size_t nodeCount)
        {
            assert_pnc(!chunk.IsNull());
            assert_pnc(firstNodeIndex >= 0);
            assert_pnc(nodeCount >= 0);

            const ChunkStructure_t& chunkStructure = chunk.GetStructure();
            Size_t countDestructible = chunkStructure.DestructibleNodeIndex.size();
            for (Size_t i = 0; i < countDestructible; ++i)
            {
                Size_t index = chunkStructure.DestructibleNodeIndex[i];
                void* componentData = chunk->GetComponentData(index);
                chunkStructure.Components[index]->DestructComponentUnsafe(componentData, firstNodeIndex, nodeCount);
            }
        }


        template<typename TChunk>
        static void ConstructNode(TChunk& chunk,
            const Size_t firstNodeIndex, const Size_t nodeCount,
            const Size_t firstChunkIndex = 0, const Size_t chunkCount = 1)
        {
            assert_pnc(!chunk.IsNull());
            assert_pnc(firstNodeIndex >= 0);
            assert_pnc(nodeCount >= 0);
            assert_pnc(firstChunkIndex >= 0);
            assert_pnc(chunkCount >= 0);
            assert_pnc(firstNodeIndex < chunk.GetNodeCount());
            assert_pnc(firstNodeIndex + nodeCount <= chunk.GetNodeCount());
            assert_pnc(firstChunkIndex < chunk.GetChunkCount());
            assert_pnc(firstChunkIndex + chunkCount <= chunk.GetChunkCount());

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
            assert_pnc(!chunk.IsNull());
            assert_pnc(firstNodeIndex >= 0);
            assert_pnc(nodeCount >= 0);
            assert_pnc(firstChunkIndex >= 0);
            assert_pnc(chunkCount >= 0);
            assert_pnc(firstNodeIndex < chunk.GetNodeCount());
            assert_pnc(firstNodeIndex + nodeCount <= chunk.GetNodeCount());
            assert_pnc(firstChunkIndex < chunk.GetChunkCount());
            assert_pnc(firstChunkIndex + chunkCount <= chunk.GetChunkCount());

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
        static void CopyNode(TChunkTo& chunkTo, const TChunkFrom& chunkFrom,
            const Size_t firstNodeIndexTo, const Size_t firstNodeIndexFrom, const Size_t nodeCount,
            const Size_t firstChunkIndexTo = 0, const Size_t firstChunkIndexFrom = 0, const Size_t chunkCount = 1)
        {
            assert_pnc(!chunkTo.IsNull());
            assert_pnc(!chunkFrom.IsNull());
            assert_pnc(IsSameChunkStructure(chunkTo, chunkFrom));
            assert_pnc(firstNodeIndexTo >= 0);
            assert_pnc(firstNodeIndexFrom >= 0);
            assert_pnc(nodeCount >= 0);
            assert_pnc(firstChunkIndexTo >= 0);
            assert_pnc(firstChunkIndexFrom >= 0);
            assert_pnc(chunkCount >= 0);
            assert_pnc(firstNodeIndexTo < chunkTo.GetNodeCount());
            assert_pnc(firstNodeIndexTo + nodeCount <= chunkTo.GetNodeCount());
            assert_pnc(firstChunkIndexTo < chunkTo.GetChunkCount());
            assert_pnc(firstChunkIndexTo + chunkCount <= chunkTo.GetChunkCount());
            assert_pnc(firstNodeIndexFrom < chunkFrom.GetNodeCount());
            assert_pnc(firstNodeIndexFrom + nodeCount <= chunkFrom.GetNodeCount());
            assert_pnc(firstChunkIndexFrom < chunkFrom.GetChunkCount());
            assert_pnc(firstChunkIndexFrom + chunkCount <= chunkFrom.GetChunkCount());

            const ChunkStructure_t& chunkStructure = chunkTo.GetStructure();
            Size_t componentCount = chunkStructure.GetComponentCount();
            for (Size_t i = 0; i < componentCount; ++i)
            {
                const ComponentType_t* componentType = chunkStructure.Components[i];
                componentType->CopyComponentUnsafe(chunkTo.GetComponentData(i), chunkFrom.GetComponentData(i),
                    firstNodeIndexTo, firstNodeIndexFrom, nodeCount,
                    firstChunkIndexTo, firstChunkIndexFrom, chunkCount);
            }
        }

        template<typename TChunkTo, typename TChunkFrom>
        static void MoveNode(TChunkTo& chunkTo, const TChunkFrom& chunkFrom,
            const Size_t firstNodeIndexTo, const Size_t firstNodeIndexFrom, const Size_t nodeCount,
            const Size_t firstChunkIndexTo = 0, const Size_t firstChunkIndexFrom = 0, const Size_t chunkCount = 1)
        {
            assert_pnc(!chunkTo.IsNull());
            assert_pnc(!chunkFrom.IsNull());
            assert_pnc(IsSameChunkStructure(chunkTo, chunkFrom));
            assert_pnc(firstNodeIndexTo >= 0);
            assert_pnc(firstNodeIndexFrom >= 0);
            assert_pnc(nodeCount >= 0);
            assert_pnc(firstChunkIndexTo >= 0);
            assert_pnc(firstChunkIndexFrom >= 0);
            assert_pnc(chunkCount >= 0);
            assert_pnc(firstNodeIndexTo < chunkTo.GetNodeCount());
            assert_pnc(firstNodeIndexTo + nodeCount <= chunkTo.GetNodeCount());
            assert_pnc(firstChunkIndexTo < chunkTo.GetChunkCount());
            assert_pnc(firstChunkIndexTo + chunkCount <= chunkTo.GetChunkCount());
            assert_pnc(firstNodeIndexFrom < chunkFrom.GetNodeCount());
            assert_pnc(firstNodeIndexFrom + nodeCount <= chunkFrom.GetNodeCount());
            assert_pnc(firstChunkIndexFrom < chunkFrom.GetChunkCount());
            assert_pnc(firstChunkIndexFrom + chunkCount <= chunkFrom.GetChunkCount());

            const ChunkStructure_t& chunkStructure = chunkTo.GetStructure();
            Size_t componentCount = chunkStructure.GetComponentCount();
            for (Size_t i = 0; i < componentCount; ++i)
            {
                const ComponentType_t* componentType = chunkStructure.Components[i];
                componentType->MoveComponentUnsafe(chunkTo.GetComponentData(i), chunkFrom.GetComponentData(i),
                    firstNodeIndexTo, firstNodeIndexFrom, nodeCount,
                    firstChunkIndexTo, firstChunkIndexFrom, chunkCount);
            }
        }

        template<typename TChunkTo, typename TChunkFrom>
        static void SwapNode(TChunkTo& chunkTo, const TChunkFrom& chunkFrom,
            const Size_t firstNodeIndexTo, const Size_t firstNodeIndexFrom, const Size_t nodeCount,
            const Size_t firstChunkIndexTo = 0, const Size_t firstChunkIndexFrom = 0, const Size_t chunkCount = 1)
        {
            assert_pnc(!chunkTo.IsNull());
            assert_pnc(!chunkFrom.IsNull());
            assert_pnc(IsSameChunkStructure(chunkTo, chunkFrom));
            assert_pnc(firstNodeIndexTo >= 0);
            assert_pnc(firstNodeIndexFrom >= 0);
            assert_pnc(nodeCount >= 0);
            assert_pnc(firstChunkIndexTo >= 0);
            assert_pnc(firstChunkIndexFrom >= 0);
            assert_pnc(chunkCount >= 0);
            assert_pnc(firstNodeIndexTo < chunkTo.GetNodeCount());
            assert_pnc(firstNodeIndexTo + nodeCount <= chunkTo.GetNodeCount());
            assert_pnc(firstChunkIndexTo < chunkTo.GetChunkCount());
            assert_pnc(firstChunkIndexTo + chunkCount <= chunkTo.GetChunkCount());
            assert_pnc(firstNodeIndexFrom < chunkFrom.GetNodeCount());
            assert_pnc(firstNodeIndexFrom + nodeCount <= chunkFrom.GetNodeCount());
            assert_pnc(firstChunkIndexFrom < chunkFrom.GetChunkCount());
            assert_pnc(firstChunkIndexFrom + chunkCount <= chunkFrom.GetChunkCount());

            const ChunkStructure_t& chunkStructure = chunkTo.GetStructure();
            Size_t componentCount = chunkStructure.GetComponentCount();
            for (Size_t i = 0; i < componentCount; ++i)
            {
                const ComponentType_t* componentType = chunkStructure.Components[i];
                componentType->SwapComponentUnsafe(chunkTo.GetComponentData(i), chunkFrom.GetComponentData(i),
                    firstNodeIndexTo, firstNodeIndexFrom, nodeCount,
                    firstChunkIndexTo, firstChunkIndexFrom, chunkCount);
            }
        }

        template<typename TChunkA, typename TChunkB>
        static bool IsSameChunkStructure(const TChunkA& a, const TChunkB& b) { return a.GetStructure() == b.GetStructure(); }





        //static void DestructComponentUnsafe(const ComponentType_t& componentType, void* const componentData, const Size_t firstNodeIndex, const Size_t nodeCount)
        //{
        //    assert_pnc(componentData);
        //    assert_pnc(firstNodeIndex >= 0);
        //    assert_pnc(nodeCount >= 0);
        //    componentType.DestructNodesUnsafe(componentData, firstNodeIndex, nodeCount);
        //}
        //static void CopyComponentUnsafe(const ComponentType_t& componentType, void* const componentDataTo, void* const componentDataFrom, const Size_t firstNodeIndexTo, const Size_t firstNodeIndexFrom, const Size_t nodeCount)
        //{
        //    assert_pnc(componentDataTo);
        //    assert_pnc(componentDataFrom);
        //    assert_pnc(firstNodeIndexTo >= 0);
        //    assert_pnc(firstNodeIndexFrom >= 0);
        //    assert_pnc(nodeCount >= 0);
        //    componentType.CopyNodesUnsafe(componentDataTo, componentDataFrom, firstNodeIndexTo, firstNodeIndexFrom, nodeCount);
        //}
        //static void MoveComponentUnsafe(const ComponentType_t& componentType, void* const componentDataTo, void* const componentDataFrom, const Size_t firstNodeIndexTo, const Size_t firstNodeIndexFrom, const Size_t nodeCount)
        //{
        //    assert_pnc(componentDataTo);
        //    assert_pnc(componentDataFrom);
        //    assert_pnc(firstNodeIndexTo >= 0);
        //    assert_pnc(firstNodeIndexFrom >= 0);
        //    assert_pnc(nodeCount >= 0);
        //    componentType.MoveNodesUnsafe(componentDataTo, componentDataFrom, firstNodeIndexTo, firstNodeIndexFrom, nodeCount);
        //}
        //static void SwapComponentUnsafe(const ComponentType_t& componentType, void* const componentDataTo, void* const componentDataFrom, const Size_t firstNodeIndexTo, const Size_t firstNodeIndexFrom, const Size_t nodeCount)
        //{
        //    assert_pnc(componentDataTo);
        //    assert_pnc(componentDataFrom);
        //    assert_pnc(firstNodeIndexTo >= 0);
        //    assert_pnc(firstNodeIndexFrom >= 0);
        //    assert_pnc(nodeCount >= 0);
        //    componentType.SwapNodesUnsafe(componentDataTo, componentDataFrom, firstNodeIndexTo, firstNodeIndexFrom, nodeCount);
        //}

        //template<typename TChunk>
        //static void ConstructNodesUnsafe(TChunk& chunk, const Size_t firstNodeIndex, const Size_t nodeCount)
        //{
        //    assert_pnc(!chunk.IsNull());
        //    assert_pnc(firstNodeIndex >= 0);
        //    assert_pnc(nodeCount >= 0);

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
        //    assert_pnc(!chunk.IsNull());
        //    assert_pnc(firstNodeIndex >= 0);
        //    assert_pnc(nodeCount >= 0);

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
