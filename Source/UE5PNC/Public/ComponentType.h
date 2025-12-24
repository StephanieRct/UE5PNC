// MIT License
// Copyright (c) 2025 Stephanie Rancourt

#pragma once
#include "common.h"

namespace NiT
{
    /// <summary>
    /// The owner of a component determine its multiplicity when allocating the component memory in a chunk
    /// </summary>
    enum class ComponentOwner
    {
        /// <summary>
        /// Creates a component instance for each node in a chunk. 
        /// All instances are allocated sequentially in memory.
        /// </summary>
        Node = 0,

        /// <summary>
        /// Creates a single component instance for each chunk which is shared by all nodes in the chunk.
        /// </summary>
        Chunk = 1,

        Begin = 0,
        End = 2,
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
        // void* const baseData, const Size_t firstIndex, const Size_t count
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
            ni_assert(TypeInfo != nullptr);
            ni_assert(Size > 0);
            ni_assert(Align > 0);
            ni_assert(Owner >= ComponentOwner::Begin && Owner < ComponentOwner::End);
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
            ni_assert(_nullptr == nullptr);
            ni_assert(owner >= ComponentOwner::Begin && owner < ComponentOwner::End);

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
        Size_t GetSize(const NodeCountT<   Size_t> nodeCount,    const ChunkCountT<   Size_t> chunkCount =    ChunkCountT<   Size_t>::V_1())const { return Size * GetComponentCount(nodeCount,    chunkCount); }
        Size_t GetSize(const NodeCapacityT<Size_t> nodeCapacity, const ChunkCapacityT<Size_t> chunkCapacity = ChunkCapacityT<Size_t>::V_1())const { return Size * GetComponentCount(nodeCapacity, chunkCapacity); }
        Size_t GetAlignment()const { return Align; }
        ComponentOwner GetOwner()const { return Owner; }
        const type_info* GetTypeInfo()const { return TypeInfo; }

        bool IsNodeComponent()const { return Owner == ComponentOwner::Node; }
        bool IsChunkComponent()const { return Owner == ComponentOwner::Chunk; }
        bool IsNonTrivialConstruct()const { return !!NonTrivialConstruct; }
        bool IsNonTrivialDestruct()const { return !!NonTrivialDestruct; }
        bool IsNonTrivialMoveConstruct()const { return !!NonTrivialMoveConstructForward; }
        bool IsNonTrivialMoveAssignment()const { return !!NonTrivialMoveAssignForward; }
        bool IsNonTrivialCopyConstruct()const { return !!NonTrivialCopyConstructForward; }
        bool IsNonTrivialCopyAssignment()const { return !!NonTrivialCopyAssignForward; }
        bool IsNonTrivialSwap()const { return !!NonTrivialSwapForward; }

        /// <summary>
        /// Construct array of component (can be either NodeComponent or ChunkComponent)
        /// </summary>
        void ConstructDataUnsafe(void* const baseData, const Size_t firstComponentIndex, const Size_t count) const
        {
            ni_assert(!!baseData);
            ni_assert(firstComponentIndex >= 0);
            ni_assert(count >= 0);
            ni_assert_owns(baseData, (firstComponentIndex + count) * Size);

#ifdef NI_MEMORY_NODE_CONSTRUCTZERO
            std::fill((uint8*)baseData + firstComponentIndex * Size, (uint8*)baseData + (firstComponentIndex + count) * Size, 0);
#endif
            if (IsNonTrivialConstruct())
                NonTrivialConstruct(baseData, firstComponentIndex, count);
#if defined(NI_MEMORY_NODE_CONSTRUCTZERO_TRIVIAL) && !defined(NI_MEMORY_NODE_CONSTRUCTZERO)
            else
                std::fill(baseData + firstComponentIndex * Size, baseData + (firstComponentIndex + count) * Size, 0);
#endif
        }
        
        /// <summary>
        /// Construct array of component using either node or chunk props depending on the component owner.
        /// </summary>
        void ConstructComponentUnsafe(void* const baseData,
                                      const            Size_t  firstNodeIndex, const             Size_t firstChunkIndex,
                                      const NodeCountT<Size_t> nodeCount,      const ChunkCountT<Size_t> chunkCount) const
        {
            ConstructDataUnsafe(baseData, GetComponentIndex(firstNodeIndex, firstChunkIndex),
                                          GetComponentCount(nodeCount,      chunkCount));
        }

