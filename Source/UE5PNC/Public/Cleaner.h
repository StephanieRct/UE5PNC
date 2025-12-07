// MIT License
// Copyright (c) 2025 Stephanie Rancourt

#pragma once

namespace NiT
{
    template<typename T, bool isEnum = std::is_enum_v<T>>
    struct Cleaner
    {
        struct MissingCleanerForType
        {

        };
        static void Clean(MissingCleanerForType a, T type)
        {
        }
    };
}
#ifdef NI_MEMORYCLEANUP
#   define ni_clean(x) ::NiT::Cleaner<decltype(x)>::Clean(x)

namespace NiT
{

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
#   define ni_clean(x) x
#endif