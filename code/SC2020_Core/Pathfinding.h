#pragma once

#include <vector>

#include "CodingameUtility\Vec2.h"
#include "CodingameUtility\VectorInPlace.h"

#include "NavmeshEdge.h"
#include "PathfindingConfigs.h"

namespace SC2020
{
    class CNavmesh;
    struct SMap;

    class CPathfinding
    {
    public:
        CPathfinding(SMap const* map, CNavmesh const* navmesh);

        CVectorInPlace<SVec2, MAX_PATHFINDING_PATH_LENGTH> FindNextSteps_ShortestPath(SVec2 const from, SVec2 const to, std::vector<SVec2> const& lockedPositions, unsigned int const maxPathLength) const;

    private:
        SMap const* m_map;
        CNavmesh const* m_navmesh;
    };
}