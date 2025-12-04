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
#include "Containers.h"
#include "DKind.h"
#include "DOfKind.h"
#include "DTreePointer.h"


namespace PNC
{

    template<typename TChunkStructure>
    using KKindPointerT = DKind<Container<TChunkStructure>>;

    template<typename TChunkStructure>
    using KChunkPointerT = DChunkPointer<ChunkPointerInternalT<TChunkStructure>, KKindPointerT<TChunkStructure>>;

    template<typename TChunkStructure, typename TChunkPointerElement>
    using KChunkArrayPointerT = DArrayPointer<ChunkArrayExtensionT<TChunkStructure, TChunkPointerElement>,
        KChunkPointerT<TChunkStructure> >;


    template<typename TChunkStructure>
    using KChunkT = DChunk<DOwn<KChunkPointerT<TChunkStructure>>>;

    template<typename TChunkStructure>
    using KBucketT = DBucket<DOwn<DBucketPointer<KChunkPointerT<TChunkStructure>>>>;

    template<typename TChunkStructure>
    using KBunchT = DBunch<DOwn<DBunchPointer<DBucketPointer<KChunkPointerT<TChunkStructure>>>>>;



    template<typename TChunkStructure, typename TChunkPointerElement>
    using KChunkArrayT = DBucketBarrel<DOwn<DBarrelPointer<KChunkArrayPointerT<TChunkStructure, ChunkPointerT<TChunkStructure>>>>>;


//    template<typename TChunkStructure>
//    using KChunkTreePointer2T =
//        DChunkPointer<ChunkPointerInternalT<TChunkStructure>,
//        DTreePointer<
//        DKind<
//        Container<TChunkStructure>>>>>;

    template<typename TChunkStructure>
    using KChunkTreePointerT = 
        DChunkPointer<ChunkPointerInternalT<TChunkStructure>,
        DOfKind<ContainerKind::ChunkTree,
        DTreePointer<
        DKind<
        Container<TChunkStructure>>>>>;
    template<typename TChunkStructure>
    // TODO replace DChunk with a DChunkTree decorator and add them to AlgorithmRunner
    using KChunkTreeT = DChunk<DOwn<KChunkTreePointerT<TChunkStructure>>>;

    template<typename TChunkStructure>
    using KBucketTreePointerT = DBucketPointer<KChunkTreePointerT<TChunkStructure>>;
    // TODO replace DBucket with a DBucketTree decorator and add them to AlgorithmRunner
    template<typename TChunkStructure>
    using KBucketTreeT = DBucket<DOwn<KBucketTreePointerT<TChunkStructure>>>;

    template<typename TChunkStructure>
    using KBunchTreePointerT = DBunchPointer<KBucketTreePointerT<TChunkStructure>>;
    // TODO replace DBucket with a DBunchTree decorator and add them to AlgorithmRunner
    template<typename TChunkStructure>
    using KBunchTreeT = DBucket<DOwn<KBunchTreePointerT<TChunkStructure>>>;

    template<typename TChunkStructure, typename TChunkPointerElement>
    using KChunkArrayTreePointerT = 
        DArrayPointer<ChunkArrayExtensionT<TChunkStructure, TChunkPointerElement>,
        DOfKind<ContainerKind::ChunkArrayTree,
        KChunkTreePointerT<TChunkStructure>>>;

    // TODO replace DChunkArray with a DChunkArrayTree decorator and add them to AlgorithmRunner
    template<typename TChunkStructure, typename TChunkPointerElement>
    using KChunkArrayTreeT = DArray<DOwn<KChunkArrayTreePointerT<TChunkStructure, ChunkPointerT<TChunkStructure>>>>;



    template<typename TChunkStructure, typename TChunkPointerElement>
    using KBucketBarrelTreeT = DBucketBarrel<DOwn<DBarrelPointer<KChunkArrayTreePointerT<TChunkStructure, ChunkPointerT<TChunkStructure>>>>>;


}

