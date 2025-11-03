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
}