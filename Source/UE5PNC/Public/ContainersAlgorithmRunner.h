// MIT License
// Copyright (c) 2025 Stephanie Rancourt

#pragma once
#include "common.h"
#include "Containers.h"
#include "KContainers.h"
#include "DKind.h"
#include "Algorithm.h"
#include "AlgorithmRunnerChunk.h"
#include "AlgorithmRunnerChunkArray.h"
#include "AlgorithmRunnerKindPointerSwitch.h"

namespace NiT
{
    template<typename TAlgorithm, typename TContainer, bool THasChunkPointer, bool THasArrayPointer, bool THasKindPointer>
    struct AlgorithmRunnerSelector {};

    //                                                                                Container                                                               Container                  Chunk, Array, Kind                  
    template<typename TA, typename TChunk, typename TBase> struct AlgorithmRunner<TA, DChunkPointer< TChunk, TBase>> : public AlgorithmRunnerSelector<TA, DChunkPointer< TChunk, TBase>, true,  false, ContainerHasDecorator<TBase, DKind>() > {};
    template<typename TA,                  typename TBase> struct AlgorithmRunner<TA, DChunk<                TBase>> : public AlgorithmRunnerSelector<TA, DChunk<                TBase>, true,  false, ContainerHasDecorator<TBase, DKind>() > {};
    template<typename TA,                  typename TBase> struct AlgorithmRunner<TA, DBucketPointer<        TBase>> : public AlgorithmRunnerSelector<TA, DBucketPointer<        TBase>, true,  false, ContainerHasDecorator<TBase, DKind>() > {};
    template<typename TA,                  typename TBase> struct AlgorithmRunner<TA, DBucket<               TBase>> : public AlgorithmRunnerSelector<TA, DBucket<               TBase>, true,  false, ContainerHasDecorator<TBase, DKind>() > {};
    template<typename TA,                  typename TBase> struct AlgorithmRunner<TA, DBunchPointer<         TBase>> : public AlgorithmRunnerSelector<TA, DBunchPointer<         TBase>, true,  false, ContainerHasDecorator<TBase, DKind>() > {};
    template<typename TA,                  typename TBase> struct AlgorithmRunner<TA, DBunch<                TBase>> : public AlgorithmRunnerSelector<TA, DBunch<                TBase>, true,  false, ContainerHasDecorator<TBase, DKind>() > {};
    template<typename TA, typename TAExt,  typename TBase> struct AlgorithmRunner<TA, DArrayPointer< TAExt,  TBase>> : public AlgorithmRunnerSelector<TA, DArrayPointer< TAExt,  TBase>, true,  true,  ContainerHasDecorator<TBase, DKind>() > {};
    template<typename TA,                  typename TBase> struct AlgorithmRunner<TA, DArray<                TBase>> : public AlgorithmRunnerSelector<TA, DArray<                TBase>, true,  true,  ContainerHasDecorator<TBase, DKind>() > {};
    template<typename TA,                  typename TBase> struct AlgorithmRunner<TA, DUniformArray<         TBase>> : public AlgorithmRunnerSelector<TA, DUniformArray<         TBase>, true,  true,  ContainerHasDecorator<TBase, DKind>() > {};
    template<typename TA,                  typename TBase> struct AlgorithmRunner<TA, DKind<                 TBase>> : public AlgorithmRunnerSelector<TA, DKind<                 TBase>, true,  true,  true > {};
    template<typename TA,                  typename TBase> struct AlgorithmRunner<TA, DTreePointer<          TBase>> : public AlgorithmRunnerSelector<TA, DTreePointer<          TBase>, true,  true,  true > {};

    //                                                                                             Chunk, Array, Kind
    template<typename TA, typename TC                    > struct AlgorithmRunnerSelector< TA, TC, true , false, false> : AlgorithmRunnerChunk<TC> {};
    template<typename TA, typename TC, bool THC          > struct AlgorithmRunnerSelector< TA, TC, THC  , true , false> : AlgorithmRunnerChunkArray<TA, TC> {};
    template<typename TA, typename TC, bool THC, bool THA> struct AlgorithmRunnerSelector< TA, TC, THC  , THA  , true > : AlgorithmRunnerKindPointerSwitch<TA, TC> {};
}
