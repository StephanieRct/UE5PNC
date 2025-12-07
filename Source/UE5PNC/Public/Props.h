// MIT License
// Copyright (c) 2025 Stephanie Rancourt

#pragma once

namespace NiT
{
    struct DPropsTag {};

    template<typename TSize>
    struct DProps
    {
        using Size_t = TSize;
    };

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
        // ComponentTypeIndexInChunk (rename to LocalComponentTypeIndex)
        // LocalNodeIndex
    };
    template<typename TValue, PropId TId >
    struct IntProp
    {
        using Base_t = void;
        using Self_t = IntProp<TValue, TId>;
        using Value_t = TValue;

        TValue Value;
        explicit IntProp(const TValue& value)
            :Value(value)
        {
        }
        operator TValue()const
        {
            return Value;
        }
        static constexpr Self_t V_0() { return Self_t((TValue)0); }
        static constexpr Self_t V_1() { return Self_t((TValue)1); }
        Self_t& operator++() { ++Value; return *this; }
        Self_t& operator--() { --Value; return *this; }
        Self_t operator++(int) { return Self_t(Value++); }
        Self_t operator--(int) { return Self_t(Value--); }
        Self_t operator~() { return Self_t(~Value); }
        Self_t operator!() { return Self_t(!Value); }
    };

    template<typename TValue, PropId TId> std::strong_ordering operator<=>(const IntProp<TValue, TId>& a, const TValue& b) { return a.Value <=> b; }
    template<typename TValue, PropId TId> IntProp<TValue, TId> operator+(  const IntProp<TValue, TId>& a, const TValue& b) { return IntProp<TValue, TId>(a.Value +   b); }
    template<typename TValue, PropId TId> IntProp<TValue, TId> operator-(  const IntProp<TValue, TId>& a, const TValue& b) { return IntProp<TValue, TId>(a.Value -   b); }
    template<typename TValue, PropId TId> IntProp<TValue, TId> operator*(  const IntProp<TValue, TId>& a, const TValue& b) { return IntProp<TValue, TId>(a.Value *   b); }
    template<typename TValue, PropId TId> IntProp<TValue, TId> operator/(  const IntProp<TValue, TId>& a, const TValue& b) { return IntProp<TValue, TId>(a.Value /   b); }
    template<typename TValue, PropId TId> IntProp<TValue, TId> operator%(  const IntProp<TValue, TId>& a, const TValue& b) { return IntProp<TValue, TId>(a.Value %   b); }
    template<typename TValue, PropId TId> IntProp<TValue, TId> operator&(  const IntProp<TValue, TId>& a, const TValue& b) { return IntProp<TValue, TId>(a.Value &   b); }
    template<typename TValue, PropId TId> IntProp<TValue, TId> operator|(  const IntProp<TValue, TId>& a, const TValue& b) { return IntProp<TValue, TId>(a.Value |   b); }
    template<typename TValue, PropId TId> IntProp<TValue, TId> operator^(  const IntProp<TValue, TId>& a, const TValue& b) { return IntProp<TValue, TId>(a.Value ^   b); }
    template<typename TValue, PropId TId> IntProp<TValue, TId> operator<<( const IntProp<TValue, TId>& a, const TValue& b) { return IntProp<TValue, TId>(a.Value <<  b); }
    template<typename TValue, PropId TId> IntProp<TValue, TId> operator>>( const IntProp<TValue, TId>& a, const TValue& b) { return IntProp<TValue, TId>(a.Value >>  b); }
    template<typename TValue, PropId TId> IntProp<TValue, TId> operator&&( const IntProp<TValue, TId>& a, const TValue& b) { return IntProp<TValue, TId>(a.Value &&  b); }
    template<typename TValue, PropId TId> IntProp<TValue, TId> operator||( const IntProp<TValue, TId>& a, const TValue& b) { return IntProp<TValue, TId>(a.Value ||  b); }
    template<typename TValue, PropId TId> IntProp<TValue, TId>& operator+=( IntProp<TValue, TId>& a, const TValue& b) { a.Value +=  b; return a; }
    template<typename TValue, PropId TId> IntProp<TValue, TId>& operator-=( IntProp<TValue, TId>& a, const TValue& b) { a.Value -=  b; return a; }
    template<typename TValue, PropId TId> IntProp<TValue, TId>& operator*=( IntProp<TValue, TId>& a, const TValue& b) { a.Value *=  b; return a; }
    template<typename TValue, PropId TId> IntProp<TValue, TId>& operator/=( IntProp<TValue, TId>& a, const TValue& b) { a.Value /=  b; return a; }
    template<typename TValue, PropId TId> IntProp<TValue, TId>& operator%=( IntProp<TValue, TId>& a, const TValue& b) { a.Value %=  b; return a; }
    template<typename TValue, PropId TId> IntProp<TValue, TId>& operator&=( IntProp<TValue, TId>& a, const TValue& b) { a.Value &=  b; return a; }
    template<typename TValue, PropId TId> IntProp<TValue, TId>& operator|=( IntProp<TValue, TId>& a, const TValue& b) { a.Value |=  b; return a; }
    template<typename TValue, PropId TId> IntProp<TValue, TId>& operator^=( IntProp<TValue, TId>& a, const TValue& b) { a.Value ^=  b; return a; }
    template<typename TValue, PropId TId> IntProp<TValue, TId>& operator<<=(IntProp<TValue, TId>& a, const TValue& b) { a.Value <<= b; return a; }
    template<typename TValue, PropId TId> IntProp<TValue, TId>& operator>>=(IntProp<TValue, TId>& a, const TValue& b) { a.Value >>= b; return a; }
    template<typename TValue, PropId TId> std::strong_ordering operator<=>(const TValue& a, const IntProp<TValue, TId>& b) { return a <=> b.Value; }
    template<typename TValue, PropId TId> IntProp<TValue, TId> operator+(  const TValue& a, const IntProp<TValue, TId>& b) { return IntProp<TValue, TId>(a +   b.Value); }
    template<typename TValue, PropId TId> IntProp<TValue, TId> operator-(  const TValue& a, const IntProp<TValue, TId>& b) { return IntProp<TValue, TId>(a -   b.Value); }
    template<typename TValue, PropId TId> IntProp<TValue, TId> operator*(  const TValue& a, const IntProp<TValue, TId>& b) { return IntProp<TValue, TId>(a *   b.Value); }
    template<typename TValue, PropId TId> IntProp<TValue, TId> operator/(  const TValue& a, const IntProp<TValue, TId>& b) { return IntProp<TValue, TId>(a /   b.Value); }
    template<typename TValue, PropId TId> IntProp<TValue, TId> operator%(  const TValue& a, const IntProp<TValue, TId>& b) { return IntProp<TValue, TId>(a %   b.Value); }
    template<typename TValue, PropId TId> IntProp<TValue, TId> operator&(  const TValue& a, const IntProp<TValue, TId>& b) { return IntProp<TValue, TId>(a &   b.Value); }
    template<typename TValue, PropId TId> IntProp<TValue, TId> operator|(  const TValue& a, const IntProp<TValue, TId>& b) { return IntProp<TValue, TId>(a |   b.Value); }
    template<typename TValue, PropId TId> IntProp<TValue, TId> operator^(  const TValue& a, const IntProp<TValue, TId>& b) { return IntProp<TValue, TId>(a ^   b.Value); }
    template<typename TValue, PropId TId> IntProp<TValue, TId> operator<<( const TValue& a, const IntProp<TValue, TId>& b) { return IntProp<TValue, TId>(a <<  b.Value); }
    template<typename TValue, PropId TId> IntProp<TValue, TId> operator>>( const TValue& a, const IntProp<TValue, TId>& b) { return IntProp<TValue, TId>(a >>  b.Value); }
    template<typename TValue, PropId TId> IntProp<TValue, TId> operator&&( const TValue& a, const IntProp<TValue, TId>& b) { return IntProp<TValue, TId>(a &&  b.Value); }
    template<typename TValue, PropId TId> IntProp<TValue, TId> operator||( const TValue& a, const IntProp<TValue, TId>& b) { return IntProp<TValue, TId>(a ||  b.Value); }
    template<typename TValue, PropId TId> TValue& operator+=( TValue& a, const IntProp<TValue, TId>& b) { a +=  b.Value; return a; }
    template<typename TValue, PropId TId> TValue& operator-=( TValue& a, const IntProp<TValue, TId>& b) { a -=  b.Value; return a; }
    template<typename TValue, PropId TId> TValue& operator*=( TValue& a, const IntProp<TValue, TId>& b) { a *=  b.Value; return a; }
    template<typename TValue, PropId TId> TValue& operator/=( TValue& a, const IntProp<TValue, TId>& b) { a /=  b.Value; return a; }
    template<typename TValue, PropId TId> TValue& operator%=( TValue& a, const IntProp<TValue, TId>& b) { a %=  b.Value; return a; }
    template<typename TValue, PropId TId> TValue& operator&=( TValue& a, const IntProp<TValue, TId>& b) { a &=  b.Value; return a; }
    template<typename TValue, PropId TId> TValue& operator|=( TValue& a, const IntProp<TValue, TId>& b) { a |=  b.Value; return a; }
    template<typename TValue, PropId TId> TValue& operator^=( TValue& a, const IntProp<TValue, TId>& b) { a ^=  b.Value; return a; }
    template<typename TValue, PropId TId> TValue& operator<<=(TValue& a, const IntProp<TValue, TId>& b) { a <<= b.Value; return a; }
    template<typename TValue, PropId TId> TValue& operator>>=(TValue& a, const IntProp<TValue, TId>& b) { a >>= b.Value; return a; }
    template<typename TValue, PropId TId> std::strong_ordering  operator<=>(const IntProp<TValue, TId>& a, const IntProp<TValue, TId>& b) { return a.Value <=> b.Value; }
    template<typename TValue, PropId TId> IntProp<TValue, TId>  operator+(  const IntProp<TValue, TId>& a, const IntProp<TValue, TId>& b) { return IntProp<TValue, TId>(a.Value +   b.Value); }
    template<typename TValue, PropId TId> IntProp<TValue, TId>  operator-(  const IntProp<TValue, TId>& a, const IntProp<TValue, TId>& b) { return IntProp<TValue, TId>(a.Value -   b.Value); }
    template<typename TValue, PropId TId> IntProp<TValue, TId>  operator*(  const IntProp<TValue, TId>& a, const IntProp<TValue, TId>& b) { return IntProp<TValue, TId>(a.Value *   b.Value); }
    template<typename TValue, PropId TId> IntProp<TValue, TId>  operator/(  const IntProp<TValue, TId>& a, const IntProp<TValue, TId>& b) { return IntProp<TValue, TId>(a.Value /   b.Value); }
    template<typename TValue, PropId TId> IntProp<TValue, TId>  operator%(  const IntProp<TValue, TId>& a, const IntProp<TValue, TId>& b) { return IntProp<TValue, TId>(a.Value %   b.Value); }
    template<typename TValue, PropId TId> IntProp<TValue, TId>  operator&(  const IntProp<TValue, TId>& a, const IntProp<TValue, TId>& b) { return IntProp<TValue, TId>(a.Value &   b.Value); }
    template<typename TValue, PropId TId> IntProp<TValue, TId>  operator|(  const IntProp<TValue, TId>& a, const IntProp<TValue, TId>& b) { return IntProp<TValue, TId>(a.Value |   b.Value); }
    template<typename TValue, PropId TId> IntProp<TValue, TId>  operator^(  const IntProp<TValue, TId>& a, const IntProp<TValue, TId>& b) { return IntProp<TValue, TId>(a.Value ^   b.Value); }
    template<typename TValue, PropId TId> IntProp<TValue, TId>  operator<<( const IntProp<TValue, TId>& a, const IntProp<TValue, TId>& b) { return IntProp<TValue, TId>(a.Value <<  b.Value); }
    template<typename TValue, PropId TId> IntProp<TValue, TId>  operator>>( const IntProp<TValue, TId>& a, const IntProp<TValue, TId>& b) { return IntProp<TValue, TId>(a.Value >>  b.Value); }
    template<typename TValue, PropId TId> IntProp<TValue, TId>  operator&&( const IntProp<TValue, TId>& a, const IntProp<TValue, TId>& b) { return IntProp<TValue, TId>(a.Value &&  b.Value); }
    template<typename TValue, PropId TId> IntProp<TValue, TId>  operator||( const IntProp<TValue, TId>& a, const IntProp<TValue, TId>& b) { return IntProp<TValue, TId>(a.Value ||  b.Value); }
    template<typename TValue, PropId TId> IntProp<TValue, TId>& operator+=( IntProp<TValue, TId>& a, const IntProp<TValue, TId>& b) { a.Value +=  b.Value; return a; }
    template<typename TValue, PropId TId> IntProp<TValue, TId>& operator-=( IntProp<TValue, TId>& a, const IntProp<TValue, TId>& b) { a.Value -=  b.Value; return a; }
    template<typename TValue, PropId TId> IntProp<TValue, TId>& operator*=( IntProp<TValue, TId>& a, const IntProp<TValue, TId>& b) { a.Value *=  b.Value; return a; }
    template<typename TValue, PropId TId> IntProp<TValue, TId>& operator/=( IntProp<TValue, TId>& a, const IntProp<TValue, TId>& b) { a.Value /=  b.Value; return a; }
    template<typename TValue, PropId TId> IntProp<TValue, TId>& operator%=( IntProp<TValue, TId>& a, const IntProp<TValue, TId>& b) { a.Value %=  b.Value; return a; }
    template<typename TValue, PropId TId> IntProp<TValue, TId>& operator&=( IntProp<TValue, TId>& a, const IntProp<TValue, TId>& b) { a.Value &=  b.Value; return a; }
    template<typename TValue, PropId TId> IntProp<TValue, TId>& operator|=( IntProp<TValue, TId>& a, const IntProp<TValue, TId>& b) { a.Value |=  b.Value; return a; }
    template<typename TValue, PropId TId> IntProp<TValue, TId>& operator^=( IntProp<TValue, TId>& a, const IntProp<TValue, TId>& b) { a.Value ^=  b.Value; return a; }
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

    template<typename TSize> using NodeCountT =            IntProp<TSize, PropId::NodeCount>;
    template<typename TSize> using NodeCapacityT =         IntProp<TSize, PropId::NodeCapacity>;
    template<typename TSize> using ChunkCountT =           IntProp<TSize, PropId::ChunkCount>;
    template<typename TSize> using ChunkCapacityT =        IntProp<TSize, PropId::ChunkCapacity>;
    template<typename TSize> using NodeCountPerChunkT =    IntProp<TSize, PropId::NodeCountPerChunk>;
    template<typename TSize> using NodeCapacityPerChunkT = IntProp<TSize, PropId::NodeCapacityPerChunk>;

    struct PropComponentDataArray
    {
        void** const ComponentDataArray;
        void** GetComponentDataArray() const { return ComponentDataArray; }
        PropComponentDataArray(void** value) : ComponentDataArray(value) { }
        operator void** ()const { return ComponentDataArray; }
        void* operator->()const { return *ComponentDataArray; }
        void*& operator*()const { return *ComponentDataArray; }
    };



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




    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    template<typename TProps>
    struct DNodeCapacity : public TProps
    {
        using typename TProps::Size_t;
        using Value_t = NodeCapacityT<Size_t>;
        NodeCapacityT<Size_t> NodeCapacity;
        NodeCapacityT<Size_t> GetNodeCapacity() const { return NodeCapacity; }
        DNodeCapacity(const TProps& props, const NodeCapacityT<Size_t> nodeCapacity) : TProps(props), NodeCapacity(nodeCapacity) { }
    };
    template<typename TProps>
    struct DNodeCount : public TProps
    {
        using typename TProps::Size_t;
        using Value_t = NodeCountT<Size_t>;
        const NodeCountT<Size_t> NodeCount;
        NodeCountT<Size_t> GetNodeCount() const { return NodeCount; }
        DNodeCount(const TProps& props, const NodeCountT<Size_t>& nodeCount) : TProps(props), NodeCount(nodeCount) { }
    };
    template<typename TProps>
    struct DChunkCapacity : public TProps
    {
        using typename TProps::Size_t;
        using Value_t = ChunkCapacityT<Size_t>;
        ChunkCapacityT<Size_t> ChunkCapacity;
        ChunkCapacityT<Size_t> GetChunkCapacity() const { return ChunkCapacity; }
        DChunkCapacity(const TProps& props, const ChunkCapacityT<Size_t> chunkCapacity) : TProps(props), ChunkCapacity(chunkCapacity) { }
    };
    template<typename TProps>
    struct DChunkCount : public TProps
    {
        using typename TProps::Size_t;
        using Value_t = ChunkCountT<Size_t>;
        const ChunkCountT<Size_t> ChunkCount;
        ChunkCountT<Size_t> GetChunkCount() const { return ChunkCount; }
        DChunkCount(const TProps& props, const ChunkCountT<Size_t>& chunkCount) : TProps(props), ChunkCount(chunkCount) { }
    };
    template<typename TProps>
    struct DNodeCapacityPerChunk : public TProps
    {
        using typename TProps::Size_t;
        using Value_t = NodeCapacityPerChunkT<Size_t>;
        NodeCapacityPerChunkT<Size_t> NodeCapacityPerChunk;
        NodeCapacityPerChunkT<Size_t> GetNodeCapacityPerChunk() const { return NodeCapacityPerChunk; }
        DNodeCapacityPerChunk(const TProps& props, const NodeCapacityPerChunkT<Size_t> nodeCapacityPerChunk) : TProps(props), NodeCapacityPerChunk(nodeCapacityPerChunk) { }
    };
    template<typename TProps>
    struct DNodeCountPerChunk : public TProps
    {
        using typename TProps::Size_t;
        using Value_t = NodeCountPerChunkT<Size_t>;
        const NodeCountPerChunkT<Size_t> NodeCountPerChunk;
        NodeCountPerChunkT<Size_t> GetNodeCountPerChunk() const { return NodeCountPerChunk; }
        DNodeCountPerChunk(const TProps& props, const NodeCountPerChunkT<Size_t>& nodeCountPerChunk) : TProps(props), NodeCountPerChunk(nodeCountPerChunk) { }
    };
    template<typename TChunkStructure, typename TProps>
    struct DStructurePtr : public TProps
    {
        using typename TProps::Size_t;
        using Value_t = const TChunkStructure*;
        using ChunkStructure_t = TChunkStructure;
        const ChunkStructure_t* const ChunkStructure;
        const ChunkStructure_t* GetStructurePtr() const { return ChunkStructure; }
        DStructurePtr(const TProps& props, const ChunkStructure_t* const chunkStructure) : TProps(props), ChunkStructure(chunkStructure) { }
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

    template<              > struct PropTraits<void**>                       : public PropTraitsDefault<DComponentDataArray> {};
    template<              > struct PropTraits<PropComponentDataArray>       : public PropTraitsDefault<DComponentDataArray> {};
    template<typename TSize> struct PropTraits<NodeCountT<           TSize>> : public PropTraitsDefault<DNodeCount> {};
    template<typename TSize> struct PropTraits<NodeCapacityT<        TSize>> : public PropTraitsDefault<DNodeCapacity> {};
    template<typename TSize> struct PropTraits<ChunkCountT<          TSize>> : public PropTraitsDefault<DChunkCount> {};
    template<typename TSize> struct PropTraits<ChunkCapacityT<       TSize>> : public PropTraitsDefault<DChunkCapacity> {};
    template<typename TSize> struct PropTraits<NodeCountPerChunkT<   TSize>> : public PropTraitsDefault<DNodeCountPerChunk> {};
    template<typename TSize> struct PropTraits<NodeCapacityPerChunkT<TSize>> : public PropTraitsDefault<DNodeCapacityPerChunk> {};

    template<typename TSize> NodeCapacityT<        TSize> PropCountToCapacity(const NodeCountT<        TSize> nodeCount) {         return NodeCapacityT<        TSize>(nodeCount.Value); }
    template<typename TSize> ChunkCapacityT<       TSize> PropCountToCapacity(const ChunkCountT<       TSize> chunkCount) {        return ChunkCapacityT<       TSize>(chunkCount.Value); }
    template<typename TSize> NodeCapacityPerChunkT<TSize> PropCountToCapacity(const NodeCountPerChunkT<TSize> nodeCountPerChunk) { return NodeCapacityPerChunkT<TSize>(nodeCountPerChunk.Value); }

    namespace Selectors
    {
        template<typename T>
        struct SelectProp
        {
            using Value_t = T;
            template<typename TProp>
            static TProp GetProp(const T value)
            {
                return TProp(value);
            }
        };
        template<typename TProp>
        struct SelectPropDefault
        {
            using Prop_t = TProp;
            using Value_t = typename TProp::Value_t;
            template<typename TProp2>
            static TProp GetProp(const Value_t value)
            {
                return TProp(value);
            }
        };
        template<typename TSize> struct SelectProp<NodeCapacityT<        TSize>> : public SelectPropDefault<NodeCapacityT<        TSize>> {};
        template<typename TSize> struct SelectProp<NodeCountT<           TSize>> : public SelectPropDefault<NodeCountT<           TSize>> {};
        template<typename TSize> struct SelectProp<ChunkCapacityT<       TSize>> : public SelectPropDefault<ChunkCapacityT<       TSize>> {};
        template<typename TSize> struct SelectProp<ChunkCountT<          TSize>> : public SelectPropDefault<ChunkCountT<          TSize>> {};
        template<typename TSize> struct SelectProp<NodeCapacityPerChunkT<TSize>> : public SelectPropDefault<NodeCapacityPerChunkT<TSize>> {};
        template<typename TSize> struct SelectProp<NodeCountPerChunkT<   TSize>> : public SelectPropDefault<NodeCountPerChunkT<   TSize>> {};
    }
    template<typename TSize, typename TIn> NodeCapacityT<        TSize> PropNodeCapacityT(        const TIn& a) { return Selectors::SelectProp<TIn>::GetProp<NodeCapacityT<        TSize>>(a); }
    template<typename TSize, typename TIn> NodeCountT<           TSize> PropNodeCountT(           const TIn& a) { return Selectors::SelectProp<TIn>::GetProp<NodeCountT<           TSize>>(a); }
    template<typename TSize, typename TIn> ChunkCapacityT<       TSize> PropChunkCapacityT(       const TIn& a) { return Selectors::SelectProp<TIn>::GetProp<ChunkCapacityT<       TSize>>(a); }
    template<typename TSize, typename TIn> ChunkCountT<          TSize> PropChunkCountT(          const TIn& a) { return Selectors::SelectProp<TIn>::GetProp<ChunkCountT<          TSize>>(a); }
    template<typename TSize, typename TIn> NodeCapacityPerChunkT<TSize> PropNodeCapacityPerChunkT(const TIn& a) { return Selectors::SelectProp<TIn>::GetProp<NodeCapacityPerChunkT<TSize>>(a); }
    template<typename TSize, typename TIn> NodeCountPerChunkT<   TSize> PropNodeCountPerChunkT(   const TIn& a) { return Selectors::SelectProp<TIn>::GetProp<NodeCountPerChunkT<   TSize>>(a); }

    /// <summary>
    /// Append Prop decorator only if props doesn't have the decorator present.
    /// </summary>
    template<typename TProps, typename TProp>
    NI_DEBUG_NOINLINE constexpr auto AppendPropSingle(const TProps& props, const TProp& newProp)
    {
        return PropTraits<TProp>::DecorateSingle(props, newProp);
    }

    /// <summary>
    /// Begin a Props type. All Props types habe the base DProps<TSize>.
    /// </summary>
    template<typename TSize, typename TProp>
    constexpr auto BeginProps(const TProp& prop)
    {
        return PropTraits<TProp>::Decorate(DProps<TSize>(), prop);
    }

    /// <summary>
    /// Append a Prop decorator to Props type
    /// </summary>
    template<typename TProp, typename TProps>
    constexpr auto AppendProp(const TProps& props, const TProp& newProp)
    {
        return PropTraits<TProp>::Decorate(props, newProp);
    }

    /// <summary>
    /// Begin a Props type from a single prop
    /// </summary>
    template <typename TSize, typename T>
    constexpr auto CombinePropsUnfold(const T& prop) {
        return BeginProps<TSize>(prop);
    }

    /// <summary>
    /// Combine all props from a parameter pack
    /// </summary>
    template <typename TSize, typename T, typename... Rest>
    constexpr auto CombinePropsUnfold(const T& first, Rest&&... rest) {
        static_assert(!std::is_same_v<TSize, T>);
        return AppendProp(CombinePropsUnfold<TSize>(rest...), first);
    }

    /// <summary>
    /// Make a Props type from a parameter pack
    /// </summary>
    template<typename TSize, typename... TProps>
    constexpr auto MakeProps(TProps&&... props)
    {
        return CombinePropsUnfold<TSize>(props...);
    }
}
