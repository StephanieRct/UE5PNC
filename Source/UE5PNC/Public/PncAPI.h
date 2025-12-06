// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "common.h"
#include "Subsystems/EngineSubsystem.h"
#include "PNCDefault.h"
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
    Ni::Containers::ComponentTypeRegistry ComponentTypeRegistry;
    Ni::Containers::ChunkStructureRegistry ChunkStructureRegistry;
    Ni::Containers::ChunkTreeRegistry ChunkRegistry;

public:
    // Add a component type we can use for our centipede chunks
    template<typename T>
    const Ni::Containers::ComponentType* GetOrAddComponentType() { return ComponentTypeRegistry.GetOrAddComponentType<T>(); }

    const Ni::Containers::ChunkStructure* GetOrAddChunkStructure(const Ni::Containers::ComponentType* component)
    {
        return ChunkStructureRegistry.GetOrAddChunkStructure(component);
    }

    // Add a chunk structure from a list of component type
    const Ni::Containers::ChunkStructure* GetOrAddChunkStructure(const std::initializer_list<const Ni::Containers::ComponentType*>& aComponents)
    {
        return ChunkStructureRegistry.GetOrAddChunkStructure(aComponents);
    }

    // Add a chunk structure from a parameter pack of component type
    template<typename... TComponentTypes>
    const Ni::Containers::ChunkStructure* GetOrAddChunkStructure(TComponentTypes... args)
    {
        return ChunkStructureRegistry.GetOrAddChunkStructure({ args... });
    }

    template<typename... TComponentTypes>
    const Ni::Containers::ChunkStructure* GetOrAddChunkStructure()
    {
        return ChunkStructureRegistry.GetOrAddChunkStructure<TComponentTypes...>(ComponentTypeRegistry);
    }

    // Add a chunk with a given chunk structure and capacity.
    Ni::Containers::KChunkTree* NewChunk(const Ni::Containers::ChunkStructure* chunkStructure, const Ni::Containers::Size_t nodeCount)
    {
        return ChunkRegistry.NewChunk(chunkStructure, Ni::Containers::PropNodeCount(nodeCount));
    }

    // Add a chunk with a given chunk structure and capacity.
    Ni::Containers::KArrayTree* NewChunkArray(const Ni::Containers::ChunkStructure* const chunkStructure, const Ni::Containers::Size_t chunkCount, const Ni::Containers::Size_t nodeCountPerChunk)
    {
        return ChunkRegistry.NewChunkArray(chunkStructure, Ni::Containers::PropChunkCount(chunkCount), Ni::Containers::PropNodeCountPerChunk(nodeCountPerChunk));
    }

public:
    virtual void Initialize(FSubsystemCollectionBase& Collection) override;
};