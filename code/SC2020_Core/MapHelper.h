#pragma once

#include "Map.h"

namespace SC2020
{
    struct SInputDataMap;

    SMap BuildMap(SInputDataMap const& inDataMap);
    CVectorInPlace<SVec2, MAX_MAP_AREA> GetVisibleCells(SMap const& map, SVec2 const origin);
}