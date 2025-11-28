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

#ifdef UE_BUILD_DEBUG //default: pnc_assert_no_entry_return(); break
#   define pnc_assert_switch_default_no_entry() 
#else
#   define pnc_assert_switch_default_no_entry() 
#endif

#define pnc_todo UE_LOG(LogTemp, Error, TEXT("TODO"))

#ifdef PNC_MEMORYCLEANUP
#   define pnc_clean(x) ::PNC::Cleaner<decltype(x)>::Clean(x)

namespace PNC
{
    template<typename T, bool isEnum = std::is_enum_v<T>>
    struct Cleaner
    {
        struct MissingCleanerForType
        {

        };
        static void Clean(MissingCleanerForType a)
        {
        }
    };

    template<typename T>
    struct Cleaner<T&, false>
    {
        static T Clean(T& ptr)
        {
            return Cleaner<T>::Clean(ptr);
        }
    };

    template<typename T>
    struct Cleaner<T*, false>
    {
    public:
        static T* Clean(T*& ptr)
        {
            T* ptr2 = ptr;
            ptr = nullptr;
            return ptr2;
        }
    };

    template<>
    struct Cleaner<int, false>
    {
    public:
        static int Clean(int& a)
        {
            int a2 = a;
            a = 0;
            return a2;
        }
    };

    template<typename T>
    struct Cleaner<T, true>
    {
    public:
        static T Clean(T& a)
        {
            T a2 = a;
            a = (T)0;
            return a2;
        }
    };

}
#else
#   define pnc_clean(x) x
#endif

#ifdef PNC_MEMORYTRACKER
#   define pnc_alloc(size, align) ::PNC::MemoryTracker::Allocate(size, align)
#   define pnc_free_clean(ptr, size, align) ::PNC::MemoryTracker::Deallocate(pnc_clean(ptr), size, align)
#   define pnc_free_dirty(ptr, size, align) ::PNC::MemoryTracker::Deallocate(ptr, size, align)
#   define pnc_new(type) new (::PNC::MemoryTracker::Allocate<type>()) type
#   define pnc_delete_clean(ptr) ::PNC::MemoryTracker::Delete(pnc_clean(ptr))
#   define pnc_delete_dirty(ptr) ::PNC::MemoryTracker::Delete(ptr)
#   define pnc_assert_owns(ptr, count) pnc_assert(::PNC::MemoryTracker::Owns(ptr, count))
#   define pnc_owns(ptr, count) ::PNC::MemoryTracker::Owns(ptr, count)
#   define pnc_allocation_count ((int)PNC::MemoryTracker::Instance.AllocationCount)

namespace PNC
{
    UE5PNC_API struct MemoryTracker
    {
    public:
        UE5PNC_API static MemoryTracker Instance;
        struct Alloc
        {
            uint8* Base;
            std::size_t Size;
            bool Includes(uint8*const ptr, const std::size_t size)const
            {
                return Base <= ptr && (Base + Size) >= (ptr + size);
            }
        };
        std::map<uint8*, Alloc> Allocations;
        int AllocationCount;
        
        __declspec(noinline) static void* Allocate(std::size_t const size, std::size_t const alignment)
        {
            MemoryTracker& instance = Instance;
            void* ptr = FMemory::Malloc(size, alignment);
            if (ptr)
            {
                ++instance.AllocationCount;
                instance.Allocations.insert({ (uint8*)ptr + size - 1, {(uint8*)ptr, size} });
                UE_LOG(LogTemp, Log, TEXT("Alloc %016x (new count %d)"), ptr, instance.AllocationCount);
            }
            return ptr;
        }
        __declspec(noinline) static void Deallocate(void* const ptr, std::size_t const size, std::size_t const alignment)
        {
            MemoryTracker& instance = Instance;
            if (ptr != nullptr)
            {
                //pnc_assert(AllocationCount > 0);
                --Instance.AllocationCount;

                UE_LOG(LogTemp, Log, TEXT("Free  %016x (new count %d)"), ptr, instance.AllocationCount);

                auto it = instance.Allocations.lower_bound((uint8*)ptr);
                pnc_assert(it != instance.Allocations.end());
                pnc_assert(it->second.Includes((uint8*)ptr, size));
                Instance.Allocations.erase(it);
            }
            FMemory::Free(ptr);
        }


        template<typename T>
        static void* Allocate()
        {
            return Allocate(sizeof(T), alignof(T));
        }
        template<typename T>
        static void Deallocate(T* const ptr)
        {
            Deallocate(ptr, sizeof(T), alignof(T));
        }



        template<typename T>
        __declspec(noinline) static void Delete(T* const ptr)
        {
            ptr->~T();
            Deallocate(ptr);
        }

        static bool Owns(const void*const ptr, const std::size_t size=1)
        {
            MemoryTracker& instance = Instance;
            auto nearest = instance.Allocations.lower_bound((uint8*)ptr);
            if (nearest == instance.Allocations.end())
                return false;
            return nearest->second.Includes((uint8*)ptr, size);
        }
        template<typename T>
        static bool OwnsT(const T*const ptr, const std::size_t count)
        {
            return Owns((void*)ptr, sizeof(T) * count);
        }
    };

