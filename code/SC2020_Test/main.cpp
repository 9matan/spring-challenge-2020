#include <string>
#include <vector>
#include <algorithm>

#include "SC2020_Core\Map.h"
#include "SC2020_Core\MapHelper.h"
#include "SC2020_Core\Navmesh.h"

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

void RunTests()
{
    // Nevmesh GetPathsToAdjacentNodes
    {
        auto const map = BuildMap(g_mapSamples[0]);
        CNavmesh navmesh(map);
        
        Nevmesh_GetPathsToAdjacentNodes_TestCase(navmesh, { 1, 3 }, { SVec2(1, 5), SVec2(1, 5) });
        Nevmesh_GetPathsToAdjacentNodes_TestCase(navmesh, { 5, 3 }, { SVec2(3, 3), SVec2(7, 3), SVec2(7, 7), SVec2(29, 2) });
        Nevmesh_GetPathsToAdjacentNodes_TestCase(navmesh, { 15, 11 }, { SVec2(15, 12), SVec2(7, 3), SVec2(7, 7), SVec2(29, 2) });        
    }

    // MapHelper GetVisibleCells
    {
        auto const map = BuildMap(g_mapSamples[0]);

        MapHelper_GetVisibleCells_TestCase(map
            , { 17, 8 } // origin
            , { SVec2(17, 7), {17, 8}, {17, 9} } // expectedPos
            , { SVec2(16, 8), {18, 8} }); // notExpectedPos
        MapHelper_GetVisibleCells_TestCase(map 
            , { 1, 1 } // origin
            , { SVec2(3, 1), {5, 1}, {33, 1}, {31, 1}, {29, 1} } // expectedPos
            , { SVec2(5, 2), {6, 1}, {28, 1}, {29, 2} }); // notExpectedPos
        MapHelper_GetVisibleCells_TestCase(map 
            , { 7, 3 } // origin
            , { SVec2(7, 1), {1, 3}, {7, 12} } // expectedPos
            , {}); // notExpectedPos
    }
}

int main()
{
    RunTests();
    return 0;
}