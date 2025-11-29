// MIT License
// Copyright (c) 2025 Stephanie Rancourt

#pragma once
#include "common.h"
#include "Node.h"

namespace PNC
{
    template<typename TChunkStructure>
    struct DCStructure
    {
    public:
        using Base_t = void;
        using Self_t = DCStructure<TChunkStructure>;
        using Size_t = typename TChunkStructure::Size_t;
        using ChunkStructure_t = TChunkStructure;
        using ComponentType_t = typename ChunkStructure_t::ComponentType_t;
        using Node_t = NodeT<TChunkStructure>;

        DCStructure() = default;
        template<typename TProps>
        DCStructure(const DPropsTag& tag, const TProps& props)
        {
        }
    };

    //template<typename TChunkStructure>
    //struct ChunkPointerInternalProtectedT : protected ChunkPointerInternalT<TChunkStructure>
    //{
    //public:
    //    using Base_t = ChunkPointerInternalT<TChunkStructure>;
    //    using Self_t = ChunkPointerInternalProtectedT<TChunkStructure>;
    //    using Base_t::Base_t;

    //};

    // ChunkPointerInternalT<TChunkStructure>

    /// <summary>
    /// A ChunkPointer points to a Chunk's Component data memory within a range of Nodes.
    /// A ChunkPointer is also a Chunk by itself and provides access to the Component data.
    /// Copying a ChunkPointer does not copy the data.
    /// </summary>
    /// <typeparam name="TChunkStructure">Structure of the Chunk's Component data.</typeparam>
    template<typename TChunkPointer, typename TBase>
    struct DChunkPointer : public TBase
    {
    public:
        using Base_t = TBase;
        using Self_t = DChunkPointer<TChunkPointer, TBase>;
        using typename Base_t::Size_t;
        using ComponentType_t = typename TChunkPointer::ComponentType_t;
        using ChunkStructure_t = typename TChunkPointer::ChunkStructure_t;
        using Node_t = typename TChunkPointer::Node_t;

        using ChunkPointerInternal_t = TChunkPointer;
        using ChunkPointer_t = TChunkPointer;
        using Chunk_t = DChunkPointer<TChunkPointer, DCStructure<ChunkStructure_t>>;

        ///// <summary>
        ///// Chunk_t is the type a ChunkPointer points to
        ///// </summary>
        //using Chunk_t = Self_t;

    protected:
        ChunkPointer_t Chunk;
    public:
        using Base_t::Base_t;

        DChunkPointer() = default;

        DChunkPointer(const StructurePtr<ChunkStructure_t>& chunkStructure)
            : Base_t()
            , Chunk(chunkStructure)
        {
        }
        DChunkPointer(const StructurePtr<ChunkStructure_t>& chunkStructure, const NodeCountT<Size_t> nodeCount, void**const componentData)
            : Base_t()
            , Chunk(chunkStructure, nodeCount, componentData)
        {
        }

        DChunkPointer(const StructurePtr<ChunkStructure_t>& chunkStructure, const NodeCountT<Size_t> nodeCount)
            : Base_t()
            , Chunk(chunkStructure, nodeCount)
        {
        }
    protected:
        template<typename TProps>
        DChunkPointer(const DPropsTag& tag, const TProps& props)
            : Base_t(tag, props)
            , Chunk(tag, props)
        {
        }

    public:
        DChunkPointer(Self_t&& o) = default;
        Self_t& operator=(Self_t&& o) = default;
        DChunkPointer(const Self_t& o) = default;
        Self_t& operator=(const Self_t& o) = default;


