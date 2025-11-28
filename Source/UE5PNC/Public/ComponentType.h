// MIT License
// Copyright (c) 2025 Stephanie Rancourt

#pragma once
#include "common.h"

namespace PNC
{
    /// <summary>
    /// The owner of a component determine its multiplicity when allocating the component memory in a chunk
    /// </summary>
    enum ComponentOwner
    {
        /// <summary>
        /// Creates a component instance for each node in a chunk. 
        /// All instances are allocated sequentially in memory.
        /// </summary>
        ComponentOwner_Node = 0,

        /// <summary>
        /// Creates a single component instance for each chunk which is shared by all nodes in the chunk.
        /// </summary>
        ComponentOwner_Chunk = 1,

        ComponentOwner__Begin = 0,
        ComponentOwner__End = 2,
    };


    template<typename TSize>
    using FnDataProcessor = void (*)(void* const base, const TSize firstIndex, const TSize count);
   
    template<typename TSize>
    using FnDataCopy = void (*)(void* const baseTo, const TSize firstIndexTo, const void* const baseFrom, const TSize firstIndexFrom, const TSize count);

    template<typename TSize>
    using FnDataMove = void (*)(void* const baseTo, const TSize firstIndexTo, void* const baseFrom, const TSize firstIndexFrom, const TSize count);

    template<typename TSize>
    using FnDataSwap = void (*)(void* const baseTo, const TSize firstIndexTo, void* const baseFrom, const TSize firstIndexFrom, const TSize count);

    ////////////////////////////////////////////////////////////////////////////////
    template<typename TSize, typename T, bool THasDefaultConstructor>
    struct GetDefaultConstructor
    {
    public:
        static FnDataProcessor<TSize> Get() { return nullptr; }
    };
    template<typename TSize, typename T>
    struct GetDefaultConstructor<TSize, T, true>
    {
    public:
        using Size_t = TSize;
        static void Construct(void* const base, const Size_t firstIndex, const Size_t count)
        {
            T* p = (T*)base;
            for (Size_t i = firstIndex; i < firstIndex + count; ++i)
            {
                new(p + i) T();
            }
        }
        // void* const baseComponentData, const Size_t firstIndex, const Size_t count
        static FnDataProcessor<TSize> Get()
        {
            return &Construct;
        }
    };

    ////////////////////////////////////////////////////////////////////////////////
    template<typename TSize, typename T, bool THasDestructor>
    struct GetDestructor
    {
    public:
        static FnDataProcessor<TSize> Get() { return nullptr; }
    };
    template<typename TSize, typename T>
    struct GetDestructor<TSize, T, true>
    {
    public:
        using Size_t = TSize;
        static void Destruct(void* const base, const Size_t firstIndex, const Size_t count)
        {
            T* p = (T*)base;
            for (Size_t i = firstIndex; i < firstIndex + count; ++i)
            {
                p[i].~T();
            }
        }
        static FnDataProcessor<TSize> Get()
        {
            return &Destruct;
        }
    };
    
    ////////////////////////////////////////////////////////////////////////////////
    template<typename TSize, typename T, bool THasMoveConstructor>
    struct GetMoveConstructor
    {
    public:
        using Size_t = TSize;
        static FnDataMove<Size_t> GetForward() { return nullptr; }
        static FnDataMove<Size_t> GetBackward() { return nullptr; }
    };
    template<typename TSize, typename T>
    struct GetMoveConstructor<TSize, T, true>
    {
    public:
        using Size_t = TSize;
        static void MoveForward(void* const baseTo, const TSize firstIndexTo, void* const baseFrom, const TSize firstIndexFrom, const TSize count)
        {
            T* pTo = (T*)baseTo + firstIndexTo;
            T* pFrom = (T*)baseFrom + firstIndexFrom;
            for (Size_t i = 0; i < count; ++i)
                new(pTo + i) T(std::move(*(pFrom + i)));
        }
        static void MoveBackward(void* const baseTo, const Size_t firstIndexTo, void* const baseFrom, const Size_t firstIndexFrom, const Size_t count)
        {
            T* pToBegin = (T*)baseTo + firstIndexTo;
            T* pTo = pToBegin + count;
            T* pFrom = (T*)baseFrom + firstIndexFrom + count;
            for (pTo; pFrom--, pTo-- > pToBegin;)
                new(pTo) T(std::move(*(pFrom)));
        }
        static FnDataMove<Size_t> GetForward() { return &MoveForward; }
        static FnDataMove<Size_t> GetBackward() { return &MoveBackward; }
    };

