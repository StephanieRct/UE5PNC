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
    /// <typeparam name="TChunkStructure"></typeparam>
    /// <typeparam name="TAlgorithm"></typeparam>
    /// <typeparam name="TKChunkPointer"></typeparam>
    template<typename TChunkStructure, typename TAlgorithm, typename TKChunkPointer = KindPointerT<TChunkStructure>>
    struct AlgorithmRunner : public AlgorithmRunnerKindPointerSwitch<TChunkStructure, TAlgorithm>
    {
    };
}
