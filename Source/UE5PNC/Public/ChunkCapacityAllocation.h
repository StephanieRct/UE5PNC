// MIT License
// Copyright (c) 2025 Stephanie Rancourt

#pragma once
#include "common.h"

namespace PNC
{
    template<typename TBase>
    struct ChunkCapacityAllocationT : public TBase
    {
    public:
        using Base_t = TBase;
        using Self_t = ChunkCapacityAllocationT<TBase>;
        using typename TBase::ChunkStructure_t;
        using typename TBase::Size_t;
        using typename Base_t::Node_t;
        using ComponentType_t = typename ChunkStructure_t::ComponentType_t;

    protected:
        using ChunkPointerInternal_t = ChunkPointerT<ChunkStructure_t>::ChunkPointerInternal_t;

    public:
        /// <summary>
        /// Create a Null Chunk without ChunkStructure.
        /// IsNull() will evaluate to true.
        /// </summary>
        ChunkCapacityAllocationT() = default;

        /// <summary>
        /// Create a Chunk of a given ChunkStructure and allocate the Component's memory
        /// The Components' memory can fit as many instances of each Components as the Chunk's capacity.
        /// Any computation performed on this Chunk will only process node within the chunk's Node count and not it's capacity.
        /// </summary>
        /// <param name="chunkStructure">Structure of the Chunk's component data.</param>
        /// <param name="nodeCapacity">Maximum number of Nodes this Chunk can grow to.</param>
        /// <param name="nodeCount"></param>
        ChunkCapacityAllocationT(const ChunkStructure_t* chunkStructure, Size_t nodeCapacity, Size_t nodeCount = 0)
            : Base_t(chunkStructure, nodeCapacity, nodeCount)
        {
            AllocateDataArray();
            AllocateAndConstructData();
        }

        /// <summary>
        /// Copy a Chunk and its Component data.
        /// The result chunk will have the same node count and capacity as the original.
        /// </summary>
        /// <param name="o"></param>
        ChunkCapacityAllocationT(const Self_t& chunkFrom)
            : Base_t(chunkFrom)
        {
            checkf(!chunkFrom.IsVoidData(), TEXT("Cannot copy a VoidData Chunk. Data cannot be copied without a structure."));
            //TODO construct from null chunk
            if (chunkFrom.IsNull())
                return;
            AllocateDataArray();
            AllocateDataCopy(*this, chunkFrom);
        }

        /// <summary>
        /// Copy a Chunk and its Component data.
        /// It will deallocate any previous data.
        /// The result Chunk will have the same node count and capacity as the one being copied.
        /// </summary>
        /// <param name="o"></param>
        Self_t& operator=(const Self_t& chunkFrom)
        {
            if (this == &chunkFrom)
                return *this;

            checkf(!chunkFrom.IsVoidData(), TEXT("Cannot copy a VoidData Chunk. Data cannot be copied without a structure."));
            checkf(!this->IsVoidData(), TEXT("Cannot copy over a VoidData Chunk. Data cannot be destroyed without a structure."));

            if (IsData())
            {
                if (chunkFrom.IsData() && GetNodeCapacity() >= chunkFrom.GetNodeCount())
                {
                    if (IsSameStructure(*this, chunkFrom))
                    {
                        // ReplaceCopyNodes
                        return *this;
                    }
                    // TODO investigate if Restructuring could be done here.
                    //else if (CanRestructure(*this, chunkFrom))
                    //{
                    //    ...
                    //    return *this;
                    //}
                }
                Destroy();
            }

            Base_t::operator=(chunkFrom);

            if (chunkFrom.IsData())
            {
                if (chunkFrom.IsStruct())
                {
                    //CopyStruct, CopyCountCap, AllocCDataArray, AllocCData, CopyCtorNodes
                    AllocateDataArray();
                    AllocateDataCopy(*this, chunkFrom);
                }
            }

            return *this;
        }
        
        //ChunkCapacityAllocationT(Self_t&& o) = default;

        //if (chunkFrom.IsVoid())
        //{
        //    if(chunkFrom.IsNull())
        //    DestructAndDeallocateData();
        //    DeallocateComponentDataArray();
        //    Structure = nullptr;
        //    ComponentData = nullptr;
        //    return *this;
        //}

        //assert_pnc(IsSameStructure(*this, chunkFrom));
        //
        //if (GetNodeCapacity() < chunkFrom.GetNodeCount())
        //{
        //    DestructAndDeallocateData();
        //    DeallocateComponentDataArray();
        //    AllocateComponentDataArray();
        //    AllocateDataCopy(*this, chunkFrom);
        //    // must allocate n
        //}
        //else
        //{
        //    // copy assign over existing nodes
        //    // copy construct remaining nodes
        //}
        //// TODO: do not realocate if has enough space already
        //DestructAndDeallocateData();
        //DeallocateComponentDataArray();
        //Base_t::operator=(chunkFrom);
        //if (o.IsNull())
        //    return *this;
        //AllocateComponentDataArray();
        //AllocateDataCopy(*this, chunkFrom);
        //return *this;