    ////////////////////////////////////////////////////////////////////////////////
    template<typename TSize, typename T, bool THasMoveAssignment>
    struct GetMoveAssignment
    {
    public:
        using Size_t = TSize;
        static FnDataMove<Size_t> GetForward() { return nullptr; }
        static FnDataMove<Size_t> GetBackward() { return nullptr; }
    };
    template<typename TSize, typename T>
    struct GetMoveAssignment<TSize, T, true>
    {
    public:
        using Size_t = TSize;
        static void MoveForward(void* const baseTo, const TSize firstIndexTo, void* const baseFrom, const TSize firstIndexFrom, const TSize count)
        {
            T* pTo = (T*)baseTo + firstIndexTo;
            T* pFrom = (T*)baseFrom + firstIndexFrom;
            for (Size_t i = 0; i < count; ++i)
                *(pTo + i) = std::move(*(pFrom + i));
        }
        static void MoveBackward(void* const baseTo, const Size_t firstIndexTo, void* const baseFrom, const Size_t firstIndexFrom, const Size_t count)
        {
            T* pToBegin = (T*)baseTo + firstIndexTo;
            T* pTo = pToBegin + count;
            T* pFrom = (T*)baseFrom + firstIndexFrom + count;
            for (pTo; pFrom--, pTo-- > pToBegin;)
                *(pTo) = std::move(*(pFrom));
        }
        static FnDataMove<Size_t> GetForward() { return &MoveForward; }
        static FnDataMove<Size_t> GetBackward() { return &MoveBackward; }
    };

    ////////////////////////////////////////////////////////////////////////////////
    template<typename TSize, typename T, bool THasCopyConstructor>
    struct GetCopyConstructor
    {
    public:
        using Size_t = TSize;
        static FnDataCopy<Size_t> GetForward() { return nullptr; }
        static FnDataCopy<Size_t> GetBackward() { return nullptr; }
    };
    template<typename TSize, typename T>
    struct GetCopyConstructor<TSize, T, true>
    {
    public:
        using Size_t = TSize;
        static void CopyForward(void* const baseTo, const TSize firstIndexTo, const void* const baseFrom, const TSize firstIndexFrom, const TSize count)
        {
            T* pTo = (T*)baseTo + firstIndexTo;
            const T* pFrom = (const T*)baseFrom + firstIndexFrom;
            for (Size_t i = 0; i < count; ++i)
                new(pTo + i) T(*(pFrom + i));
        }
        static void CopyBackward(void* const baseTo, const Size_t firstIndexTo, const void* const baseFrom, const Size_t firstIndexFrom, const Size_t count)
        {
            T* pToBegin = (T*)baseTo + firstIndexTo;
            T* pTo = pToBegin + count;
            const T* pFrom = (const T*)baseFrom + firstIndexFrom + count;
            for (pTo; pFrom--, pTo-- > pToBegin;)
                new(pTo) T(*pFrom);
        }
        static FnDataCopy<Size_t> GetForward() { return &CopyForward; }
        static FnDataCopy<Size_t> GetBackward() { return &CopyBackward; }
    };

    ////////////////////////////////////////////////////////////////////////////////
    template<typename TSize, typename T, bool THasCopyAssignment>
    struct GetCopyAssignment
    {
    public:
        using Size_t = TSize;
        static FnDataCopy<Size_t> GetForward() { return nullptr; }
        static FnDataCopy<Size_t> GetBackward() { return nullptr; }
    };
    template<typename TSize, typename T>
    struct GetCopyAssignment<TSize, T, true>
    {
    public:
        using Size_t = TSize;
        static void CopyForward(void* const baseTo, const TSize firstIndexTo, const void* const baseFrom, const TSize firstIndexFrom, const TSize count)
        {
            T* pTo = (T*)baseTo + firstIndexTo;
            T* pFrom = (T*)baseFrom + firstIndexFrom;
            for (Size_t i = 0; i < count; ++i)
                *(pTo + i) = *(pFrom + i);
        }
        static void CopyBackward(void* const baseTo, const Size_t firstIndexTo, const void* const baseFrom, const Size_t firstIndexFrom, const Size_t count)
        {
            T* pToBegin = (T*)baseTo + firstIndexTo;
            T* pTo = pToBegin + count;
            T* pFrom = (T*)baseFrom + firstIndexFrom + count;
            for (pTo; pFrom--, pTo-- > pToBegin;)
                *(pTo) = *(pFrom);
        }
        static FnDataCopy<Size_t> GetForward() { return &CopyForward; }
        static FnDataCopy<Size_t> GetBackward() { return &CopyBackward; }
    };

