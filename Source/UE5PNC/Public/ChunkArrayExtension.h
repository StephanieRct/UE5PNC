// MIT License
// Copyright (c) 2025 Stephanie Rancourt

#pragma once
#include "common.h"
#include "ChunkPointerInternal.h"

namespace NiT
{
    /// <summary>
    /// Extend a ChunkPointer to include the required data field to point to an array of chunks.
    /// </summary>
    /// <typeparam name="TChunkStructure">Structure of the chunk's component data.</typeparam>
    /// <typeparam name="TChunkPointerElement">Structure of the chunk pointer in the array.</typeparam>
    template<typename TChunkStructure, typename TChunkPointerElement>
    struct ChunkArrayExtensionT
    {
    public:
        using Self = ChunkArrayExtensionT<TChunkStructure, TChunkPointerElement>;
        using ChunkStructure_t = TChunkStructure;
        using ComponentType_t = typename ChunkStructure_t::ComponentType_t;
        using ChunkPointerElement_t = TChunkPointerElement;
        using Size_t = typename ChunkStructure_t::Size_t;
        using ChunkPointerInternal_t = ChunkPointerInternalT<TChunkStructure>;
        using ChunkPointerElementInternal_t = typename ChunkPointerElement_t::ChunkPointerInternal_t;
        
    public:
        /// <summary>
        /// Points to an array of ChunkPointerElement_t with at least ChunkCount elements
        /// </summary>
        ChunkPointerElement_t* Chunks;

        /// <summary>
        /// Number of chunk in the Chunks array
        /// </summary>
        ChunkCountT<Size_t> ChunkCount;

    public:
        template<typename TProps>
        ChunkArrayExtensionT(const DPropsTag& tag, const TProps& props)
            : Chunks(nullptr)
            , ChunkCount(props.GetChunkCount())
        {
        }

        template<typename TProps>
        ChunkArrayExtensionT(const ChunkPointerElement_t*const chunks, const ChunkCountT<Size_t> chunkCount)
            : Chunks(chunks)
            , ChunkCount(chunkCount)
        {
        }

#ifdef NI_MEMORYCLEANUP
        ~ChunkArrayExtensionT()
        {
            ni_clean(Chunks);
            ni_clean(ChunkCount);
        }
#endif

        ChunkCountT<Size_t> GetChunkCount()const { return ChunkCount; }

        const ChunkPointerElement_t& GetChunk(const ChunkPointerInternal_t& chunkArray, Size_t index)const { return Chunks[index]; }
              ChunkPointerElement_t& GetChunk(      ChunkPointerInternal_t& chunkArray, Size_t index)      { return Chunks[index]; }

        const ChunkPointerElementInternal_t& GetInternalChunk(const ChunkPointerInternal_t& chunkArray, Size_t index)const { return ChunkPointerElement_t::GetInternalChunk(Chunks[index]); }
              ChunkPointerElementInternal_t& GetInternalChunk(      ChunkPointerInternal_t& chunkArray, Size_t index)      { return ChunkPointerElement_t::GetInternalChunk(Chunks[index]); }

        /// <summary>
        /// Notes:
        ///     Called by DArray
        /// </summary>
        template<typename TChunk>
        void Allocate(TChunk& chunk, const ChunkCapacityT<Size_t> chunkCapacity, const NodeCapacityT<Size_t> nodeCapacity)
        { 
            Chunks = Allocate(chunkCapacity);
        }

        /// <summary>
        /// Notes:
        ///     Called by DArray
        /// </summary>
        template<typename TChunk>
        void Deallocate(TChunk& chunk)
        {
            const ChunkCapacityT<Size_t> chunkCapacity = chunk.GetChunkCapacity();
            Deallocate(Chunks, chunkCapacity);
        }

        /// <summary>
        /// Notes:
        ///     Called by DArrayPointer
        /// </summary>
        template<typename TChunk>
        void ConstructElement(TChunk& chunk, const Size_t elementIndex, const Size_t firstNodeInArray, const NodeCountT<Size_t> nodeCount)
        {
            typename TChunk::ChunkPointerInternal_t& internalChunk = TChunk::GetInternalChunk(chunk);
            const ChunkStructure_t& structure = chunk.GetStructure();
            const Size_t componentCount = structure.GetComponentCount();
            SetComponentDataArrayValues(structure, internalChunk.ComponentDataArray, componentCount,
                /*elementIndex:*/    elementIndex, 
                /*firstNodeIndex:*/  firstNodeInArray, 
                /*firstChunkIndex:*/ elementIndex);
            void** elementComponentData = &internalChunk.ComponentDataArray[elementIndex * componentCount];
            new(&Chunks[elementIndex]) ChunkPointerElement_t(&structure, nodeCount, elementComponentData);
        }

