// MIT License
// Copyright (c) 2025 Stephanie Rancourt

#pragma once
#include "common.h"
#include "ChunkPointerInternal.h"
#include "DChunkPointer.h"


namespace PNC
{
    template<typename TChunkStructure>
    using ChunkPointerT = DChunkPointer< ChunkPointerInternalT<TChunkStructure>, DStructure<TChunkStructure>>;


    /// <summary>
    /// A ChunkPointer points to a Chunk's Component data memory within a range of Nodes.
    /// A ChunkPointer is also a Chunk by itself and provides access to the Component data.
    /// Copying a ChunkPointer does not copy the data.
    /// </summary>
    ///// <typeparam name="TChunkStructure">Structure of the Chunk's Component data.</typeparam>
    //template<typename TChunkStructure>
    //struct ChunkPointerT : protected ChunkPointerInternalT<TChunkStructure>
    //{
    //public:
    //    using Base_t = ChunkPointerInternalT<TChunkStructure>;
    //    using Self_t = ChunkPointerT<TChunkStructure>;
    //    using ChunkStructure_t = TChunkStructure;
    //    using Base_t::Size_t;
    //    using Base_t::ComponentType_t;
    //    using Base_t::Node_t;

    //    /// <summary>
    //    /// Reinterprete_cast this object to this type to get read-write access to its private data fields
    //    /// Use with caution.
    //    /// </summary>
    //    using ChunkPointerInternal_t = ChunkPointerInternalT<TChunkStructure>;
    //    using ChunkPointer_t = Self_t;

    //    /// <summary>
    //    /// Chunk_t is the type a ChunkPointer points to
    //    /// </summary>
    //    using Chunk_t = Self_t;

    //public:
    //    using Base_t::Base_t;

    //    ChunkPointerT(Self_t&& o) = default;
    //    Self_t& operator=(Self_t&& o) = default;
    //    ChunkPointerT(const Self_t& o) = default;
    //    Self_t& operator=(const Self_t& o) = default;

    //protected:
    //    /// <summary>
    //    /// Create a StructNull Chunk with a node count.
    //    /// </summary>
    //    ChunkPointerT(const ChunkStructure_t* const chunkStructure, const Size_t nodeCount)
    //        : Base_t(chunkStructure, nodeCount)
    //    {
    //    }

    //    // used by ChunkCapacityAllocationT<ChunkPointerT>, ignores the capacity parameter
    //    ChunkPointerT(const ChunkStructure_t* const chunkStructure, const Size_t nodeCapacity, const Size_t nodeCount)
    //        : Base_t(chunkStructure, nodeCount)
    //    {
    //    }

    //public:
    //    PNC_USING_CHUNKPOINTERINTERNAL_INTERFACE();
    //    using Base_t::GetChunkCount;
    //    using Base_t::GetChunkCapacity;
    //    using Base_t::GetNodeCapacity;
    //    static bool IsSameStructure(const Self_t& a, const Self_t& b) { return Base_t::IsSameStructure(a, b); }
    //    static bool IsSameData(const Self_t& a, const Self_t& b) { return Base_t::IsSameData(a, b); }

    //    /// <summary>
    //    /// Create a null chunk without structure nor component data.
    //    /// </summary>
    //    /// <returns></returns>
    //    static Self_t Null() { return Self_t(); }

    //    PNC_IMPLEMENT_CHUNKPOINTER_SELFPOINTER();

    //public:
    //    /// <summary>
    //    /// Get the pointer to a component's memory array using the component type index in the ChunkStructure ComponentTypeSet.
    //    /// For components with ComponentOwner_Node, the array will be at least the length of the size of the chunk.
    //    /// For components with ComponentOwner_Chunk, the array will be of length 1.
    //    /// This is the fastest way to access the chunk's component data.
    //    /// </summary>
    //    /// <param name="componentIndexInChunk">index in the ChunkStructure::Components ComponentTypeSet</param>
    //    /// <returns>Pointer to the component memory array</returns>
    //    void* GetComponentData(const Size_t componentTypeIndexInChunk)
    //    {
    //        pnc_assert(!IsNull());
    //        return this->ComponentData[componentTypeIndexInChunk];
    //    }

