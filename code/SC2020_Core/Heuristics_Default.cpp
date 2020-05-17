#include "Core_PCH.h"
#include "Heuristics_Default.h"

#include "BotImpl.h"
#include "HeuristicsData.h"
#include "StrategyExecutionContext.h"

using namespace std;

namespace SC2020
{
    SHeuristicsEvalResult EvaluateHeuristics_Default(SHeuristicsData const& data)
    {
        SHeuristicsEvalResult result;
        auto const& pac = data.m_bot->GetPacEntity(data.m_pacId, true);

        auto const paths = data.m_bot->GetPathfinding().GetPathsToAdjacentCrossroads(pac.m_pos, data.m_context->m_lockedPositions);

        SVec2 bestGoalPos(-1, -1);
        float bestScore = -1.0f;
        auto const& map = data.m_bot->GetMap();
        for (auto const& path: paths)
        {
            float curScore = 0.0f;
            for (auto const pos: path)
            {
                curScore += map.GetCell(pos).m_pelletScore;
            }
            curScore /= path.size();

            if (curScore > bestScore)
            {
                bestScore = curScore;
                bestGoalPos = path.back();
            }
        }

        result.m_pacGoalPos = bestGoalPos;
        result.m_totalScore = bestScore;
        result.m_success = true;
        return result;
    }
}