#include "Core_PCH.h"
#include "Pathfinding.h"

#include "Map.h"
#include "Navmesh.h"

using namespace std;

namespace SC2020
{
    namespace
    {
        struct SBFSNode
        {
            unsigned short m_distance = MAX_MAP_AREA + 1;
            bool m_isUsed = false;
        };

        SGameGrid2D<SBFSNode> RunBFS(SVec2 const from, SVec2 const to, std::vector<SVec2> const& lockedPositions, SMap const* map)
        {
            assert(map->IsValidPos(from));
            SGameGrid2D<SBFSNode> bfsCells(map->GetWidth(), map->GetHeight());
            for (auto const lockedPos : lockedPositions)
            {
                if (lockedPos == to)
                {
#ifdef DEBUG_WARNINGS_ENABLED
                    cerr << "[WARNING] CPathfinding::FindNextSteps_ClosestPath target pos is locked!\n";
#endif // DEBUG_WARNINGS_ENABLED
                    continue;
                }

                SBFSNode node;
                node.m_isUsed = true;
                bfsCells.SetElement(lockedPos, node);
            }

            // BFS
            {
                SBFSNode node;
                node.m_distance = 0;
                node.m_isUsed = true;
                bfsCells.SetElement(from, node);

                queue<SVec2> q;
                q.push(from);
                while (!q.empty())
                {
                    auto const curPos = q.front();
                    q.pop();
                    auto const curNode = bfsCells.GetElementCopy(curPos);
                    for (auto const nextPos : map->GetCell(curPos).m_adjacentCells)
                    {
                        auto nextNode = bfsCells.GetElementCopy(nextPos);
                        if (nextNode.m_isUsed) continue;

                        nextNode.m_isUsed = true;
                        nextNode.m_distance = curNode.m_distance + 1;
                        bfsCells.SetElement(nextPos, nextNode);
                        q.push(nextPos);

                        // we foound a way
                        if (nextPos == to)
                        {
                            queue<SVec2> emptyQ;
                            swap(emptyQ, q);
                        }
                    }
                }
            }
            return bfsCells;
        }
    }

    CPathfinding::CPathfinding(SMap const* map, CNavmesh const* navmesh)
        : m_map(map)
        , m_navmesh(navmesh)
    {
    }

    CVectorInPlace<SVec2, MAX_PATHFINDING_PATH_LENGTH> CPathfinding::FindNextSteps_ShortestPath(SVec2 const from, SVec2 const to, std::vector<SVec2> const& lockedPositions, unsigned int const maxPathLength) const
    {
#ifdef DEBUG_VALIDATE_ERRORS
        if (maxPathLength > MAX_PATHFINDING_PATH_LENGTH)
        {
            cerr << "[ERROR] CPathfinding::FindNextSteps_ClosestPath maxPathLength > MAX_PATHFINDING_PATH_LENGTH\n";
        }
#endif // #ifdef DEBUG_VALIDATE_ERRORS
        auto const bfsCells = RunBFS(to, from, lockedPositions, m_map);

        CVectorInPlace<SVec2, MAX_PATHFINDING_PATH_LENGTH> path;
        SVec2 curPos = from;
        unsigned int length = 0;
#ifdef DEBUG_VALIDATE_ERRORS
        while (curPos != to && length < maxPathLength && !path.is_full())
#else
        while (curPos != to && length < maxPathLength)
#endif
        {
            SVec2 curNextPos = curPos;
            for (auto const nextPos : m_map->GetCell(curPos).m_adjacentCells)
            {
                auto const nextNode = bfsCells.GetElementCopy(nextPos);
                auto const curNextNode = bfsCells.GetElementCopy(curNextPos);
                if (nextNode.m_distance < curNextNode.m_distance)
                {
                    curNextPos = nextPos;
                }
            }
            path.push_back(curNextPos);
            curPos = curNextPos;
            ++length;
        }
        return path;
    }

    CVectorInPlace<SNavmeshEdge::Path, 4> CPathfinding::GetPathsToAdjacentCrossroads(SVec2 const origin, std::vector<SVec2> const& lockedPositions) const
    {
        assert(find(lockedPositions.begin(), lockedPositions.end(), origin) == lockedPositions.end());
        auto paths = m_navmesh->GetPathsToAdjacentNodes(origin);
        CVectorInPlace<SNavmeshEdge::Path, 4> resultPaths;
        for (auto& path : paths)
        {
            bool isLockedPath = false;
            for (auto const lockedPos : lockedPositions)
            {
                if (find(path.begin(), path.end(), lockedPos) != path.end())
                {
                    isLockedPath = true;
                    break;
                }
            }
            if (!isLockedPath)
            {
                resultPaths.emplace_back(path);
            }
        }
        return paths;
    }

    CPathfinding::SDistanceInfo CPathfinding::FindClosestPosition(SVec2 const from, std::vector<SVec2> const& to, std::vector<SVec2> const& lockedPositions) const
    {
        auto const bfsCells = RunBFS(from, SVec2(-1, -1), lockedPositions, m_map);
        SVec2 curClosestPos = to.front();
        for (auto const pos : to)
        {
            if (bfsCells.GetElementCopy(pos).m_distance < bfsCells.GetElementCopy(curClosestPos).m_distance)
            {
                curClosestPos = pos;
            }
        }

        SDistanceInfo result;
        result.m_pos = curClosestPos;
        result.m_distance = bfsCells.GetElementCopy(curClosestPos).m_distance;
        return result;
    }
}