    template<typename T = void>
    struct PncAllocator : public std::pointer_traits<T>
    {
    public:
        using value_type = T;
        using size_type = std::size_t;
        using pointer = T*;
        using const_pointer = const T*;
        using difference_type = typename std::pointer_traits<pointer>::difference_type;

        PncAllocator() = default;

        template<typename U>
        PncAllocator(const PncAllocator<U>&) {};

        T* allocate(std::size_t const count)
        {
            return (T*)pnc_alloc(count * sizeof(T), alignof(T));
        }

        void deallocate(T* ptr, std::size_t const count)
        {
            pnc_free_dirty(ptr, count * sizeof(T), alignof(T));
        }

        template<typename U>
        struct rebind
        {
            typedef PncAllocator<U> other;
        };
    };

    template<typename T, typename U>
    constexpr bool operator==(const PncAllocator<T>&, const PncAllocator<U>&) { return true; }

    template<typename T, typename U>
    constexpr bool operator!=(const PncAllocator<T>&, const PncAllocator<U>&) { return false; }

    template<typename T, typename THaser = std::hash<T>, typename TEqualer = std::equal_to<T>>
    using HashSet = std::unordered_set<T, THaser, TEqualer, PncAllocator<T>>;

    template<typename TKey, typename TValue, typename THaser = std::hash<TKey>, typename TEqualer = std::equal_to<TKey>>
    using HashMap = std::unordered_map<TKey, TValue, THaser, TEqualer, PncAllocator<std::pair<const TKey,TValue>>>;

    template<typename T>
    using Vector = std::vector<T, PncAllocator<T>>;

    template<typename T>
    using List = std::list<T, PncAllocator<T>>;

}
#else
#   define pnc_alloc(size, align) FMemory::Malloc(size, align)
#   define pnc_free_dirty(ptr, size, align) FMemory::Free(ptr)
#   define pnc_free_clean(ptr, size, align) FMemory::Free(pnc_clean(ptr))
#   define pnc_new(type) new type
#   define pnc_delete_dirty(ptr) delete ptr
#   define pnc_delete_clean(ptr) delete pnc_clean(ptr)
#   define pnc_assert_owns(ptr, count) 
#   define pnc_owns(ptr, count) true
#   define pnc_allocation_count ((std::size_t)0)

template<int tI = 0>
struct MemoryTracker
{
public:
    enum { AllocationCount = 0 };
};
namespace PNC
{
    template<typename T>
    using HashSet = std::unordered_set<T>;
}
#endif


namespace PNC
{
    template<typename T>
    struct Deleter
    {
        void operator()(T* ptr)const
        {
            pnc_delete_dirty(ptr);
        }
    };

    template<typename T>
    using Unique_Ptr = std::unique_ptr<T, Deleter<T>>;

    template<typename T, class... TArgumentTypes>
    Unique_Ptr<T> Make_Unique(TArgumentTypes&&... args)
    {
        return Unique_Ptr<T>(pnc_new(T)(std::forward<TArgumentTypes>(args)...));
    }

    template<typename TArg>
    struct ArgTraits
    {

    };

    template<template<typename TBase2> typename TDecorator>
    struct ArgTraitsDefault
    {
        template<typename TBase>
        static TDecorator<TBase> Decorate(typename TDecorator<TBase>::Value_t value, const TBase& baseCopy)
        {
            return TDecorator<TBase>(value, baseCopy);
        }
    };

    enum class PropId 
    {
        NodeCount,
        NodeCapacity,
        ChunkCount,
        ChunkCapacity,
        NodeCountPerChunk,
        NodeCapacityPerChunk,
        ArrayNodeCount,
        ArrayNodeCapacity,
        // 
        // ComponentTypeIndexInChunk (rename to LocalComponentTypeIndex)
        // LocalNodeIndex
    };
    template<typename TValue, PropId TId >
    struct Prop
    {
        using Base_t = void;
        using Self_t = Prop<TValue, TId>;

        TValue Value;
        Prop(const TValue value)
            :Value(value) 
        {
        }
        operator TValue()const
        {
            return Value;
        }

