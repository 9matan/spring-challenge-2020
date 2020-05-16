#include <string>
#include <vector>
#include <algorithm>

#include "SC2020_Core\Map.h"
#include "SC2020_Core\MapHelper.h"
#include "SC2020_Core\Navmesh.h"
#include "SC2020_Core\Pathfinding.h"

#include "MapSamples.h"

using namespace std;
using namespace SC2020;

void Nevmesh_GetPathsToAdjacentNodes_TestCase(CNavmesh const& navmesh, SVec2 const origin, CVectorInPlace<SVec2, 4> const& expectedPos)
{
    auto const paths = navmesh.GetPathsToAdjacentNodes(origin);
    for (auto const pos : expectedPos)
    {
        bool found = false;
        for (auto const& path : paths)
        {
            if (find(path.begin(), path.end(), pos) != path.end())
            {
                found = true;
                break;
            }
        }
        assert(found);
    }
}

void MapHelper_GetVisibleCells_TestCase(SMap const& map, SVec2 const origin, CVectorInPlace<SVec2, 32> const& expectedPos, CVectorInPlace<SVec2, 32> const& notExpectedPos)
{
    auto const visiblePos = GetVisibleCells(map, origin);
    for (auto const pos : expectedPos)
    {
        if (find(visiblePos.begin(), visiblePos.end(), pos) == visiblePos.end())
        {
            assert(false);
        }
    }
    for (auto const pos : notExpectedPos)
    {
        if (find(visiblePos.begin(), visiblePos.end(), pos) != visiblePos.end())
        {
            assert(false);
        }
    }
}

void Pathfinding_FindNextSteps_ShortestPath_TestCase(
    CPathfinding const& pathfinding, 
    SVec2 const from, 
    SVec2 const to,
    vector<SVec2> const& lockedPositions,
    unsigned int const maxPathLength,
    CVectorInPlace<SVec2, MAX_PATHFINDING_PATH_LENGTH> const& expectedPathPrefix)
{
    assert(maxPathLength <= MAX_PATHFINDING_PATH_LENGTH);
    auto const path = pathfinding.FindNextSteps_ShortestPath(from, to, lockedPositions, maxPathLength);
    
    assert (path.size() >= expectedPathPrefix.size());
    for (size_t i = 0; i < expectedPathPrefix.size(); ++i)
    {
        assert(path[i] == expectedPathPrefix[i]);
    }
}

void RunTests()
{
    // MapHelper GetVisibleCells
    {
        auto const map = BuildMap(g_mapSamples[0]);

        MapHelper_GetVisibleCells_TestCase(map
            , { 17, 8 } // origin
            , { SVec2(17, 7), {17, 8}, {17, 9} } // expectedPos
            , { SVec2(16, 8), {18, 8} }// notExpectedPos
        ); 
        MapHelper_GetVisibleCells_TestCase(map
            , { 1, 1 } // origin
            , { SVec2(3, 1), {5, 1}, {33, 1}, {31, 1}, {29, 1} } // expectedPos
            , { SVec2(5, 2), {6, 1}, {28, 1}, {29, 2} } // notExpectedPos
        ); 
        MapHelper_GetVisibleCells_TestCase(map
            , { 7, 3 } // origin
            , { SVec2(7, 1), {1, 3}, {7, 12} } // expectedPos
            , {} // notExpectedPos
        );
    }

    // Nevmesh GetPathsToAdjacentNodes
    {
        auto const map = BuildMap(g_mapSamples[0]);
        CNavmesh navmesh(map);
        
        Nevmesh_GetPathsToAdjacentNodes_TestCase(navmesh, { 1, 3 }, { SVec2(1, 5), SVec2(1, 5) });
        Nevmesh_GetPathsToAdjacentNodes_TestCase(navmesh, { 5, 3 }, { SVec2(3, 3), SVec2(7, 3), SVec2(7, 7), SVec2(29, 2) });
        Nevmesh_GetPathsToAdjacentNodes_TestCase(navmesh, { 15, 11 }, { {15, 12}, {19, 11}, {17, 9} });
    }

    //Pathfinding FindNextSteps_ClosestPath
    {
        auto const map = BuildMap(g_mapSamples[0]);
        CNavmesh navmesh(map);
        CPathfinding pathfinding(&map, &navmesh);

        Pathfinding_FindNextSteps_ShortestPath_TestCase(pathfinding
            , {3, 5} // from
            , {5, 7} // to
            , {} // lockedPositions
            , 2 // maxPathLength
            , { {3, 4}, {3, 3} }// expectedPathPrefix
        ); 
        Pathfinding_FindNextSteps_ShortestPath_TestCase(pathfinding
            , { 11, 3 } // from
            , { 11, 4 } // to
            , {} // lockedPositions
            , 2 // maxPathLength
            , { {11, 4} }// expectedPathPrefix
        ); 
        Pathfinding_FindNextSteps_ShortestPath_TestCase(pathfinding
            , { 9, 3 } // from
            , { 13, 7 } // to
            , { {11, 6} } // lockedPositions
            , MAX_PATHFINDING_PATH_LENGTH // maxPathLength
            , { {10, 3}, {11, 3}, {11, 4}, {11, 5}, {10, 5}, {9, 5}, {9, 6}, {9, 7}, {9, 8} }// expectedPathPrefix
        );
    }
}

int main()
{
    RunTests();
    return 0;
}