// MIT License
// Copyright (c) 2025 Stephanie Rancourt

#pragma once
#include "common.h"
#include "AlgorithmRunnerKindPointerSwitch.h"

namespace PNC
{
    /// <summary>
    /// Add a template specialization if you need a special implementation for any specific type that inherit of KindPointer
    /// </summary>
    /// <typeparam name="TChunkType"></typeparam>
    /// <typeparam name="TAlgorithm"></typeparam>
    /// <typeparam name="TKChunkPointer"></typeparam>
    template<typename TChunkType, typename TAlgorithm, typename TKChunkPointer = KindPointerT<TChunkType>>
    struct AlgorithmRunner : public AlgorithmRunnerKindPointerSwitch<TChunkType, TAlgorithm>
    {
    };
}