        Self_t& operator++() { ++Value; return *this; }
        Self_t& operator--() { --Value; return *this; }
        Self_t operator++(int) { return Self_t(Value++); }
        Self_t operator--(int) { return Self_t(Value--); }
        Self_t operator~() { return Self_t(~Value); }
        Self_t operator!() { return Self_t(!Value); }
    };

    template<typename TValue, PropId TId> std::strong_ordering operator<=>(const Prop<TValue, TId>& a, const TValue& b) { return a.Value <=> b; }
    template<typename TValue, PropId TId> Prop<TValue, TId> operator+(const Prop<TValue, TId>& a, const TValue& b) { return a.Value + b; }
    template<typename TValue, PropId TId> Prop<TValue, TId> operator-(const Prop<TValue, TId>& a, const TValue& b) { return a.Value - b; }
    template<typename TValue, PropId TId> Prop<TValue, TId> operator*(const Prop<TValue, TId>& a, const TValue& b) { return a.Value * b; }
    template<typename TValue, PropId TId> Prop<TValue, TId> operator/(const Prop<TValue, TId>& a, const TValue& b) { return a.Value / b; }
    template<typename TValue, PropId TId> Prop<TValue, TId> operator%(const Prop<TValue, TId>& a, const TValue& b) { return a.Value % b; }
    template<typename TValue, PropId TId> Prop<TValue, TId> operator&(const Prop<TValue, TId>& a, const TValue& b) { return a.Value & b; }
    template<typename TValue, PropId TId> Prop<TValue, TId> operator|(const Prop<TValue, TId>& a, const TValue& b) { return a.Value | b; }
    template<typename TValue, PropId TId> Prop<TValue, TId> operator^(const Prop<TValue, TId>& a, const TValue& b) { return a.Value ^ b; }
    template<typename TValue, PropId TId> Prop<TValue, TId> operator<<(const Prop<TValue, TId>& a, const TValue& b) { return a.Value << b; }
    template<typename TValue, PropId TId> Prop<TValue, TId> operator>>(const Prop<TValue, TId>& a, const TValue& b) { return a.Value >> b; }
    template<typename TValue, PropId TId> Prop<TValue, TId> operator&&(const Prop<TValue, TId>& a, const TValue& b) { return a.Value && b; }
    template<typename TValue, PropId TId> Prop<TValue, TId> operator||(const Prop<TValue, TId>& a, const TValue& b) { return a.Value || b; }
    template<typename TValue, PropId TId> Prop<TValue, TId>& operator+=(Prop<TValue, TId>& a, const TValue& b) { a.Value += b; return a; }
    template<typename TValue, PropId TId> Prop<TValue, TId>& operator-=(Prop<TValue, TId>& a, const TValue& b) { a.Value -= b; return a; }
    template<typename TValue, PropId TId> Prop<TValue, TId>& operator*=(Prop<TValue, TId>& a, const TValue& b) { a.Value *= b; return a; }
    template<typename TValue, PropId TId> Prop<TValue, TId>& operator/=(Prop<TValue, TId>& a, const TValue& b) { a.Value /= b; return a; }
    template<typename TValue, PropId TId> Prop<TValue, TId>& operator%=(Prop<TValue, TId>& a, const TValue& b) { a.Value %= b; return a; }
    template<typename TValue, PropId TId> Prop<TValue, TId>& operator&=(Prop<TValue, TId>& a, const TValue& b) { a.Value &= b; return a; }
    template<typename TValue, PropId TId> Prop<TValue, TId>& operator|=(Prop<TValue, TId>& a, const TValue& b) { a.Value |= b; return a; }
    template<typename TValue, PropId TId> Prop<TValue, TId>& operator^=(Prop<TValue, TId>& a, const TValue& b) { a.Value ^= b; return a; }
    template<typename TValue, PropId TId> Prop<TValue, TId>& operator<<=(Prop<TValue, TId>& a, const TValue& b) { a.Value <<= b; return a; }
    template<typename TValue, PropId TId> Prop<TValue, TId>& operator>>=(Prop<TValue, TId>& a, const TValue& b) { a.Value >>= b; return a; }


