// MIT License
// Copyright (c) 2025 Stephanie Rancourt

#pragma once
#include "common.h"
#include "Node.h"
#include "ChunkPointerInternal.h"


namespace PNC
{

    /// <summary>
    /// A ChunkPointer points to a Chunk's Component data memory within a range of Nodes.
    /// A ChunkPointer is also a Chunk by itself and provides access to the Component data.
    /// Copying a ChunkPointer does not copy the data.
    /// </summary>
    /// <typeparam name="TChunkStructure">Structure of the Chunk's Component data.</typeparam>
    template<typename TChunkStructure>
    struct ChunkPointerT : protected ChunkPointerInternalT<TChunkStructure>
    {
    public:
        using Base_t = ChunkPointerInternalT<TChunkStructure>;
        using Self_t = ChunkPointerT<TChunkStructure>;
        using ChunkStructure_t = TChunkStructure;
        using Size_t = typename ChunkStructure_t::Size_t;
        using ComponentType_t = typename ChunkStructure_t::ComponentType_t;
        using Node_t = NodeT<TChunkStructure>;

        /// <summary>
        /// Reinterprete_cast this object to this type to get read-write access to its private data fields
        /// Use with caution.
        /// </summary>
        using ChunkPointerInternal_t = ChunkPointerInternalT<TChunkStructure>;
        using ChunkPointer_t = Self_t;

        /// <summary>
        /// Chunk_t is the type a ChunkPointer points to
        /// </summary>
        using Chunk_t = Self_t;

    public:
        using Base_t::Base_t;

        ChunkPointerT(Self_t&& o) = default;
        Self_t& operator=(Self_t&& o) = default;
        ChunkPointerT(const Self_t& o) = default;
        Self_t& operator=(const Self_t& o) = default;

    protected:
        /// <summary>
        /// Create a StructNull Chunk with a node count.
        /// </summary>
        ChunkPointerT(const ChunkStructure_t* const chunkStructure, const Size_t nodeCount)
            : Base_t(chunkStructure, nodeCount)
        {
        }

        // used by ChunkCapacityAllocationT<ChunkPointerT>, ignores the capacity parameter
        ChunkPointerT(const ChunkStructure_t* const chunkStructure, const Size_t nodeCapacity, const Size_t nodeCount)
            : Base_t(chunkStructure, nodeCount)
        {
        }

    public:
        PNC_USING_CHUNKPOINTER_INTERFACE();
        using Base_t::GetChunkCount;
        using Base_t::GetChunkCapacity;
        using Base_t::GetNodeCapacity;

        /// <summary>
        /// Create a null chunk without structure nor component data.
        /// </summary>
        /// <returns></returns>
        static Self_t Null() { return Self_t(); }

        PNC_IMPLEMENT_CHUNKPOINTER_SELFPOINTER();

    public:
        /// <summary>
        /// Get the pointer to a component's memory array using the component type index in the ChunkStructure ComponentTypeSet.
        /// For components with ComponentOwner_Node, the array will be at least the length of the size of the chunk.
        /// For components with ComponentOwner_Chunk, the array will be of length 1.
        /// This is the fastest way to access the chunk's component data.
        /// </summary>
        /// <param name="componentIndexInChunk">index in the ChunkStructure::Components ComponentTypeSet</param>
        /// <returns>Pointer to the component memory array</returns>
        void* GetComponentData(const Size_t componentTypeIndexInChunk)
        {
            pnc_assert(!IsNull());
            return this->ComponentData[componentTypeIndexInChunk];
        }

