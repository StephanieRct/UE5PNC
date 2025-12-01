// MIT License
// Copyright (c) 2025 Stephanie Rancourt
// 
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
// 
// The above copyright notice and this permission notice shall be included in all
// copies or substantial portions of the Software.
// 
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
// SOFTWARE.

#pragma once
#include "common.h"
#include "ComponentType.h"
#include "ComponentTypeSet.h"
#include "ChunkStructure.h"


#include "DChunkPointer.h"
#include "DBucketPointer.h"
#include "DBarrelPointer.h"
#include "DArrayPointer.h"

#include "DOwn.h"
#include "DGrow.h"

#include "DChunk.h"
#include "DBucket.h"
#include "DBunch.h"
#include "DUniformChunkArray.h"
#include "DChunkArray.h"
#include "DChunkBarrel.h"
#include "DChunkBunch.h"
#include "DBucketBarrel.h"
#include "DBucketBunch.h"

#include "ChunkArrayExtension.h"

#include "ChunkPointerInternal.h"

namespace PNC
{

    template<typename TChunkStructure>
    struct Container
    {
    public:
        using Base_t = void;
        using Self_t = Container<TChunkStructure>;
        using Size_t = typename TChunkStructure::Size_t;
        using ChunkStructure_t = TChunkStructure;
        using ComponentType_t = typename ChunkStructure_t::ComponentType_t;

        Container() = default;
        template<typename TProps>
        Container(const DPropsTag& tag, const TProps& props)
        {
        }
    };


    template<typename TChunkStructure>
    using ChunkPointerT = DChunkPointer< ChunkPointerInternalT<TChunkStructure>, Container<TChunkStructure>>;

    //template<typename TChunkStructure>
    //using BucketChunk = ChunkCapacityAllocationT<BucketChunkPointerT<ChunkPointerT<TChunkStructure>>>;

    //template<typename TChunkStructure, typename TChunkPointerElement>
    //using BucketChunkArray = ChunkArrayCapacityAllocationT<BucketChunkArrayPointerT<ChunkArrayPointerT<ChunkStructure, TChunkPointerElement>>>;

    //ChunkPointerT
    //ChunkT
    // -----------------
    // |#|#|#|#|#|#|#|#|
    // -----------------
    template<typename TChunkStructure>
    using ChunkT = DChunk<DOwn<ChunkPointerT<TChunkStructure>>>;

    //template<typename TChunkStructure>
    //using KChunkTreePointerT = DChunkPointer<ChunkPointerT<TChunkStructure>, KTreePointerT<TChunkStructure>>;
    //template<typename TChunkStructure>
    //using KChunkTreeT = DOwnT<DBucketPointerT<KChunkTreePointerT<TChunkStructure>>>;

    //BucketPointerT
    //BucketT
    // -----------------
    // |#|#|#|#|#| | | |
    // -----------------
    template<typename TChunkStructure>
    using BucketPointerT = DBucketPointer<ChunkPointerT<TChunkStructure>>;

    template<typename TChunkStructure>
    using BucketT = DBucket<DOwn<BucketPointerT<TChunkStructure>>>;


    //BunchPointerT
    //BunchT
    // -----------------
    // |#|#|#|#|#| | | | ...
    // -----------------
    template<typename TChunkStructure>
    using BunchPointerT = DGrow<DBucketPointer<ChunkPointerT<TChunkStructure>>>;

    template<typename TChunkStructure>
    using BunchT = DBunch<DOwn<BunchPointerT<TChunkStructure>>>;





    template<typename TChunkStructure, typename TChunkPointerElement = ChunkPointerT<TChunkStructure>>
    using ChunkArrayPointerT = DArrayPointerT<ChunkArrayExtensionT<TChunkStructure, TChunkPointerElement>,
        ChunkPointerT<TChunkStructure>>;

    template<typename TChunkStructure, typename TChunkPointerElement>
    using ChunkArrayT = DChunkArray<DOwn<ChunkArrayPointerT<TChunkStructure, TChunkPointerElement>>>;




    //ChunkArrayPointerT
    //UniformChunkArrayPointerT
    //UniformChunkArray
    // -------------------------------------------------------
    // |-----------------|-----------------|-----------------|
    // ||#|#|#|#|#|#|#|#|||#|#|#|#|#|#|#|#|||#|#|#|#|#|#|#|#||
    // |-----------------|-----------------|-----------------|
    // -------------------------------------------------------
    template<typename TChunkStructure, typename TChunkPointerElement = ChunkPointerT<TChunkStructure>>
    using UniformChunkArrayPointerT = ChunkArrayPointerT<TChunkStructure, TChunkPointerElement>;
    template<typename TChunkStructure, typename TChunkPointerElement = ChunkPointerT<TChunkStructure>>
    using UniformChunkArrayT = DUniformChunkArray<DOwn<UniformChunkArrayPointerT<TChunkStructure, TChunkPointerElement>>>;


