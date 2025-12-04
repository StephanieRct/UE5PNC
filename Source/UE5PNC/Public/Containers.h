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
#include "DChunk.h"
#include "DBucketPointer.h"
#include "DBucket.h"
#include "DBunchPointer.h"
#include "DBunch.h"

#include "DArrayPointer.h"
#include "DArray.h"
#include "DUniformArray.h"
#include "DBarrelPointer.h"
#include "DBarrel.h"
#include "DCarryPointer.h"
#include "DCarry.h"

#include "DBucketArray.h"
#include "DBucketBarrel.h"
#include "DBucketCarry.h"

#include "ChunkPointerInternal.h"
#include "ChunkArrayExtension.h"

#include "DOwn.h"

namespace PNC
{
    /// <summary>
    /// Container is the base of all containers.
    /// All container types are sequences of decorators over the Container struct.
    /// </summary>
    /// <typeparam name="TChunkStructure"></typeparam>
    template<typename TChunkStructure>
    struct Container
    {
    public:
        using Base_t = void;
        using Self_t = Container<TChunkStructure>;
        using Size_t = typename TChunkStructure::Size_t;
        using ChunkStructure_t = TChunkStructure;
        using ComponentType_t = typename ChunkStructure_t::ComponentType_t;
        using ComponentTypeSet_t = typename ChunkStructure_t::ComponentTypeSet_t;

        Container() = default;

        template<typename TProps>
        Container(const DPropsTag& tag, const TProps& props)
        {
        }
    };

    // Chunk
    //  ---------------
    // |#|#|#|#|#|#|#|#|
    //  ---------------
    template<typename TChunkStructure>
    using ChunkPointerT = 
              DChunkPointer< ChunkPointerProtectedT<TChunkStructure>,
              Container<TChunkStructure>>;
    template<typename TChunkStructure>
    using ChunkT = 
              DChunk<
              DOwn<
              ChunkPointerT<TChunkStructure>>>;

    // Bucket
    //  ---------------
    // |#|#|#|#|#| | | |
    //  ---------------
    template<typename TChunkStructure>
    using BucketPointerT = 
              DBucketPointer<
              ChunkPointerT<TChunkStructure>>;
    template<typename TChunkStructure>
    using BucketT = 
              DBucket<
              DOwn<
              BucketPointerT<TChunkStructure>>>;

    // Bunch
    //  -----------------
    // |#|#|#|#|#| | | | |...
    //  -----------------
    template<typename TChunkStructure>
    using BunchPointerT = 
              DBunchPointer<
              BucketPointerT<TChunkStructure>>;
    template<typename TChunkStructure>
    using BunchT = 
              DBunch<
              DOwn<
              BunchPointerT<TChunkStructure>>>;

    // Array
    //  -----------------------------------
    // |-----------|-----------------|-----|
    // |#|#|#|#|#|#|#|#|#|#|#|#|#|#|#|#|#|#|
    // |-----------|-----------------|-----|
    //  -----------------------------------
    template<typename TChunkStructure, typename TChunkPointerElement = ChunkPointerT<TChunkStructure>>
    using ArrayPointerT = 
              DArrayPointer<ChunkArrayExtensionT<TChunkStructure, TChunkPointerElement>,
              ChunkPointerT<TChunkStructure>>;
    template<typename TChunkStructure, typename TChunkPointerElement>
    using ArrayT = 
              DArray<
              DOwn<
              ArrayPointerT<TChunkStructure, TChunkPointerElement>>>;

    // UniformArray
    //  -----------------------------------------------------
    // |-----------------|-----------------|-----------------|
    // |#|#|#|#|#|#|#|#|#|#|#|#|#|#|#|#|#|#|#|#|#|#|#|#|#|#|#|
    // |-----------------|-----------------|-----------------|
    //  -----------------------------------------------------
    template<typename TChunkStructure, typename TChunkPointerElement = ChunkPointerT<TChunkStructure>>
    using UniformArrayPointerT = 
              ArrayPointerT<TChunkStructure, TChunkPointerElement>;
    template<typename TChunkStructure, typename TChunkPointerElement = ChunkPointerT<TChunkStructure>>
    using UniformArrayT = 
              DUniformArray<
              DOwn<
              UniformArrayPointerT<TChunkStructure, TChunkPointerElement>>>;

    // Barrel
    //  ------------------------------------------------
    // |-----------|-----------------|-----|------------|
    // |#|#|#|#|#|#|#|#|#|#|#|#|#|#|#|#|#|#| | | | | |  |
    // |-----------|-----------------|-----|------------|
    //  ------------------------------------------------
    template<typename TChunkStructure, typename TChunkPointerElement = ChunkPointerT<TChunkStructure>>
    using BarrelPointerT =
            DBarrelPointer<ArrayPointerT<TChunkStructure, TChunkPointerElement>>;
    template<typename TChunkStructure, typename TChunkPointerElement>
    using ChunkBarrelT =
              DBarrel<
              DOwn<
              BarrelPointerT<TChunkStructure, TChunkPointerElement>>>;