    //    /// <summary>
    //    /// Get the const pointer to a component's memory array using the component type index in the ChunkStructure ComponentTypeSet.
    //    /// For components with ComponentOwner_Node, the array will be at least the length of the size of the chunk.
    //    /// For components with ComponentOwner_Chunk, the array will be of length 1.
    //    /// This is the fastest way to access the chunk's component data.
    //    /// </summary>
    //    /// <param name="componentTypeIndexInChunk">index in the ChunkStructure::Components ComponentTypeSet</param>
    //    /// <returns>Const pointer to the component memory array</returns>
    //    const void* GetComponentData(const Size_t componentTypeIndexInChunk)const
    //    {
    //        pnc_assert(!IsNull());
    //        return this->ComponentData[componentTypeIndexInChunk];
    //    }

    //    /// <summary>
    //    /// Get the pointer to a component's memory array using the component's type const type_info* from &typeid(ComponentTypename).
    //    /// For components with ComponentOwner_Node, the array will be at least the length of the size of the chunk.
    //    /// For components with ComponentOwner_Chunk, the array will be of length 1.
    //    /// This is a slow way to access the chunk's component data as it require to do a map lookup of the 
    //    /// component's const type_info* pointer to the component type index in the chunk's ChunkStructure ComponentTypeSet.
    //    /// </summary>
    //    /// <param name="componentType">const type_info* pointer obtained from &typeid(ComponentTypename)</param>
    //    /// <returns>Pointer to the component memory array</returns>
    //    void* GetComponentData(const type_info* const componentType)
    //    {
    //        pnc_assert(!IsNull());
    //        auto index = this->Structure->Components.GetComponentTypeIndexInChunk(componentType);
    //        if (index < 0)
    //            return nullptr;
    //        return this->GetComponentData(index);
    //    }

    //    /// <summary>
    //    /// Get the const pointer to a component's memory array using the component's type const type_info* from &typeid(ComponentTypename).
    //    /// For components with ComponentOwner_Node, the array will be at least the length of the size of the chunk.
    //    /// For components with ComponentOwner_Chunk, the array will be of length 1.
    //    /// This is a slow way to access the chunk's component data as it require to do a map lookup of the 
    //    /// component's const type_info* pointer to the component type index in the chunk's ChunkStructure ComponentTypeSet.
    //    /// </summary>
    //    /// <param name="componentType">const type_info* pointer obtained from &typeid(ComponentTypename)</param>
    //    /// <returns>Const pointer to the component memory array</returns>
    //    const void* GetComponentData(const type_info* const componentType)const
    //    {
    //        pnc_assert(!IsNull());
    //        auto index = this->Structure->Components.GetComponentTypeIndexInChunk(componentType);
    //        if (index < 0)
    //            return nullptr;
    //        return this->GetComponentData(index);
    //    }


    //    /// <summary>
    //    /// Get the pointer to a component's memory array using the component's typename.
    //    /// For components with ComponentOwner_Node, the array will be at least the length of the size of the chunk.
    //    /// For components with ComponentOwner_Chunk, the array will be of length 1.
    //    /// This is a slow way to access the chunk's component data as it require to do a map lookup of the associated
    //    /// component typename const type_info* pointer to the component type index in the chunk's ChunkStructure ComponentTypeSet.
    //    /// </summary>
    //    /// <typeparam name="TComponent">Component typename for the desired component's memory array</typeparam>
    //    /// <returns>Pointer to the component memory array</returns>
    //    template<typename TComponent>
    //    TComponent* GetComponentData()
    //    {
    //        pnc_assert(!IsNull());
    //        return (TComponent*)this->GetComponentData(&typeid(TComponent));
    //    }

