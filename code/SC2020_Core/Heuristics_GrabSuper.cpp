#include "Core_PCH.h"
#include "Heuristics_Default.h"

#include "BotImpl.h"
#include "HeuristicsData.h"
#include "StrategyExecutionContext.h"

namespace SC2020
{
    SHeuristicsEvalResult EvaluateHeuristics_GrabSuper(SHeuristicsData const& data)
    {
        SHeuristicsEvalResult result;
        result.m_success = false;
        if (data.m_context->m_superPellets.empty())
        {            
            return result;
        }

        auto const& pac = data.m_bot->GetPacEntity(data.m_pacId, true);
        auto const& pathfinding = data.m_bot->GetPathfinding();
        auto const closestDistanceInfo = pathfinding.FindClosestPosition(pac.m_pos, data.m_context->m_superPellets, data.m_context->m_lockedPositions);
        if (!pathfinding.IsReachableDistance(closestDistanceInfo.m_distance))
        {
            return result;
        }

        result.m_success = true;
        result.m_totalScore = (float)-closestDistanceInfo.m_distance;
        result.m_pacGoalPos = closestDistanceInfo.m_pos;
        return result;
    }
}