        /// <summary>
        /// Get the const pointer to a component's memory array using the component type index in the ChunkStructure ComponentTypeSet.
        /// For components with ComponentOwner_Node, the array will be at least the length of the size of the chunk.
        /// For components with ComponentOwner_Chunk, the array will be of length 1.
        /// This is the fastest way to access the chunk's component data.
        /// </summary>
        /// <param name="componentTypeIndexInChunk">index in the ChunkStructure::Components ComponentTypeSet</param>
        /// <returns>Const pointer to the component memory array</returns>
        const void* GetComponentData(const Size_t componentTypeIndexInChunk)const
        {
            pnc_assert(!IsNull());
            return this->ComponentData[componentTypeIndexInChunk];
        }

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
            auto index = this->Structure->Components.GetComponentTypeIndexInChunk(componentType);
            if (index < 0)
                return nullptr;
            return this->GetComponentData(index);
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
            auto index = this->Structure->Components.GetComponentTypeIndexInChunk(componentType);
            if (index < 0)
                return nullptr;
            return this->GetComponentData(index);
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
            return (TComponent*)this->GetComponentData(&typeid(TComponent));
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
            return (TComponent*)this->GetComponentData(&typeid(TComponent));
        }

        /// <summary>
        /// Test if 2 chunk have the same ChunkStructure
        /// </summary>
        /// <param name="a"></param>
        /// <param name="b"></param>
        /// <returns></returns>
        static bool IsSameStructure(const Self_t& a, const Self_t& b) { return a.Structure == b.Structure; }
        static bool IsSameData(const Self_t& a, const Self_t& b) { return a.ComponentData == b.ComponentData; }

        static ChunkPointerInternal_t& GetInternalChunk(Self_t& chunkPointer) { return reinterpret_cast<ChunkPointerInternal_t&>(chunkPointer.GetChunk()); }
        static const ChunkPointerInternal_t& GetInternalChunk(const Self_t& chunkPointer) { return reinterpret_cast<const ChunkPointerInternal_t&>(chunkPointer.GetChunk()); }
    //protected:
    //    ChunkPointerInternal_t& GetInternalChunk() { return reinterpret_cast<ChunkPointerInternal_t&>(GetChunk()); }

    protected:
            
        /// <summary>
        /// Notes:
        ///     Derived structs may call this function
        ///     Does not destruct/free data on this chunk before allocating the new data
        ///     Does not set NodeCapacity nor NodeCount on chunkToOverwrite
        ///     chunkToOverwrite cannot be the same as chunkFrom
        /// </summary>
        template<typename TChunk>
        static void AllocateDataCopy(TChunk& chunkToOverwrite, const TChunk& chunkFrom, const Size_t newNodeCapacity)
        {
            pnc_assert(!chunkToOverwrite.IsNull());
            pnc_assert(!chunkFrom.IsNull());
            pnc_assert(IsSameStructure(chunkToOverwrite, chunkFrom));
            pnc_assert(&chunkToOverwrite != &chunkFrom);

            const ChunkStructure_t& structure = chunkToOverwrite.GetStructure();
            const Size_t componentCount = structure.GetComponentCount();
            const Size_t nodeCount = chunkFrom.GetNodeCount();

            void**const componentDataArrayTo = TChunk::GetInternalChunk(chunkToOverwrite).ComponentData;
            for (Size_t i = 0; i < componentCount; ++i)
            {
                const ComponentType_t& componentType = *structure.Components[i];
                componentDataArrayTo[i] = (void*)pnc_alloc(componentType.GetSize(newNodeCapacity), componentType.GetAlignment());
                componentType.CopyConstructComponentForwardUnsafe(
                    componentDataArrayTo[i],       0, 0,
                    chunkFrom.GetComponentData(i), 0, 0, nodeCount, 1);
            }
        }
        template<typename TChunk>
        static void AllocateDataCopy(TChunk& chunkToOverwrite, const TChunk& chunkFrom)
        {
            AllocateDataCopy(chunkToOverwrite, chunkFrom, chunkFrom.GetNodeCapacity());
        }