        /// <summary>
        /// Destruct array of component (can be either NodeComponent or ChunkComponent)
        /// </summary>
        void DestructDataUnsafe(void* const baseData, const Size_t firstComponentIndex, const Size_t count) const
        {
            ni_assert(!!baseData);
            ni_assert(firstComponentIndex >= 0);
            ni_assert(count >= 0);
            ni_assert_owns(baseData, (firstComponentIndex + count) * Size);
            if (IsNonTrivialDestruct())
                NonTrivialDestruct(baseData, firstComponentIndex, count);
#ifdef NI_MEMORY_NODE_DESTRUCTZERO
            std::fill((uint8*)baseData + firstComponentIndex * Size, (uint8*)baseData + (firstComponentIndex + count) * Size, 0);
#endif
        }

        /// <summary>
        /// Destruct array of component using either node or chunk props depending on the component owner.
        /// </summary>
        void DestructComponentUnsafe(void* const baseData, 
                                     const            Size_t firstNodeIndex, const             Size_t  firstChunkIndex,
                                     const NodeCountT<Size_t> nodeCount,     const ChunkCountT<Size_t> chunkCount) const
        {
            DestructDataUnsafe(baseData, GetComponentIndex(firstNodeIndex, firstChunkIndex),
                                                  GetComponentCount(nodeCount,      chunkCount));
        }

        /// <summary>
        /// Move-Construct components between 2 arrays
        /// Notes:
        ///     baseDataTo and baseDataFrom CAN be the same but must not overlap forward.
        /// </summary>
        void MoveConstructDataForwardUnsafe(void* const baseDataTo,   const Size_t firstComponentIndexTo,
                                            void* const baseDataFrom, const Size_t firstComponentIndexFrom,
                                            const Size_t count) const
        {
            ni_assert(!!baseDataTo);
            ni_assert(!!baseDataFrom);
            ni_assert(firstComponentIndexTo >= 0);
            ni_assert(firstComponentIndexFrom >= 0);
            ni_assert(count >= 0);
            ni_assert(!IsOverlappingForward(baseDataTo, firstComponentIndexTo, baseDataFrom, firstComponentIndexFrom, count));
            ni_assert_owns(baseDataTo,   (firstComponentIndexTo   + count) * Size);
            ni_assert_owns(baseDataFrom, (firstComponentIndexFrom + count) * Size);

            if (IsNonTrivialMoveConstruct())
                NonTrivialMoveConstructForward(baseDataTo, firstComponentIndexTo, baseDataFrom, firstComponentIndexFrom, count);
            else
            {
                auto dataLength = count * Size;
                memcpy_s((uint8*)baseDataTo + firstComponentIndexTo * Size, dataLength, (uint8*)baseDataFrom + firstComponentIndexFrom * Size, dataLength);

            }
        }

        /// <summary>
        /// Move-Construct components between 2 arrays using either node or chunk props depending on the component owner.
        /// Notes:
        ///     baseDataTo and baseDataFrom CAN be the same but must not overlap forward.
        /// </summary>
        void MoveConstructComponentForwardUnsafe(
                void* const baseDataTo,   const            Size_t  firstNodeIndexTo,   const             Size_t  firstChunkIndexTo,
                void* const baseDataFrom, const            Size_t  firstNodeIndexFrom, const             Size_t  firstChunkIndexFrom,
                                          const NodeCountT<Size_t> nodeCount,          const ChunkCountT<Size_t> chunkCount)const
        {
            MoveConstructDataForwardUnsafe(baseDataTo,   GetComponentIndex(firstNodeIndexTo,   firstChunkIndexTo),
                                           baseDataFrom, GetComponentIndex(firstNodeIndexFrom, firstChunkIndexFrom),
                                                                  GetComponentCount(nodeCount,          chunkCount));
        }

        /// <summary>
        /// Move-Assign components between 2 arrays
        /// Notes:
        ///     baseDataTo and baseDataFrom CAN be the same but must not overlap forward.
        /// </summary>
        void MoveAssignDataForwardUnsafe(void* const baseDataTo,   const Size_t firstComponentIndexTo,
                                         void* const baseDataFrom, const Size_t firstComponentIndexFrom,
                                                                   const Size_t count)const
        {
            ni_assert(!!baseDataTo);
            ni_assert(!!baseDataFrom);
            ni_assert(firstComponentIndexTo >= 0);
            ni_assert(firstComponentIndexFrom >= 0);
            ni_assert(count >= 0);
            ni_assert(!IsOverlappingForward(baseDataTo, firstComponentIndexTo, baseDataFrom, firstComponentIndexFrom, count));
            ni_assert_owns(baseDataTo,   (firstComponentIndexTo   + count) * Size);
            ni_assert_owns(baseDataFrom, (firstComponentIndexFrom + count) * Size);

            if (IsNonTrivialMoveConstruct())
                NonTrivialMoveAssignForward(baseDataTo, firstComponentIndexTo, baseDataFrom, firstComponentIndexFrom, count);
            else
            {
                auto dataLength = count * Size;
                memcpy_s((uint8*)baseDataTo + firstComponentIndexTo * Size, dataLength, (uint8*)baseDataFrom + firstComponentIndexFrom * Size, dataLength);
            }
        }

