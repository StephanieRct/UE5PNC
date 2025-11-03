// Copyright Epic Games, Inc. All Rights Reserved.

#include "UE5PNC.h"
#include "UE5PNC/public/common.h"

#define LOCTEXT_NAMESPACE "FUE5PNCModule"

void FUE5PNCModule::StartupModule()
{
	// This code will execute after your module is loaded into memory; the exact timing is specified in the .uplugin file per-module
}

void FUE5PNCModule::ShutdownModule()
{
	// This function may be called during shutdown to clean up your module.  For modules that support dynamic reloading,
	// we call this function before unloading the module.
}

#undef LOCTEXT_NAMESPACE
	
IMPLEMENT_MODULE(FUE5PNCModule, UE5PNC)


namespace PNC
{

	MemoryTracker MemoryTracker::Instance;

	//std::atomic<int> MemoryTracker::AllocationCount = 0;

	//std::map<uint8*, std::size_t> MemoryTracker::Allocations;
}