        /// <summary>
        /// Notes:
        ///     Derived structs may call this function
        ///     Will destruct and free data from chunkTo.
        ///     Does not set NodeCapacity nor NodeCount on chunkTo.
        ///     chunkTo cannot be the same as chunkFrom
        ///     newNodeCapacity must be greater or equal to chunkFrom.GetNodeCapacity()
        /// </summary>
        template<typename TChunk>
        static void ReallocateDataCopy(TChunk& chunkToReallocate, const TChunk& chunkFrom, const Size_t newNodeCapacity)
        {
            pnc_assert(!chunkToReallocate.IsNull());
            pnc_assert(!chunkFrom.IsNull());
            pnc_assert(IsSameStructure(chunkToReallocate, chunkFrom));
            pnc_assert(&chunkToReallocate != &chunkFrom);
            pnc_assert(newNodeCapacity >= chunkFrom.GetNodeCapacity());

            const ChunkStructure_t& structure = chunkToReallocate.GetStructure();
            const Size_t componentCount = structure.GetComponentCount();
            const Size_t nodeCountFrom = chunkFrom.GetNodeCount();
            const Size_t nodeCountTo = chunkToReallocate.GetNodeCount();
            const Size_t nodeCapacityTo = chunkToReallocate.GetNodeCapacity();
            typename TChunk::ChunkPointerInternal_t& internalChunkTo = TChunk::GetInternalChunk(chunkToReallocate);
            void**const componentDataArrayTo = internalChunkTo.ComponentData;
            for (Size_t i = 0; i < componentCount; ++i)
            {
                const ComponentType_t& componentType = *structure.Components[i];
                const void* const dataFrom = chunkFrom.GetComponentData(i);
                switch (componentType.GetOwner())
                {
                case ComponentOwner_Chunk:
                    componentType.CopyAssignDataForwardUnsafe(
                        componentDataArrayTo[i], 0,
                        dataFrom, 0, 1);
                    break;
                case ComponentOwner_Node:
                    void* const dataNew = (void*)pnc_alloc(componentType.GetSize(newNodeCapacity), componentType.GetAlignment());
                    componentType.CopyConstructDataForwardUnsafe(
                        dataNew, 0,
                        dataFrom, 0, nodeCountFrom);

                    componentType.DestructDataUnsafe(componentDataArrayTo[i], 0, nodeCountTo);
                    pnc_free_dirty(componentDataArrayTo[i], componentType.GetSize(nodeCapacityTo), componentType.GetAlignment());

                    componentDataArrayTo[i] = dataNew;
                    break;
                    pnc_assert_switch_default_no_entry();
                }
            }
        }
        template<typename TChunk>
        static void ReallocateDataCopy(TChunk& chunkToReallocate, const TChunk& chunkFrom)
        {
            return ReallocateDataCopy(chunkToReallocate, chunkFrom, chunkFrom.GetNodeCapacity());
        }
        
