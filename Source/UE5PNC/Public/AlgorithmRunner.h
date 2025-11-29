// MIT License
// Copyright (c) 2025 Stephanie Rancourt

#pragma once
#include "common.h"


namespace PNC
{
    /// <summary>
    /// Add a template specialization if you need a special implementation for any specific type that inherit of KindPointer
    /// </summary>
    /// <typeparam name="TChunkStructure"></typeparam>
    /// <typeparam name="TAlgorithm"></typeparam>
    /// <typeparam name="TKChunkPointer"></typeparam>
    template<typename TChunkStructure, typename TAlgorithm, typename TChunkPointer>// = KindPointerT<TChunkStructure>>
    struct AlgorithmRunner 
        //: public AlgorithmRunnerKindPointerSwitch<TChunkStructure, TAlgorithm>
    {
    };

}
// TODO bring this back in another file
//#include "KindPointer.h"
//#include "AlgorithmRunnerKindPointerSwitch.h"
//namespace PNC
//{
//    template<typename TChunkStructure, typename TAlgorithm>
//    struct AlgorithmRunner<TChunkStructure, TAlgorithm, KindPointerT<TChunkStructure>>
//        : public AlgorithmRunnerKindPointerSwitch<TChunkStructure, TAlgorithm>
//    {
//    };
//
//    template<typename TChunkStructure, typename TAlgorithm>
//    struct AlgorithmRunner< TChunkStructure, TAlgorithm, ChunkPointerT<TChunkStructure>>
//        : public AlgorithmRunnerChunk<TAlgorithm, ChunkPointerT<TChunkStructure>>
//    {
//    };
//
//
//    template<typename TChunkStructure, typename TAlgorithm, typename TChunkPointerElement>
//    struct AlgorithmRunner< TChunkStructure, TAlgorithm, ChunkArrayPointerT<TChunkStructure, TChunkPointerElement>>
//        : public AlgorithmRunnerChunkArray<TAlgorithm, ChunkArrayPointerT<TChunkStructure, TChunkPointerElement>>
//    {
//    };
//}