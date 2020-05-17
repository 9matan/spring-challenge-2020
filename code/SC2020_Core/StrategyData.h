#pragma once

#include "CodingameUtility\VectorInPlace.h"

#include "GameConfig.h"

namespace SC2020
{
    class CBotImpl;

    struct SStrategyData
    {
        CBotImpl const* m_bot;
        CVectorInPlace<unsigned int, MAX_PACS_CNT_PER_PLAYER> m_pacIdsOrder;
    };
}