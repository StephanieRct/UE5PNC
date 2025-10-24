// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "common.h"
#include "Subsystems/EngineSubsystem.h"
#include "PNCDefault.h"
#include "PncAPI.generated.h"

/**
*
*/
UCLASS()
class UE5PNC_API UPncAPI : public UEngineSubsystem
{
    GENERATED_BODY()

public:
    static UPncAPI* Get()
    {
        if (!GEngine) return nullptr;
        return GEngine->GetEngineSubsystem<UPncAPI>();
    }

public:
    PNC::ComponentTypeRegistry ComponentTypeRegistry;
    PNC::ChunkStructureRegistry ChunkStructureRegistry;
    PNC::ChunkTreeRegistry ChunkRegistry;

public:
    // Add a component type we can use for our centipede chunks
    template<typename T>
    const PNC::ComponentType* GetOrAddComponentType() { return ComponentTypeRegistry.GetOrAddComponentType<T>(); }

    const PNC::ChunkStructure* GetOrAddChunkStructure(const PNC::ComponentType* component)
    {
        return ChunkStructureRegistry.GetOrAddChunkStructure(component);
    }

    // Add a chunk structure from a list of component type
    const PNC::ChunkStructure* GetOrAddChunkStructure(const std::initializer_list<const PNC::ComponentType*>& aComponents)
    {
        return ChunkStructureRegistry.GetOrAddChunkStructure(aComponents);
    }

    // Add a chunk structure from a parameter pack of component type
    template<typename... TComponentTypes>
    const PNC::ChunkStructure* GetOrAddChunkStructure(TComponentTypes... args)
    {
        return ChunkStructureRegistry.GetOrAddChunkStructure({ args... });
    }

    template<typename... TComponentTypes>
    const PNC::ChunkStructure* GetOrAddChunkStructure()
    {
        return ChunkStructureRegistry.GetOrAddChunkStructure<TComponentTypes...>(ComponentTypeRegistry);
    }

    // Add a chunk with a given chunk structure and capacity.
    PNC::KChunkTree* NewChunk(const PNC::ChunkStructure* chunkStructure, size_t nodeCapacity, size_t nodeCount = 0)
    {
        return ChunkRegistry.NewChunk(chunkStructure, nodeCapacity, nodeCount);
    }

    // Add a chunk with a given chunk structure and capacity.
    PNC::KChunkArrayTree* NewChunkArray(const PNC::ChunkStructure* chunkStructure, size_t nodeCapacityPerChunk, size_t chunkCapacity, size_t chunkCount = 0, size_t nodeCountPerChunk = 0)
    {
        return ChunkRegistry.NewChunkArray(chunkStructure, nodeCapacityPerChunk, chunkCapacity, chunkCount, nodeCountPerChunk);
    }

public:
    virtual void Initialize(FSubsystemCollectionBase& Collection) override;
};