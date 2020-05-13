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
    for (auto const expectedPos : expectedPos)
    {
        bool found = false;
        for (auto const& path : paths)
        {
            if (find(path.begin(), path.end(), expectedPos) != path.end())
            {
                found = true;
                break;
            }
        }
        assert(found);
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
}

int main()
{
    RunTests();
    return 0;
}