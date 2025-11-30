// MIT License
// Copyright (c) 2025 Stephanie Rancourt

#pragma once
#include "common.h"
#include "Containers.h"

//#include "routing\AlgorithmRouter.h"
//#include "routing\AlgorithmCacheRouter.h"
#include "Algorithm.h"
#include "AlgorithmRunnerChunk.h"
#include "AlgorithmRunnerChunkArray.h"

namespace PNC
{
    template<typename TChunkStructure, typename TAlgorithm, typename TChunk, typename TBase> struct AlgorithmRunner< TChunkStructure, TAlgorithm, DChunkPointer <TChunk, TBase>> : public AlgorithmRunnerChunk<DChunkPointer <TChunk, TBase>> {};
    template<typename TChunkStructure, typename TAlgorithm,                  typename TBase> struct AlgorithmRunner< TChunkStructure, TAlgorithm, DChunk        <        TBase>> : public AlgorithmRunnerChunk<DChunk        <        TBase>> {};
    template<typename TChunkStructure, typename TAlgorithm,                  typename TBase> struct AlgorithmRunner< TChunkStructure, TAlgorithm, DBucketPointer<        TBase>> : public AlgorithmRunnerChunk<DBucketPointer<        TBase>> {};
    template<typename TChunkStructure, typename TAlgorithm,                  typename TBase> struct AlgorithmRunner< TChunkStructure, TAlgorithm, DBucket       <        TBase>> : public AlgorithmRunnerChunk<DBucket       <        TBase>> {};
    template<typename TChunkStructure, typename TAlgorithm,                  typename TBase> struct AlgorithmRunner< TChunkStructure, TAlgorithm, DGrow         <        TBase>> : public AlgorithmRunnerChunk<DGrow         <        TBase>> {};
    template<typename TChunkStructure, typename TAlgorithm,                  typename TBase> struct AlgorithmRunner< TChunkStructure, TAlgorithm, DBunch        <        TBase>> : public AlgorithmRunnerChunk<DBunch        <        TBase>> {};

    


    template<typename TChunkStructure, typename TAlgorithm, typename TArrayExtension, typename TBase> struct AlgorithmRunner< TChunkStructure, TAlgorithm, DArrayPointerT    <TArrayExtension, TBase>> : public AlgorithmRunnerChunkArray<TAlgorithm, DArrayPointerT    <TArrayExtension, TBase>> { };
    template<typename TChunkStructure, typename TAlgorithm,                           typename TBase> struct AlgorithmRunner< TChunkStructure, TAlgorithm, DChunkArray       <                 TBase>> : public AlgorithmRunnerChunkArray<TAlgorithm, DChunkArray       <                 TBase>> { };
    template<typename TChunkStructure, typename TAlgorithm,                           typename TBase> struct AlgorithmRunner< TChunkStructure, TAlgorithm, DUniformChunkArray<                 TBase>> : public AlgorithmRunnerChunkArray<TAlgorithm, DUniformChunkArray<                 TBase>> { };
}