        /// <summary>
        /// Notes:
        ///     Called by DArray
        ///     chunk and chunkFrom CANNOT be the same
        /// </summary>
        template<typename TChunk>
        void ConstructCopy(TChunk& chunk, const TChunk& chunkFrom, const Self& arrayFrom, const NodeCapacityT<Size_t> newNodeCapacity, const ChunkCapacityT<Size_t> newChunkCapacity)
        {
            ni_assert(IsSameStructure(chunk, chunkFrom));
            ni_assert(!IsSameData(chunk, chunkFrom));
            typename TChunk::ChunkPointerInternal_t& internalChunkTo =   TChunk::GetInternalChunk(chunk);
            typename TChunk::ChunkPointerInternal_t& internalChunkFrom = TChunk::GetInternalChunk(chunkFrom);
            const Size_t chunkCountFrom = chunkFrom.GetChunkCount();
            const ChunkStructure_t* structure = &chunk.GetStructure();
            const Size_t componentCount = structure->GetComponentCount();

            MatchComponentDataArrayValues(internalChunkTo.ComponentDataArray, internalChunkFrom.ComponentDataArray, chunkCountFrom, componentCount);

            for (Size_t iChunk = 0; iChunk < chunkCountFrom; ++iChunk)
            {
                new(&Chunks[iChunk]) ChunkPointerElement_t(arrayFrom.Chunks[iChunk]);
                SetElementComponentDataArray(chunk, Chunks, iChunk, componentCount);
            }
        }

        /// <summary>
        /// Notes:
        ///     Called by DArray
        ///     chunkToReallocate and chunkFrom CANNOT be the same
        /// </summary>
        template<typename TChunk>
        void ReallocateCopy(TChunk& chunkToReallocate, const TChunk& chunkFrom, const Self& arrayFrom, const NodeCapacityT<Size_t> newNodeCapacity, const ChunkCapacityT<Size_t> newChunkCapacity)
        {
            ni_assert(IsSameStructure(chunkToReallocate, chunkFrom));
            ni_assert(!IsSameData(chunkToReallocate, chunkFrom));
            typename TChunk::ChunkPointerInternal_t& internalChunkTo =   TChunk::GetInternalChunk(chunkToReallocate);
            typename TChunk::ChunkPointerInternal_t& internalChunkFrom = TChunk::GetInternalChunk(chunkFrom);
            const Size_t chunkCountTo =    chunkToReallocate.GetChunkCount();
            const Size_t chunkCapacityTo = chunkToReallocate.GetChunkCapacity();
            const Size_t chunkCountFrom =  chunkFrom.GetChunkCount();

            const ChunkStructure_t* structure = &chunkToReallocate.GetStructure();
            const Size_t componentCount = structure->GetComponentCount();
            // if the new chunk capacity is the same, keep the same Chunks memory and copy assign
            if (newChunkCapacity == chunkCapacityTo)
            {
                for (Size_t i = 0; i < chunkCountFrom; ++i)
                {
                    Chunks[i] = arrayFrom.Chunks[i];
                }
            }
            else
            {
                // Alloc
                ChunkPointerElement_t* chunks = Allocate(newChunkCapacity);
                // Copy
                for (Size_t i = 0; i < chunkCountFrom; ++i)
                {
                    new(&chunks[i]) ChunkPointerElement_t(arrayFrom.Chunks[i]);
                    SetElementComponentDataArray(chunkToReallocate, chunks, i, componentCount);
                }

                // Destruct
                for (Size_t i = 0; i < chunkCountTo; ++i)
                    Chunks[i].~ChunkPointerElement_t();
                // Free
                Deallocate(Chunks, chunkCapacityTo);

                Chunks = chunks;
            }
            MatchComponentDataArrayValues(internalChunkTo.ComponentDataArray, internalChunkFrom.ComponentDataArray, chunkCountFrom, componentCount);
        }

