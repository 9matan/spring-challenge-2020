#pragma once

#include "CodingameUtility\Vec2.h"
#include "CodingameUtility\VectorInPlace.h"

#include "EHeuristicsType.h"

namespace SC2020
{
    struct SPacGoal
    {
        SVec2 m_goalPos = SVec2(-1, -1);
        // higher - more importance
        EHeuristicsType m_heuristicsType = EHeuristicsType::Default;
    };
}