    //    /// <summary>
    //    /// Get the const pointer to a component's memory array using the component's typename.
    //    /// For components with ComponentOwner_Node, the array will be at least the length of the size of the chunk.
    //    /// For components with ComponentOwner_Chunk, the array will be of length 1.
    //    /// This is a slow way to access the chunk's component data as it require to do a map lookup of the associated
    //    /// component typename const type_info* pointer to the component type index in the chunk's ChunkStructure ComponentTypeSet.
    //    /// </summary>
    //    /// <typeparam name="TComponent">Component typename for the desired component's memory array</typeparam>
    //    /// <returns>Const Pointer to the component memory array</returns>
    //    template<typename TComponent>
    //    const TComponent* GetComponentData()const
    //    {
    //        pnc_assert(!IsNull());
    //        return (TComponent*)this->GetComponentData(&typeid(TComponent));
    //    }


    //    static ChunkPointerInternal_t& GetInternalChunk(Self_t& chunkPointer) { return reinterpret_cast<ChunkPointerInternal_t&>(chunkPointer.GetChunk()); }
    //    static const ChunkPointerInternal_t& GetInternalChunk(const Self_t& chunkPointer) { return reinterpret_cast<const ChunkPointerInternal_t&>(chunkPointer.GetChunk()); }

    //protected:

    //    /// <summary>
    //    /// Notes:
    //    ///     Called by derived structs
    //    /// </summary>
    //    template<typename TChunk>
    //    static void AllocateComponentDataArray(TChunk& chunk)
    //    {
    //        pnc_assert(chunk.IsStruct());
    //        typename TChunk::ChunkPointerInternal_t& internalChunk = TChunk::GetInternalChunk(chunk);
    //        internalChunk.ComponentData = (void**)pnc_alloc(internalChunk.Structure->Components.GetSize() * sizeof(void*), alignof(void*));
    //    }

    //    /// <summary>
    //    /// Notes:
    //    ///     Called by derived structs
    //    /// </summary>
    //    template<typename TChunk>
    //    static void FreeComponentDataArray(TChunk& chunk)
    //    {
    //        pnc_assert(chunk.IsStruct());
    //        typename TChunk::ChunkPointerInternal_t& internalChunk = TChunk::GetInternalChunk(chunk);
    //        pnc_free_clean(internalChunk.ComponentData, internalChunk.Structure->Components.GetSize() * sizeof(void*), alignof(void*));
    //    }


    //    /// <summary>
    //    /// Notes:
    //    ///     Called by derived structs
    //    ///     Does not set the NodeCapacity nor NodeCount on chunk. It only allocate, construct and set componentData
    //    /// </summary>
    //    template<typename TChunk>
    //    static void AllocateConstruct(TChunk& chunk, const Size_t nodeCapacity, const Size_t chunkCapacity)
    //    {
    //        const Size_t nodeCount = chunk.GetNodeCount();
    //        Node_t::AllocateConstructAllComponentsUnsafe(chunk, 0, 0, nodeCount, 1, nodeCapacity, chunkCapacity);
    //    }
    //    template<typename TChunk>
    //    static void AllocateConstruct(TChunk& chunk)
    //    {
    //        AllocateConstruct(chunk, chunk.GetNodeCapacity(), chunk.GetChunkCapacity());
    //    }

    //    /// <summary>
    //    /// Notes:
    //    ///     Called by derived structs
    //    ///     Does not set the NodeCapacity nor NodeCount on chunk. It only allocate, construct and set componentData
    //    ///     chunk and chunkFrom cannot be the same chunk
    //    /// </summary>
    //    template<typename TChunk>
    //    static void AllocateCopy(TChunk& chunk, const TChunk& chunkFrom, const Size_t nodeCapacity, const Size_t chunkCapacity)
    //    {
    //        pnc_assert(!IsSameData(chunk, chunkFrom));
    //        const Size_t nodeCount = chunkFrom.GetNodeCount();
    //        const Size_t chunkCount = chunkFrom.GetChunkCount();
    //        Node_t::AllocateCopyConstructAllComponentsForwardUnsafe(chunk, 0, 0, chunkFrom, 0, 0, nodeCount, chunkCount, nodeCapacity, chunkCapacity);