    ////////////////////////////////////////////////////////////////////////////////
    template<typename TSize, typename T, bool TIsSwappable>
    struct GetSwapper
    {
    public:
        using Size_t = TSize;
        static FnDataSwap<Size_t> GetForward() { return nullptr; }
        static FnDataSwap<Size_t> GetBackward() { return nullptr; }
    };
    template<typename TSize, typename T>
    struct GetSwapper<TSize, T, true>
    {
    public:
        using Size_t = TSize;
        static void SwapForward(void* const baseTo, const Size_t firstIndexTo, void* const baseFrom, const Size_t firstIndexFrom, const Size_t count)
        {
            T* pTo = (T*)baseTo + firstIndexTo;
            T* pFrom = (T*)baseFrom + firstIndexFrom;
            std::swap_ranges(pTo, pTo + count, pFrom);
        }
        static void SwapBackward(void* const baseTo, const Size_t firstIndexTo, void* const baseFrom, const Size_t firstIndexFrom, const Size_t count)
        {
            T* pToBegin = (T*)baseTo + firstIndexTo;
            T* pTo = pToBegin + count;
            T* pFrom = (T*)baseFrom + firstIndexFrom + count;
            for (pTo; pFrom--, pTo-- > pToBegin;)
                std::swap(*pTo, *pFrom);
        }
        static FnDataSwap<Size_t> GetForward() { return &SwapForward; }
        static FnDataSwap<Size_t> GetBackward() { return &SwapBackward; }
    };

    /// <summary>
    /// Provide a way to uniquely identify each component types, their owner and how to allocate component memory on demand.
    /// </summary>
    template< typename TSize>
    struct ComponentTypeT 
    {
    public:
        using Self_t = ComponentTypeT<TSize>;
        using Size_t = TSize;

    protected:
        const type_info* TypeInfo;
        Size_t Size;
        Size_t Align;
        ComponentOwner Owner;
    private:
        FnDataProcessor<Size_t> NonTrivialConstruct;
        FnDataProcessor<Size_t> NonTrivialDestruct;
        FnDataMove<Size_t> NonTrivialMoveConstructForward;
        FnDataMove<Size_t> NonTrivialMoveConstructBackward;
        FnDataMove<Size_t> NonTrivialMoveAssignForward;
        FnDataMove<Size_t> NonTrivialMoveAssignBackward;
        FnDataCopy<Size_t> NonTrivialCopyConstructForward;
        FnDataCopy<Size_t> NonTrivialCopyConstructBackward;
        FnDataCopy<Size_t> NonTrivialCopyAssignForward;
        FnDataCopy<Size_t> NonTrivialCopyAssignBackward;
        FnDataSwap<Size_t> NonTrivialSwapForward;
        FnDataSwap<Size_t> NonTrivialSwapBackward;

    public:
        /// <summary>
        /// Create a ComponentType from the component's type_info.
        /// </summary>
        /// <param name="typeInfo">type_info of the component type. Must be a valid pointer.</param>
        /// <param name="size">Size of the component in bytes. Must be greater than 0.</param>
        /// <param name="align">Alignment of the component in bytes. Must be greater than 0.</param>
        /// <param name="owner">Owner of this component type.</param>
        ComponentTypeT(const std::type_info* const typeInfo, const Size_t size, const Size_t align, const ComponentOwner owner)
            : TypeInfo(typeInfo)
            , Size(size)
            , Align(align)
            , Owner(owner) 
        {
            pnc_assert(TypeInfo != nullptr);
            pnc_assert(Size > 0);
            pnc_assert(Align > 0);
            pnc_assert(Owner >= ComponentOwner__Begin && Owner < ComponentOwner__End);
        }

