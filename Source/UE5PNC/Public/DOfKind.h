// MIT License
// Copyright (c) 2025 Stephanie Rancourt

#pragma once
#include "common.h"
#include "DKind.h"

namespace NiT
{
    /// <summary>
    /// Force a compile-time ContainerKind value.
    /// The most derived decorator of the same type will take precedence
    /// </summary>
    template<ContainerKind TKind, typename TBase>
    struct DOfKind : public TBase
    {
    public:
        using Base_t = TBase;
        using Self_t = DOfKind<TKind, TBase>;
        using ChunkStructure_t = Base_t::ChunkStructure_t;

        DOfKind()
            :Base_t(TKind)
        {
        }

    protected:
        template<typename TProps>
        NI_DEBUG_NOINLINE DOfKind(const DPropsTag& tag, const TProps& props)
            : Base_t(tag, AppendPropSingle(props, TKind))
        {
        }
    };
}