    public:
        bool IsVoid()const { return Chunk.IsVoid(); }
        bool IsStruct()const { return Chunk.IsStruct(); }
        bool IsNull()const { return Chunk.IsNull(); }
        bool IsData()const { return Chunk.IsData(); }
        bool IsVoidNull()const { return Chunk.IsVoidNull(); }
        bool IsVoidData()const { return Chunk.IsVoidData(); }
        bool IsStructNull()const { return Chunk.IsStructNull(); }
        bool IsStructData()const { return Chunk.IsStructData(); }
        const ChunkStructure_t& GetStructure()const { return Chunk.GetStructure(); }
        NodeCountT<Size_t> GetNodeCount()const { return Chunk.GetNodeCount(); }
        NodeCapacityT<Size_t> GetNodeCapacity()const { return Chunk.GetNodeCapacity(); }
        ChunkCountT<Size_t> GetChunkCount()const { return Chunk.GetChunkCount(); }
        ChunkCapacityT<Size_t> GetChunkCapacity()const { return Chunk.GetChunkCapacity(); }
        void* GetComponentData(const Size_t componentTypeIndexInChunk) { return Chunk.GetComponentData(componentTypeIndexInChunk); }
        const void* GetComponentData(const Size_t componentTypeIndexInChunk)const { return Chunk.GetComponentData(componentTypeIndexInChunk); }
        Chunk_t& GetChunk() { return *reinterpret_cast<Chunk_t*>(this); }
        const Chunk_t& GetChunk()const { return *reinterpret_cast<const Chunk_t*>(this); }

        static bool IsSameStructure(const Self_t& a, const Self_t& b) { return ChunkPointer_t::IsSameStructure(a.Chunk, b.Chunk); }
        static bool IsSameData(const Self_t& a, const Self_t& b) { return ChunkPointer_t::IsSameData(a.Chunk, b.Chunk); }


    public:
        /// <summary>
        /// Get the pointer to a component's memory array using the component's type const type_info* from &typeid(ComponentTypename).
        /// For components with ComponentOwner_Node, the array will be at least the length of the size of the chunk.
        /// For components with ComponentOwner_Chunk, the array will be of length 1.
        /// This is a slow way to access the chunk's component data as it require to do a map lookup of the 
        /// component's const type_info* pointer to the component type index in the chunk's ChunkStructure ComponentTypeSet.
        /// </summary>
        /// <param name="componentType">const type_info* pointer obtained from &typeid(ComponentTypename)</param>
        /// <returns>Pointer to the component memory array</returns>
        void* GetComponentData(const type_info* const componentType)
        {
            pnc_assert(!IsNull());
            auto index = GetStructure().Components.GetComponentTypeIndexInChunk(componentType);
            if (index < 0)
                return nullptr;
            return Chunk.GetComponentData(index);
        }

        /// <summary>
        /// Get the const pointer to a component's memory array using the component's type const type_info* from &typeid(ComponentTypename).
        /// For components with ComponentOwner_Node, the array will be at least the length of the size of the chunk.
        /// For components with ComponentOwner_Chunk, the array will be of length 1.
        /// This is a slow way to access the chunk's component data as it require to do a map lookup of the 
        /// component's const type_info* pointer to the component type index in the chunk's ChunkStructure ComponentTypeSet.
        /// </summary>
        /// <param name="componentType">const type_info* pointer obtained from &typeid(ComponentTypename)</param>
        /// <returns>Const pointer to the component memory array</returns>
        const void* GetComponentData(const type_info* const componentType)const
        {
            pnc_assert(!IsNull());
            auto index = GetStructure().Components.GetComponentTypeIndexInChunk(componentType);
            if (index < 0)
                return nullptr;
            return Chunk.GetComponentData(index);
        }


        /// <summary>
        /// Get the pointer to a component's memory array using the component's typename.
        /// For components with ComponentOwner_Node, the array will be at least the length of the size of the chunk.
        /// For components with ComponentOwner_Chunk, the array will be of length 1.
        /// This is a slow way to access the chunk's component data as it require to do a map lookup of the associated
        /// component typename const type_info* pointer to the component type index in the chunk's ChunkStructure ComponentTypeSet.
        /// </summary>
        /// <typeparam name="TComponent">Component typename for the desired component's memory array</typeparam>
        /// <returns>Pointer to the component memory array</returns>
        template<typename TComponent>
        TComponent* GetComponentData()
        {
            pnc_assert(!IsNull());
            return (TComponent*)GetComponentData(&typeid(TComponent));
        }

        /// <summary>
        /// Get the const pointer to a component's memory array using the component's typename.
        /// For components with ComponentOwner_Node, the array will be at least the length of the size of the chunk.
        /// For components with ComponentOwner_Chunk, the array will be of length 1.
        /// This is a slow way to access the chunk's component data as it require to do a map lookup of the associated
        /// component typename const type_info* pointer to the component type index in the chunk's ChunkStructure ComponentTypeSet.
        /// </summary>
        /// <typeparam name="TComponent">Component typename for the desired component's memory array</typeparam>
        /// <returns>Const Pointer to the component memory array</returns>
        template<typename TComponent>
        const TComponent* GetComponentData()const
        {
            pnc_assert(!IsNull());
            return (TComponent*)GetComponentData(&typeid(TComponent));
        }