        /// <summary>
        /// Create a ComponentType from a typename.
        /// </summary>
        /// <typeparam name="T">The component typename</typeparam>
        /// <param name="_nullptr">Should always be equal to (const T*)nullptr. Provides a way to specify the component typename argument.</param>
        /// <param name="aOwner">Owner for this component type.</param>
        template<typename T>
        ComponentTypeT(const T* const _nullptr, const ComponentOwner owner = T::Owner)
            : TypeInfo(&typeid(T))
            , Size(sizeof(T))
            , Align(alignof(T))
            , Owner(owner)
        {
            pnc_assert(_nullptr == nullptr);
            pnc_assert(owner >= ComponentOwner__Begin && owner < ComponentOwner__End);

            NonTrivialConstruct = GetDefaultConstructor<Size_t, T, std::is_default_constructible<T>::value>::Get();
            NonTrivialDestruct = GetDestructor<Size_t, T, std::is_destructible<T>::value>::Get();
            NonTrivialMoveConstructForward  = GetMoveConstructor<Size_t, T, std::is_move_constructible<T>::value>::GetForward();
            NonTrivialMoveConstructBackward = GetMoveConstructor<Size_t, T, std::is_move_constructible<T>::value>::GetBackward();
            NonTrivialMoveAssignForward = GetMoveAssignment<Size_t, T, std::is_move_assignable_v<T>>::GetForward();
            NonTrivialMoveAssignForward = GetMoveAssignment<Size_t, T, std::is_move_assignable_v<T>>::GetBackward();
            NonTrivialCopyConstructForward  = GetCopyConstructor<Size_t, T, std::is_copy_constructible<T>::value>::GetForward();
            NonTrivialCopyConstructBackward = GetCopyConstructor<Size_t, T, std::is_copy_constructible<T>::value>::GetBackward();
            NonTrivialCopyAssignForward = GetCopyAssignment<Size_t, T, std::is_copy_assignable_v<T>>::GetForward();
            NonTrivialCopyAssignForward = GetCopyAssignment<Size_t, T, std::is_copy_assignable_v<T>>::GetBackward();
            NonTrivialSwapForward = GetSwapper<Size_t, T, std::is_swappable<T>::value>::GetForward();
            NonTrivialSwapBackward = GetSwapper<Size_t, T, std::is_swappable<T>::value>::GetBackward();
        }

    public:
        Size_t GetSize()const { return Size; }
        Size_t GetSize(const NodeCountT<Size_t> nodeCount, const ChunkCountT<Size_t> chunkCount = 1)const { return Size * GetComponentCount(nodeCount, chunkCount); }
        Size_t GetSize(const NodeCapacityT<Size_t> nodeCapacity, const ChunkCapacityT<Size_t> chunkCapacity = 1)const { return Size * GetComponentCount(nodeCapacity, chunkCapacity); }
        Size_t GetAlignment()const { return Align; }
        ComponentOwner GetOwner()const { return Owner; }
        const type_info* GetTypeInfo()const { return TypeInfo; }

        bool IsNodeComponent()const { return Owner == ComponentOwner_Node; }
        bool IsChunkComponent()const { return Owner == ComponentOwner_Chunk; }
        bool IsNonTrivialConstruct()const { return !!NonTrivialConstruct; }
        bool IsNonTrivialDestruct()const { return !!NonTrivialDestruct; }
        bool IsNonTrivialMoveConstruct()const { return !!NonTrivialMoveConstructForward; }
        bool IsNonTrivialMoveAssignment()const { return !!NonTrivialMoveAssignForward; }
        bool IsNonTrivialCopyConstruct()const { return !!NonTrivialCopyConstructForward; }
        bool IsNonTrivialCopyAssignment()const { return !!NonTrivialCopyAssignForward; }
        bool IsNonTrivialSwap()const { return !!NonTrivialSwapForward; }


        void ConstructDataUnsafe(void* const baseComponentData, const Size_t firstComponentIndex, const Size_t count) const
        {
            pnc_assert(!!baseComponentData);
            pnc_assert(firstComponentIndex >= 0);
            pnc_assert(count >= 0);
            pnc_assert_owns(baseComponentData, (firstComponentIndex + count) * Size);

#ifdef PNC_MEMORY_NODE_CONSTRUCTZERO
            std::fill((uint8*)baseComponentData + firstComponentIndex * Size, (uint8*)baseComponentData + (firstComponentIndex + count) * Size, 0);
#endif
            if (IsNonTrivialConstruct())
                NonTrivialConstruct(baseComponentData, firstComponentIndex, count);
#if defined(PNC_MEMORY_NODE_CONSTRUCTZERO_TRIVIAL) && !defined(PNC_MEMORY_NODE_CONSTRUCTZERO)
            else
                std::fill(baseComponentData + firstComponentIndex * Size, baseComponentData + (firstComponentIndex + count) * Size, 0);
#endif
        }
        
