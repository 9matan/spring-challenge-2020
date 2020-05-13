#pragma once

#include <memory>

namespace SC2020
{
    struct SMap;
    class CNavmesh;

    struct SBotData
    {
        std::unique_ptr<SMap> m_map;
        std::unique_ptr<CNavmesh> m_navmesh;
    };
}