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
#   define PNC_MEMORY_ALLOC_LOG
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
#ifdef PNC_MEMORY_ALLOC_LOG
                UE_LOG(LogTemp, Log, TEXT("Alloc %016x (new count %d)"), ptr, instance.AllocationCount);
#endif
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
#ifdef PNC_MEMORY_ALLOC_LOG
                UE_LOG(LogTemp, Log, TEXT("Free  %016x (new count %d)"), ptr, instance.AllocationCount);
#endif
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

    template<typename TProp>
    struct PropTraits
    {

    };

    template<template<typename TProps2> typename TDecorator>
    struct PropTraitsDefault
    {
        
        template<typename TProps>
        constexpr static TDecorator<TProps> Decorate(const TProps& props, typename TDecorator<TProps>::Value_t value)
        {
            return TDecorator<TProps>(props, value);
        }

        template<typename TProps>
        constexpr static auto DecorateSingle(const TProps& props, typename TDecorator<TProps>::Value_t value)
        {
            return DecorateIfNotSameDecorate(props, props, value);
        }

        template<typename TBase, typename TProps>
        constexpr static auto DecorateIfNotSameDecorate(const TDecorator<TBase> d, const TProps& props, typename TDecorator<TProps>::Value_t value)
        {
            return props;
        }
        template<typename TBase, typename TProps, template<typename TProps2Other> typename TDecoratorOther>
        constexpr static auto DecorateIfNotSameDecorate(TDecoratorOther<TBase> d, const TProps& props, typename TDecorator<TProps>::Value_t value)
        {
            return Decorate(props, value);
        }
    };
    template<typename TValue, typename TValue2, template<typename, typename > typename TDecorator>
    struct PropTraitsDefault2
    {

        template<typename TProps>
        constexpr static TDecorator<TValue, TProps> Decorate(const TProps& props, const TValue2 value)
        {
            return TDecorator<TValue, TProps>(props, value);
        }

        template<typename TProps>
        constexpr static auto DecorateSingle(const TProps& props, const TValue2 value)
        {
            return DecorateIfNotSameDecorate(props, props, value);
        }

        template<typename TBase, typename TProps>
        constexpr static auto DecorateIfNotSameDecorate(const TDecorator<TValue, TBase> d, const TProps& props, const TValue2 value)
        {
            return props;
        }
        template<typename TBase, typename TProps, template<typename TProps2Other> typename TDecoratorOther>
        constexpr static auto DecorateIfNotSameDecorate(TDecoratorOther<TBase> d, const TProps& props, TValue2 value)
        {
            return Decorate(props, value);
        }
        template<typename TValueOther, typename TBase, typename TProps, template<typename, typename> typename TDecoratorOther>
        constexpr static auto DecorateIfNotSameDecorate(TDecoratorOther<TValueOther, TBase> d, const TProps& props, TValue2 value)
        {
            return Decorate(props, value);
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
        //ArrayNodeCount,
        //ArrayNodeCapacity,
        // 
        // ComponentTypeIndexInChunk (rename to LocalComponentTypeIndex)
        // LocalNodeIndex
    };
    template<typename TValue, PropId TId >
    struct IntProp
    {
        using Base_t = void;
        using Self_t = IntProp<TValue, TId>;

        TValue Value;
        IntProp(const TValue value)
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

    template<typename TValue, PropId TId> std::strong_ordering operator<=>(const IntProp<TValue, TId>& a, const TValue& b) { return a.Value <=> b; }
    template<typename TValue, PropId TId> IntProp<TValue, TId> operator+(const IntProp<TValue, TId>& a, const TValue& b) { return a.Value + b; }
    template<typename TValue, PropId TId> IntProp<TValue, TId> operator-(const IntProp<TValue, TId>& a, const TValue& b) { return a.Value - b; }
    template<typename TValue, PropId TId> IntProp<TValue, TId> operator*(const IntProp<TValue, TId>& a, const TValue& b) { return a.Value * b; }
    template<typename TValue, PropId TId> IntProp<TValue, TId> operator/(const IntProp<TValue, TId>& a, const TValue& b) { return a.Value / b; }
    template<typename TValue, PropId TId> IntProp<TValue, TId> operator%(const IntProp<TValue, TId>& a, const TValue& b) { return a.Value % b; }
    template<typename TValue, PropId TId> IntProp<TValue, TId> operator&(const IntProp<TValue, TId>& a, const TValue& b) { return a.Value & b; }
    template<typename TValue, PropId TId> IntProp<TValue, TId> operator|(const IntProp<TValue, TId>& a, const TValue& b) { return a.Value | b; }
    template<typename TValue, PropId TId> IntProp<TValue, TId> operator^(const IntProp<TValue, TId>& a, const TValue& b) { return a.Value ^ b; }
    template<typename TValue, PropId TId> IntProp<TValue, TId> operator<<(const IntProp<TValue, TId>& a, const TValue& b) { return a.Value << b; }
    template<typename TValue, PropId TId> IntProp<TValue, TId> operator>>(const IntProp<TValue, TId>& a, const TValue& b) { return a.Value >> b; }
    template<typename TValue, PropId TId> IntProp<TValue, TId> operator&&(const IntProp<TValue, TId>& a, const TValue& b) { return a.Value && b; }
    template<typename TValue, PropId TId> IntProp<TValue, TId> operator||(const IntProp<TValue, TId>& a, const TValue& b) { return a.Value || b; }
    template<typename TValue, PropId TId> IntProp<TValue, TId>& operator+=(IntProp<TValue, TId>& a, const TValue& b) { a.Value += b; return a; }
    template<typename TValue, PropId TId> IntProp<TValue, TId>& operator-=(IntProp<TValue, TId>& a, const TValue& b) { a.Value -= b; return a; }
    template<typename TValue, PropId TId> IntProp<TValue, TId>& operator*=(IntProp<TValue, TId>& a, const TValue& b) { a.Value *= b; return a; }
    template<typename TValue, PropId TId> IntProp<TValue, TId>& operator/=(IntProp<TValue, TId>& a, const TValue& b) { a.Value /= b; return a; }
    template<typename TValue, PropId TId> IntProp<TValue, TId>& operator%=(IntProp<TValue, TId>& a, const TValue& b) { a.Value %= b; return a; }
    template<typename TValue, PropId TId> IntProp<TValue, TId>& operator&=(IntProp<TValue, TId>& a, const TValue& b) { a.Value &= b; return a; }
    template<typename TValue, PropId TId> IntProp<TValue, TId>& operator|=(IntProp<TValue, TId>& a, const TValue& b) { a.Value |= b; return a; }
    template<typename TValue, PropId TId> IntProp<TValue, TId>& operator^=(IntProp<TValue, TId>& a, const TValue& b) { a.Value ^= b; return a; }
    template<typename TValue, PropId TId> IntProp<TValue, TId>& operator<<=(IntProp<TValue, TId>& a, const TValue& b) { a.Value <<= b; return a; }
    template<typename TValue, PropId TId> IntProp<TValue, TId>& operator>>=(IntProp<TValue, TId>& a, const TValue& b) { a.Value >>= b; return a; }


    template<typename TValue, PropId TId> std::strong_ordering operator<=>(const TValue& a, const IntProp<TValue, TId>& b) { return a <=> b.Value; }
    template<typename TValue, PropId TId> IntProp<TValue, TId> operator+(const TValue& a, const IntProp<TValue, TId>& b) { return a + b.Value ; }
    template<typename TValue, PropId TId> IntProp<TValue, TId> operator-(const TValue& a, const IntProp<TValue, TId>& b) { return a - b.Value ; }
    template<typename TValue, PropId TId> IntProp<TValue, TId> operator*(const TValue& a, const IntProp<TValue, TId>& b) { return a * b.Value ; }
    template<typename TValue, PropId TId> IntProp<TValue, TId> operator/(const TValue& a, const IntProp<TValue, TId>& b) { return a / b.Value ; }
    template<typename TValue, PropId TId> IntProp<TValue, TId> operator%(const TValue& a, const IntProp<TValue, TId>& b) { return a % b.Value ; }
    template<typename TValue, PropId TId> IntProp<TValue, TId> operator&(const TValue& a, const IntProp<TValue, TId>& b) { return a & b.Value ; }
    template<typename TValue, PropId TId> IntProp<TValue, TId> operator|(const TValue& a, const IntProp<TValue, TId>& b) { return a | b.Value ; }
    template<typename TValue, PropId TId> IntProp<TValue, TId> operator^(const TValue& a, const IntProp<TValue, TId>& b) { return a ^ b.Value ; }
    template<typename TValue, PropId TId> IntProp<TValue, TId> operator<<(const TValue& a, const IntProp<TValue, TId>& b) { return a << b.Value ; }
    template<typename TValue, PropId TId> IntProp<TValue, TId> operator>>(const TValue& a, const IntProp<TValue, TId>& b) { return a >> b.Value ; }
    template<typename TValue, PropId TId> IntProp<TValue, TId> operator&&(const TValue& a, const IntProp<TValue, TId>& b) { return a && b.Value ; }
    template<typename TValue, PropId TId> IntProp<TValue, TId> operator||(const TValue& a, const IntProp<TValue, TId>& b) { return a || b.Value ; }
    template<typename TValue, PropId TId> TValue& operator+=(TValue& a, const IntProp<TValue, TId>& b) { a += b.Value; return a; }
    template<typename TValue, PropId TId> TValue& operator-=(TValue& a, const IntProp<TValue, TId>& b) { a -= b.Value; return a; }
    template<typename TValue, PropId TId> TValue& operator*=(TValue& a, const IntProp<TValue, TId>& b) { a *= b.Value; return a; }
    template<typename TValue, PropId TId> TValue& operator/=(TValue& a, const IntProp<TValue, TId>& b) { a /= b.Value; return a; }
    template<typename TValue, PropId TId> TValue& operator%=(TValue& a, const IntProp<TValue, TId>& b) { a %= b.Value; return a; }
    template<typename TValue, PropId TId> TValue& operator&=(TValue& a, const IntProp<TValue, TId>& b) { a &= b.Value; return a; }
    template<typename TValue, PropId TId> TValue& operator|=(TValue& a, const IntProp<TValue, TId>& b) { a |= b.Value; return a; }
    template<typename TValue, PropId TId> TValue& operator^=(TValue& a, const IntProp<TValue, TId>& b) { a ^= b.Value; return a; }
    template<typename TValue, PropId TId> TValue& operator<<=(TValue& a, const IntProp<TValue, TId>& b) { a <<= b.Value; return a; }
    template<typename TValue, PropId TId> TValue& operator>>=(TValue& a, const IntProp<TValue, TId>& b) { a >>= b.Value; return a; }

    template<typename TValue, PropId TId> std::strong_ordering operator<=>(const IntProp<TValue, TId>& a, const IntProp<TValue, TId>& b) { return a.Value <=> b.Value; }
    template<typename TValue, PropId TId> IntProp<TValue, TId> operator+(const IntProp<TValue, TId>& a, const IntProp<TValue, TId>& b) { return a.Value + b.Value; }
    template<typename TValue, PropId TId> IntProp<TValue, TId> operator-(const IntProp<TValue, TId>& a, const IntProp<TValue, TId>& b) { return a.Value - b.Value; }
    template<typename TValue, PropId TId> IntProp<TValue, TId> operator*(const IntProp<TValue, TId>& a, const IntProp<TValue, TId>& b) { return a.Value * b.Value; }
    template<typename TValue, PropId TId> IntProp<TValue, TId> operator/(const IntProp<TValue, TId>& a, const IntProp<TValue, TId>& b) { return a.Value / b.Value; }
    template<typename TValue, PropId TId> IntProp<TValue, TId> operator%(const IntProp<TValue, TId>& a, const IntProp<TValue, TId>& b) { return a.Value % b.Value; }
    template<typename TValue, PropId TId> IntProp<TValue, TId> operator&(const IntProp<TValue, TId>& a, const IntProp<TValue, TId>& b) { return a.Value & b.Value; }
    template<typename TValue, PropId TId> IntProp<TValue, TId> operator|(const IntProp<TValue, TId>& a, const IntProp<TValue, TId>& b) { return a.Value | b.Value; }
    template<typename TValue, PropId TId> IntProp<TValue, TId> operator^(const IntProp<TValue, TId>& a, const IntProp<TValue, TId>& b) { return a.Value ^ b.Value; }
    template<typename TValue, PropId TId> IntProp<TValue, TId> operator<<(const IntProp<TValue, TId>& a, const IntProp<TValue, TId>& b) { return a.Value << b.Value; }
    template<typename TValue, PropId TId> IntProp<TValue, TId> operator>>(const IntProp<TValue, TId>& a, const IntProp<TValue, TId>& b) { return a.Value >> b.Value; }
    template<typename TValue, PropId TId> IntProp<TValue, TId> operator&&(const IntProp<TValue, TId>& a, const IntProp<TValue, TId>& b) { return a.Value && b.Value; }
    template<typename TValue, PropId TId> IntProp<TValue, TId> operator||(const IntProp<TValue, TId>& a, const IntProp<TValue, TId>& b) { return a.Value || b.Value; }
    template<typename TValue, PropId TId> IntProp<TValue, TId>& operator+=(IntProp<TValue, TId>& a, const IntProp<TValue, TId>& b) { a.Value += b.Value; return a; }
    template<typename TValue, PropId TId> IntProp<TValue, TId>& operator-=(IntProp<TValue, TId>& a, const IntProp<TValue, TId>& b) { a.Value -= b.Value; return a; }
    template<typename TValue, PropId TId> IntProp<TValue, TId>& operator*=(IntProp<TValue, TId>& a, const IntProp<TValue, TId>& b) { a.Value *= b.Value; return a; }
    template<typename TValue, PropId TId> IntProp<TValue, TId>& operator/=(IntProp<TValue, TId>& a, const IntProp<TValue, TId>& b) { a.Value /= b.Value; return a; }
    template<typename TValue, PropId TId> IntProp<TValue, TId>& operator%=(IntProp<TValue, TId>& a, const IntProp<TValue, TId>& b) { a.Value %= b.Value; return a; }
    template<typename TValue, PropId TId> IntProp<TValue, TId>& operator&=(IntProp<TValue, TId>& a, const IntProp<TValue, TId>& b) { a.Value &= b.Value; return a; }
    template<typename TValue, PropId TId> IntProp<TValue, TId>& operator|=(IntProp<TValue, TId>& a, const IntProp<TValue, TId>& b) { a.Value |= b.Value; return a; }
    template<typename TValue, PropId TId> IntProp<TValue, TId>& operator^=(IntProp<TValue, TId>& a, const IntProp<TValue, TId>& b) { a.Value ^= b.Value; return a; }
    template<typename TValue, PropId TId> IntProp<TValue, TId>& operator<<=(IntProp<TValue, TId>& a, const IntProp<TValue, TId>& b) { a.Value <<= b.Value; return a; }
    template<typename TValue, PropId TId> IntProp<TValue, TId>& operator>>=(IntProp<TValue, TId>& a, const IntProp<TValue, TId>& b) { a.Value >>= b.Value; return a; }

    template<typename TValue, PropId TId>
    struct Cleaner<IntProp<TValue, TId>, false>
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
    using NodeCountT = IntProp<TSize, PropId::NodeCount>;
    template<typename TSize>
    using NodeCapacityT = IntProp<TSize, PropId::NodeCapacity>;
    template<typename TSize>
    using ChunkCountT = IntProp<TSize, PropId::ChunkCount>;
    template<typename TSize>
    using ChunkCapacityT = IntProp<TSize, PropId::ChunkCapacity>;
    template<typename TSize>
    using NodeCountPerChunkT = IntProp<TSize, PropId::NodeCountPerChunk>;
    template<typename TSize>
    using NodeCapacityPerChunkT = IntProp<TSize, PropId::NodeCapacityPerChunk>;
    //template<typename TSize>
    //using ArrayNodeCountT = IntProp<TSize, PropId::ArrayNodeCount>;
    //template<typename TSize>
    //using ArrayNodeCapacityT = IntProp<TSize, PropId::ArrayNodeCapacity>;

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

    //template<typename TSize>
    //NodeCapacityT<TSize> PropArrayToChunk(const ArrayNodeCapacityT<TSize> arrayNodeCapacity)
    //{
    //    return NodeCapacityT<TSize>(arrayNodeCapacity.Value);
    //}
    //template<typename TSize>
    //NodeCountT<TSize> PropArrayToChunk(const ArrayNodeCountT<TSize> arrayNodeCount)
    //{
    //    return NodeCountT<TSize>(arrayNodeCount.Value);
    //}


    //template<typename TSize>
    //ArrayNodeCapacityT<TSize> PropCountToCapacity(const ArrayNodeCountT<TSize> arrayNodeCount)
    //{
    //    return ArrayNodeCapacityT<TSize>(arrayNodeCount.Value);
    //}
    //template<typename TSize>
    //ArrayNodeCapacityT<TSize> PropChunkToArray(const NodeCapacityT<TSize> nodeCapacity)
    //{
    //    return ArrayNodeCapacityT<TSize>(nodeCapacity.Value);
    //}

    //template<typename TSize>
    //ArrayNodeCountT<TSize> PropChunkToArray(const NodeCountT<TSize> nodeCount)
    //{
    //    return ArrayNodeCountT<TSize>(nodeCount.Value);
    //}



    template<typename TSize>
    NodeCountT<TSize> operator-(const NodeCapacityT<TSize>& nodeCapacity, const NodeCountT<TSize>& nodeCount)
    {
        return NodeCountT<TSize>(nodeCapacity.Value - nodeCount.Value);
    }


    template<typename TSize>
    ChunkCountT<TSize> operator-(const ChunkCapacityT<TSize>& chunkCapacity, const ChunkCountT<TSize>& chunkCount)
    {
        return ChunkCountT<TSize>(chunkCapacity.Value - chunkCount.Value);
    }


    template<typename TSize>
    NodeCountT<TSize> operator*(const ChunkCountT<TSize>& chunkCount, const NodeCountPerChunkT<TSize>& nodeCountPerChunk)
    {
        return NodeCountT<TSize>(chunkCount.Value * nodeCountPerChunk.Value);
    }
    template<typename TSize>
    NodeCountT<TSize> operator*(const NodeCountPerChunkT<TSize>& nodeCountPerChunk, const ChunkCountT<TSize>& chunkCount)
    {
        return NodeCountT<TSize>(nodeCountPerChunk.Value * chunkCount.Value);
    }

    template<typename TSize>
    NodeCapacityT<TSize> operator*(const ChunkCapacityT<TSize>& chunkCapacity, const NodeCapacityPerChunkT<TSize>& nodeCapacityPerChunk)
    {
        return NodeCapacityT<TSize>(chunkCapacity.Value * nodeCapacityPerChunk.Value);
    }
    template<typename TSize>
    NodeCapacityT<TSize> operator*(const NodeCapacityPerChunkT<TSize>& nodeCapacityPerChunk, const ChunkCapacityT<TSize>& chunkCapacity)
    {
        return NodeCapacityT<TSize>(nodeCapacityPerChunk.Value * chunkCapacity.Value);
    }


    //template <typename TArg, typename... TArgs>
    //constexpr TArg ArgOfType(TArgs&&... args) {
    //    return std::get<TArg>(std::forward_as_tuple(std::forward<TArgs>(args)...));
    //}


    struct DPropsTag {};

    template<typename TSize>
    struct DProps
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
    
    template<typename TChunkStructure, typename TProps>
    struct DStructurePtr : public TProps
    {
        using typename TProps::Size_t;
        using Value_t = const TChunkStructure*;
        using ChunkStructure_t = TChunkStructure;
        const ChunkStructure_t* const ChunkStructure;
        const ChunkStructure_t* GetStructurePtr() const { return ChunkStructure; }

        DStructurePtr(const TProps& props, const StructurePtr<ChunkStructure_t>& chunkStructure)
            : TProps(props)
            , ChunkStructure(chunkStructure)
        {
        }
        DStructurePtr(const TProps& props, const ChunkStructure_t* const chunkStructure)
            : TProps(props)
            , ChunkStructure(chunkStructure)
        {
        }
    };

    template<typename TChunkStructure>
    struct PropTraits<StructurePtr<TChunkStructure>> : public PropTraitsDefault2<TChunkStructure, StructurePtr<TChunkStructure>, DStructurePtr>
    {
    };

    struct PropComponentDataArray
    {
        void** const ComponentDataArray;
        void** GetComponentDataArray() const { return ComponentDataArray; }

        PropComponentDataArray(void** value)
            : ComponentDataArray(value)
        {
        }
        operator void** ()const { return ComponentDataArray; }

        void* operator->()const { return *ComponentDataArray; }
        void*& operator*()const { return *ComponentDataArray; }
    };
    template<typename TProps>
    struct DComponentDataArray : public TProps
    {
        using typename TProps::Size_t;
        using Value_t = void**;
        void** const ComponentDataArray;
        void** GetComponentDataArray() const { return ComponentDataArray; }

        DComponentDataArray(const TProps& props, void** const componentDataArray)
            : TProps(props)
            , ComponentDataArray(componentDataArray)
        {
        }
    };
    template<>
    struct PropTraits<void**> : public PropTraitsDefault<DComponentDataArray> { };
    template<>
    struct PropTraits<PropComponentDataArray> : public PropTraitsDefault<DComponentDataArray> { };

    template<typename TProps>
    struct DNodeCount : public TProps
    {
        using typename TProps::Size_t;
        using Value_t = NodeCountT<Size_t>;
        const NodeCountT<Size_t> NodeCount;
        NodeCountT<Size_t> GetNodeCount() const { return NodeCount; }

        DNodeCount(const TProps& props, const NodeCountT<Size_t>& nodeCount)
            : TProps(props)
            , NodeCount(nodeCount)
        {
        }
    };
    template<typename TSize>
    struct PropTraits<NodeCountT<TSize>> : public PropTraitsDefault<DNodeCount>
    {
    };
    //
    template<typename TProps>
    struct DNodeCapacity : public TProps
    {
        using typename TProps::Size_t;
        using Value_t = NodeCapacityT<Size_t>;
        NodeCapacityT<Size_t> NodeCapacity;
        NodeCapacityT<Size_t> GetNodeCapacity() const { return NodeCapacity; }
        DNodeCapacity(const TProps& props, const NodeCapacityT<Size_t> nodeCapacity)
            : TProps(props)
            , NodeCapacity(nodeCapacity)
        {
        }
    };
    template<typename TSize>
    struct PropTraits<NodeCapacityT<TSize>> : public PropTraitsDefault<DNodeCapacity>
    {
    };

    template<typename TProps>
    struct DChunkCount : public TProps
    {
        using typename TProps::Size_t;
        using Value_t = ChunkCountT<Size_t>;
        const ChunkCountT<Size_t> ChunkCount;
        ChunkCountT<Size_t> GetChunkCount() const { return ChunkCount; }

        DChunkCount(const TProps& props, const ChunkCountT<Size_t>& chunkCount)
            : TProps(props)
            , ChunkCount(chunkCount)
        {
        }
    };
    template<typename TSize>
    struct PropTraits<ChunkCountT<TSize>> : public PropTraitsDefault<DChunkCount>
    {
    };

    template<typename TProps>
    struct DChunkCapacity : public TProps
    {
        using typename TProps::Size_t;
        using Value_t = ChunkCapacityT<Size_t>;
        ChunkCapacityT<Size_t> ChunkCapacity;
        ChunkCapacityT<Size_t> GetChunkCapacity() const { return ChunkCapacity; }
        DChunkCapacity(const TProps& props, const ChunkCapacityT<Size_t> chunkCapacity)
            : TProps(props)
            , ChunkCapacity(chunkCapacity)
        {
        }
    };
    template<typename TSize>
    struct PropTraits<ChunkCapacityT<TSize>> : public PropTraitsDefault<DChunkCapacity>
    {
    };

    template<typename TProps>
    struct DNodeCountPerChunk : public TProps
    {
        using typename TProps::Size_t;
        using Value_t = NodeCountPerChunkT<Size_t>;
        const NodeCountPerChunkT<Size_t> NodeCountPerChunk;
        NodeCountPerChunkT<Size_t> GetNodeCountPerChunk() const { return NodeCountPerChunk; }

        DNodeCountPerChunk(const TProps& props, const NodeCountPerChunkT<Size_t>& nodeCountPerChunk)
            : TProps(props)
            , NodeCountPerChunk(nodeCountPerChunk)
        {
        }
    };
    template<typename TSize>
    struct PropTraits<NodeCountPerChunkT<TSize>> : public PropTraitsDefault<DNodeCountPerChunk>
    {
    };

    template<typename TProps>
    struct DNodeCapacityPerChunk : public TProps
    {
        using typename TProps::Size_t;
        using Value_t = NodeCapacityPerChunkT<Size_t>;
        NodeCapacityPerChunkT<Size_t> NodeCapacityPerChunk;
        NodeCapacityPerChunkT<Size_t> GetNodeCapacityPerChunk() const { return NodeCapacityPerChunk; }
        DNodeCapacityPerChunk(const TProps& props, const NodeCapacityPerChunkT<Size_t> nodeCapacityPerChunk)
            : TProps(props)
            , NodeCapacityPerChunk(nodeCapacityPerChunk)
        {
        }
    };
    template<typename TSize>
    struct PropTraits<NodeCapacityPerChunkT<TSize>> : public PropTraitsDefault<DNodeCapacityPerChunk>
    {
    };


    template<typename TProps, typename TProp>
    PNC_DEBUG_NOINLINE constexpr auto AppendPropSingle(const TProps& props, const TProp& newProp)
    {
        return PropTraits<TProp>::DecorateSingle(props, newProp);
        //using Decorator_t = PropTraits<TProp>::Decorator_t;
        //return PropDecorateSingle<TProps, Decorator_t>::Decorate(props, newProp);
        //return PropDecorateIf<!std::is_base_of_v<TProp, TProps>>::Decorate(props, newProp);
    }




    template<typename TSize, typename TProp>
    constexpr auto MakeProp(const TProp& prop)
    {
        return PropTraits<TProp>::Decorate(DProps<TSize>(), prop);
    }

    template<typename TProp, typename TProps>
    constexpr auto AppendProp(const TProps& props, const TProp& newProp)
    {
        return PropTraits<TProp>::Decorate(props, newProp);
    }


    template <typename TSize, typename T>
    constexpr auto MakePropUnfold(const T& prop) {
        return MakeProp<TSize>(prop);
    }

    template <typename TSize, typename T, typename... Rest>
    constexpr auto MakePropUnfold(const T& first, Rest&&... rest) {
        static_assert(!std::is_same_v<TSize, T>);
        return AppendProp(MakePropUnfold<TSize>(rest...), first);
    }

    template<typename TSize, typename... TProps>
    constexpr auto MakeProps(TProps&&... props)
    {
        return MakePropUnfold<TSize>(props...);
    }


}
//
//
//#define PNC_IMPLEMENT_CHUNKPOINTER_SELFPOINTER()\
//    const Chunk_t& operator*()const { return *this; }\
//    Chunk_t& operator*() { return *this; }\
//    const Chunk_t* operator->()const { return this; }\
//    Chunk_t* operator->() { return this; }\
//    const Chunk_t& GetChunk()const { return *this; }\
//    Chunk_t& GetChunk() { return *this; }\
//    static_assert(true)

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