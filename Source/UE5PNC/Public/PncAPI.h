// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "common.h"
#include "Subsystems/EngineSubsystem.h"
#include "Ni.h"
#include "Registry.h"
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
    Ni::ComponentTypeRegistry ComponentTypeRegistry;
    Ni::ChunkStructureRegistry ChunkStructureRegistry;
    Ni::ChunkTreeRegistry ChunkRegistry;

public:
    // Add a component type we can use for our centipede chunks
    template<typename T>
    const Ni::ComponentType* GetOrAddComponentType() { return ComponentTypeRegistry.GetOrAddComponentType<T>(); }

    const Ni::ChunkStructure* GetOrAddChunkStructure(const Ni::ComponentType* component)
    {
        return ChunkStructureRegistry.GetOrAddChunkStructure(component);
    }

    // Add a chunk structure from a list of component type
    const Ni::ChunkStructure* GetOrAddChunkStructure(const std::initializer_list<const Ni::ComponentType*>& aComponents)
    {
        return ChunkStructureRegistry.GetOrAddChunkStructure(aComponents);
    }

    // Add a chunk structure from a parameter pack of component type
    template<typename... TComponentTypes>
    const Ni::ChunkStructure* GetOrAddChunkStructure(TComponentTypes... args)
    {
        return ChunkStructureRegistry.GetOrAddChunkStructure({ args... });
    }

    template<typename... TComponentTypes>
    const Ni::ChunkStructure* GetOrAddChunkStructure()
    {
        return ChunkStructureRegistry.GetOrAddChunkStructure<TComponentTypes...>(ComponentTypeRegistry);
    }

    // Add a chunk with a given chunk structure and capacity.
    Ni::KChunkTree* NewChunk(const Ni::ChunkStructure* chunkStructure, const Ni::Size_t nodeCount)
    {
        return ChunkRegistry.NewChunk(chunkStructure, Ni::PropNodeCount(nodeCount));
    }

    // Add a chunk with a given chunk structure and capacity.
    Ni::KArrayTree* NewChunkArray(const Ni::ChunkStructure* const chunkStructure, const Ni::Size_t chunkCount, const Ni::Size_t nodeCountPerChunk)
    {
        return ChunkRegistry.NewChunkArray(chunkStructure, Ni::PropChunkCount(chunkCount), Ni::PropNodeCountPerChunk(nodeCountPerChunk));
    }

public:
    virtual void Initialize(FSubsystemCollectionBase& Collection) override;
};