    template<typename TValue, PropId TId> std::strong_ordering operator<=>(const TValue& a, const Prop<TValue, TId>& b) { return a <=> b.Value; }
    template<typename TValue, PropId TId> Prop<TValue, TId> operator+(const TValue& a, const Prop<TValue, TId>& b) { return a + b.Value ; }
    template<typename TValue, PropId TId> Prop<TValue, TId> operator-(const TValue& a, const Prop<TValue, TId>& b) { return a - b.Value ; }
    template<typename TValue, PropId TId> Prop<TValue, TId> operator*(const TValue& a, const Prop<TValue, TId>& b) { return a * b.Value ; }
    template<typename TValue, PropId TId> Prop<TValue, TId> operator/(const TValue& a, const Prop<TValue, TId>& b) { return a / b.Value ; }
    template<typename TValue, PropId TId> Prop<TValue, TId> operator%(const TValue& a, const Prop<TValue, TId>& b) { return a % b.Value ; }
    template<typename TValue, PropId TId> Prop<TValue, TId> operator&(const TValue& a, const Prop<TValue, TId>& b) { return a & b.Value ; }
    template<typename TValue, PropId TId> Prop<TValue, TId> operator|(const TValue& a, const Prop<TValue, TId>& b) { return a | b.Value ; }
    template<typename TValue, PropId TId> Prop<TValue, TId> operator^(const TValue& a, const Prop<TValue, TId>& b) { return a ^ b.Value ; }
    template<typename TValue, PropId TId> Prop<TValue, TId> operator<<(const TValue& a, const Prop<TValue, TId>& b) { return a << b.Value ; }
    template<typename TValue, PropId TId> Prop<TValue, TId> operator>>(const TValue& a, const Prop<TValue, TId>& b) { return a >> b.Value ; }
    template<typename TValue, PropId TId> Prop<TValue, TId> operator&&(const TValue& a, const Prop<TValue, TId>& b) { return a && b.Value ; }
    template<typename TValue, PropId TId> Prop<TValue, TId> operator||(const TValue& a, const Prop<TValue, TId>& b) { return a || b.Value ; }
    template<typename TValue, PropId TId> TValue& operator+=(TValue& a, const Prop<TValue, TId>& b) { a += b.Value; return a; }
    template<typename TValue, PropId TId> TValue& operator-=(TValue& a, const Prop<TValue, TId>& b) { a -= b.Value; return a; }
    template<typename TValue, PropId TId> TValue& operator*=(TValue& a, const Prop<TValue, TId>& b) { a *= b.Value; return a; }
    template<typename TValue, PropId TId> TValue& operator/=(TValue& a, const Prop<TValue, TId>& b) { a /= b.Value; return a; }
    template<typename TValue, PropId TId> TValue& operator%=(TValue& a, const Prop<TValue, TId>& b) { a %= b.Value; return a; }
    template<typename TValue, PropId TId> TValue& operator&=(TValue& a, const Prop<TValue, TId>& b) { a &= b.Value; return a; }
    template<typename TValue, PropId TId> TValue& operator|=(TValue& a, const Prop<TValue, TId>& b) { a |= b.Value; return a; }
    template<typename TValue, PropId TId> TValue& operator^=(TValue& a, const Prop<TValue, TId>& b) { a ^= b.Value; return a; }
    template<typename TValue, PropId TId> TValue& operator<<=(TValue& a, const Prop<TValue, TId>& b) { a <<= b.Value; return a; }
    template<typename TValue, PropId TId> TValue& operator>>=(TValue& a, const Prop<TValue, TId>& b) { a >>= b.Value; return a; }