        /// <summary>
        /// Move-Assign components between 2 arrays using either node or chunk props depending on the component owner.
        /// Notes:
        ///     baseDataTo and baseDataFrom CAN be the same but must not overlap forward.
        /// </summary>
        void MoveAssignComponentForwardUnsafe(
                 void* const baseDataTo,   const            Size_t  firstNodeIndexTo,   const             Size_t  firstChunkIndexTo,
                 void* const baseDataFrom, const            Size_t  firstNodeIndexFrom, const             Size_t  firstChunkIndexFrom,
                                           const NodeCountT<Size_t> nodeCount,          const ChunkCountT<Size_t> chunkCount)const
        {
            MoveAssignDataForwardUnsafe(baseDataTo,   GetComponentIndex(firstNodeIndexTo,   firstChunkIndexTo),
                                        baseDataFrom, GetComponentIndex(firstNodeIndexFrom, firstChunkIndexFrom),
                                                      GetComponentCount(nodeCount,          chunkCount));
        }

        /// <summary>
        /// Copy-Construct components between 2 arrays
        /// Notes:
        ///     baseDataTo and baseDataFrom CAN be the same but must not overlap forward.
        /// </summary>
        void CopyConstructDataForwardUnsafe(void* const baseDataTo,   const Size_t firstComponentIndexTo,
                                      const void* const baseDataFrom, const Size_t firstComponentIndexFrom,
                                                                      const Size_t count)const
        {
            ni_assert(!!baseDataTo);
            ni_assert(!!baseDataFrom);
            ni_assert(firstComponentIndexTo >= 0);
            ni_assert(firstComponentIndexFrom >= 0);
            ni_assert(count >= 0);
            ni_assert(!IsOverlappingForward(baseDataTo, firstComponentIndexTo, baseDataFrom, firstComponentIndexFrom, count));
            ni_assert_owns(baseDataTo,   (firstComponentIndexTo   + count) * Size);
            ni_assert_owns(baseDataFrom, (firstComponentIndexFrom + count) * Size);

            if (IsNonTrivialCopyConstruct())
                NonTrivialCopyConstructForward(baseDataTo, firstComponentIndexTo, baseDataFrom, firstComponentIndexFrom, count);
            else
            {
                auto dataLength = count * Size;
                memcpy_s((uint8*)baseDataTo + firstComponentIndexTo * Size, dataLength, (uint8*)baseDataFrom + firstComponentIndexFrom * Size, dataLength);
            }
        }

        /// <summary>
        /// Copy-Construct components between 2 arrays using either node or chunk props depending on the component owner.
        /// Notes:
        ///     baseDataTo and baseDataFrom CAN be the same but must not overlap forward.
        /// </summary>
        void CopyConstructComponentForwardUnsafe(
                       void* const baseDataTo,   const            Size_t  firstNodeIndexTo,   const             Size_t  firstChunkIndexTo,
                 const void* const baseDataFrom, const            Size_t  firstNodeIndexFrom, const             Size_t  firstChunkIndexFrom,
                                                 const NodeCountT<Size_t> nodeCount,          const ChunkCountT<Size_t> chunkCount)const
        {
            CopyConstructDataForwardUnsafe(baseDataTo,   GetComponentIndex(firstNodeIndexTo,   firstChunkIndexTo),
                                           baseDataFrom, GetComponentIndex(firstNodeIndexFrom, firstChunkIndexFrom),
                                                         GetComponentCount(nodeCount,          chunkCount));
        }
        
