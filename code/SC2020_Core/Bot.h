#pragma once

#include "CodingameUtility\Vec2.h"
#include "CodingameUtility\VectorInPlace.h"

#include "OutputData.h"

namespace SC2020
{
    struct SInitInputData;
    struct SInputData;

    class CBot
    {
    public:
        SOutputData FirstUpdate(SInitInputData const& initInData, SInputData const& inData);
        SOutputData Update(SInputData const& inData);

    private:
        CVectorInPlace<SVec2, MAX_MAP_AREA> m_floorCells;
    };
}