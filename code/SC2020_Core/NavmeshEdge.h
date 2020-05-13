#pragma once

#include <vector>

#include "CodingameUtility\Vec2.h"

namespace SC2020
{
    struct SNavmeshEdge
    {
        using Path = std::vector<SVec2>;

        Path m_path;

        SVec2 GetAPos() const { return m_path.front(); }
        SVec2 GetBPos() const { return m_path.back(); }
    };
}