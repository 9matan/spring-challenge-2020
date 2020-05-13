#pragma once

#include "CodingameUtility\VectorInPlace.h"

namespace SC2020
{
    struct SNavmeshNode
    {
        CVectorInPlace<short, 4> m_edgeIds;
    };
}