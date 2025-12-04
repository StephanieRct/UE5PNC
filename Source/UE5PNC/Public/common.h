// MIT License
// Copyright (c) 2025 Stephanie Rancourt

#pragma once
#include <type_traits>
#include <assert.h>
#include <stdexcept>
#include <typeinfo>
#include <memory>
#include <concepts>
#include <map>
#include <list>
#include <unordered_map>
#include <unordered_set>
#include <vector>
#include <algorithm> 
#include "CoreMinimal.h"

#ifdef WITH_EDITOR
#   define PNC_ASSERT_THROW
#   define PNC_MEMORYTRACKER
#endif

// TODO: turns some of these off by default
#ifdef UE_BUILD_DEBUG
// Zero out nodes' memory before construction on any component types
#   define PNC_MEMORY_NODE_CONSTRUCTZERO
// Zero out nodes' memory before construction on trivial component types
#   define PNC_MEMORY_NODE_CONSTRUCTZERO_TRIVIAL
// Zero out nodes' memory after destruction
#   define PNC_MEMORY_NODE_DESTRUCTZERO
// Clean up data fields on destruction (ex.: set pointers to null after freeing them)
#   define PNC_MEMORYCLEANUP
//#   define PNC_MEMORY_ALLOC_LOG
#   define PNC_DEBUG_NOINLINE __declspec(noinline)
#else
#   define PNC_DEBUG_NOINLINE
#endif

#define pnc_ensure ensure
#define pnc_ensuref ensureMsgf

#define PNC_STRINGIFY(x) #x
#define PNC_TO_STRING(x) PNC_STRINGIFY(x)

#define PNC_ASSERT_LOC(condition) "Condition: " #condition "\n" "Location: (" __FILE__ ":" PNC_TO_STRING(__LINE__) ")"

#ifdef PNC_ASSERT_THROW
#   define pnc_assert(isTrue) if(!(isTrue)) throw FString(PNC_ASSERT_LOC(isTrue))
//#   define pnc_assert(isTrue) if(!(isTrue)) throw FString(#isTrue ## " (" ## __FILE__ ## ":" ## __LINE__ ##")")
#   define pnc_assertf(isTrue, text, ...) if(!(isTrue)) throw FString::Printf(TEXT("Message: " text "\n" PNC_ASSERT_LOC(isTrue)), ##__VA_ARGS__)
#   define pnc_assert_slow pnc_assert
#   define pnc_assertf_slow pnc_assertf
#   define pnc_assert_no_entry_return(expression) throw FString(TEXT("No Entry"))
#else
#   define pnc_assert check
#   define pnc_assertf checkf
#   define pnc_assert_slow checkSlow
#   define pnc_assertf_slow checkfSlow
#   define pnc_assert_no_entry_return(expression) checkNoEntry(); return (expression);
#endif

#ifdef UE_BUILD_DEBUG //
#   define pnc_assert_switch_default_no_entry() default: pnc_assert_no_entry_return(); break
#else
#   define pnc_assert_switch_default_no_entry() 
#endif

#define pnc_todo UE_LOG(LogTemp, Error, TEXT("TODO"))

#include "Cleaner.h"
#include "MemoryTracker.h"
#include "Props.h"
