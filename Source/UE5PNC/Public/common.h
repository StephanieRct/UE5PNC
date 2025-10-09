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

#if UE_BUILD_DEVELOPMENT
#   define assert_pnc check
#elif UE_BUILD_SHIPPING
#   define assert_pnc assert
#endif


#define pnc_alloc(size, align) FMemory::Malloc(size, align)
#define pnc_free(ptr, size, align) FMemory::Free(ptr)