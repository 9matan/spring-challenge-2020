#pragma once

#include <memory>
#include <vector>

#include "CodingameUtility\Vec2.h"

#include "GameConfig.h"
#include "PacEntity.h"

namespace SC2020
{
    struct SMap;
    class CNavmesh;

    struct SBotData
    {
        unsigned int m_turnIndex = 0;
        std::unique_ptr<SMap> m_map;
        std::unique_ptr<CNavmesh> m_navmesh;
        std::vector<SVec2> m_superPelletPos;
        CVectorInPlace<SPacEntity, MAX_PACS_CNT_PER_PLAYER* MAX_PLAYERS_CNT> m_pacs;
    };
}