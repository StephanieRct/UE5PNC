// MIT License
// Copyright (c) 2025 Stephanie Rancourt

#pragma once
#include "common.h"

namespace PNC
{
    template<typename TBase>
    struct DOwningChunkT : public TBase
    {
    public:
        using Base_t = TBase;
        using Self_t = DOwningChunkT<TBase>;
        using typename Base_t::ChunkStructure_t;
        using typename Base_t::Chunk_t;
        using typename Base_t::Size_t;
        using typename Base_t::Node_t;
        using ComponentType_t = typename ChunkStructure_t::ComponentType_t;

    protected:
        using ChunkPointerInternal_t = ChunkPointerT<ChunkStructure_t>::ChunkPointerInternal_t;

    public:
        /// <summary>
        /// Create a VoidNull Chunk
        /// </summary>
        DOwningChunkT() = default;


        /// <summary>
        /// Create a Chunk of a given ChunkStructure and allocate the Component's memory
        /// The Components' memory can fit as many instances of each Components as the Chunk's capacity.
        /// Any computation performed on this Chunk will only process node within the chunk's Node count and not it's capacity.
        /// </summary>
        /// <param name="chunkStructure">Structure of the Chunk's component data.</param>
        /// <param name="nodeCapacity">Maximum number of Nodes this Chunk can grow to.</param>
        /// <param name="nodeCount"></param>
        DOwningChunkT(const ChunkStructure_t* chunkStructure, Size_t nodeCapacity, Size_t nodeCount)
            : Base_t(chunkStructure, nodeCapacity, nodeCount)
        {
            AllocateDataArray(*this);
            AllocateAndConstructData(*this, nodeCapacity, nodeCount);
        }

        DOwningChunkT(const ChunkStructure_t* chunkStructure, Size_t nodeCapacity)
            : Base_t(chunkStructure, nodeCapacity)
        {
            AllocateDataArray(*this);
            AllocateAndConstructData(*this, nodeCapacity, nodeCapacity);
        }

        DOwningChunkT(Self_t&& chunkFrom) = default;
        Self_t& operator=(Self_t&& chunkFrom)
        {
            if (this == &chunkFrom)
                return *this;
            pnc_assertf(!IsVoidData(), TEXT("Cannot move over a VoidData Chunk without the Structure."));
            if (IsData())
                Destroy(*this);
            Base_t::operator=(std::forward<Self_t>(chunkFrom));
            return *this;
        }

        /// <summary>
        /// Copy a Chunk and its Component data.
        /// The result chunk will have the same node count and capacity as the original.
        /// </summary>
        /// <param name="o"></param>
        DOwningChunkT(const Self_t& chunkFrom)
            : Base_t(chunkFrom)
        {
            pnc_assertf(!chunkFrom.IsVoidData(), TEXT("Cannot copy a VoidData Chunk. Data cannot be copied without a structure."));
            //TODO construct from null chunk
            if (chunkFrom.IsNull())
                return;
            AllocateDataArray(*this);
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
                if (chunkFrom.IsData() && IsSameStructure(*this, chunkFrom))
                {
                    ReallocateDataCopy(*this, chunkFrom);
                    void** const keepComponentDataArray = GetInternalChunk(*this).ComponentData;
                    Base_t::operator=(chunkFrom);
                    GetInternalChunk(*this).ComponentData = keepComponentDataArray;
                    return *this;
                }
                else
                {
                    Destroy(*this);
                }
            }
            Base_t::operator=(chunkFrom);
            if (chunkFrom.IsData())
            {
                AllocateDataArray(*this);
                AllocateDataCopy(*this, chunkFrom);
            }

            return *this;
        }
        
        /// <summary>
        /// Deallocate data if not a Null Chunk
        /// </summary>
        ~DOwningChunkT() noexcept(false)
        {
            pnc_assertf(!this->IsVoidData(), TEXT("A VoidData Chunk is being destructed. Data cannot be destructed and freed without a structure. Use force_structure."));
            if(IsData())
                Destroy(*this);
        }

    public:
        PNC_USING_CHUNKPOINTER_INTERFACE();
        using Base_t::GetNodeCapacity;
        PNC_IMPLEMENT_CHUNKPOINTER_SELFPOINTER();
    protected:


        using Base_t::AllocateDataArray;
        using Base_t::AllocateDataCopy;
        using Base_t::ReallocateDataCopy;
        using Base_t::ReallocateDataMove;
        using Base_t::AllocateAndConstructData;
        using Base_t::Destroy;


    };
}
