#include "Core_PCH.h"
#include "HeuristicsHelper.h"

#include "Heuristics_Default.h"
#include "Heuristics_GrabSuper.h"

namespace SC2020
{
    SHeuristicsEvalResult EvaluateHeuristics(EHeuristicsType const heuristicsType, SHeuristicsData const& data)
    {
        SHeuristicsEvalResult result;
        switch (heuristicsType)
        {
        case EHeuristicsType::Default:
            result = EvaluateHeuristics_Default(data);
            break;
        case EHeuristicsType::GrabSuper:
            result = EvaluateHeuristics_GrabSuper(data);
            break;
        default:
            assert(true);
            break;
        }
        return result;
    }
}