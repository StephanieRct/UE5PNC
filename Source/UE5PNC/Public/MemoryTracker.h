// MIT License
// Copyright (c) 2025 Stephanie Rancourt

#pragma once
#include "common.h"

#ifdef PNC_MEMORYTRACKER
#   define pnc_alloc(size, align) ::NiT::MemoryTracker::Allocate(size, align)
#   define pnc_free_clean(ptr, size, align) ::NiT::MemoryTracker::Deallocate(pnc_clean(ptr), size, align)
#   define pnc_free_dirty(ptr, size, align) ::NiT::MemoryTracker::Deallocate(ptr, size, align)
#   define pnc_new(type) new (::NiT::MemoryTracker::Allocate<type>()) type
#   define pnc_delete_clean(ptr) ::NiT::MemoryTracker::Delete(pnc_clean(ptr))
#   define pnc_delete_dirty(ptr) ::NiT::MemoryTracker::Delete(ptr)
#   define pnc_assert_owns(ptr, count) pnc_assert(::NiT::MemoryTracker::Owns(ptr, count))
#   define pnc_owns(ptr, count) ::NiT::MemoryTracker::Owns(ptr, count)
#   define pnc_allocation_count ((int)::NiT::MemoryTracker::Instance.AllocationCount)

namespace NiT
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
        
        PNC_DEBUG_NOINLINE static void* Allocate(std::size_t const size, std::size_t const alignment)
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
        PNC_DEBUG_NOINLINE static void Deallocate(void* const ptr, std::size_t const size, std::size_t const alignment)
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
        PNC_DEBUG_NOINLINE static void Delete(T* const ptr)
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
    using std_unordered_set = std::unordered_set<T, THaser, TEqualer, PncAllocator<T>>;

    template<typename TKey, typename TValue, typename THaser = std::hash<TKey>, typename TEqualer = std::equal_to<TKey>>
    using std_unordered_map = std::unordered_map<TKey, TValue, THaser, TEqualer, PncAllocator<std::pair<const TKey,TValue>>>;

    template<typename T>
    using std_vector = std::vector<T, PncAllocator<T>>;

    template<typename T>
    using std_list = std::list<T, PncAllocator<T>>;

    template<typename T>
    struct std_deleter
    {
        void operator()(T* ptr)const
        {
            pnc_delete_dirty(ptr);
        }
    };

    template<typename T>
    using Unique_Ptr = std::unique_ptr<T, std_deleter<T>>;

    template<typename T, class... TArgumentTypes>
    Unique_Ptr<T> std_make_unique(TArgumentTypes&&... args)
    {
        return Unique_Ptr<T>(pnc_new(T)(std::forward<TArgumentTypes>(args)...));
    }
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
namespace NiT
{
    template<typename T>
    using HashSet = std::unordered_set<T>;
}
#endif
