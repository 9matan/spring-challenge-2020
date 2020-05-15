#pragma once

#include <memory>
#include <vector>

#include "CodingameUtility\Vec2.h"

namespace SC2020
{
    struct SMap;
    class CNavmesh;

    struct SBotData
    {
        std::unique_ptr<SMap> m_map;
        std::unique_ptr<CNavmesh> m_navmesh;
        std::vector<SVec2> m_superPelletPos;
    };
}