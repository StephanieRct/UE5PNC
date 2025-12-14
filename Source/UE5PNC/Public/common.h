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
#   define NI_ASSERT_THROW
#   define NI_MEMORYTRACKER
#endif

#define NI_STRINGIFY(x) #x
#define NI_TO_STRING(x) NI_STRINGIFY(x)
// TODO: turns some of these off by default
#ifdef UE_BUILD_DEBUG
// Zero out nodes' memory before construction on any component types
#   define NI_MEMORY_NODE_CONSTRUCTZERO
// Zero out nodes' memory before construction on trivial component types
#   define NI_MEMORY_NODE_CONSTRUCTZERO_TRIVIAL
// Zero out nodes' memory after destruction
#   define NI_MEMORY_NODE_DESTRUCTZERO
// Clean up data fields on destruction (ex.: set pointers to null after freeing them)
#   define NI_MEMORYCLEANUP
//#   define PNC_MEMORY_ALLOC_LOG
#   define NI_DEBUG_NOINLINE __declspec(noinline)
#   define ni_todo(msg) _Pragma(NI_TO_STRING(message("TODO: " msg " (" __FILE__ ":" NI_TO_STRING(__LINE__) ")" )))
#else
#   define NI_DEBUG_NOINLINE
#   define ni_todo(msg)
#endif
#define NI_FORCEINLINE __forceinline 

#define NI_ASSERT_LOC(condition) "Condition: " #condition "\n" "Location: (" __FILE__ ":" NI_TO_STRING(__LINE__) ")"

#ifndef NI_OVERRIDE_ASSERTS
#   define ni_ensure ensure
#   define ni_ensuref ensureMsgf
#   ifdef NI_ASSERT_THROW
#      define ni_assert(isTrue) if(!(isTrue)) throw FString(NI_ASSERT_LOC(isTrue))
#      define ni_assertf(isTrue, text, ...) if(!(isTrue)) throw FString::Printf(TEXT("Message: " text "\n" NI_ASSERT_LOC(isTrue)), ##__VA_ARGS__)
#      define ni_assert_slow ni_assert
#      define ni_assertf_slow ni_assertf
#      define ni_assert_no_entry_return(expression) throw FString(TEXT("No Entry"))
#   else
#      define ni_assert check
#      define ni_assertf checkf
#      define ni_assert_slow checkSlow
#      define ni_assertf_slow checkfSlow
#      define ni_assert_no_entry_return(expression) checkNoEntry(); return (expression);
#   endif
#   ifdef UE_BUILD_DEBUG //
#      define ni_assert_switch_default_no_entry() default: ni_assert_no_entry_return(); break
#   else
#      define ni_assert_switch_default_no_entry() 
#   endif
#endif

#include "Cleaner.h"
#include "MemoryTracker.h"
#include "Props.h"