        void ConstructComponentUnsafe(void* const baseComponentData, const Size_t firstNodeIndex, const Size_t firstChunkIndex,
                                                                     const NodeCountT<Size_t> nodeCount, const ChunkCountT<Size_t> chunkCount) const
        {
            ConstructDataUnsafe(baseComponentData, GetComponentIndex(firstNodeIndex, firstChunkIndex),
                                                   GetComponentCount(nodeCount,      chunkCount));
        }

        void DestructDataUnsafe(void* const baseComponentData, const Size_t firstComponentIndex, const Size_t count) const
        {
            pnc_assert(!!baseComponentData);
            pnc_assert(firstComponentIndex >= 0);
            pnc_assert(count >= 0);
            pnc_assert_owns(baseComponentData, (firstComponentIndex + count) * Size);
            if (IsNonTrivialDestruct())
                NonTrivialDestruct(baseComponentData, firstComponentIndex, count);
#ifdef PNC_MEMORY_NODE_DESTRUCTZERO
            std::fill((uint8*)baseComponentData + firstComponentIndex * Size, (uint8*)baseComponentData + (firstComponentIndex + count) * Size, 0);
#endif
        }
        
        void DestructComponentUnsafe(void* const baseComponentData, const Size_t firstNodeIndex, const Size_t firstChunkIndex,
                                                                    const NodeCountT<Size_t> nodeCount,      const ChunkCountT<Size_t> chunkCount) const
        {
            DestructDataUnsafe(baseComponentData, GetComponentIndex(firstNodeIndex, firstChunkIndex),
                                                  GetComponentCount(nodeCount,      chunkCount));
        }

        void MoveConstructDataForwardUnsafe(void* const baseComponentDataTo,   const Size_t firstComponentIndexTo,
                                            void* const baseComponentDataFrom, const Size_t firstComponentIndexFrom,
                                            const Size_t count) const
        {
            pnc_assert(!!baseComponentDataTo);
            pnc_assert(!!baseComponentDataFrom);
            pnc_assert(firstComponentIndexTo >= 0);
            pnc_assert(firstComponentIndexFrom >= 0);
            pnc_assert(count >= 0);
            pnc_assert(!IsOverlappingForward(baseComponentDataTo, firstComponentIndexTo, baseComponentDataFrom, firstComponentIndexFrom, count));
            pnc_assert_owns(baseComponentDataTo,   (firstComponentIndexTo   + count) * Size);
            pnc_assert_owns(baseComponentDataFrom, (firstComponentIndexFrom + count) * Size);

            if (IsNonTrivialMoveConstruct())
                NonTrivialMoveConstructForward(baseComponentDataTo, firstComponentIndexTo, baseComponentDataFrom, firstComponentIndexFrom, count);
            else
            {
                auto dataLength = count * Size;
                memcpy_s((uint8*)baseComponentDataTo + firstComponentIndexTo * Size, dataLength, (uint8*)baseComponentDataFrom + firstComponentIndexFrom * Size, dataLength);

            }
        }
        void MoveConstructComponentForwardUnsafe(void* const baseComponentDataTo,   const Size_t firstNodeIndexTo,   const Size_t firstChunkIndexTo,
                                                 void* const baseComponentDataFrom, const Size_t firstNodeIndexFrom, const Size_t firstChunkIndexFrom,
                                                 const NodeCountT<Size_t> nodeCount, const ChunkCountT<Size_t> chunkCount)const
        {
            MoveConstructDataForwardUnsafe(baseComponentDataTo,   GetComponentIndex(firstNodeIndexTo,   firstChunkIndexTo),
                                           baseComponentDataFrom, GetComponentIndex(firstNodeIndexFrom, firstChunkIndexFrom),
                                                                  GetComponentCount(nodeCount,          chunkCount));
        }
        
