#pragma once

#include "CodingameUtility\VectorInPlace.h"

#include "EHeuristicsType.h"

namespace SC2020
{
    constexpr int MAX_HEURISTICS_PER_STARTEGY = 4;

    struct SStrategy
    {
        struct SHeuristicsEntry
        {
            float m_scoreMultiplier = 1.0f;
            EHeuristicsType m_heuristicsType = EHeuristicsType::Default;
        };

        CVectorInPlace<SHeuristicsEntry, MAX_HEURISTICS_PER_STARTEGY> m_heuristics;
    };

    inline SStrategy::SHeuristicsEntry CreateHeuristicsEntry(EHeuristicsType const type, float const scoreMultiplier)
    {
        SStrategy::SHeuristicsEntry entry;
        entry.m_heuristicsType = type;
        entry.m_scoreMultiplier = scoreMultiplier;
        return entry;
    }
}