        /// <summary>
        /// Notes:
        ///     Derived structs may call this function
        ///     Will destruct and free (or reuse) data from chunkToReallocate.
        ///     Does not set NodeCapacity nor NodeCount on chunkToReallocate.
        ///     chunkToReallocate and chunkFrom can be the same
        ///     newNodeCapacity must be greater or equal to chunkFrom.GetNodeCapacity()
        /// </summary>
        template<typename TChunk>
        static void ReallocateDataMove(TChunk& chunkToReallocate, TChunk& chunkFrom, const Size_t newNodeCapacity)
        {
            pnc_assert(!chunkToReallocate.IsNull());
            pnc_assert(!chunkFrom.IsNull());
            pnc_assert(IsSameStructure(chunkToReallocate, chunkFrom));
            pnc_assert(newNodeCapacity >= chunkFrom.GetNodeCapacity());

            const ChunkStructure_t& structure = chunkToReallocate.GetStructure();
            const Size_t componentCount = structure.GetComponentCount();
            const Size_t nodeCountFrom = chunkFrom.GetNodeCount();
            const Size_t nodeCountTo = chunkToReallocate.GetNodeCount();
            const Size_t nodeCapacityTo = chunkToReallocate.GetNodeCapacity();

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
                    void* const dataNew = (void*)pnc_alloc(componentType.GetSize(newNodeCapacity), componentType.GetAlignment());
                    componentType.MoveConstructDataForwardUnsafe(
                        dataNew , 0,
                        dataFrom, 0, nodeCountFrom);

                    componentType.DestructDataUnsafe(componentDataArrayTo[i], 0, nodeCountTo);
                    pnc_free_dirty(componentDataArrayTo[i], componentType.GetSize(nodeCapacityTo), componentType.GetAlignment());

                    componentDataArrayTo[i] = dataNew;
                    break;
                pnc_assert_switch_default_no_entry();
                }
            }
        }
        template<typename TChunk>
        static void ReallocateDataMove(TChunk& chunkToReallocate, TChunk& chunkFrom)
        {
            return ReallocateDataMove(chunkToReallocate, chunkFrom, chunkFrom.GetNodeCapacity());
        }



        /// <summary>
        /// Notes:
        ///     Derived structs may call this function
        ///     Does not set the NodeCapacity nor NodeCount on chunkOverwrite. It only allocate, construct and set componentData
        /// </summary>
        template<typename TChunk>
        static void AllocateAndConstructData(TChunk& chunkToOverwrite, const Size_t nodeCapacity, const Size_t nodeCount)
        {
            typename TChunk::ChunkPointerInternal_t& internalChunk = TChunk::GetInternalChunk(chunkToOverwrite);
            pnc_assert(!internalChunk.IsNull());
            if (nodeCount == 0)
            {
                // TODO: Construct only ChunkComponents
                //return;
            }
            auto componentCount = internalChunk.Structure->Components.GetSize();
            for (Size_t i = 0; i < componentCount; ++i)
            {
                const ComponentType_t& componentType = *internalChunk.Structure->Components[i];
                internalChunk.ComponentData[i] = (void*)pnc_alloc(componentType.GetSize(nodeCapacity), componentType.GetAlignment());
                componentType.ConstructComponentUnsafe(internalChunk.ComponentData[i], 0, nodeCount);
            }
        }

        template<typename TChunk>
        static void AllocateDataArray(TChunk& chunk)
        {
            typename TChunk::ChunkPointerInternal_t& internalChunk = TChunk::GetInternalChunk(chunk);
            internalChunk.ComponentData = (void**)pnc_alloc(internalChunk.Structure->Components.GetSize() * sizeof(void*), alignof(void*));
        }

        template<typename TChunk>
        static void DestructAndFreeData(TChunk& chunk)
        {
            typename TChunk::ChunkPointerInternal_t& internalChunk = TChunk::GetInternalChunk(chunk);

            auto componentCount = internalChunk.Structure->GetComponentCount();
            Size_t capacity = chunk.GetNodeCapacity();
            for (Size_t i = 0; i < componentCount; ++i)
            {
                const ComponentType_t& componentType = *internalChunk.Structure->Components[i];
                componentType.DestructComponentUnsafe(internalChunk.ComponentData[i], 0, internalChunk.NodeCount);
                pnc_free_clean(internalChunk.ComponentData[i], componentType.GetSize(capacity), componentType.GetAlignment());
            }
        }

        template<typename TChunk>
        static void FreeDataArray(TChunk& chunk)
        {
            typename TChunk::ChunkPointerInternal_t& internalChunk = TChunk::GetInternalChunk(chunk);
            pnc_free_clean(internalChunk.ComponentData, internalChunk.Structure->Components.GetSize() * sizeof(void*), alignof(void*));
        }

        template<typename TChunk>
        static void Destroy(TChunk& chunk)
        {
            DestructAndFreeData(chunk);
            FreeDataArray(chunk);
        }

        template<typename TChunk, typename TBase>
        friend struct DChunkPointer;
    };
}


