#pragma once

#include "CodingameUtility\Vec2.h"
#include "CodingameUtility\VectorInPlace.h"

namespace SC2020
{
    enum class EPacGoalType : unsigned char
    {
        GrabSuper = 0,
        Explore
    };

    struct SPacGoal
    {
        EPacGoalType m_type;
        SVec2 m_goalPos;
    };
}