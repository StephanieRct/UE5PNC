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
        /// Create a VoidNull Chunk
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
        ChunkCapacityAllocationT(const ChunkStructure_t* chunkStructure, Size_t nodeCapacity, Size_t nodeCount)
            : Base_t(chunkStructure, nodeCapacity, nodeCount)
        {
            AllocateDataArray();
            AllocateAndConstructData();
        }

        ChunkCapacityAllocationT(const ChunkStructure_t* chunkStructure, Size_t nodeCapacity)
            : Base_t(chunkStructure, nodeCapacity)
        {
            AllocateDataArray();
            AllocateAndConstructData();
        }

        ChunkCapacityAllocationT(Self_t&& chunkFrom) = default;
        Self_t& operator=(Self_t&& chunkFrom)
        {
            if (this == &chunkFrom)
                return *this;
            pnc_assertf(!IsVoidData(), TEXT("Cannot move over a VoidData Chunk without the Structure."));
            if (IsData())
                Destroy();
            Base_t::operator=(std::forward<Self_t>(chunkFrom));
            return *this;
        }

        /// <summary>
        /// Copy a Chunk and its Component data.
        /// The result chunk will have the same node count and capacity as the original.
        /// </summary>
        /// <param name="o"></param>
        ChunkCapacityAllocationT(const Self_t& chunkFrom)
            : Base_t(chunkFrom)
        {
            pnc_assertf(!chunkFrom.IsVoidData(), TEXT("Cannot copy a VoidData Chunk. Data cannot be copied without a structure."));
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

            pnc_assertf(!chunkFrom.IsVoidData(), TEXT("Cannot copy a VoidData Chunk without the Structure."));
            pnc_assertf(!this->IsVoidData(), TEXT("Cannot copy over a VoidData Chunk without the Structure."));

            if (IsData())
            {
                const Size_t nodeCountTo = GetNodeCount();
                const Size_t nodeCountFrom = chunkFrom.GetNodeCount();
                if (chunkFrom.IsData() && GetNodeCapacity() >= nodeCountFrom)
                {
                    if (IsSameStructure(*this, chunkFrom))
                    {
                        // TODO Investigate if calling the copy assignement on the component data that overlap would slow speed down or not
                        //if (nodeCountTo > nodeCountFrom)
                        //{
                        //    const Size_t nodeCountToCopyAssign = nodeCountFrom;
                        //    const Size_t nodeCountToDestroy = nodeCountTo - nodeCountFrom;
                        //}
                        //else if (nodeCountTo < nodeCountFrom)
                        //{
                        //    const Size_t nodeCountToCopyAssign = nodeCountTo;
                        //    const Size_t nodeCountToCopyConstruct = nodeCountTo - nodeCountFrom;
                        //}
                        //else
                        //{
                        //    // copy assign all nodes
                        //}
                        auto& internalChunk = GetInternalChunk(*this);
                        Node_t::DestructAllComponentsUnsafe(internalChunk, 0, internalChunk.NodeCount);
                        auto keepComponentDataArray = internalChunk.ComponentData;
                        Base_t::operator=(chunkFrom);
                        internalChunk.ComponentData = keepComponentDataArray;
                        Node_t::CopyConstructAllComponentsForwardUnsafe(*this, 0, 0, chunkFrom, 0, 0, chunkFrom.GetNodeCount(), 1);
                        return *this;
                    }
                    // TODO investigate if Restructuring could be done here. Would reuse the existing bufffers for common components
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
                AllocateDataArray();
                AllocateDataCopy(*this, chunkFrom);
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

        //pnc_assert(IsSameStructure(*this, chunkFrom));
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
        ~ChunkCapacityAllocationT() noexcept(false)
        {
            pnc_assertf(!this->IsVoidData(), TEXT("A VoidData Chunk is being destructed. Data cannot be destructed and freed without a structure. Use force_structure."));
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


        void Destroy()
        {
            DestructAndFreeData();
            FreeDataArray();
        }

        void DestructAndFreeData()
        {
            auto& chunk = GetInternalChunk(*this);

            auto componentCount = chunk.Structure->GetComponentCount();
            Size_t capacity = GetNodeCapacity();
            for (Size_t i = 0; i < componentCount; ++i)
            {
                const ComponentType_t& componentType = *chunk.Structure->Components[i];
                componentType.DestructComponentUnsafe(chunk.ComponentData[i], 0, chunk.NodeCount);
                pnc_free_clean(chunk.ComponentData[i], componentType.GetSize(capacity), componentType.GetAlignment());
            }
        }
        void FreeDataArray()
        {
            auto& chunk = GetInternalChunk(*this);
            pnc_free_clean(chunk.ComponentData, chunk.Structure->Components.GetSize() * sizeof(void*), alignof(void*));
        }


        void AllocateAndConstructData()
        {
            auto& internalChunk = GetInternalChunk(*this);
            pnc_assert(!internalChunk.IsNull());
            if (internalChunk.NodeCount == 0)
            {
                //Construct only ChunkComponents
            }
            auto componentCount = internalChunk.Structure->Components.GetSize();
            Size_t nodeCapacity = GetNodeCapacity();
            for (Size_t i = 0; i < componentCount; ++i)
            {
                const ComponentType_t& componentType = *internalChunk.Structure->Components[i];
                internalChunk.ComponentData[i] = (void*)pnc_alloc(componentType.GetSize(nodeCapacity), componentType.GetAlignment());
                componentType.ConstructComponentUnsafe(internalChunk.ComponentData[i], 0, internalChunk.NodeCount);
            }
        }

        static void AllocateDataCopy(Self_t& chunkTo, const Self_t& chunkFrom)
        {
            pnc_assert(!chunkTo.IsNull());
            pnc_assert(!chunkFrom.IsNull());
            pnc_assert(IsSameStructure(chunkTo, chunkFrom));

            const ChunkStructure_t& structure = chunkTo.GetStructure();
            const Size_t componentCount = structure.GetComponentCount();
            const Size_t nodeCapacity = chunkFrom.GetNodeCapacity();
            const Size_t nodeCount = chunkFrom.GetNodeCount();

            void**const componentDataArrayTo = GetInternalChunk(chunkTo).ComponentData;
            for (Size_t i = 0; i < componentCount; ++i)
            {
                const ComponentType_t& componentType = *structure.Components[i];
                componentDataArrayTo[i] = (void*)pnc_alloc(componentType.GetSize(nodeCapacity), componentType.GetAlignment());
                componentType.CopyConstructComponentForwardUnsafe(
                    componentDataArrayTo[i],       0, 0,
                    chunkFrom.GetComponentData(i), 0, 0, nodeCount, 1);
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