    //    }
    //    /// <summary>
    //    /// Notes:
    //    ///     Called by derived structs
    //    ///     Does not set the NodeCapacity nor NodeCount on chunk. It only allocate, construct and set componentData
    //    /// </summary>
    //    template<typename TChunk>
    //    static void AllocateCopy(TChunk& chunk, const TChunk& chunkFrom)
    //    {
    //        AllocateCopy(chunk, chunkFrom, chunkFrom.GetNodeCapacity(), chunkFrom.GetChunkCapacity());
    //    }

    //    /// <summary>
    //    /// Notes:
    //    ///     Called by derived structs
    //    ///     Will destruct and free data from chunkTo.
    //    ///     Does not set NodeCapacity nor NodeCount on chunkTo.
    //    ///     chunkTo cannot be the same as chunkFrom
    //    ///     newNodeCapacity must be greater or equal to chunkFrom.GetNodeCapacity()
    //    /// </summary>
    //    template<typename TChunk>
    //    static void ReallocateCopy(TChunk& chunkToReallocate, const TChunk& chunkFrom, const Size_t newNodeCapacity, const Size_t newChunkCapacity)
    //    {
    //        pnc_assert(newNodeCapacity >= chunkFrom.GetNodeCapacity());
    //        pnc_assert(newChunkCapacity >= chunkFrom.GetChunkCapacity());
    //        pnc_assert(!IsSameData(chunkToReallocate, chunkFrom));
    //        if (newNodeCapacity == chunkToReallocate.GetNodeCapacity())
    //            return;
    //        const Size_t nodeCount  = chunkFrom.GetNodeCount();
    //        const Size_t chunkCount = chunkFrom.GetChunkCount();
    //        Node_t::ReallocateCopyConstructAllComponentsForwardUnsafe(chunkToReallocate, 0, 0, 
    //                                                                  chunkFrom,         0, 0, 
    //                                                                  nodeCount, chunkCount, 
    //                                                                  newNodeCapacity, newChunkCapacity);
    //    }
    //    template<typename TChunk>
    //    static void ReallocateCopy(TChunk& chunkToReallocate, const TChunk& chunkFrom)
    //    {
    //        return ReallocateCopy(chunkToReallocate, chunkFrom, chunkFrom.GetNodeCapacity(), chunkFrom.GetChunkCapacity());
    //    }
    //    
    //    /// <summary>
    //    /// Notes:
    //    ///     Called by derived structs
    //    ///     Will destruct and free (or reuse) data from chunkToReallocate.
    //    ///     Does not set NodeCapacity nor NodeCount on chunkToReallocate.
    //    ///     chunkToReallocate and chunkFrom can be the same
    //    ///     newNodeCapacity must be greater or equal to chunkFrom.GetNodeCapacity()
    //    /// </summary>
    //    template<typename TChunk>
    //    static void ReallocateMove(TChunk& chunkToReallocate, TChunk& chunkFrom, const Size_t newNodeCapacity, const Size_t newChunkCapacity)
    //    {
    //        pnc_assert(!chunkToReallocate.IsNull());
    //        pnc_assert(!chunkFrom.IsNull());
    //        pnc_assert(IsSameStructure(chunkToReallocate, chunkFrom));
    //        pnc_assert(newNodeCapacity >= chunkFrom.GetNodeCapacity());

    //        const ChunkStructure_t& structure = chunkToReallocate.GetStructure();
    //        const Size_t componentCount = structure.GetComponentCount();
    //        const Size_t nodeCountFrom = chunkFrom.GetNodeCount();
    //        const Size_t nodeCountTo = chunkToReallocate.GetNodeCount();
    //        const Size_t nodeCapacityTo = chunkToReallocate.GetNodeCapacity();

