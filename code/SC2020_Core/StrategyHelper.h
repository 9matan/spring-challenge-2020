#pragma once

#include "StrategyEvalResult.h"

namespace SC2020
{
    struct SStrategy;
    struct SStrategyData;

    SStrategyEvalResult EvaluateStrategy(SStrategy const& strategy, SStrategyData const& strategyData);
}