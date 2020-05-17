#pragma once

#include <vector>

#include "CodingameUtility\Vec2.h"

namespace SC2020
{
    struct SStrategyEvaluationContext
    {
        std::vector<SVec2> m_lockedPositions;
        std::vector<SVec2> m_superPellets;
    };
}