        static ChunkPointerInternal_t& GetInternalChunk(Self_t& a) { return a.Chunk; }
        static const ChunkPointerInternal_t& GetInternalChunk(const Self_t& a) { return a.Chunk; }

    protected:

        /// <summary>
        /// Notes:
        ///     Called by derived structs
        /// </summary>
        template<typename TChunk>
        static void AllocateComponentDataArray(TChunk& chunk)
        {
            pnc_assert(chunk.IsStruct());
            typename TChunk::ChunkPointerInternal_t& internalChunk = TChunk::GetInternalChunk(chunk);
            internalChunk.ComponentData = (void**)pnc_alloc(internalChunk.Structure->Components.GetSize() * sizeof(void*), alignof(void*));
        }

        /// <summary>
        /// Notes:
        ///     Called by derived structs
        /// </summary>
        template<typename TChunk>
        static void FreeComponentDataArray(TChunk& chunk)
        {
            pnc_assert(chunk.IsStruct());
            typename TChunk::ChunkPointerInternal_t& internalChunk = TChunk::GetInternalChunk(chunk);
            pnc_free_clean(internalChunk.ComponentData, internalChunk.Structure->Components.GetSize() * sizeof(void*), alignof(void*));
        }


        /// <summary>
        /// Notes:
        ///     Called by derived structs
        ///     Does not set the NodeCapacity nor NodeCount on chunk. It only allocate, construct and set componentData
        /// </summary>
        template<typename TChunk, typename TArgs>
        static void AllocateConstruct(TChunk& chunk, const TArgs& args)
        {
            const auto nodeCount = chunk.GetNodeCount();
            Node_t::AllocateConstructAllComponentsUnsafe(chunk, 0, 0, nodeCount, 1, PropCountToCapacity(nodeCount), 1);
        }

        /// <summary>
        /// Notes:
        ///     Called by derived structs
        ///     Does not set the NodeCapacity nor NodeCount on chunk. It only allocate, construct and set componentData
        ///     chunk and chunkFrom cannot be the same chunk
        /// </summary>
        template<typename TChunk>
        static void AllocateCopy(TChunk& chunk, const TChunk& chunkFrom, const NodeCountT<Size_t> nodeCount)
        {
            pnc_assert(!IsSameData(chunk, chunkFrom));
            Node_t::AllocateCopyConstructAllComponentsForwardUnsafe(chunk, 0, 0, chunkFrom, 0, 0, 
                nodeCount,                        ChunkCountT<Size_t>(1), 
                PropCountToCapacity(nodeCount), ChunkCapacityT<Size_t>(1));

        }
        /// <summary>
        /// Notes:
        ///     Called by derived structs
        ///     Does not set the NodeCapacity nor NodeCount on chunk. It only allocate, construct and set componentData
        /// </summary>
        template<typename TChunk>
        static void AllocateCopy(TChunk& chunk, const TChunk& chunkFrom)
        {
            AllocateCopy(chunk, chunkFrom, chunkFrom.GetNodeCount());
        }

        /// <summary>
        /// Notes:
        ///     Called by derived structs
        ///     Will destruct and free data from chunkTo.
        ///     Does not set NodeCapacity nor NodeCount on chunkTo.
        ///     chunkTo cannot be the same as chunkFrom
        ///     newNodeCapacity must be greater or equal to chunkFrom.GetNodeCapacity()
        ///     if newNodeCount > chunkFrom.GetNodeCount(), nodes after chunkFrom.GetNodeCount() will not be constructed, caller is responsible for those nodes
        /// </summary>
        template<typename TChunk>
        static void ReallocateCopy(TChunk& chunkToReallocate, const TChunk& chunkFrom, const NodeCountT<Size_t> newNodeCount)
        {
            pnc_assert(newNodeCount >= chunkFrom.GetNodeCount());
            pnc_assert(!IsSameData(chunkToReallocate, chunkFrom));
            Node_t::ReallocateCopyConstructAllComponentsForwardUnsafe(chunkToReallocate, 0, 0, 
                                                                      chunkFrom,         0, 0, 
                                                                      newNodeCount,                      ChunkCountT<Size_t>(1),
                                                                      PropCountToCapacity(newNodeCount), ChunkCapacityT<Size_t>(1));
        }
        template<typename TChunk>
        static void ReallocateCopy(TChunk& chunkToReallocate, const TChunk& chunkFrom)
        {
            return ReallocateCopy(chunkToReallocate, chunkFrom, chunkFrom.GetNodeCount());
        }
        