    template<typename TValue, PropId TId> std::strong_ordering operator<=>(const Prop<TValue, TId>& a, const Prop<TValue, TId>& b) { return a.Value <=> b.Value; }
    template<typename TValue, PropId TId> Prop<TValue, TId> operator+(const Prop<TValue, TId>& a, const Prop<TValue, TId>& b) { return a.Value + b.Value; }
    template<typename TValue, PropId TId> Prop<TValue, TId> operator-(const Prop<TValue, TId>& a, const Prop<TValue, TId>& b) { return a.Value - b.Value; }
    template<typename TValue, PropId TId> Prop<TValue, TId> operator*(const Prop<TValue, TId>& a, const Prop<TValue, TId>& b) { return a.Value * b.Value; }
    template<typename TValue, PropId TId> Prop<TValue, TId> operator/(const Prop<TValue, TId>& a, const Prop<TValue, TId>& b) { return a.Value / b.Value; }
    template<typename TValue, PropId TId> Prop<TValue, TId> operator%(const Prop<TValue, TId>& a, const Prop<TValue, TId>& b) { return a.Value % b.Value; }
    template<typename TValue, PropId TId> Prop<TValue, TId> operator&(const Prop<TValue, TId>& a, const Prop<TValue, TId>& b) { return a.Value & b.Value; }
    template<typename TValue, PropId TId> Prop<TValue, TId> operator|(const Prop<TValue, TId>& a, const Prop<TValue, TId>& b) { return a.Value | b.Value; }
    template<typename TValue, PropId TId> Prop<TValue, TId> operator^(const Prop<TValue, TId>& a, const Prop<TValue, TId>& b) { return a.Value ^ b.Value; }
    template<typename TValue, PropId TId> Prop<TValue, TId> operator<<(const Prop<TValue, TId>& a, const Prop<TValue, TId>& b) { return a.Value << b.Value; }
    template<typename TValue, PropId TId> Prop<TValue, TId> operator>>(const Prop<TValue, TId>& a, const Prop<TValue, TId>& b) { return a.Value >> b.Value; }
    template<typename TValue, PropId TId> Prop<TValue, TId> operator&&(const Prop<TValue, TId>& a, const Prop<TValue, TId>& b) { return a.Value && b.Value; }
    template<typename TValue, PropId TId> Prop<TValue, TId> operator||(const Prop<TValue, TId>& a, const Prop<TValue, TId>& b) { return a.Value || b.Value; }
    template<typename TValue, PropId TId> Prop<TValue, TId>& operator+=(Prop<TValue, TId>& a, const Prop<TValue, TId>& b) { a.Value += b.Value; return a; }
    template<typename TValue, PropId TId> Prop<TValue, TId>& operator-=(Prop<TValue, TId>& a, const Prop<TValue, TId>& b) { a.Value -= b.Value; return a; }
    template<typename TValue, PropId TId> Prop<TValue, TId>& operator*=(Prop<TValue, TId>& a, const Prop<TValue, TId>& b) { a.Value *= b.Value; return a; }
    template<typename TValue, PropId TId> Prop<TValue, TId>& operator/=(Prop<TValue, TId>& a, const Prop<TValue, TId>& b) { a.Value /= b.Value; return a; }
    template<typename TValue, PropId TId> Prop<TValue, TId>& operator%=(Prop<TValue, TId>& a, const Prop<TValue, TId>& b) { a.Value %= b.Value; return a; }
    template<typename TValue, PropId TId> Prop<TValue, TId>& operator&=(Prop<TValue, TId>& a, const Prop<TValue, TId>& b) { a.Value &= b.Value; return a; }
    template<typename TValue, PropId TId> Prop<TValue, TId>& operator|=(Prop<TValue, TId>& a, const Prop<TValue, TId>& b) { a.Value |= b.Value; return a; }
    template<typename TValue, PropId TId> Prop<TValue, TId>& operator^=(Prop<TValue, TId>& a, const Prop<TValue, TId>& b) { a.Value ^= b.Value; return a; }
    template<typename TValue, PropId TId> Prop<TValue, TId>& operator<<=(Prop<TValue, TId>& a, const Prop<TValue, TId>& b) { a.Value <<= b.Value; return a; }
    template<typename TValue, PropId TId> Prop<TValue, TId>& operator>>=(Prop<TValue, TId>& a, const Prop<TValue, TId>& b) { a.Value >>= b.Value; return a; }

    template<typename TValue, PropId TId>
    struct Cleaner<Prop<TValue, TId>, false>
    {
    public:
        template<typename TProp>
        static TProp Clean(TProp& a)
        {
            TProp a2 = a;
            a.Value = 0;
            return a2;
        }
    };

    template<typename TSize>
    using NodeCountT = Prop<TSize, PropId::NodeCount>;
    template<typename TSize>
    using NodeCapacityT = Prop<TSize, PropId::NodeCapacity>;
    template<typename TSize>
    using ChunkCountT = Prop<TSize, PropId::ChunkCount>;
    template<typename TSize>
    using ChunkCapacityT = Prop<TSize, PropId::ChunkCapacity>;
    template<typename TSize>
    using NodeCountPerChunkT = Prop<TSize, PropId::NodeCountPerChunk>;
    template<typename TSize>
    using NodeCapacityPerChunkT = Prop<TSize, PropId::NodeCapacityPerChunk>;
    template<typename TSize>
    using ArrayNodeCountT = Prop<TSize, PropId::ArrayNodeCount>;
    template<typename TSize>
    using ArrayNodeCapacityT = Prop<TSize, PropId::ArrayNodeCapacity>;

    template<typename TSize>
    NodeCapacityT<TSize> PropCountToCapacity(const NodeCountT<TSize> nodeCount)
    {
        return NodeCapacityT<TSize>(nodeCount.Value);
    }
    template<typename TSize>
    ChunkCapacityT<TSize> PropCountToCapacity(const ChunkCountT<TSize> chunkCount)
    {
        return ChunkCapacityT<TSize>(chunkCount.Value);
    }
    template<typename TSize>
    NodeCapacityPerChunkT<TSize> PropCountToCapacity(const NodeCountPerChunkT<TSize> nodeCountPerChunk)
    {
        return NodeCapacityPerChunkT<TSize>(nodeCountPerChunk.Value);
    }
    template<typename TSize>
    ArrayNodeCapacityT<TSize> PropCountToCapacity(const ArrayNodeCountT<TSize> arrayNodeCount)
    {
        return ArrayNodeCapacityT<TSize>(arrayNodeCount.Value);
    }


