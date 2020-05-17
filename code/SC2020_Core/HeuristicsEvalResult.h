#pragma once

#include "GameConfig.h"
#include "PacGoal.h"

namespace SC2020
{
    struct SHeuristicsEvalResult
    {
        float m_totalScore = 0.0f;
        SVec2 m_pacGoalPos;
        bool m_success = false;
    };
}