        void MoveAssignDataForwardUnsafe(void* const baseComponentDataTo,   const Size_t firstComponentIndexTo,
                                         void* const baseComponentDataFrom, const Size_t firstComponentIndexFrom,
                                         const Size_t count)const
        {
            pnc_assert(!!baseComponentDataTo);
            pnc_assert(!!baseComponentDataFrom);
            pnc_assert(firstComponentIndexTo >= 0);
            pnc_assert(firstComponentIndexFrom >= 0);
            pnc_assert(count >= 0);
            pnc_assert(!IsOverlappingForward(baseComponentDataTo, firstComponentIndexTo, baseComponentDataFrom, firstComponentIndexFrom, count));
            pnc_assert_owns(baseComponentDataTo,   (firstComponentIndexTo   + count) * Size);
            pnc_assert_owns(baseComponentDataFrom, (firstComponentIndexFrom + count) * Size);

            if (IsNonTrivialMoveConstruct())
                NonTrivialMoveAssignForward(baseComponentDataTo, firstComponentIndexTo, baseComponentDataFrom, firstComponentIndexFrom, count);
            else
            {
                auto dataLength = count * Size;
                memcpy_s((uint8*)baseComponentDataTo + firstComponentIndexTo * Size, dataLength, (uint8*)baseComponentDataFrom + firstComponentIndexFrom * Size, dataLength);

            }
        }
        void MoveAssignComponentForwardUnsafe(void* const baseComponentDataTo,   const Size_t firstNodeIndexTo,   const Size_t firstChunkIndexTo,
                                              void* const baseComponentDataFrom, const Size_t firstNodeIndexFrom, const Size_t firstChunkIndexFrom,
                                              const NodeCountT<Size_t> nodeCount, const ChunkCountT<Size_t> chunkCount)const
        {
            MoveAssignDataForwardUnsafe(baseComponentDataTo,   GetComponentIndex(firstNodeIndexTo,   firstChunkIndexTo),
                                        baseComponentDataFrom, GetComponentIndex(firstNodeIndexFrom, firstChunkIndexFrom),
                                                               GetComponentCount(nodeCount,          chunkCount));
        }

        void CopyConstructDataForwardUnsafe(void* const baseComponentDataTo,   const Size_t firstComponentIndexTo,
                                      const void* const baseComponentDataFrom, const Size_t firstComponentIndexFrom,
                                      const Size_t count)const
        {
            pnc_assert(!!baseComponentDataTo);
            pnc_assert(!!baseComponentDataFrom);
            pnc_assert(firstComponentIndexTo >= 0);
            pnc_assert(firstComponentIndexFrom >= 0);
            pnc_assert(count >= 0);
            pnc_assert(!IsOverlappingForward(baseComponentDataTo, firstComponentIndexTo, baseComponentDataFrom, firstComponentIndexFrom, count));
            pnc_assert_owns(baseComponentDataTo,   (firstComponentIndexTo   + count) * Size);
            pnc_assert_owns(baseComponentDataFrom, (firstComponentIndexFrom + count) * Size);

            if (IsNonTrivialCopyConstruct())
                NonTrivialCopyConstructForward(baseComponentDataTo, firstComponentIndexTo, baseComponentDataFrom, firstComponentIndexFrom, count);
            else
            {
                auto dataLength = count * Size;
                memcpy_s((uint8*)baseComponentDataTo + firstComponentIndexTo * Size, dataLength, (uint8*)baseComponentDataFrom + firstComponentIndexFrom * Size, dataLength);
            }
        }
        void CopyConstructComponentForwardUnsafe(void* const baseComponentDataTo,   const Size_t firstNodeIndexTo,   const Size_t firstChunkIndexTo,
                                           const void* const baseComponentDataFrom, const Size_t firstNodeIndexFrom, const Size_t firstChunkIndexFrom,
                                           const NodeCountT<Size_t> nodeCount, const ChunkCountT<Size_t> chunkCount)const
        {
            CopyConstructDataForwardUnsafe(baseComponentDataTo,   GetComponentIndex(firstNodeIndexTo,   firstChunkIndexTo),
                                           baseComponentDataFrom, GetComponentIndex(firstNodeIndexFrom, firstChunkIndexFrom),
                                                                  GetComponentCount(nodeCount,          chunkCount));
        }
        

        
        void CopyAssignDataForwardUnsafe(void* const baseComponentDataTo,   const Size_t firstComponentIndexTo,
                                   const void* const baseComponentDataFrom, const Size_t firstComponentIndexFrom,
                                   const Size_t count)const
        {
            pnc_assert(!!baseComponentDataTo);
            pnc_assert(!!baseComponentDataFrom);
            pnc_assert(firstComponentIndexTo >= 0);
            pnc_assert(firstComponentIndexFrom >= 0);
            pnc_assert(count >= 0);
            pnc_assert(!IsOverlappingForward(baseComponentDataTo, firstComponentIndexTo, baseComponentDataFrom, firstComponentIndexFrom, count));
            pnc_assert_owns(baseComponentDataTo,   (firstComponentIndexTo   + count) * Size);
            pnc_assert_owns(baseComponentDataFrom, (firstComponentIndexFrom + count) * Size);

            if (IsNonTrivialCopyConstruct())
                NonTrivialCopyAssignForward(baseComponentDataTo, firstComponentIndexTo, baseComponentDataFrom, firstComponentIndexFrom, count);
            else
            {
                auto dataLength = count * Size;
                memcpy_s((uint8*)baseComponentDataTo + firstComponentIndexTo * Size, dataLength, (uint8*)baseComponentDataFrom + firstComponentIndexFrom * Size, dataLength);
            }
        }
        void CopyAssignComponentForwardUnsafe(void* const baseComponentDataTo,   const Size_t firstNodeIndexTo,   const Size_t firstChunkIndexTo,
                                        const void* const baseComponentDataFrom, const Size_t firstNodeIndexFrom, const Size_t firstChunkIndexFrom,
                                        const NodeCountT<Size_t> nodeCount, const ChunkCountT<Size_t> chunkCount)const
        {
            CopyAssignDataForwardUnsafe(baseComponentDataTo,   GetComponentIndex(firstNodeIndexTo,   firstChunkIndexTo),
                                        baseComponentDataFrom, GetComponentIndex(firstNodeIndexFrom, firstChunkIndexFrom),
                                                               GetComponentCount(nodeCount,          chunkCount));
        }
        














