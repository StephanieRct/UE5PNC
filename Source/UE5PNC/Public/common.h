// MIT License
// Copyright (c) 2025 Stephanie Rancourt

#pragma once
#include <assert.h>
#include <stdexcept>
#include <typeinfo>
#include <concepts>
#include <map>
#include <unordered_map>
#include <vector>
#include "CoreMinimal.h"

#if UE_BUILD_DEVELOPMENT
#   define assert_pnc check
#elif UE_BUILD_SHIPPING
#   define assert_pnc assert
#endif