        /// <summary>
        /// Copy-Assign components between 2 arrays.
        /// Notes:
        ///     baseDataTo and baseDataFrom CAN be the same but must not overlap forward.
        /// </summary>
        void CopyAssignDataForwardUnsafe(void* const baseDataTo,   const Size_t firstComponentIndexTo,
                                   const void* const baseDataFrom, const Size_t firstComponentIndexFrom,
                                                                   const Size_t count)const
        {
            ni_assert(!!baseDataTo);
            ni_assert(!!baseDataFrom);
            ni_assert(firstComponentIndexTo >= 0);
            ni_assert(firstComponentIndexFrom >= 0);
            ni_assert(count >= 0);
            ni_assert(!IsOverlappingForward(baseDataTo, firstComponentIndexTo, baseDataFrom, firstComponentIndexFrom, count));
            ni_assert_owns(baseDataTo,   (firstComponentIndexTo   + count) * Size);
            ni_assert_owns(baseDataFrom, (firstComponentIndexFrom + count) * Size);

            if (IsNonTrivialCopyConstruct())
                NonTrivialCopyAssignForward(baseDataTo, firstComponentIndexTo, baseDataFrom, firstComponentIndexFrom, count);
            else
            {
                auto dataLength = count * Size;
                memcpy_s((uint8*)baseDataTo + firstComponentIndexTo * Size, dataLength, (uint8*)baseDataFrom + firstComponentIndexFrom * Size, dataLength);
            }
        }

        /// <summary>
        /// Copy-Assign components between 2 arrays using either node or chunk props depending on the component owner.
        /// Notes:
        ///     baseDataTo and baseDataFrom CAN be the same but must not overlap forward.
        /// </summary>
        void CopyAssignComponentForwardUnsafe(
                       void* const baseDataTo,   const            Size_t  firstNodeIndexTo,   const             Size_t  firstChunkIndexTo,
                 const void* const baseDataFrom, const            Size_t  firstNodeIndexFrom, const             Size_t  firstChunkIndexFrom,
                                                 const NodeCountT<Size_t> nodeCount,          const ChunkCountT<Size_t> chunkCount)const
        {
            CopyAssignDataForwardUnsafe(baseDataTo,   GetComponentIndex(firstNodeIndexTo,   firstChunkIndexTo),
                                        baseDataFrom, GetComponentIndex(firstNodeIndexFrom, firstChunkIndexFrom),
                                                      GetComponentCount(nodeCount,          chunkCount));
        }
        
        /// <summary>
        /// Offset a pointer forward by a number of nodes or chunks depending on the component owner.
        /// </summary>
        void* Forward(void* const ptr, const NodeCountT<Size_t> nodeCount, const ChunkCountT<Size_t> chunkCount)const
        {
            return (uint8*)ptr + GetComponentCount(nodeCount, chunkCount) * Size;
        }

        /// <summary>
        /// Offset a pointer backward by a number of nodes or chunks depending on the component owner.
        /// </summary>
        void* Backward(void* const ptr, const NodeCountT<Size_t> nodeCount, const ChunkCountT<Size_t> chunkCount)const
        {
            return (uint8*)ptr - GetComponentCount(nodeCount, chunkCount) * Size;
        }

        /// <summary>
        /// Returns if 2 ranges of memory overlap in a forward operation.
        /// </summary>
        bool IsOverlappingForward(const void* const baseDataTo, const Size_t firstComponentIndexTo, const void* const baseDataFrom, const Size_t firstComponentIndexFrom, const Size_t count)const
        {
            char* toBegin = (char*)baseDataTo + firstComponentIndexTo * Size;
            char* fromBegin = (char*)baseDataFrom + firstComponentIndexFrom * Size;
            char* fromEnd = fromBegin + count * Size;
            return toBegin > fromBegin && toBegin < fromEnd;
        }

        /// <summary>
        /// Returns if 2 ranges of memory overlap in a backward operation.
        /// </summary>
        bool IsOverlappingBackward(const void* const baseDataTo, const Size_t firstComponentIndexTo, const void* const baseDataFrom, const Size_t firstComponentIndexFrom, const Size_t count)const
        {
            return IsOverlappingForward(baseDataFrom, firstComponentIndexFrom, baseDataTo, firstComponentIndexTo, count);
        }

        /// <summary>
        /// Figure out the index into an array of this component type where a node's component instance is stored.
        /// </summary>
        /// <param name="nodeIndex"></param>
        /// <returns></returns>
        Size_t GetComponentIndex(const Size_t nodeIndex, const Size_t chunkIndex)const
        {
            switch (Owner)
            {
            case ComponentOwner::Node:
                return nodeIndex;
            case ComponentOwner::Chunk:
                return chunkIndex;
            default:
                ni_assert_no_entry_return(-1);
            }
        }
        Size_t GetComponentCount(const NodeCountT<Size_t> nodeCount, const ChunkCountT<Size_t> chunkCount)const
        {
            return GetComponentIndex(nodeCount, chunkCount);
        }
        Size_t GetComponentCount(const NodeCapacityT<Size_t> nodeCount, const ChunkCapacityT<Size_t> chunkCount)const
        {
            return GetComponentIndex(nodeCount, chunkCount);
        }
    };
}
