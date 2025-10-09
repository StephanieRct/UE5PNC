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
    using FnDataProcessor2 = void (*)(void* const baseTo, void* const baseFrom, const TSize firstIndexTo, const TSize firstIndexFrom, const TSize count);


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
    template<typename TSize, typename T, bool THasCopyConstructor>
    struct GetCopyConstructor
    {
    public:
        static FnDataProcessor<TSize> Get() { return nullptr; }
    };
    template<typename TSize, typename T>
    struct GetCopyConstructor<TSize, T, true>
    {
    public:
        using Size_t = TSize;
        static void Copy(void* const baseTo, void* const baseFrom, const TSize firstIndexTo, const TSize firstIndexFrom, const TSize count)
        {
            T* pTo = (T*)baseTo + firstIndexTo;
            T* pFrom = (T*)baseFrom + firstIndexFrom;
            for (Size_t i = 0; i < count; ++i)
            {
                new(pTo + i) T(*(pFrom+i));
            }
        }
        static FnDataProcessor2<TSize> Get()
        {
            return &Copy;
        }
    };

    ////////////////////////////////////////////////////////////////////////////////
    template<typename TSize, typename T, bool THasMoveConstructor>
    struct GetMoveConstructor
    {
    public:
        static FnDataProcessor<TSize> Get() { return nullptr; }
    };
    template<typename TSize, typename T>
    struct GetMoveConstructor<TSize, T, true>
    {
    public:
        using Size_t = TSize;
        static void Move(void* const baseTo, void* const baseFrom, const TSize firstIndexTo, const TSize firstIndexFrom, const TSize count)
        {
            T* pTo = (T*)baseTo + firstIndexTo;
            T* pFrom = (T*)baseFrom + firstIndexFrom;
            for (Size_t i = 0; i < count; ++i)
            {
                new(pTo + i) T(std::move(*(pFrom + i)));
            }
        }
        static FnDataProcessor2<TSize> Get()
        {
            return &Move;
        }
    };

    ////////////////////////////////////////////////////////////////////////////////
    template<typename TSize, typename T, bool TIsSwappable>
    struct GetSwapper
    {
    public:
        static FnDataProcessor<TSize> Get() { return nullptr; }
    };
    template<typename TSize, typename T>
    struct GetSwapper<TSize, T, true>
    {
    public:
        using Size_t = TSize;
        static void Swap(void* const baseTo, void* const baseFrom, const TSize firstIndexTo, const TSize firstIndexFrom, const TSize count)
        {
            T* pTo = (T*)baseTo + firstIndexTo;
            T* pFrom = (T*)baseFrom + firstIndexFrom;
            std::swap_ranges(pTo, pTo + count, pFrom);
        }
        static FnDataProcessor2<TSize> Get()
        {
            return &Swap;
        }
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
        FnDataProcessor<Size_t> NodeConstruct;
        FnDataProcessor2<Size_t> NodeCopy;
        FnDataProcessor2<Size_t> NodeMove;
        FnDataProcessor2<Size_t> NodeSwap;
        FnDataProcessor<Size_t> NodeDestruct;
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
            assert_pnc(TypeInfo != nullptr);
            assert_pnc(Size > 0);
            assert_pnc(Align > 0);
            assert_pnc(Owner >= ComponentOwner__Begin && Owner < ComponentOwner__End);
        }

        /// <summary>
        /// Create a ComponentType from a typename.
        /// </summary>
        /// <typeparam name="T">The component typename</typeparam>
        /// <param name="_nullptr">Should always be equal to (const T*)nullptr. Provides a way to specify the component typename argument.</param>
        /// <param name="aOwner">Owner for this component type.</param>
        template<typename T>
        ComponentTypeT(const T* const _nullptr, const ComponentOwner owner)
            : TypeInfo(&typeid(T))
            , Size(sizeof(T))
            , Align(alignof(T))
            , Owner(owner)
        {
            assert_pnc(_nullptr == nullptr);
            assert_pnc(owner >= ComponentOwner__Begin && owner < ComponentOwner__End);

            NodeConstruct = GetDefaultConstructor<Size_t, T, std::is_default_constructible<T>::value>::Get();
            NodeCopy = GetCopyConstructor<Size_t, T, std::is_copy_constructible<T>::value>::Get();
            NodeMove = GetMoveConstructor<Size_t, T, std::is_move_constructible<T>::value>::Get();
            NodeSwap = GetSwapper<Size_t, T, std::is_swappable<T>::value>::Get();
            NodeDestruct = GetDestructor<Size_t, T, std::is_destructible<T>::value>::Get();
        }

    public:
        Size_t GetSize()const { return Size; }
        Size_t GetSize(const Size_t nodeCount, const Size_t chunkCount = 1)const { return Size * GetComponentCount(nodeCount, chunkCount); }
        Size_t GetAlignment()const { return Align; }
        ComponentOwner GetOwner()const { return Owner; }
        const type_info* GetTypeInfo()const { return TypeInfo; }

        bool IsNodeComponent()const { return Owner == ComponentOwner_Node; }
        bool IsChunkComponent()const { return Owner == ComponentOwner_Chunk; }
        bool IsUserConstructible()const { return !!NodeConstruct; }
        bool IsUserCopyable()const { return !!NodeCopy; }
        bool IsUserMovable()const { return !!NodeMove; }
        bool IsUserSwappable()const { return !!NodeSwap; }
        bool IsUserDestructible()const { return !!NodeDestruct; }

        //// TODO Remove
        //void* Allocate(const Size_t nodeCount, const Size_t chunkCount = 1)const
        //{
        //    auto count = GetComponentCount(nodeCount, chunkCount) ;
        //    return pnc_alloc(Size * count, Align);
        //}

        //// TODO Remove
        //void* AllocateConstruct(const Size_t nodeCount, const Size_t chunkCount = 1)const
        //{
        //    auto ptr = Allocate(nodeCount, chunkCount);
        //    ConstructNodesUnsafe(ptr, 0, nodeCount, chunkCount);
        //    return ptr;
        //}

        //// TODO Remove
        //void* AllocateCopy(void* const from, const Size_t nodeCapacity, const Size_t nodeCount, const Size_t chunkCapacity = 1, const Size_t chunkCount = 1)const
        //{
        //    auto ptr = Allocate(nodeCapacity, chunkCapacity);
        //    CopyNodes(ptr, from, 0, 0, nodeCount, chunkCount);
        //    return ptr;
        //}

        //// TODO Remove
        //void Deallocate(void* const ptr, const Size_t nodeCapacity, const Size_t chunkCapacity = 1)const
        //{
        //    pnc_free(ptr, GetComponentCount(nodeCapacity, chunkCapacity) * Size, Align);
        //}


        //void ConstructNodesUnsafe(void* const baseComponentData, const Size_t first, const Size_t nodeCount, const Size_t chunkIndex = 0)const
        //{
        //    if (IsUserConstructible())
        //        UserConstructNodesUnsafe(baseComponentData, first, nodeCount, chunkIndex);
        //}

        //void DestructNodesUnsafe(void* const baseComponentData, const Size_t first, const Size_t nodeCount, const Size_t chunkIndex = 0)const
        //{
        //    if (IsUserDestructible())
        //        UserDestructNodesUnsafe(baseComponentData, first, nodeCount, chunkIndex);
        //}
        //void MoveNodesUnsafe(void* const baseTo, void* const baseFrom, const Size_t firstTo, const Size_t firstFrom, const Size_t nodeCount, const Size_t chunkIndexTo = 0, const Size_t chunkIndexFrom = 0)const
        //{
        //    if (IsUserMovable())
        //        UserMoveNodesUnsafe(baseTo, baseFrom, firstTo, firstFrom, nodeCount, chunkIndexTo, chunkIndexFrom);
        //    else
        //    {
        //        auto dataLength = GetComponentCount(nodeCount) * Size;
        //        memcpy_s((uint8*)baseTo + GetComponentIndex(firstTo, chunkIndexTo) * Size, dataLength, (uint8*)baseFrom + GetComponentIndex(firstFrom, chunkIndexFrom) * Size, dataLength);
        //    }
        //}

        //void SwapNodesUnsafe(void* const baseTo, void* const baseFrom, const Size_t firstTo, const Size_t firstFrom, const Size_t nodeCount, const Size_t chunkIndexTo = 0, const Size_t chunkIndexFrom = 0)const
        //{
        //    if (IsUserSwappable())
        //        UserSwapNodesUnsafe(baseTo, baseFrom, firstTo, firstFrom, nodeCount, chunkIndexTo, chunkIndexFrom);
        //    else
        //    {
        //        auto dataCount = GetComponentCount(nodeCount) * Size;
        //        auto to = (uint8*)baseTo + GetComponentIndex(firstTo, chunkIndexTo) * Size;
        //        std::swap_ranges(to, to + dataCount, (uint8*)baseFrom + GetComponentIndex(firstFrom, chunkIndexFrom) * Size);
        //    }
        //}

        //void UserConstructNodesUnsafe(void* const baseComponentData, const Size_t first, const Size_t nodeCount, const Size_t chunkCount = 1)const
        //{
        //    NodeConstruct((uint8*)baseComponentData +  (first, chunkCount) * Size, GetComponentCount(nodeCount));
        //}
        //void UserDestructNodesUnsafe(void* const baseComponentData, const Size_t first, const Size_t nodeCount, const Size_t chunkIndex = 0)const
        //{
        //    NodeDestruct((uint8*)baseComponentData + GetComponentIndex(first, chunkIndex) * Size, GetComponentCount(nodeCount));
        //}

        //void UserCopyNodesUnsafe(void* const baseTo, void* const baseFrom, const Size_t firstTo, const Size_t firstFrom, const Size_t nodeCount, const Size_t chunkIndexTo = 0, const Size_t chunkIndexFrom = 0)const
        //{
        //    NodeCopy((uint8*)baseTo + GetComponentIndex(firstTo, chunkIndexTo) * Size, (uint8*)baseFrom + GetComponentIndex(firstFrom, chunkIndexFrom) * Size, GetComponentCount(nodeCount));
        //}

        //void UserMoveNodesUnsafe(void* const baseTo, void* const baseFrom, const Size_t firstTo, const Size_t firstFrom, const Size_t nodeCount, const Size_t chunkIndexTo = 0, const Size_t chunkIndexFrom = 0)const
        //{
        //    NodeMove((uint8*)baseTo + GetComponentIndex(firstTo, chunkIndexTo) * Size, (uint8*)baseFrom + GetComponentIndex(firstFrom, chunkIndexFrom) * Size, GetComponentCount(nodeCount));
        //}

        //void UserSwapNodesUnsafe(void* const baseTo, void* const baseFrom, const Size_t firstTo, const Size_t firstFrom, const Size_t nodeCount, const Size_t chunkIndexTo = 0, const Size_t chunkIndexFrom = 0)const
        //{
        //    NodeSwap((uint8*)baseTo + GetComponentIndex(firstTo, chunkIndexTo) * Size, (uint8*)baseFrom + GetComponentIndex(firstFrom, chunkIndexFrom) * Size, GetComponentCount(nodeCount));
        //}




        //void UserConstructComponentUnsafe(void* const baseComponentData, const Size_t firstIndex, const Size_t count)const { NodeConstruct((uint8*)baseComponentData + firstIndex * Size, count); }
        //void UserDestructComponentUnsafe (void* const baseComponentData, const Size_t firstIndex, const Size_t count)const { NodeDestruct((uint8*)baseComponentData + firstIndex * Size, count); }
        //void UserCopyComponentUnsafe(void* const baseTo, void* const baseFrom, const Size_t firstIndexTo, const Size_t firstIndexFrom, const Size_t count)const { NodeCopy((uint8*)baseTo + firstIndexTo * Size, (uint8*)baseFrom + firstIndexFrom * Size, count); }
        //void UserMoveComponentUnsafe(void* const baseTo, void* const baseFrom, const Size_t firstIndexTo, const Size_t firstIndexFrom, const Size_t count)const { NodeMove((uint8*)baseTo + firstIndexTo * Size, (uint8*)baseFrom + firstIndexFrom * Size, count); }
        //void UserSwapComponentUnsafe(void* const baseTo, void* const baseFrom, const Size_t firstIndexTo, const Size_t firstIndexFrom, const Size_t count)const { NodeSwap((uint8*)baseTo + firstIndexTo * Size, (uint8*)baseFrom + firstIndexFrom * Size, count); }


        void ConstructComponentDataUnsafe(void* const baseComponentData, const Size_t firstComponentIndex, const Size_t count)const
        {
            assert_pnc(!!baseComponentData);
            assert_pnc(firstComponentIndex >= 0);
            assert_pnc(count >= 0);
            if (IsUserConstructible())
                NodeConstruct(baseComponentData, firstComponentIndex, count);
        }

        void DestructComponentDataUnsafe(void* const baseComponentData, const Size_t firstComponentIndex, const Size_t count)const
        {
            assert_pnc(!!baseComponentData);
            assert_pnc(firstComponentIndex >= 0);
            assert_pnc(count >= 0);
            if (IsUserDestructible())
                NodeDestruct(baseComponentData, firstComponentIndex, count);
        }

        void CopyComponentDataUnsafe(void* const baseComponentDataTo, void* const baseComponentDataFrom, const Size_t firstComponentIndexTo, const Size_t firstComponentIndexFrom, const Size_t count)const
        {
            assert_pnc(!!baseComponentDataTo);
            assert_pnc(!!baseComponentDataFrom);
            assert_pnc(firstComponentIndexTo >= 0);
            assert_pnc(firstComponentIndexFrom >= 0);
            assert_pnc(count >= 0);
            if (IsUserCopyable())
                NodeCopy(baseComponentDataTo, baseComponentDataFrom, firstComponentIndexTo, firstComponentIndexFrom, count);
            else
            {
                auto dataLength = count * Size;
                memcpy_s((uint8*)baseComponentDataTo + firstComponentIndexTo * Size, dataLength, (uint8*)baseComponentDataFrom + firstComponentIndexFrom * Size, dataLength);
            }
        }

        void MoveComponentDataUnsafe(void* const baseComponentDataTo, void* const baseComponentDataFrom, const Size_t firstComponentIndexTo, const Size_t firstComponentIndexFrom, const Size_t count)const
        {
            assert_pnc(!!baseComponentDataTo);
            assert_pnc(!!baseComponentDataFrom);
            assert_pnc(firstComponentIndexTo >= 0);
            assert_pnc(firstComponentIndexFrom >= 0);
            assert_pnc(count >= 0);
            if (IsUserMovable())
                NodeMove(baseComponentDataTo, baseComponentDataFrom, firstComponentIndexTo, firstComponentIndexFrom, count);
            else
            {
                auto dataLength = count * Size;
                memcpy_s((uint8*)baseComponentDataTo + firstComponentIndexTo * Size, dataLength, (uint8*)baseComponentDataFrom + firstComponentIndexFrom * Size, dataLength);
            }
        }

        void SwapComponentDataUnsafe(void* const baseComponentDataTo, void* const baseComponentDataFrom, const Size_t firstComponentIndexTo, const Size_t firstComponentIndexFrom, const Size_t count)const
        {
            assert_pnc(!!baseComponentDataTo);
            assert_pnc(!!baseComponentDataFrom);
            assert_pnc(firstComponentIndexTo >= 0);
            assert_pnc(firstComponentIndexFrom >= 0);
            assert_pnc(count >= 0);
            if (IsUserSwappable())
                NodeSwap(baseComponentDataTo, baseComponentDataFrom, firstComponentIndexTo, firstComponentIndexFrom, count);
            else
            {
                auto dataCount = GetComponentCount(count) * Size;
                auto to = (uint8*)baseComponentDataTo + firstComponentIndexTo * Size;
                auto from = (uint8*)baseComponentDataFrom + firstComponentIndexFrom * Size;
                std::swap_ranges(to, to + dataCount, from);
            }
        }



        void ConstructComponentUnsafe(void* const baseComponentData,
            const Size_t firstNodeIndex, const Size_t nodeCount,
            const Size_t firstChunkIndex = 0, const Size_t chunkCount = 1)const
        {
            ConstructComponentDataUnsafe(baseComponentData, 
                GetComponentIndex(firstNodeIndex, firstChunkIndex), 
                GetComponentCount(nodeCount, chunkCount));
        }

        void DestructComponentUnsafe(void* const baseComponentData,
            const Size_t firstNodeIndex, const Size_t nodeCount,
            const Size_t firstChunkIndex = 0, const Size_t chunkCount = 1)const
        {
            DestructComponentDataUnsafe(baseComponentData, 
                GetComponentIndex(firstNodeIndex, firstChunkIndex), 
                GetComponentCount(nodeCount, chunkCount));
        }


        void CopyComponentUnsafe(void* const baseComponentDataTo, void* const baseComponentDataFrom,
            const Size_t firstNodeIndexTo, const Size_t firstNodeIndexFrom, const Size_t nodeCount,
            const Size_t firstChunkIndexTo = 0, const Size_t firstChunkIndexFrom = 0, const Size_t chunkCount = 1)const
        {
            CopyComponentDataUnsafe(baseComponentDataTo, baseComponentDataFrom, 
                GetComponentIndex(firstNodeIndexTo, firstChunkIndexTo), 
                GetComponentIndex(firstNodeIndexFrom, firstChunkIndexFrom),
                GetComponentCount(nodeCount, chunkCount));
        }

        void MoveComponentUnsafe(void* const baseComponentDataTo, void* const baseComponentDataFrom,
            const Size_t firstNodeIndexTo, const Size_t firstNodeIndexFrom, const Size_t nodeCount,
            const Size_t firstChunkIndexTo = 0, const Size_t firstChunkIndexFrom = 0, const Size_t chunkCount = 1)const
        {
            MoveComponentDataUnsafe(baseComponentDataTo, baseComponentDataFrom, 
                GetComponentIndex(firstNodeIndexTo, firstChunkIndexTo),
                GetComponentIndex(firstNodeIndexFrom, firstChunkIndexFrom),
                GetComponentCount(nodeCount, chunkCount));
        }

        void SwapComponentUnsafe(void* const baseComponentDataTo, void* const baseComponentDataFrom,
            const Size_t firstNodeIndexTo, const Size_t firstNodeIndexFrom, const Size_t nodeCount,
            const Size_t firstChunkIndexTo = 0, const Size_t firstChunkIndexFrom = 0, const Size_t chunkCount = 1)const
        {
            SwapComponentDataUnsafe(baseComponentDataTo, baseComponentDataFrom, 
                GetComponentIndex(firstNodeIndexTo, firstChunkIndexTo),
                GetComponentIndex(firstNodeIndexFrom, firstChunkIndexFrom),
                GetComponentCount(nodeCount, chunkCount));
        }


        ///// <summary>
        ///// Copy component data from one chunk of memory to another.
        ///// </summary>
        ///// <param name="to">destination memory</param>
        ///// <param name="from">source memory</param>
        ///// <param name="nodeCount">How many component instances to copy</param>
        //void Copy(void* const to, void const* from, const Size_t nodeCount, const Size_t chunkCapacity = 1)const
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
        //        checkNoEntry();
        //        return -1;
        //    }
        //}

        void* Forward(void* const ptr, const Size_t nodeCount, const Size_t chunkCount = 1)const
        {
            return (uint8*)ptr + GetComponentCount(nodeCount, chunkCount) * Size;
        }
        void* Backward(void* const ptr, const Size_t nodeCount, const Size_t chunkCount = 1)const
        {
            return (uint8*)ptr - GetComponentCount(nodeCount, chunkCount) * Size;
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
                checkNoEntry();
                return -1;
            }
        }
        Size_t GetComponentCount(const Size_t nodeCount, const Size_t chunkCount = 1)const
        {
            return GetComponentIndex(nodeCount, chunkCount);
        }
    };
}