    //        typename TChunk::ChunkPointerInternal_t& internalChunk = TChunk::GetInternalChunk(chunkToReallocate);
    //        void**const componentDataArrayTo = internalChunk.ComponentData;
    //        for (Size_t i = 0; i < componentCount; ++i)
    //        {
    //            const ComponentType_t& componentType = *structure.Components[i];
    //            void* const dataFrom = chunkFrom.GetComponentData(i);
    //            switch(componentType.GetOwner())
    //            {
    //            case ComponentOwner_Chunk:
    //                if (componentDataArrayTo[i] != dataFrom)
    //                {
    //                    //componentType.DestructDataUnsafe(componentDataArrayTo[i], 0, 1);
    //                    componentType.MoveAssignDataForwardUnsafe(
    //                        componentDataArrayTo[i], 0,
    //                        dataFrom, 0, 1);
    //                }
    //                break;
    //            case ComponentOwner_Node:
    //                void* const dataNew = (void*)pnc_alloc(componentType.GetSize(newNodeCapacity), componentType.GetAlignment());
    //                componentType.MoveConstructDataForwardUnsafe(
    //                    dataNew , 0,
    //                    dataFrom, 0, nodeCountFrom);

    //                componentType.DestructDataUnsafe(componentDataArrayTo[i], 0, nodeCountTo);
    //                pnc_free_dirty(componentDataArrayTo[i], componentType.GetSize(nodeCapacityTo), componentType.GetAlignment());

    //                componentDataArrayTo[i] = dataNew;
    //                break;
    //            pnc_assert_switch_default_no_entry();
    //            }
    //        }
    //    }
    //    template<typename TChunk>
    //    static void ReallocateMove(TChunk& chunkToReallocate, TChunk& chunkFrom)
    //    {
    //        return ReallocateMove(chunkToReallocate, chunkFrom, chunkFrom.GetNodeCapacity(), chunkFrom.GetChunkCapacity());
    //    }

    //    /// <summary>
    //    /// Notes:
    //    ///     Called by derived structs
    //    /// </summary>
    //    template<typename TChunk>
    //    static void FreeDestruct(TChunk& chunk)
    //    {
    //        const Size_t nodeCount     = chunk.GetNodeCount();
    //        const Size_t nodeCapacity  = chunk.GetNodeCapacity();
    //        const Size_t chunkCount    = chunk.GetChunkCount();
    //        const Size_t chunkCapacity = chunk.GetChunkCapacity();
    //        void** const componentDataArrayTo = TChunk::GetInternalChunk(chunk).ComponentData;
    //        const ChunkStructure_t& structure = chunk.GetStructure();
    //        auto componentCount = structure.GetComponentCount();
    //        for (Size_t i = 0; i < componentCount; ++i)
    //        {
    //            const ComponentType_t& componentType = structure.GetComponentType(i);
    //            componentType.DestructComponentUnsafe(componentDataArrayTo[i], 0, 0, nodeCount, chunkCount);
    //            pnc_free_clean(componentDataArrayTo[i], componentType.GetSize(nodeCapacity, chunkCapacity), componentType.GetAlignment());
    //        }
    //    }

    //    template<typename TChunk, typename TBase>
    //    friend struct DChunkPointer2;
    //};

    //
    //template<typename TChunkPointer, typename TBase>
    //struct DChunkPointer2 : public TBase
    //{
    //public:
    //    using Base_t = TBase;
    //    using Self_t = DChunkPointer2<TChunkPointer, TBase>;

    //    using typename Base_t::Size_t;
    //    using ComponentType_t = typename TChunkPointer::ComponentType_t;
    //    using ChunkStructure_t = typename TChunkPointer::ChunkStructure_t;
    //    using Node_t = typename TChunkPointer::Node_t;
    //    using ChunkPointerInternal_t = typename TChunkPointer::ChunkPointerInternal_t;
    //    using ChunkPointer_t = TChunkPointer;
    //    using Chunk_t = typename TChunkPointer::Chunk_t;

    //protected:
    //    ChunkPointer_t Chunk;

    //public:
    //    DChunkPointer2()
    //    {}

    //    DChunkPointer2(const ChunkStructure_t* const chunkStructure, const Size_t nodeCount, void** componentData)
    //        : Base_t()
    //        , Chunk(chunkStructure, nodeCount, componentData)
    //    {}
    //    template<class... TBaseCTorArgumentTypes>
    //    DChunkPointer2(const ChunkStructure_t* const chunkStructure, const Size_t nodeCount, void** const componentData, TBaseCTorArgumentTypes&&... args)
    //        : Base_t(args...)
    //        , Chunk(chunkStructure, nodeCount, componentData)
    //    {}