        /// <summary>
        /// Notes:
        ///     Called by DArray
        ///     chunkToReallocate and chunkFrom CAN be the same
        /// </summary>
        template<typename TChunk>
        void ReallocateMove(TChunk& chunkToReallocate, const TChunk& chunkFrom, const Self& arrayFrom, const NodeCapacityT<Size_t> newNodeCapacity, const ChunkCapacityT<Size_t> newChunkCapacity)
        {
            ni_assert(IsSameStructure(chunkToReallocate, chunkFrom));
            const Size_t chunkCountTo =    chunkToReallocate.GetChunkCount();
            const Size_t chunkCapacityTo = chunkToReallocate.GetChunkCapacity();
            const Size_t chunkCountFrom =  chunkFrom.GetChunkCount();
            const Size_t componentCount =  chunkToReallocate.GetStructure().GetComponentCount();

            // if the new chunk capacity is the same, keep the same Chunks memory
            if (newChunkCapacity == chunkCapacityTo)
                return;

            // Alloc
            ChunkPointerElement_t* chunks = Allocate(newChunkCapacity);
            // Move
            for (Size_t i = 0; i < chunkCountFrom; ++i)
            {
                new(&chunks[i]) ChunkPointerElement_t(std::move(arrayFrom.Chunks[i]));
                SetElementComponentDataArray(chunkToReallocate, chunks, i, componentCount);
            }
            // TODO
            // Destruct
            for (Size_t i = 0; i < chunkCountTo; ++i)
                Chunks[i].~ChunkPointerElement_t();
            // Free
            Deallocate(Chunks, chunkCapacityTo);

            Chunks = chunks;
        }

        /// <summary>
        /// Notes:
        ///     Called by DArray
        /// </summary>
        template<typename TChunk>
        void Destruct(TChunk& chunk)
        {
            const ChunkCountT<Size_t> chunkCount = chunk.GetChunkCount();
            for (Size_t i = 0; i < chunkCount; ++i)
                Chunks[i].~ChunkPointerElement_t();
        }

    protected:
        ChunkPointerElement_t* Allocate(const ChunkCapacityT<Size_t> chunkCapacity)
        {
            return (ChunkPointerElement_t*)ni_alloc(chunkCapacity * sizeof(ChunkPointerElement_t), alignof(ChunkPointerElement_t));
        }

        void Deallocate(ChunkPointerElement_t*& chunks, const ChunkCapacityT<Size_t> chunkCapacity)
        {
            ni_free_clean(chunks, chunkCapacity * sizeof(ChunkPointerElement_t), alignof(ChunkPointerElement_t));
        }

        void** GetComponentDataArrayForChunk(const Size_t chunkIndex)
        {
            ChunkPointerInternal_t& internalChunk = GetInternalChunk(*this);
            ni_assert(!internalChunk.IsNull());
            return &internalChunk.ComponentDataArray[chunkIndex * internalChunk.Structure->GetComponentCount()];
        }

        /// <summary>
        /// Set all the void* to each component data in a ComponentDataArray (componentDataTo) to the same relative
        /// offsets (from the fist void* components in the same ComponentDataArray) as another ComponentDataArray (componentDataFrom)
        /// </summary>
        void MatchComponentDataArrayValues(void** const componentDataTo, const void** const componentDataFrom, const ChunkCountT<Size_t> chunkCount, const Size_t componentCount)
        {
            for (Size_t i = componentCount; i < chunkCount * componentCount; ++i)
            {
                componentDataTo[i] = (uint8*)componentDataFrom[i] - (uint8*)componentDataFrom[i % componentCount];
            }
        }

        void SetComponentDataArrayValues(const ChunkStructure_t& structure, void** baseComponentData, 
                                         const Size_t componentCount, const Size_t elementIndex, 
                                         const Size_t fistNodeIndex, const Size_t fistChunkIndex)
        {
            void** elementComponentData = baseComponentData + elementIndex * componentCount;
            for (Size_t iComp = 0; iComp < componentCount; ++iComp)
            {
                const ComponentType_t& componentType = structure.GetComponentType(iComp);
                elementComponentData[iComp] = componentType.Forward(baseComponentData[iComp], 
                                                                    PropNodeCountT< Size_t>(fistNodeIndex), 
                                                                    PropChunkCountT<Size_t>(fistChunkIndex));
            }
        }

        /// <summary>
        /// Set the ComponentDataArray value of a chunk elements in an array of ChunkPointerElement_t to
        /// an offset into the base array chunk ComponentDataArray.
        /// Note:
        ///     The first chunk element in the array has the same ComponentDataArray pointer as the base array chunk ComponentDataArray pointer.
        /// </summary>
        template<typename TChunk>
        void SetElementComponentDataArray(TChunk& chunk, ChunkPointerElement_t* chunks, 
                                          const Size_t chunkIndex, const Size_t componentCount)
        {
            typename TChunk::ChunkPointerInternal_t& internalChunk = TChunk::GetInternalChunk(chunk);
            typename TChunk::ChunkPointerElementInternal_t& internalChunkElement = TChunk::GetInternalChunkElement(chunks[chunkIndex]);
            internalChunkElement.ComponentDataArray = &internalChunk.ComponentDataArray[chunkIndex * componentCount];
        }
    };
}
