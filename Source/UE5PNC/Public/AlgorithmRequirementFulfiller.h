// MIT License
// Copyright (c) 2025 Stephanie Rancourt

#pragma once
#include "common.h"

namespace PNC
{
    /// <summary>
    /// An AlgorithmRequirementFulfiller is used to receive the requirements of 
    /// an algorithm such as component data access.
    /// This struct is used as a base for all AlgorithmRequirementFulfillers and
    /// it will fail all the requirements for any algorithm.
    /// </summary>
    struct AlgorithmRequirementFulfiller
    {
    public:

        /// <summary>
        /// Get access to a specific component in the current chunk.
        /// </summary>
        /// <typeparam name="T"></typeparam>
        /// <param name="component"></param>
        /// <returns></returns>
        template<typename TComponent>
        bool Component(TComponent*& component)
        {
            return false;
        }

        /// <summary>
        /// Get access to a specific component in the parent chunk.
        /// </summary>
        /// <typeparam name="TComponent"></typeparam>
        /// <param name="component"></param>
        /// <returns></returns>
        template<typename TComponent>
        bool ParentComponent(TComponent*& component)
        {
            return false;
        }

        /// <summary>
        /// Get the current chunk index when processing arrays of chunks
        /// </summary>
        /// <typeparam name="TSize"></typeparam>
        /// <param name="index"></param>
        /// <returns></returns>
        template<typename TSize>
        bool ChunkIndex(TSize& index)
        {
            return false;
        }


        template<typename TChunkPointer>
        bool ParentChunk(TChunkPointer*& parent)
        {
            return false;
        }

        template<typename TChunkPointer>
        bool ChildrenChunk(TChunkPointer*& children)
        {
            return false;
        }
    };
}
