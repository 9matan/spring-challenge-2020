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
        SGameGrid2D<SBFSNode> bfsCells(m_map->GetWidth(), m_map->GetHeight());
        for (auto const lockedPos : lockedPositions)
        {
            if (lockedPos == to)
            {
#ifdef DEBUG_WARNINGS_ENABLED
                cerr << "[WARNING] CPathfinding::FindNextSteps_ClosestPath target path is locked!\n";
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
            bfsCells.SetElement(to, node);

            queue<SVec2> q;
            q.push(to);
            while (!q.empty())
            {
                auto const curPos = q.front();
                q.pop();
                auto const curNode = bfsCells.GetElementCopy(curPos);
                for (auto const nextPos : m_map->GetCell(curPos).m_adjacentCells)
                {
                    auto nextNode = bfsCells.GetElementCopy(nextPos);
                    if (nextNode.m_isUsed) continue;

                    nextNode.m_isUsed = true;
                    nextNode.m_distance = curNode.m_distance + 1;
                    bfsCells.SetElement(nextPos, nextNode);
                    q.push(nextPos);
                    
                    // we foound a way
                    if (nextPos == from)
                    {
                        queue<SVec2> emptyQ;
                        swap(emptyQ, q);
                    }
                }
            }
        }

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
}