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

namespace NiT
{
    template<typename TChunkStructure>
    using KindPointerT = DKind<Container<TChunkStructure>>;

    template<typename TChunkStructure>
    using KChunkPointerT = DChunkPointer<ChunkPointerProtectedT<TChunkStructure>,
                           DOfKind<ContainerKind::Chunk,
                           KindPointerT<TChunkStructure>>>;

    template<typename TChunkStructure>
    using KBucketPointerT = DBucketPointer< 
                            KChunkPointerT<TChunkStructure>>;

    template<typename TChunkStructure>
    using KBunchPointerT = DBunchPointer< 
                           KBucketPointerT<TChunkStructure>>;

    template<typename TChunkStructure, typename TChunkPointerElement>
    using KArrayPointerT = DArrayPointer<ChunkArrayExtensionT<TChunkStructure, TChunkPointerElement>,
                           KChunkPointerT<TChunkStructure> >;

    template<typename TChunkStructure>
    using KChunkT = DChunk<
                    DOwn<
                    KChunkPointerT<TChunkStructure>>>;

    template<typename TChunkStructure>
    using KBucketT = DBucket<
                     DOwn<
                     KBucketPointerT<TChunkStructure>>>;

    template<typename TChunkStructure>
    using KBunchT = DBunch<
                    DOwn<
                    KBunchPointerT<TChunkStructure>>>;

    template<typename TChunkStructure, typename TChunkPointerElement>
    using KArrayT = DArray<
                    DOwn<
                    KArrayPointerT<TChunkStructure, TChunkPointerElement>>>;


    template<typename TChunkStructure>
    using KTreePointerT = DTreePointer<
                               DKind<
                               Container<TChunkStructure>>>;

    template<typename TChunkStructure>
    using KChunkTreePointerT = DChunkPointer<ChunkPointerProtectedT<TChunkStructure>,
                               DOfKind<ContainerKind::ChunkTree,
                               KTreePointerT<TChunkStructure>>>;

    template<typename TChunkStructure>
    using KChunkTreeT = DChunk< // TODO replace DChunk with a DChunkTree decorator and add them to AlgorithmRunner
                        DOwn<
                        KChunkTreePointerT<TChunkStructure>>>;

    template<typename TChunkStructure>
    using KBucketTreePointerT = DBucketPointer<
                                KChunkTreePointerT<TChunkStructure>>;

    template<typename TChunkStructure>
    using KBucketTreeT = DBucket< // TODO replace DBucket with a DBucketTree decorator and add them to AlgorithmRunner
                         DOwn<
                         KBucketTreePointerT<TChunkStructure>>>;

    template<typename TChunkStructure>
    using KBunchTreePointerT = DBunchPointer<
                               KBucketTreePointerT<TChunkStructure>>;

    template<typename TChunkStructure>
    using KBunchTreeT = DBucket< // TODO replace DBucket with a DBunchTree decorator and add them to AlgorithmRunner
                        DOwn<
                        KBunchTreePointerT<TChunkStructure>>>;

    template<typename TChunkStructure, typename TChunkPointerElement>
    using KArrayTreePointerT = DArrayPointer<ChunkArrayExtensionT<TChunkStructure, TChunkPointerElement>,
                               DOfKind<ContainerKind::ArrayTree,
                               KChunkTreePointerT<TChunkStructure>>>;

    template<typename TChunkStructure, typename TChunkPointerElement>
    using KArrayTreeT = DArray< // TODO replace DArray with a DChunkArrayTree decorator and add them to AlgorithmRunner
                        DOwn<
                        KArrayTreePointerT<TChunkStructure, ChunkPointerT<TChunkStructure>>>>;

    // TODO complete
    template<typename TChunkStructure, typename TChunkPointerElement>
    using KBucketBarrelTreeT = DBucketBarrel<DOwn<DBarrelPointer<KArrayTreePointerT<TChunkStructure, ChunkPointerT<TChunkStructure>>>>>;
}

