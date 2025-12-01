// MIT License
// Copyright (c) 2025 Stephanie Rancourt

#pragma once
#include "common.h"
#include "Containers.h"
#include "KContainers.h"
#include "DKind.h"

//#include "routing\AlgorithmRouter.h"
//#include "routing\AlgorithmCacheRouter.h"
#include "Algorithm.h"
#include "AlgorithmRunnerChunk.h"
#include "AlgorithmRunnerChunkArray.h"
#include "AlgorithmRunnerKindPointerSwitch.h"

namespace PNC
{

    template<typename TAlgorithm, typename TContainer, bool THasChunkPointer, bool THasArrayPointer, bool THasKindPointer>
    struct AlgorithmRunnerSelector
    {

    };

    //template<typename TAlgorithm, typename TContainer, typename TChunk>// = KindPointerT<TChunkStructure>>
    //struct AlgorithmRunner2
    //    : public AlgorithmRunnerSelector < TAlgorithm, TContainer,
    //    /*THasChunkPointer:*/ContainerHasDecorator<TContainer, DChunkPointer, TChunk     >(),
    //    /*THasArrayPointer:*/ContainerHasDecorator<TContainer, DArrayPointer, TChunkArray>(),
    //    /*THasKindPointer: */false>
    //    //: public AlgorithmRunnerKindPointerSwitch<TChunkStructure, TAlgorithm>
    //{
    //};
    //ContainerHasDecorator<TContainer, KindPointerT>()
    //                                                                                Container                                                               Container                          Chunk , Array, Kind                  
    template<typename TA, typename TChunk, typename TBase> struct AlgorithmRunner<TA, DChunkPointer     <TChunk, TBase>> : public AlgorithmRunnerSelector<TA, DChunkPointer     <TChunk, TBase>, true  , false, ContainerHasDecorator<TBase, DKind>() > {};
    template<typename TA,                  typename TBase> struct AlgorithmRunner<TA, DChunk            <        TBase>> : public AlgorithmRunnerSelector<TA, DChunk            <        TBase>, true  , false, ContainerHasDecorator<TBase, DKind>() > {};
    template<typename TA,                  typename TBase> struct AlgorithmRunner<TA, DBucketPointer    <        TBase>> : public AlgorithmRunnerSelector<TA, DBucketPointer    <        TBase>, true  , false, ContainerHasDecorator<TBase, DKind>() > {};
    template<typename TA,                  typename TBase> struct AlgorithmRunner<TA, DBucket           <        TBase>> : public AlgorithmRunnerSelector<TA, DBucket           <        TBase>, true  , false, ContainerHasDecorator<TBase, DKind>() > {};
    template<typename TA,                  typename TBase> struct AlgorithmRunner<TA, DGrow             <        TBase>> : public AlgorithmRunnerSelector<TA, DGrow             <        TBase>, true  , false, ContainerHasDecorator<TBase, DKind>() > {};
    template<typename TA,                  typename TBase> struct AlgorithmRunner<TA, DBunch            <        TBase>> : public AlgorithmRunnerSelector<TA, DBunch            <        TBase>, true  , false, ContainerHasDecorator<TBase, DKind>() > {};
    template<typename TA, typename TAExt,  typename TBase> struct AlgorithmRunner<TA, DArrayPointerT    <TAExt,  TBase>> : public AlgorithmRunnerSelector<TA, DArrayPointerT    <TAExt,  TBase>, true  , true , ContainerHasDecorator<TBase, DKind>() > {};
    template<typename TA,                  typename TBase> struct AlgorithmRunner<TA, DChunkArray       <        TBase>> : public AlgorithmRunnerSelector<TA, DChunkArray       <        TBase>, true  , true , ContainerHasDecorator<TBase, DKind>() > {};
    template<typename TA,                  typename TBase> struct AlgorithmRunner<TA, DUniformChunkArray<        TBase>> : public AlgorithmRunnerSelector<TA, DUniformChunkArray<        TBase>, true  , true , ContainerHasDecorator<TBase, DKind>() > {};
    template<typename TA,                  typename TBase> struct AlgorithmRunner<TA, DKind      <        TBase>> : public AlgorithmRunnerSelector<TA, DKind      <        TBase>, true  , true , true > {};
    template<typename TA,                  typename TBase> struct AlgorithmRunner<TA, DTreePointer     <        TBase>> : public AlgorithmRunnerSelector<TA, DTreePointer     <        TBase>, true  , true , true > {};

    //                                                                                             Chunk, Array, Kind
    template<typename TA, typename TC                    > struct AlgorithmRunnerSelector< TA, TC, true , false, false> : AlgorithmRunnerChunk<TC> {};
    template<typename TA, typename TC, bool THC          > struct AlgorithmRunnerSelector< TA, TC, THC  , true , false> : AlgorithmRunnerChunkArray<TA, TC> {};
    template<typename TA, typename TC, bool THC, bool THA> struct AlgorithmRunnerSelector< TA, TC, THC  , THA  , true > : AlgorithmRunnerKindPointerSwitch<TA, TC> {};

    //
    //template<typename TAlgorithm, typename TChunk, typename TBase> struct AlgorithmRunner<TAlgorithm, DChunkPointer <TChunk, TBase>> : public AlgorithmRunnerChunk<DChunkPointer <TChunk, TBase>> {};
    //template<typename TAlgorithm,                  typename TBase> struct AlgorithmRunner<TAlgorithm, DChunk        <        TBase>> : public AlgorithmRunnerChunk<DChunk        <        TBase>> {};
    //template<typename TAlgorithm,                  typename TBase> struct AlgorithmRunner<TAlgorithm, DBucketPointer<        TBase>> : public AlgorithmRunnerChunk<DBucketPointer<        TBase>> {};
    //template<typename TAlgorithm,                  typename TBase> struct AlgorithmRunner<TAlgorithm, DBucket       <        TBase>> : public AlgorithmRunnerChunk<DBucket       <        TBase>> {};
    //template<typename TAlgorithm,                  typename TBase> struct AlgorithmRunner<TAlgorithm, DGrow         <        TBase>> : public AlgorithmRunnerChunk<DGrow         <        TBase>> {};
    //template<typename TAlgorithm,                  typename TBase> struct AlgorithmRunner<TAlgorithm, DBunch        <        TBase>> : public AlgorithmRunnerChunk<DBunch        <        TBase>> {};
    //
    //
    //
    //
    //template<typename TAlgorithm, typename TArrayExtension, typename TBase> struct AlgorithmRunner<TAlgorithm, DArrayPointerT    <TArrayExtension, TBase>> : public AlgorithmRunnerChunkArray<TAlgorithm, DArrayPointerT    <TArrayExtension, TBase>> { };
    //template<typename TAlgorithm,                           typename TBase> struct AlgorithmRunner<TAlgorithm, DChunkArray       <                 TBase>> : public AlgorithmRunnerChunkArray<TAlgorithm, DChunkArray       <                 TBase>> { };
    //template<typename TAlgorithm,                           typename TBase> struct AlgorithmRunner<TAlgorithm, DUniformChunkArray<                 TBase>> : public AlgorithmRunnerChunkArray<TAlgorithm, DUniformChunkArray<                 TBase>> { };
}
