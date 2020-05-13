#pragma once

#include "CodingameUtility/Vec2.h"
#include "CodingameUtility/VectorInPlace.h"

namespace SC2020
{
    struct SCell
    {
        CVectorInPlace<SVec2, 4> m_adjacentCells;
        float m_pelletScore = 0.0f;
    };
}