    //    template<class... TBaseCTorArgumentTypes>
    //    DChunkPointer2(TBaseCTorArgumentTypes&&... args, const ChunkStructure_t* const chunkStructure, const Size_t nodeCount, void** const componentData)
    //        : Base_t(args...)
    //        , Chunk(chunkStructure, nodeCount, componentData)
    //    {
    //    }
    //    template<class... TBaseCTorArgumentTypesBefore, class... TBaseCTorArgumentTypesAfter>
    //    DChunkPointer2(TBaseCTorArgumentTypesBefore&&... argsBefore, const ChunkStructure_t* const chunkStructure, const Size_t nodeCount, void** const componentData, TBaseCTorArgumentTypesAfter&&... argsAfter)
    //        : Base_t(argsBefore..., argsAfter...)
    //        , Chunk(chunkStructure, nodeCount, componentData)
    //    {
    //    }

    //    DChunkPointer2(const ChunkStructure_t* const chunkStructure, const Size_t nodeCount)
    //        : Base_t()
    //        , Chunk(chunkStructure, nodeCount)
    //    {}
    //    template<class... TBaseCTorArgumentTypes>
    //    DChunkPointer2(const ChunkStructure_t*const chunkStructure, const Size_t nodeCount, TBaseCTorArgumentTypes&&... args)
    //        : Base_t(args...)
    //        , Chunk(chunkStructure, nodeCount)
    //    {}
    //    template<class... TBaseCTorArgumentTypes>
    //    DChunkPointer2(TBaseCTorArgumentTypes&&... args, const ChunkStructure_t* const chunkStructure, const Size_t nodeCount)
    //        : Base_t(args...)
    //        , Chunk(chunkStructure, nodeCount)
    //    {
    //    }
    //    template<class... TBaseCTorArgumentTypesBefore, class... TBaseCTorArgumentTypesAfter>
    //    DChunkPointer2(TBaseCTorArgumentTypesBefore&&... argsBefore, const ChunkStructure_t* const chunkStructure, const Size_t nodeCount, TBaseCTorArgumentTypesAfter&&... argsAfter)
    //        : Base_t(argsBefore..., argsAfter...)
    //        , Chunk(chunkStructure, nodeCount)
    //    {
    //    }



    //    template<class... TBaseCTorArgumentTypes>
    //    DChunkPointer2(TBaseCTorArgumentTypes&&... args)
    //        : Base_t(args...)
    //    {}

    //public:
    //    bool IsVoid()const { return Chunk.IsVoid(); }
    //    bool IsStruct()const { return Chunk.IsStruct(); }
    //    bool IsNull()const { return Chunk.IsNull(); }
    //    bool IsData()const { return Chunk.IsData(); }
    //    bool IsVoidNull()const { return Chunk.IsVoidNull(); }
    //    bool IsVoidData()const { return Chunk.IsVoidData(); }
    //    bool IsStructNull()const { return Chunk.IsStructNull(); }
    //    bool IsStructData()const { return Chunk.IsStructData(); }
    //    const ChunkStructure_t& GetStructure()const { return Chunk.GetStructure(); }
    //    Size_t GetNodeCount()const { return Chunk.GetNodeCount(); }
    //    Size_t GetNodeCapacity()const { return Chunk.GetNodeCapacity(); }
    //    Size_t GetChunkCount()const { return Chunk.GetChunkCount(); }
    //    Size_t GetChunkCapacity()const { return Chunk.GetChunkCapacity(); }
    //    void* GetComponentData(const Size_t componentTypeIndexInChunk) { return Chunk.GetComponentData(componentTypeIndexInChunk); }
    //    void* GetComponentData(const Size_t componentTypeIndexInChunk)const { return Chunk.GetComponentData(componentTypeIndexInChunk); }