        /// <summary>
        /// Notes:
        ///     Called by derived structs
        ///     Will destruct and free (or reuse) data from chunkToReallocate.
        ///     Does not set NodeCapacity nor NodeCount on chunkToReallocate.
        ///     chunkToReallocate and chunkFrom can be the same
        ///     newNodeCapacity must be greater or equal to chunkFrom.GetNodeCapacity()
        ///     if newNodeCount > chunkFrom.GetNodeCount(), nodes after chunkFrom.GetNodeCount() will not be constructed, caller is responsible for those nodes.
        /// </summary>
        template<typename TChunk>
        static void ReallocateMove(TChunk& chunkToReallocate, TChunk& chunkFrom, const NodeCountT<Size_t> newNodeCount)
        {
            pnc_assert(!chunkToReallocate.IsNull());
            pnc_assert(!chunkFrom.IsNull());
            pnc_assert(IsSameStructure(chunkToReallocate, chunkFrom));
            pnc_assert(newNodeCount >= chunkFrom.GetNodeCount());

            const ChunkStructure_t& structure = chunkToReallocate.GetStructure();
            const auto componentCount = structure.GetComponentCount();
            const auto nodeCountFrom = chunkFrom.GetNodeCount();
            const auto nodeCountTo = chunkToReallocate.GetNodeCount();

            typename TChunk::ChunkPointerInternal_t& internalChunk = TChunk::GetInternalChunk(chunkToReallocate);
            void**const componentDataArrayTo = internalChunk.ComponentData;
            for (Size_t i = 0; i < componentCount; ++i)
            {
                const ComponentType_t& componentType = *structure.Components[i];
                void* const dataFrom = chunkFrom.GetComponentData(i);
                switch(componentType.GetOwner())
                {
                case ComponentOwner_Chunk:
                    if (componentDataArrayTo[i] != dataFrom)
                    {
                        //componentType.DestructDataUnsafe(componentDataArrayTo[i], 0, 1);
                        componentType.MoveAssignDataForwardUnsafe(
                            componentDataArrayTo[i], 0,
                            dataFrom, 0, 1);
                    }
                    break;
                case ComponentOwner_Node:
                    void* const dataNew = (void*)pnc_alloc(componentType.GetSize(newNodeCount), componentType.GetAlignment());
                    componentType.MoveConstructDataForwardUnsafe(
                        dataNew , 0,
                        dataFrom, 0, nodeCountFrom);

                    componentType.DestructDataUnsafe(componentDataArrayTo[i], 0, nodeCountTo);
                    pnc_free_dirty(componentDataArrayTo[i], componentType.GetSize(nodeCountTo), componentType.GetAlignment());

                    componentDataArrayTo[i] = dataNew;
                    break;
                pnc_assert_switch_default_no_entry();
                }
            }
        }
        template<typename TChunk>
        static void ReallocateMove(TChunk& chunkToReallocate, TChunk& chunkFrom)
        {
            return ReallocateMove(chunkToReallocate, chunkFrom, chunkFrom.GetNodeCount());
        }

        /// <summary>
        /// Notes:
        ///     Called by derived structs
        /// </summary>
        template<typename TChunk>
        static void FreeDestruct(TChunk& chunk)
        {
            const auto nodeCount     = chunk.GetNodeCount();
            void** const componentDataArrayTo = TChunk::GetInternalChunk(chunk).ComponentData;
            const ChunkStructure_t& structure = chunk.GetStructure();
            auto componentCount = structure.GetComponentCount();
            for (Size_t i = 0; i < componentCount; ++i)
            {
                const ComponentType_t& componentType = structure.GetComponentType(i);
                componentType.DestructComponentUnsafe(componentDataArrayTo[i], 0, 0, nodeCount, ChunkCountT<Size_t>(1));
                pnc_free_clean(componentDataArrayTo[i], componentType.GetSize(nodeCount), componentType.GetAlignment());
            }
        }

    };




}