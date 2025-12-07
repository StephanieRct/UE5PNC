// MIT License
// Copyright (c) 2025 Stephanie Rancourt

#pragma once
#include "common.h"

namespace NiT
{
    /// <summary>
    /// Adds ownership of the allocated/freed ComponentDataArray and component memory.
    /// </summary>
    /// <typeparam name="TBase"></typeparam>
    template<typename TBase>
    struct DOwn : public TBase
    {
    public:
        using Base_t = TBase;
        using Self_t = DOwn<TBase>;
        using typename Base_t::Size_t;
        using typename Base_t::Node_t;
        using typename Base_t::ComponentType_t;
        using typename Base_t::ChunkStructure_t;
        using typename Base_t::ChunkPointerInternal_t;
        using typename Base_t::ChunkPointer_t;

    public:
        DOwn() = default;

        DOwn(Self_t&& containerFrom) = default;
        Self_t& operator=(Self_t&& containerFrom)
        {
            if (this == &containerFrom)
                return *this;
            ni_assertf(!IsVoidData(), TEXT("Cannot move over a VoidData Chunk without the Structure."));
            if (IsData())
            {
                FreeDestruct(*this);
                FreeComponentDataArray(*this);
            }
            Base_t::operator=(std::forward<Self_t>(containerFrom));
            return *this;
        }

        /// <summary>
        /// Copy a Chunk and its Component data.
        /// The result chunk will have the same node count and capacity as the original.
        /// </summary>
        /// <param name="o"></param>
        DOwn(const Self_t& containerFrom)
            : Base_t(containerFrom)
        {
            ni_assertf(!containerFrom.IsVoidData(), TEXT("Cannot copy a VoidData Chunk. Data cannot be copied without a structure."));
            //TODO construct from null chunk
            if (containerFrom.IsNull())
                return;
            AllocateComponentDataArray(*this);
            AllocateCopy(*this, containerFrom);
        }

        /// <summary>
        /// Copy a Chunk and its Component data.
        /// It will deallocate any previous data.
        /// The result Chunk will have the same node count and capacity as the one being copied.
        /// </summary>
        /// <param name="o"></param>
        Self_t& operator=(const Self_t& containerFrom)
        {
            if (this == &containerFrom)
                return *this;

            ni_assertf(!containerFrom.IsVoidData(), TEXT("Cannot copy a VoidData Chunk without the Structure."));
            ni_assertf(!this->IsVoidData(), TEXT("Cannot copy over a VoidData Chunk without the Structure."));

            if (IsData())
            {
                if (containerFrom.IsData() && IsSameStructure(*this, containerFrom))
                {
                    ReallocateCopy(*this, containerFrom);
                    void** const keepComponentDataArray = GetInternalChunk(*this).ComponentDataArray;
                    Base_t::operator=(containerFrom);
                    GetInternalChunk(*this).ComponentDataArray = keepComponentDataArray;
                    return *this;
                }
                else
                {
                    FreeDestruct(*this);
                    FreeComponentDataArray(*this);
                }
            }
            Base_t::operator=(containerFrom);
            if (containerFrom.IsData())
            {
                AllocateComponentDataArray(*this);
                AllocateCopy(*this, containerFrom);
            }

            return *this;
        }
        
    public:
        using Base_t::GetNodeCapacity;
        using Base_t::GetChunk;
        using Base_t::IsVoidData;
        using Base_t::IsData;
        using Base_t::IsSameStructure;
        using Base_t::GetInternalChunk;

    protected:
        template<typename TProps>
        DOwn(const DPropsTag& tag, const TProps& props)
            : Base_t(tag, AppendProp(props, PropComponentDataArray(nullptr)))
        {
            AllocateComponentDataArray(*this);
            AllocateConstruct(*this, props);
        }

        template< typename... TArgs>
        DOwn(const ChunkStructure_t* const chunkStructure, TArgs&&... args)
            : DOwn(DPropsTag(), MakeProps<Size_t>(chunkStructure, (TArgs...)args...))
        {
        }

        /// <summary>
        /// Deallocate data if not a Null Chunk
        /// </summary>
        ~DOwn() noexcept(false)
        {
            ni_assertf(!IsVoidData(), TEXT("A VoidData Chunk is being destructed. Data cannot be destructed and freed without a structure. Use force_structure."));
            if (IsData())
            {
                FreeDestruct(*this);
                FreeComponentDataArray(*this);
            }
        }

        using Base_t::AllocateComponentDataArray;
        using Base_t::FreeComponentDataArray;
        using Base_t::AllocateConstruct;
        using Base_t::AllocateCopy;
        using Base_t::ReallocateCopy;
        using Base_t::ReallocateMove;
        using Base_t::FreeDestruct;
    };
}
