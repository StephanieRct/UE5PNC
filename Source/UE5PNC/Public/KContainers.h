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

#include "KChunkArrayPointer.h"
#include "KTreePointer.h"


namespace PNC
{

    template<typename TChunkStructure>
    using KChunkPointerT = DChunkPointer<ChunkPointerInternalT<TChunkStructure>, KindPointerT<TChunkStructure>>;

    template<typename TChunkStructure, typename TChunkPointerElement>
    using KChunkArrayPointerT = DArrayPointerT<ChunkArrayExtensionT<TChunkStructure, TChunkPointerElement>,
        KChunkPointerT<TChunkStructure> >;


    // TODO Remove bucket part
    template<typename TChunkStructure>
    using KChunkT = DChunk<DOwn<DBucketPointer<KChunkPointerT<TChunkStructure>>>>;

    template<typename TChunkStructure>
    using KBucketT = DBucket<DOwn<DBucketPointer<KChunkPointerT<TChunkStructure>>>>;

    // TODO replace DBucket with DBunch
    template<typename TChunkStructure>
    using KBunchT = DBucket<DOwn<DGrow<DBucketPointer<KChunkPointerT<TChunkStructure>>>>>;



    template<typename TChunkStructure, typename TChunkPointerElement>
    using KChunkArrayT = DBucketBarrel<DOwn<DBarrelPointer<KChunkArrayPointerT<TChunkStructure, ChunkPointerT<TChunkStructure>>>>>;



    template<typename TChunkStructure>
    using KChunkTreePointerT = DChunkPointer<ChunkPointerInternalT<TChunkStructure>,
        DKindT<ChunkKind::ChunkTree,
        KTreePointerT<TChunkStructure>>>;

    template<typename TChunkStructure>
    // TODO replace DChunk with a DChunkTree decorator
    using KChunkTreeT = DChunk<DOwn<KChunkTreePointerT<TChunkStructure>>>;

    // TODO replace DBucket with a DBucketTree decorator
    template<typename TChunkStructure>
    using KBucketTreeT = DBucket<DOwn<DBucketPointer<KChunkTreePointerT<TChunkStructure>>>>;

    // TODO replace DBucket with a DBunchTree decorator
    template<typename TChunkStructure>
    using KBunchTreeT = DBucket<DOwn<DBucketPointer<KChunkTreePointerT<TChunkStructure>>>>;



    template<typename TChunkStructure, typename TChunkPointerElement>
    using KChunkArrayTreePointerT = DArrayPointerT<ChunkArrayExtensionT<TChunkStructure, TChunkPointerElement>,
        DKindT<ChunkKind::ChunkArrayTree,
        KChunkTreePointerT<TChunkStructure>>>;

    template<typename TChunkStructure, typename TChunkPointerElement>
    using KChunkArrayTreeT = DChunkArray<DOwn<DBarrelPointer<KChunkArrayTreePointerT<TChunkStructure, ChunkPointerT<TChunkStructure>>>>>;


    template<typename TChunkStructure, typename TChunkPointerElement>
    using KBucketBarrelTreeT = DBucketBarrel<DOwn<DBarrelPointer<KChunkArrayTreePointerT<TChunkStructure, ChunkPointerT<TChunkStructure>>>>>;


}

