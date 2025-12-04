// MIT License
// Copyright (c) 2025 Stephanie Rancourt

#pragma once

namespace PNC
{
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
        IntProp(const TValue& value)
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
    template<typename TValue, PropId TId> IntProp<TValue, TId> operator+(const TValue& a, const IntProp<TValue, TId>& b) { return a + b.Value; }
    template<typename TValue, PropId TId> IntProp<TValue, TId> operator-(const TValue& a, const IntProp<TValue, TId>& b) { return a - b.Value; }
    template<typename TValue, PropId TId> IntProp<TValue, TId> operator*(const TValue& a, const IntProp<TValue, TId>& b) { return a * b.Value; }
    template<typename TValue, PropId TId> IntProp<TValue, TId> operator/(const TValue& a, const IntProp<TValue, TId>& b) { return a / b.Value; }
    template<typename TValue, PropId TId> IntProp<TValue, TId> operator%(const TValue& a, const IntProp<TValue, TId>& b) { return a % b.Value; }
    template<typename TValue, PropId TId> IntProp<TValue, TId> operator&(const TValue& a, const IntProp<TValue, TId>& b) { return a & b.Value; }
    template<typename TValue, PropId TId> IntProp<TValue, TId> operator|(const TValue& a, const IntProp<TValue, TId>& b) { return a | b.Value; }
    template<typename TValue, PropId TId> IntProp<TValue, TId> operator^(const TValue& a, const IntProp<TValue, TId>& b) { return a ^ b.Value; }
    template<typename TValue, PropId TId> IntProp<TValue, TId> operator<<(const TValue& a, const IntProp<TValue, TId>& b) { return a << b.Value; }
    template<typename TValue, PropId TId> IntProp<TValue, TId> operator>>(const TValue& a, const IntProp<TValue, TId>& b) { return a >> b.Value; }
    template<typename TValue, PropId TId> IntProp<TValue, TId> operator&&(const TValue& a, const IntProp<TValue, TId>& b) { return a && b.Value; }
    template<typename TValue, PropId TId> IntProp<TValue, TId> operator||(const TValue& a, const IntProp<TValue, TId>& b) { return a || b.Value; }
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

    template<typename TSize> NodeCountT<TSize> PropNodeCount(const NodeCountT<TSize> a) { return a; }
    template<typename TSize> NodeCapacityT<TSize> PropNodeCapacity(const NodeCapacityT<TSize> a) { return a; }
    template<typename TSize> ChunkCountT<TSize> PropChunkCount(const ChunkCountT<TSize> a) { return a; }
    template<typename TSize> ChunkCapacityT<TSize> PropChunkCapacity(const ChunkCapacityT<TSize> a) { return a; }

    template<typename TSize> NodeCountT<TSize> PropNodeCount(const int a) { return NodeCountT<TSize>(a); }
    template<typename TSize> NodeCapacityT<TSize> PropNodeCapacity(const int a) { return NodeCapacityT<TSize>(a); }
    template<typename TSize> ChunkCountT<TSize> PropChunkCount(const int a) { return ChunkCountT<TSize>(a); }
    template<typename TSize> ChunkCapacityT<TSize> PropChunkCapacity(const int a) { return ChunkCapacityT<TSize>(a); }
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

        StructurePtr(const ChunkStructure_t* const structurePtr)
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
    struct PropTraits<void**> : public PropTraitsDefault<DComponentDataArray> {};
    template<>
    struct PropTraits<PropComponentDataArray> : public PropTraitsDefault<DComponentDataArray> {};

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