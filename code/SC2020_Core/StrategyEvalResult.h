#pragma once

#include "EHeuristicsType.h"
#include "GameConfig.h"
#include "PacGoal.h"

namespace SC2020
{
    struct SStrategyEvalResult
    {
        struct SPacGoalEntry
        {
            SVec2 m_pacGoalPos;
            unsigned char m_pacId = -1;
            // higher - more important
            EHeuristicsType m_heuristicsType;
        };

        float m_totalScore = 0.0f;
        CVectorInPlace<SPacGoalEntry, MAX_PACS_CNT_PER_PLAYER> m_pacGoals;
    };
}