        //void CopyConstructComponentForwardUnsafe(void* const baseComponentDataTo,   const Size_t firstNodeIndexTo,
        //                                   const void* const baseComponentDataFrom, const Size_t firstNodeIndexFrom,
        //                                   const NodeCountT<Size_t> nodeCount)const
        //{
        //    CopyConstructDataForwardUnsafe(baseComponentDataTo,   firstNodeIndexTo,
        //                                   baseComponentDataFrom, firstNodeIndexFrom, 1);
        //}






        //void MoveComponentDataForwardUnsafe(void* const baseComponentDataTo, void* const baseComponentDataFrom, const Size_t firstComponentIndexTo, const Size_t firstComponentIndexFrom, const Size_t count)const
        //{
        //    pnc_assert(!!baseComponentDataTo);
        //    pnc_assert(!!baseComponentDataFrom);
        //    pnc_assert(firstComponentIndexTo >= 0);
        //    pnc_assert(firstComponentIndexFrom >= 0);
        //    pnc_assert(count >= 0);
        //    pnc_assert(!IsOverlappingForward(baseComponentDataTo, firstComponentIndexTo, baseComponentDataFrom, firstComponentIndexFrom, count));

        //    if (IsUserMovable())
        //        NonTrivialMoveConstructForward(baseComponentDataTo, baseComponentDataFrom, firstComponentIndexTo, firstComponentIndexFrom, count);
        //    else
        //    {
        //        auto dataLength = count * Size;
        //        memcpy_s((uint8*)baseComponentDataTo + firstComponentIndexTo * Size, dataLength, (uint8*)baseComponentDataFrom + firstComponentIndexFrom * Size, dataLength);
        //    }
        //}

        //void SwapComponentDataForwardUnsafe(void* const baseComponentDataTo, void* const baseComponentDataFrom, const Size_t firstComponentIndexTo, const Size_t firstComponentIndexFrom, const Size_t count)const
        //{
        //    pnc_assert(!!baseComponentDataTo);
        //    pnc_assert(!!baseComponentDataFrom);
        //    pnc_assert(firstComponentIndexTo >= 0);
        //    pnc_assert(firstComponentIndexFrom >= 0);
        //    pnc_assert(count >= 0);
        //    pnc_assert(!IsOverlappingForward(baseComponentDataTo, firstComponentIndexTo, baseComponentDataFrom, firstComponentIndexFrom, count));

        //    if (IsUserSwappable())
        //        NodeSwapForward(baseComponentDataTo, baseComponentDataFrom, firstComponentIndexTo, firstComponentIndexFrom, count);
        //    else
        //    {
        //        auto dataCount = GetComponentCount(count) * Size;
        //        auto to = (uint8*)baseComponentDataTo + firstComponentIndexTo * Size;
        //        auto from = (uint8*)baseComponentDataFrom + firstComponentIndexFrom * Size;
        //        std::swap_ranges(to, to + dataCount, from);
        //    }
        //}



        //void MoveComponentForwardUnsafe(void* const baseComponentDataTo, void* const baseComponentDataFrom,
        //    const Size_t firstNodeIndexTo, const Size_t firstNodeIndexFrom, const NodeCountT<Size_t> nodeCount,
        //    const Size_t firstChunkIndexTo = 0, const Size_t firstChunkIndexFrom = 0, const ChunkCountT<Size_t> chunkCount = 1)const
        //{
        //    MoveComponentDataForwardUnsafe(baseComponentDataTo, baseComponentDataFrom,
        //        GetComponentIndex(firstNodeIndexTo, firstChunkIndexTo),
        //        GetComponentIndex(firstNodeIndexFrom, firstChunkIndexFrom),
        //        GetComponentCount(nodeCount, chunkCount));
        //}