        /// <summary>
        /// Deallocate data if not a Null Chunk
        /// </summary>
        ~ChunkCapacityAllocationT()
        {
            checkf(!this->IsVoidData(), TEXT("A VoidData Chunk is being destructed. Data cannot be destructed and freed without a structure. Use force_structure."));
            if(IsData())
                Destroy();
        }

    public:
        using Base_t::operator*;
        using Base_t::operator->;
        using TBase::GetStructure;
        using TBase::GetNodeCount;
        using TBase::GetNodeCapacity;
        using TBase::GetInternalChunk;
        using TBase::IsSameStructure;

        using TBase::IsVoid;
        using TBase::IsStruct;
        using TBase::IsNull;
        using TBase::IsData;
        using TBase::IsVoidNull;
        using TBase::IsVoidData;
        using TBase::IsStructNull;
        using TBase::IsStructData;
    protected:
        using TBase::NodeCapacity;


        void Destroy()
        {
            DestructAndFreeData();
            FreeDataArray();
        }

        void DestructAndFreeData()
        {
            auto& chunk = GetInternalChunk(*this);

            auto componentCount = chunk.Structure->GetComponentCount();
            for (Size_t i = 0; i < componentCount; ++i)
            {
                const ComponentType_t& componentType = *chunk.Structure->Components[i];
                Node_t::DestructComponentUnsafe(componentType, chunk.ComponentData[i], 0, chunk.NodeCount);
                pnc_free_clean(chunk.ComponentData[i], componentType.GetSize(NodeCapacity), componentType.GetAlignment());
            }
        }
        void FreeDataArray()
        {
            auto& chunk = GetInternalChunk(*this);
            pnc_free_clean(chunk.ComponentData, chunk.Structure->Components.GetSize() * sizeof(void*), alignof(void*));
        }









        void AllocateAndConstructData()
        {
            auto& chunk = GetInternalChunk(*this);
            assert_pnc(!chunk.IsNull());
            auto componentCount = chunk.Structure->Components.GetSize();
            Size_t nodeCapacity = GetNodeCapacity();
            if (chunk.NodeCount == 0)
                for (Size_t i = 0; i < componentCount; ++i)
                {
                    const ComponentType_t& componentType = *chunk.Structure->Components[i];
                    chunk.ComponentData[i] = (void*)pnc_alloc(componentType.GetSize(nodeCapacity), componentType.GetAlignment());
                }
            else
                for (Size_t i = 0; i < componentCount; ++i)
                {
                    const ComponentType_t& componentType = *chunk.Structure->Components[i];
                    chunk.ComponentData[i] = (void*)pnc_alloc(componentType.GetSize(nodeCapacity), componentType.GetAlignment());
                    Node_t::ConstructComponentUnsafe(componentType, chunk.ComponentData[i], 0, chunk.NodeCount);
                }
        }

        static void AllocateDataCopy(Self_t& chunkTo, const Self_t& chunkFrom)
        {
            assert_pnc(!chunkTo.IsNull());
            assert_pnc(!chunkFrom.IsNull());
            assert_pnc(IsSameStructure(chunkTo, chunkFrom));

            const ChunkStructure_t& structure = chunkTo.GetStructure();
            const Size_t componentCount = structure.Components.GetSize();
            const Size_t nodeCapacity = chunkFrom.GetNodeCapacity();
            const Size_t nodeCount = chunkFrom.GetNodeCount();

            void**const componentDataArrayTo = GetInternalChunk(chunkTo).ComponentData;
            for (Size_t i = 0; i < componentCount; ++i)
            {
                const ComponentType_t& componentType = *structure.Components[i];
                componentDataArrayTo[i] = (void*)pnc_alloc(componentType.GetSize(nodeCapacity), componentType.GetAlignment());
                Node_t::CopyComponentForwardUnsafe(componentType, componentDataArrayTo[i], 0, chunkFrom.GetComponentData(i), 0, nodeCount);
            }
        }


        void AllocateDataArray()
        {
            auto& chunk = GetInternalChunk(*this);
            chunk.ComponentData = (void**)pnc_alloc(chunk.Structure->Components.GetSize() * sizeof(void*), alignof(void*));
        }


        void CopyDataArray(void** componentData)const
        {
            auto& chunk = GetInternalChunk(*this);
            auto componentCount = chunk.Structure->Components.GetSize();
            for (Size_t i = 0; i < componentCount; ++i)
            {
                chunk.ComponentData[i] = componentData[i];
            }
        }
    };
}
