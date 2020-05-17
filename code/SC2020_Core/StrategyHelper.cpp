#include "Core_PCH.h"
#include "StrategyHelper.h"

#include "CodingameUtility\ContainerHelper.h"

#include "BotImpl.h"
#include "HeuristicsData.h"
#include "HeuristicsHelper.h"
#include "Strategy.h"
#include "StrategyData.h"
#include "StrategyExecutionContext.h"

namespace SC2020
{
    namespace
    {
        SStrategyEvaluationContext CreateEvaluationContext(SStrategyData const& strategyData)
        {
            SStrategyEvaluationContext context;
            context.m_lockedPositions.reserve(32);
            for (auto const& pac : strategyData.m_bot->GetPacs())
            {
                if (!pac.m_isAlive) continue;
                context.m_lockedPositions.push_back(pac.m_pos);
            }
            auto const& map = strategyData.m_bot->GetMap();
            auto const& initSuperPositions = strategyData.m_bot->GetInitSuperPelletPos();
            context.m_superPellets.reserve(initSuperPositions.size());
            for (auto const superPos : initSuperPositions)
            {
                if (map.GetCell(superPos).m_pelletScore > 0.0f)
                {
                    context.m_superPellets.push_back(superPos);
                }
            }
            return context;
        }
    }

    SStrategyEvalResult EvaluateStrategy(SStrategy const& strategy, SStrategyData const& strategyData)
    {
        SStrategyEvaluationContext context = CreateEvaluationContext(strategyData);

        SStrategyEvalResult result;
        result.m_totalScore = 0.0f;
        for (auto const pacId : strategyData.m_pacIdsOrder)
        {
            assert(pacId >= 0);
            auto const pacPos = strategyData.m_bot->GetPacEntity(pacId, true).m_pos;
            EraseSwapWithLast(context.m_lockedPositions, pacPos);

            for (auto const heuristicsEntry : strategy.m_heuristics)
            {
                SHeuristicsData heuristicsData;
                heuristicsData.m_bot = strategyData.m_bot;
                heuristicsData.m_context = &context;
                heuristicsData.m_pacId = pacId;
                auto const heuristicsResult = EvaluateHeuristics(heuristicsEntry.m_heuristicsType, heuristicsData);

                if (!heuristicsResult.m_success) continue;

                result.m_totalScore += heuristicsResult.m_totalScore * heuristicsEntry.m_scoreMultiplier;
                SStrategyEvalResult::SPacGoalEntry goalEntry;
                goalEntry.m_heuristicsType = heuristicsEntry.m_heuristicsType;
                goalEntry.m_pacGoalPos = heuristicsResult.m_pacGoalPos;
                goalEntry.m_pacId = pacId;
                result.m_pacGoals.push_back(goalEntry);
            }

            context.m_lockedPositions.push_back(pacPos);
        }
        return result;
    }
}