#pragma once

namespace SC2020
{
    class CBotImpl;
    struct SStrategyEvaluationContext;

    struct SHeuristicsData
    {
        CBotImpl const* m_bot = nullptr;
        SStrategyEvaluationContext* m_context = nullptr;
        unsigned char m_pacId = -1;
    };
}