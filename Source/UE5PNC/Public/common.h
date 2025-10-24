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

#define PNC_MEMORYTRACKER
#define PNC_MEMORYCLEANUP


#define assert_pnc check
#define pnc_todo UE_LOG(LogTemp, Error, TEXT("TODO"))

#ifdef PNC_MEMORYCLEANUP
#   define pnc_clean(x) ::PNC::Cleaner<decltype(x)>::Clean(x)

namespace PNC
{
    template<typename T>
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
    struct Cleaner<T&>
    {
        static T Clean(T& ptr)
        {
            Cleaner<T>::Clean(ptr);
        }
    };

    template<typename T>
    struct Cleaner<T*>
    {
    public:
        static T* Clean(T*& ptr)
        {
            auto* ptr2 = ptr;
            ptr = nullptr;
            return ptr2;
        }
    };

    template<>
    struct Cleaner<int>
    {
    public:
        static int Clean(int& a)
        {
            auto a2 = a;
            a = 0;
            return a2;
        }
    };

    //template<typename T>
    //struct Cleaner<T*>
    //{
    //public:
    //    static T* Clean(T* ptr)
    //    {
    //        return ptr;
    //    }
    //};
    //template<typename T>
    //struct Cleaner<const T*>
    //{
    //    static T* Clean(T*& ptr)
    //    {
    //        auto* ptr2 = ptr;
    //        ptr = nullptr;
    //        return ptr2;
    //    }
    //};

    //template<>
    //struct Cleaner<std::size_t&>
    //{
    //    static std::size_t Clean(std::size_t& a)
    //    {
    //        auto a2 = a;
    //        a = 0;
    //        return a2;
    //    }
    //};
    //template<>
    //struct Cleaner<int>
    //{
    //    static int Clean(int& a)
    //    {
    //        auto a2 = a;
    //        a = 0;
    //        return a2;
    //    }
    //};
}
#else
#   define pnc_clean(x) x
#endif

#ifdef PNC_MEMORYTRACKER
#   define pnc_alloc(size, align) ::PNC::MemoryTracker<>::Allocate(size, align)
#   define pnc_free_clean(ptr, size, align) ::PNC::MemoryTracker<>::Deallocate(pnc_clean(ptr), size, align);
#   define pnc_free_dirty(ptr, size, align) ::PNC::MemoryTracker<>::Deallocate(ptr, size, align);
#   define pnc_new(type) new (::PNC::MemoryTracker<>::Allocate<type>()) type
#   define pnc_delete_clean(ptr) ::PNC::MemoryTracker<>::Delete(pnc_clean(ptr)); 
#   define pnc_delete_dirty(ptr) ::PNC::MemoryTracker<>::Delete(ptr); 


namespace PNC
{
    template<int tI=0>
    struct MemoryTracker
    {
    public:
        static std::atomic<std::size_t> AllocationCount;
        static void* Allocate(std::size_t const size, std::size_t const alignment)
        {
            void* ptr = FMemory::Malloc(size, alignment);
            if (ptr)
                ++AllocationCount;
            return ptr;
        }
        static void Deallocate(void* const ptr, std::size_t const size, std::size_t const alignment)
        {
            assert_pnc(AllocationCount > 0);
            if(ptr != nullptr)
                --AllocationCount;
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
        static void Delete(T* const ptr)
        {
            ptr->~T();
            Deallocate(ptr);
        }

    };

    template<int tI>
    std::atomic<std::size_t> MemoryTracker<tI>::AllocationCount = 0;

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
#   define pnc_free_dirty(ptr, size, align) FMemory::Free(ptr);
#   define pnc_free_clean(ptr, size, align) FMemory::Free(pnc_clean(ptr));
#   define pnc_new(type) new type
#   define pnc_delete_dirty(ptr) delete ptr;
#   define pnc_delete_clean(ptr) delete pnc_clean(ptr);

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
