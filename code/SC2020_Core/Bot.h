#pragma once

#include "CodingameUtility\Vec2.h"
#include "CodingameUtility\VectorInPlace.h"

#include "BotData.h"
#include "OutputData.h"

namespace SC2020
{
    struct SInitInputData;
    struct SInputData;

    class CBot
    {
    public:
        CBot(SInitInputData const& initInData);

        SOutputData FirstUpdate(SInputData const& inData);
        SOutputData Update(SInputData const& inData);

    private:
        CVectorInPlace<SVec2, MAX_MAP_AREA> m_floorCells;
        SBotData m_data;
    };
}