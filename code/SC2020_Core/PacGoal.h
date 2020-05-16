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
        // contains crossroads; the first pos - is the goal, the last pos - the next pos move to
        CVectorInPlace<SVec2, 32> m_reversedPath;
    };
}