    // Carry
    //  ------------------------------------------------
    // |-----------|-----------------|-----|------------|
    // |#|#|#|#|#|#|#|#|#|#|#|#|#|#|#|#|#|#| | | | | |  |...
    // |-----------|-----------------|-----|------------|
    //  ------------------------------------------------
    template<typename TChunkStructure, typename TChunkPointerElement = ChunkPointerT<TChunkStructure>>
    using CarryPointerT =
              DCarryPointer<BarrelPointerT<TChunkStructure, TChunkPointerElement>>;
    template<typename TChunkStructure, typename TChunkPointerElement>
    using ChunkCarryT =
              DCarry<
              DOwn<
              CarryPointerT<TChunkStructure, TChunkPointerElement>>>;

    // TODO BucketArray
    //  -----------------------------------
    // |-----------|-----------------|-----|
    // |#|#|#|#| | |#| | | | | | | | |#|#|#|
    // |-----------|-----------------|-----|
    //  -----------------------------------
    template<typename TChunkStructure, typename TChunkPointerElement = ChunkPointerT<TChunkStructure>>
    using BucketArrayPointerT =
              DArrayPointer<ChunkArrayExtensionT<TChunkStructure, TChunkPointerElement>,
              BucketPointerT<TChunkStructure>>;
    template<typename TChunkStructure, typename TChunkPointerElement>
    using BucketArrayT =
              DBucketArray<
              DOwn<
              BucketArrayPointerT<TChunkStructure, TChunkPointerElement>>>;

    // TODO BucketBarrel
    //  ------------------------------------------------
    // |-----------|-----------------|-----|------------|
    // |#|#|#|#| | |#| | | | | | | | |#|#|#| | | | | |  |
    // |-----------|-----------------|-----|------------|
    //  ------------------------------------------------
    template<typename TChunkStructure, typename TChunkPointerElement = ChunkPointerT<TChunkStructure>>
    using BucketBarrelPointerT =
              DBarrelPointer<
              BucketPointerT<TChunkStructure>>;
    template<typename TChunkStructure, typename TChunkPointerElement>
    using BucketBarrelT =
              DBucketBarrel<
              DOwn<
              BucketBarrelPointerT<TChunkStructure, TChunkPointerElement>>>;

    // TODO BucketCarry
    //  ------------------------------------------------
    // |-----------|-----------------|-----|------------|
    // |#|#|#|#| | |#| | | | | | | | |#|#|#| | | | | |  |...
    // |-----------|-----------------|-----|------------|
    //  ------------------------------------------------
    template<typename TChunkStructure, typename TChunkPointerElement = ChunkPointerT<TChunkStructure>>
    using BucketCarryPointerT =
              DCarryPointer<
              BucketBarrelPointerT<TChunkStructure, TChunkPointerElement>>;
    template<typename TChunkStructure, typename TChunkPointerElement>
    using BucketCarryT =
              DBucketCarry<
              DOwn<
              BucketCarryPointerT<TChunkStructure, TChunkPointerElement>>>;


    namespace 
    {
        template<template<typename> typename TDecorator0>
        struct ContainerHasDecorator0T
        {

            template<typename TContainer, typename TBase>
            consteval static bool Select(const TContainer* c, const TDecorator0<TBase>* d) { return true; }
            template<typename TContainer, typename TChunkStructure>
            consteval static bool Select(const TContainer* c, const Container<TChunkStructure>* d) { return false; }
        };
        template<template<typename, typename > typename TDecorator1, typename TValue>
        struct ContainerHasDecorator1T
        {
            template<typename TContainer, typename TBase>
            consteval static bool Select(const TContainer* c, const TDecorator1<TValue, TBase>* d) { return true; }
            template<typename TContainer, typename TChunkStructurer>
            consteval static bool Select(const TContainer* c, const Container<TChunkStructurer>* d) { return false; }
        };
    }

    /// <summary>
    /// Evaluates to true if the container type has a decorator type as one of its bases
    /// </summary>
    template<typename TContainer, template<typename> typename TDecorator>
    consteval bool ContainerHasDecorator()
    {
        return ContainerHasDecorator0T< TDecorator>::Select((const TContainer*)nullptr, (const TContainer*)nullptr);
    }

    /// <summary>
    /// Evaluates to true if the container type has a decorator type as one of its bases
    /// </summary>
    template<typename TContainer, template<typename, typename> typename TDecorator, typename TValue >
    consteval bool ContainerHasDecorator()
    {
        return ContainerHasDecorator1T< TDecorator, TValue>::Select((const TContainer*)nullptr, (const TContainer*)nullptr);
    }
}