    //UniformBucketArray
    // -------------------------------------------------------
    // |-----------------|-----------------|-----------------|
    // ||#|#|#| | | | | |||#|#|#|#|#|#| | |||#| | | | | | | ||
    // |-----------------|-----------------|-----------------|
    // -------------------------------------------------------
    //
    //UniformChunkBarrel
    // -------------------------------------------------------
    // |-----------------|-----------------|                 |
    // ||#|#|#|#|#|#|#|#|||#|#|#|#|#|#|#|#||                 |
    // |-----------------|-----------------|                 |
    // -------------------------------------------------------
    //
    //UniformBucketBarrel
    // -------------------------------------------------------
    // |-----------------|-----------------|                 |
    // ||#|#|#|#|#| | | |||#|#| | | | | | ||                 |
    // |-----------------|-----------------|                 |
    // -------------------------------------------------------



    //ChunkArray
    // -------------------------------------------------------
    // |------|-----------------|--------|-------------------|
    // ||#|#|#||#|#|#|#|#|#|#|#|||#|#|#|#||#|#|#|#|#|#|#|#|#||
    // |------|-----------------|--------|-------------------|
    // -------------------------------------------------------

    //ChunkBarrel
    // -------------------------------------------------------
    // |------|-----------------|                            |
    // ||#|#|#||#|#|#|#|#|#|#|#||                            |
    // |------|-----------------|                            |
    // -------------------------------------------------------


    //BucketBarrel
    // -------------------------------------------------------
    // |------|-----------------|--------|                   |
    // ||#|#| ||#|#|#| | | | | |||#|#|#|#|                   |
    // |------|-----------------|--------|                   |
    // -------------------------------------------------------


    //template<typename TChunkStructure, typename TChunkPointerElement>
    //using ArrayChunkPointerT = ChunkArrayPointerT<TChunkStructure, TChunkPointerElement>;
    //
    //template<typename TChunkStructure, typename TChunkPointerElement>
    //using ArrayChunkT = ChunkArrayCapacityAllocationT<ChunkArrayPointerT<TChunkStructure, TChunkPointerElement>>;
    //template<typename TChunkStructure, typename TAlgorithm, typename TChunkPointerElement>
    //struct AlgorithmRunner< TChunkStructure, TAlgorithm, ArrayChunkT<TChunkStructure, TChunkPointerElement>> : public AlgorithmRunnerChunkArray<TAlgorithm, ArrayChunkPointerT<TChunkStructure, TChunkPointerElement>>{};



    ///////////////////////////////////////////////////////////////////


    template<template<typename> typename TDecorator0>
    struct ContainerHasDecorator0T
    {

        template<typename TContainer, typename TBase>
        consteval static bool Select(const TContainer* c, const TDecorator0<TBase>* d)
        {
            //return TContainer::nothing;
            //static_assert(false);
            return true;
        }
        template<typename TContainer, typename TChunkStructure>
        consteval static bool Select(const TContainer* c, const Container<TChunkStructure>* d)
        {
            //return TContainer::nothing;
            return false;
        }
        //template<typename TContainer, typename TBase, template<typename> typename TDecoratorOther>
        //consteval static bool Select(const TContainer* c, const TDecoratorOther<TBase>* d)
        //{
        //    return TContainer::nothing;
        //    return false;
        //}
        //template<typename TContainer, typename TBase, typename TValueOther, template<typename, typename> typename TDecoratorOther>
        //consteval static bool Select(const TContainer* c, const TDecoratorOther<TValueOther, TBase>* d)
        //{
        //    return TContainer::nothing;
        //    return false;
        //}
    };
    template<template<typename, typename > typename TDecorator1, typename TValue>
    struct ContainerHasDecorator1T
    {
        template<typename TContainer, typename TBase>
        consteval static bool Select(const TContainer* c, const TDecorator1<TValue, TBase>* d)
        {
            //return TContainer::nothing;
            return true;
        }
        template<typename TContainer, typename TChunkStructurer>
        consteval static bool Select(const TContainer* c, const Container<TChunkStructurer>* d)
        {
            //return TContainer::nothing;
            return false;
        }
    };

    template<typename TContainer, template<typename> typename TDecorator>
    consteval bool ContainerHasDecorator()
    {
        return ContainerHasDecorator0T< TDecorator>::Select((const TContainer*)nullptr, (const TContainer*)nullptr);
    }
    template<typename TContainer, template<typename, typename> typename TDecorator, typename TValue >
    consteval bool ContainerHasDecorator()
    {
        return ContainerHasDecorator1T< TDecorator, TValue>::Select((const TContainer*)nullptr, (const TContainer*)nullptr);
    }



}
