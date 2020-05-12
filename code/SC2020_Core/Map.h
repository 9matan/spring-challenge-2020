#pragma once

#include <assert.h>

#include "Cell.h"
#include "GameConfig.h"
#include "GameGrid2D.h"

namespace SC2020
{
    struct SMap : public SGameGrid2D<SCell>
    {
    public:
        using SGameGrid2D<SCell>::SGameGrid2D;

        inline SCell const& GetCell(SVec2 const pos) const { return GetElement(pos); }
        inline SCell& GetCell(SVec2 const pos) { return GetElement(pos); }
    };
}