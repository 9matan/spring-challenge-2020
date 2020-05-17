#pragma once

#include "EHeuristicsType.h"
#include "HeuristicsEvalResult.h"

namespace SC2020
{
    struct SHeuristicsData;

    SHeuristicsEvalResult EvaluateHeuristics(EHeuristicsType const heuristicsType, SHeuristicsData const& data);
}