    template<typename TSize>
    NodeCapacityT<TSize> PropArrayToChunk(const ArrayNodeCapacityT<TSize> arrayNodeCapacity)
    {
        return NodeCapacityT<TSize>(arrayNodeCapacity.Value);
    }
    template<typename TSize>
    ArrayNodeCapacityT<TSize> PropChunkToArray(const NodeCapacityT<TSize> nodeCapacity)
    {
        return ArrayNodeCapacityT<TSize>(nodeCapacity.Value);
    }

    template<typename TSize>
    NodeCountT<TSize> PropArrayToChunk(const ArrayNodeCountT<TSize> arrayNodeCount)
    {
        return NodeCountT<TSize>(arrayNodeCount.Value);
    }
    template<typename TSize>
    ArrayNodeCountT<TSize> PropChunkToArray(const NodeCountT<TSize> nodeCount)
    {
        return ArrayNodeCountT<TSize>(nodeCount.Value);
    }



    template<typename TSize>
    NodeCountT<TSize> operator-(const NodeCapacityT<TSize>& nodeCapacity, const NodeCountT<TSize>& nodeCount)
    {
        return NodeCountT<TSize>(nodeCapacity.Value * nodeCount.Value);
    }


    template<typename TSize>
    ChunkCountT<TSize> operator-(const ChunkCapacityT<TSize>& chunkCapacity, const ChunkCountT<TSize>& chunkCount)
    {
        return ChunkCountT<TSize>(chunkCapacity.Value * chunkCount.Value);
    }


    template<typename TSize>
    ArrayNodeCountT<TSize> operator*(const ChunkCountT<TSize>& chunkCount, const NodeCountPerChunkT<TSize>& nodeCountPerChunk)
    {
        return ArrayNodeCountT<TSize>(chunkCount.Value * nodeCountPerChunk.Value);
    }
    template<typename TSize>
    ArrayNodeCountT<TSize> operator*(const NodeCountPerChunkT<TSize>& nodeCountPerChunk, const ChunkCountT<TSize>& chunkCount)
    {
        return ArrayNodeCountT<TSize>(nodeCountPerChunk.Value * chunkCount.Value);
    }

    template<typename TSize>
    ArrayNodeCapacityT<TSize> operator*(const ChunkCapacityT<TSize>& chunkCapacity, const NodeCapacityPerChunkT<TSize>& nodeCapacityPerChunk)
    {
        return ArrayNodeCapacityT<TSize>(chunkCapacity.Value * nodeCapacityPerChunk.Value);
    }
    template<typename TSize>
    ArrayNodeCapacityT<TSize> operator*(const NodeCapacityPerChunkT<TSize>& nodeCapacityPerChunk, const ChunkCapacityT<TSize>& chunkCapacity)
    {
        return ArrayNodeCapacityT<TSize>(nodeCapacityPerChunk.Value * chunkCapacity.Value);
    }


    template <typename TArg, typename... TArgs>
    constexpr TArg ArgOfType(TArgs&&... args) {
        return std::get<TArg>(std::forward_as_tuple(std::forward<TArgs>(args)...));
    }


    struct DArgsTag {};

    template<typename TSize>
    struct DArgs
    {
        using Size_t = TSize;
    };



    template<typename TChunkStructure>
    struct StructurePtr
    {
        using ChunkStructure_t = TChunkStructure;
        const ChunkStructure_t* const StructurePtr;
        const ChunkStructure_t* GetStructurePtr() const { return StructurePtr; }

        StructurePtr(const ChunkStructure_t*const structurePtr)
            : StructurePtr(structurePtr)
        {
        }
        operator const ChunkStructure_t* ()const { return StructurePtr; }

        const ChunkStructure_t* operator->()const { return StructurePtr; }
        const ChunkStructure_t& operator*()const { return *StructurePtr; }
    };
    
    template<typename TChunkStructure, typename TBase>
    struct DStructurePtr : public TBase
    {
        using typename TBase::Size_t;
        using Value_t = const TChunkStructure*;
        using ChunkStructure_t = TChunkStructure;
        const ChunkStructure_t* const ChunkStructure;
        const ChunkStructure_t* GetStructurePtr() const { return ChunkStructure; }

        DStructurePtr(const StructurePtr<ChunkStructure_t>& chunkStructure, const TBase& baseCopy)
            : TBase(baseCopy)
            , ChunkStructure(chunkStructure)
        {
        }
    };

