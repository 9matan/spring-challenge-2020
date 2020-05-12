#pragma once

#include "CodingameUtility\SGrid2DInPlace.h"

#include "GameConfig.h"

namespace SC2020
{
    template<typename TElem>
    using SGameGrid2D = SGrid2DInPlace<TElem, MAX_MAP_WIDTH, MAX_MAP_HEIGHT>;
}