        //void SwapComponentForwardUnsafe(void* const baseComponentDataTo, void* const baseComponentDataFrom,
        //    const Size_t firstNodeIndexTo, const Size_t firstNodeIndexFrom, const NodeCountT<Size_t> nodeCount,
        //    const Size_t firstChunkIndexTo = 0, const Size_t firstChunkIndexFrom = 0, const ChunkCountT<Size_t> chunkCount = 1)const
        //{
        //    SwapComponentDataForwardUnsafe(baseComponentDataTo, baseComponentDataFrom,
        //        GetComponentIndex(firstNodeIndexTo, firstChunkIndexTo),
        //        GetComponentIndex(firstNodeIndexFrom, firstChunkIndexFrom),
        //        GetComponentCount(nodeCount, chunkCount));
        //}


        ///// <summary>
        ///// Copy component data from one chunk of memory to another.
        ///// </summary>
        ///// <param name="to">destination memory</param>
        ///// <param name="from">source memory</param>
        ///// <param name="nodeCount">How many component instances to copy</param>
        //void Copy(void* const to, void const* from, const NodeCountT<Size_t> nodeCount, const ChunkCapacityT<Size_t> chunkCapacity = 1)const
        //{
        //    auto count = GetComponentIndex(nodeCount, chunkCapacity);
        //    memcpy_s(to, count * Size, from, count * Size);
        //}

        //void* SubChunk(void* ptr, Size_t count)const
        //{
        //    switch (Owner)
        //    {
        //    case ComponentOwner_Node:
        //        return ptr + count * Size;
        //    case ComponentOwner_Chunk:
        //        return ptr + Size;
        //    default:
        //        pnc_assert_no_entry();
        //        return -1;
        //    }
        //}

        void* Forward(void* const ptr, const NodeCountT<Size_t> nodeCount, const ChunkCountT<Size_t> chunkCount = 1)const
        {
            return (uint8*)ptr + GetComponentCount(nodeCount, chunkCount) * Size;
        }
        void* Backward(void* const ptr, const NodeCountT<Size_t> nodeCount, const ChunkCountT<Size_t> chunkCount = 1)const
        {
            return (uint8*)ptr - GetComponentCount(nodeCount, chunkCount) * Size;
        }

        bool IsOverlappingForward(const void* const baseComponentDataTo, const Size_t firstComponentIndexTo, const void* const baseComponentDataFrom, const Size_t firstComponentIndexFrom, const Size_t count)const
        {
            char* toBegin = (char*)baseComponentDataTo + firstComponentIndexTo * Size;
            char* fromBegin = (char*)baseComponentDataFrom + firstComponentIndexFrom * Size;
            char* fromEnd = fromBegin + count * Size;
            return toBegin > fromBegin && toBegin < fromEnd;
        }
        bool IsOverlappingBackward(const void* const baseComponentDataTo, const Size_t firstComponentIndexTo, const void* const baseComponentDataFrom, const Size_t firstComponentIndexFrom, const Size_t count)const
        {
            return IsOverlappingForward(baseComponentDataFrom, firstComponentIndexFrom, baseComponentDataTo, firstComponentIndexTo, count);
        }


        /// <summary>
        /// Figure out the index into an array of this component type where a node's component instance is stored.
        /// </summary>
        /// <param name="nodeIndex"></param>
        /// <returns></returns>
        Size_t GetComponentIndex(const Size_t nodeIndex, const Size_t chunkIndex = 0)const
        {
            switch (Owner)
            {
            case ComponentOwner_Node:
                return nodeIndex;
            case ComponentOwner_Chunk:
                return chunkIndex;
            default:
                pnc_assert_no_entry_return(-1);
            }
        }
        Size_t GetComponentCount(const NodeCountT<Size_t> nodeCount, const ChunkCountT<Size_t> chunkCount = 1)const
        {
            return GetComponentIndex(nodeCount, chunkCount);
        }
        Size_t GetComponentCount(const NodeCapacityT<Size_t> nodeCount, const ChunkCapacityT<Size_t> chunkCount = 1)const
        {
            return GetComponentIndex(nodeCount, chunkCount);
        }
    };
}