    template<typename TChunkStructure>
    struct ArgTraits<StructurePtr<TChunkStructure>>
    {
        template<typename TBase>
        static DStructurePtr<TChunkStructure, TBase> Decorate(const StructurePtr<TChunkStructure> value, const TBase& baseCopy)
        {
            return DStructurePtr<TChunkStructure, TBase>(value, baseCopy);
        }
    };

    template<typename TBase>
    struct DComponentDataArray : public TBase
    {
        using typename TBase::Size_t;
        using Value_t = void**;
        void**const ComponentDataArray;
        void** GetComponentDataArray() const { return ComponentDataArray; }

        DComponentDataArray(void** const componentDataArray, const TBase& baseCopy)
            : TBase(baseCopy)
            , ComponentDataArray(componentDataArray)
        {
        }
    };
    template<>
    struct ArgTraits<void**> : public ArgTraitsDefault<DComponentDataArray>
    {
    };

    template<typename TBase>
    struct DNodeCount : public TBase
    {
        using typename TBase::Size_t;
        using Value_t = NodeCountT<Size_t>;
        const NodeCountT<Size_t> NodeCount;
        NodeCountT<Size_t> GetNodeCount() const { return NodeCount; }

        DNodeCount(const NodeCountT<Size_t>& nodeCount, const TBase& baseCopy)
            : TBase(baseCopy)
            , NodeCount(nodeCount)
        {
        }
    };
    template<typename TSize>
    struct ArgTraits<NodeCountT<TSize>> : public ArgTraitsDefault<DNodeCount>
    {
    };
    //
    template<typename TBase>
    struct DNodeCapacity : public TBase
    {
        using typename TBase::Size_t;
        using Value_t = NodeCapacityT<Size_t>;
        NodeCapacityT<Size_t> NodeCapacity;
        NodeCapacityT<Size_t> GetNodeCapacity() const { return NodeCapacity; }
        DNodeCapacity(const NodeCapacityT<Size_t> nodeCapacity, const TBase& baseCopy)
            : TBase(baseCopy)
            , NodeCapacity(nodeCapacity)
        {
        }
    };
    template<typename TSize>
    struct ArgTraits<NodeCapacityT<TSize>> : public ArgTraitsDefault<DNodeCapacity>
    {
    };

    template<typename TBase>
    struct DChunkCount : public TBase
    {
        using typename TBase::Size_t;
        using Value_t = ChunkCountT<Size_t>;
        const ChunkCountT<Size_t> ChunkCount;
        ChunkCountT<Size_t> GetChunkCount() const { return ChunkCount; }

        DChunkCount(const ChunkCountT<Size_t>& chunkCount, const TBase& baseCopy)
            : TBase(baseCopy)
            , ChunkCount(chunkCount)
        {
        }
    };
    template<typename TSize>
    struct ArgTraits<ChunkCountT<TSize>> : public ArgTraitsDefault<DChunkCount>
    {
    };

    template<typename TBase>
    struct DChunkCapacity : public TBase
    {
        using typename TBase::Size_t;
        using Value_t = ChunkCapacityT<Size_t>;
        ChunkCapacityT<Size_t> ChunkCapacity;
        ChunkCapacityT<Size_t> GetChunkCapacity() const { return ChunkCapacity; }
        DChunkCapacity(const ChunkCapacityT<Size_t> chunkCapacity, const TBase& baseCopy)
            : TBase(baseCopy)
            , ChunkCapacity(chunkCapacity)
        {
        }
    };
    template<typename TSize>
    struct ArgTraits<ChunkCapacityT<TSize>> : public ArgTraitsDefault<DChunkCapacity>
    {
    };

    template<typename TBase>
    struct DNodeCountPerChunk : public TBase
    {
        using typename TBase::Size_t;
        using Value_t = NodeCountPerChunkT<Size_t>;
        const NodeCountPerChunkT<Size_t> NodeCountPerChunk;
        NodeCountPerChunkT<Size_t> GetNodeCountPerChunk() const { return NodeCountPerChunk; }

        DNodeCountPerChunk(const NodeCountPerChunkT<Size_t>& nodeCountPerChunk, const TBase& baseCopy)
            : TBase(baseCopy)
            , NodeCountPerChunk(nodeCountPerChunk)
        {
        }
    };
    template<typename TSize>
    struct ArgTraits<NodeCountPerChunkT<TSize>> : public ArgTraitsDefault<DNodeCountPerChunk>
    {
    };