    //    void* GetComponentData(const type_info* const componentType) { return Chunk.GetComponentData(componentType); }
    //    void* GetComponentData(const type_info* const componentType)const { return Chunk.GetComponentData(componentType); }
    //    template<typename TComponent>
    //    TComponent* GetComponentData() { return Chunk.GetComponentData<TComponent>(); }
    //    template<typename TComponent>
    //    TComponent* GetComponentData()const { return Chunk.GetComponentData<TComponent>(); }
    //    const Chunk_t& operator*()const { return (const Chunk_t&)Chunk; }
    //    Chunk_t& operator*() { return (Chunk_t&)Chunk; }
    //    const Chunk_t* operator->()const { return &(const Chunk_t&)Chunk; }
    //    Chunk_t* operator->() { return &(Chunk_t&)Chunk; }
    //    const Chunk_t& GetChunk()const { return (const Chunk_t&)Chunk; }
    //    Chunk_t& GetChunk() { return (Chunk_t&)Chunk; }
    //    static bool IsSameStructure(const Self_t& a, const Self_t& b) { return ChunkPointer_t::IsSameStructure(a.Chunk, b.Chunk); }
    //    static bool IsSameData(const Self_t& a, const Self_t& b) { return ChunkPointer_t::IsSameData(a.Chunk, b.Chunk); }
    //    static ChunkPointerInternal_t& GetInternalChunk(Self_t& chunkPointer) { return reinterpret_cast<ChunkPointerInternal_t&>(chunkPointer.Chunk); }
    //protected:
    //    template<typename TChunk>
    //    static void AllocateComponentDataArray(TChunk& chunk)
    //        { return ChunkPointer_t::AllocateComponentDataArray(chunk); }
    //    template<typename TChunk>
    //    static void FreeComponentDataArray(TChunk& chunk)
    //        { return ChunkPointer_t::FreeComponentDataArray(chunk); }
    //    template<typename TChunk>
    //    static void AllocateConstruct(TChunk& chunk, const Size_t nodeCapacity, const Size_t chunkCapacity)
    //        { return ChunkPointer_t::AllocateConstruct(chunk, nodeCapacity, chunkCapacity); }
    //    template<typename TChunk>
    //    static void AllocateConstruct(TChunk& chunk)
    //        { return ChunkPointer_t::AllocateConstruct(chunk); }
    //    template<typename TChunk>
    //    static void AllocateCopy(TChunk& chunkToOverwrite, const TChunk& chunkFrom, const Size_t nodeCapacity, const Size_t chunkCapacity)
    //        { return ChunkPointer_t::AllocateCopy(chunkToOverwrite, chunkFrom, nodeCapacity, chunkCapacity); }
    //    template<typename TChunk>
    //    static void AllocateCopy(TChunk& chunkToOverwrite, const TChunk& chunkFrom)
    //        { AllocateCopy(chunkToOverwrite, chunkFrom, chunkFrom.GetNodeCapacity()); }
    //    template<typename TChunk>
    //    static void ReallocateCopy(TChunk& chunkTo, const TChunk& chunkFrom, const Size_t newNodeCapacity, const Size_t newChunkCapacity)
    //        { return ChunkPointer_t::ReallocateCopy(chunkTo, chunkFrom, newNodeCapacity, newChunkCapacity); }
    //    template<typename TChunk>
    //    static void ReallocateCopy(TChunk& chunkTo, const TChunk& chunkFrom)
    //        { return ReallocateCopy(chunkTo, chunkFrom, chunkFrom.GetNodeCapacity()); }
    //    template<typename TChunk>
    //    static void ReallocateMove(TChunk& chunkToReallocate, TChunk& chunkFrom, const Size_t newNodeCapacity, const Size_t newChunkCapacity)
    //        { return ChunkPointer_t::ReallocateMove(chunkToReallocate, chunkFrom, newNodeCapacity, newChunkCapacity); }
    //    template<typename TChunk>
    //    static void ReallocateMove(TChunk& chunkToReallocate, TChunk& chunkFrom)
    //        { return ReallocateMove(chunkToReallocate, chunkFrom, chunkFrom.GetNodeCapacity()); }
    //    template<typename TChunk>
    //    static void FreeDestruct(TChunk& chunk)
    //        { return ChunkPointer_t::FreeDestruct(chunk); }
    //};

}