    template<typename TBase>
    struct DNodeCapacityPerChunk : public TBase
    {
        using typename TBase::Size_t;
        using Value_t = NodeCapacityPerChunkT<Size_t>;
        NodeCapacityPerChunkT<Size_t> NodeCapacityPerChunk;
        NodeCapacityPerChunkT<Size_t> GetNodeCapacityPerChunk() const { return NodeCapacityPerChunk; }
        DNodeCapacityPerChunk(const NodeCapacityPerChunkT<Size_t> nodeCapacityPerChunk, const TBase& baseCopy)
            : TBase(baseCopy)
            , NodeCapacityPerChunk(nodeCapacityPerChunk)
        {
        }
    };
    template<typename TSize>
    struct ArgTraits<NodeCapacityPerChunkT<TSize>> : public ArgTraitsDefault<DNodeCapacityPerChunk>
    {
    };


    // TODO may not be needed
    template<typename TBase>
    struct DArrayNodeCount : public TBase
    {
        using typename TBase::Size_t;
        using Value_t = ArrayNodeCountT<Size_t>;
        const ArrayNodeCountT<Size_t> ArrayNodeCount;
        ArrayNodeCountT<Size_t> GetArrayNodeCount() const { return ArrayNodeCount; }

        DArrayNodeCount(const ArrayNodeCountT<Size_t>& arrayNodeCount, const TBase& baseCopy)
            : TBase(baseCopy)
            , ArrayNodeCount(arrayNodeCount)
        {
        }
    };
    template<typename TSize>
    struct ArgTraits<ArrayNodeCountT<TSize>> : public ArgTraitsDefault<DArrayNodeCount>
    {
    };

    // TODO may not be needed
    template<typename TBase>
    struct DArrayNodeCapacity : public TBase
    {
        using typename TBase::Size_t;
        using Value_t = ArrayNodeCapacityT<Size_t>;
        const ArrayNodeCapacityT<Size_t> ArrayNodeCapacity;
        ArrayNodeCapacityT<Size_t> GetArrayNodeCapacity() const { return ArrayNodeCapacity; }

        DArrayNodeCapacity(const ArrayNodeCapacityT<Size_t>& arrayNodeCapacity, const TBase& baseCopy)
            : TBase(baseCopy)
            , ArrayNodeCapacity(arrayNodeCapacity)
        {
        }
    };
    template<typename TSize>
    struct ArgTraits<ArrayNodeCapacityT<TSize>> : public ArgTraitsDefault<DArrayNodeCapacity>
    {
    };






    template<typename TSize, typename TArg>
    auto MakeArg(const TArg& arg)
    {
        return ArgTraits<TArg>::Decorate(arg, DArgs<TSize>());
    }
    template<typename TArg, typename TBase>
    auto MakeArg(const TArg& arg, const TBase& baseCopy)
    {
        return ArgTraits<TArg>::Decorate(arg, baseCopy);
    }


    template <typename TSize, typename T>
    auto MakeArgRecursive(const T& arg) {
        return MakeArg<TSize>(arg);
    }

    template <typename TSize, typename T, typename... Rest>
    auto MakeArgRecursive(const T& first, Rest&&... rest) {
        static_assert(!std::is_same_v<TSize, T>);
        return MakeArg(first, MakeArgRecursive<TSize>(rest...));
    }

    template<typename TSize, typename... TArgs>
    auto MakeArgs(TArgs&&... args)
    {
        return MakeArgRecursive<TSize>(args...);
    }
    //template <typename... TArgs>
    //auto Args(TArgs... args)
    //{
    //    //(TArgs<);
    //}

    //template <typename T1, typename... TArgs>
    //auto Args1(T1 a1, TArgs... args)
    //{
    //    
    //}
}


#define PNC_IMPLEMENT_CHUNKPOINTER_SELFPOINTER()\
    const Chunk_t& operator*()const { return *this; }\
    Chunk_t& operator*() { return *this; }\
    const Chunk_t* operator->()const { return this; }\
    Chunk_t* operator->() { return this; }\
    const Chunk_t& GetChunk()const { return *this; }\
    Chunk_t& GetChunk() { return *this; }\
    static_assert(true)

#define PNC_USING_CHUNKPOINTERINTERNAL_INTERFACE()\
    using Base_t::IsVoid;\
    using Base_t::IsStruct;\
    using Base_t::IsNull;\
    using Base_t::IsData;\
    using Base_t::IsVoidNull;\
    using Base_t::IsVoidData;\
    using Base_t::IsStructNull;\
    using Base_t::IsStructData;\
    using Base_t::GetStructure;\
    using Base_t::GetNodeCount;\
    using Base_t::IsSameStructure;\
    using Base_t::IsSameData;\
    static_assert(true)

#define PNC_USING_CHUNKPOINTER_INTERFACE()\
    PNC_USING_CHUNKPOINTERINTERNAL_INTERFACE();\
    using Base_t::GetInternalChunk;\
    static_assert(true)