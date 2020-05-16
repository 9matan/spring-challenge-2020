#pragma once

#include "GameGrid2D.h"
#include "NavmeshEdge.h"
#include "NavmeshNode.h"

namespace SC2020
{
    struct SMap;
    struct SCell;

    class CNavmesh
    {
    public:
        CNavmesh(SMap const& map);

        CVectorInPlace<SNavmeshEdge::Path, 4> GetPathsToAdjacentNodes(SVec2 const pos) const;
        CVectorInPlace<SVec2, 4> GetAdjacentNodes(SVec2 const pos) const;
        SNavmeshEdge::Path const& GetEdgePath(short const edgeId) const { return m_edges[edgeId].m_path; }
        short GetEdgeId(SVec2 const pos) const { return m_navCells.GetElement(pos).m_edgeId; }
        //bool IsNode(SVec2 const pos) const;
        //SNavmeshEdge const& GetEdge(short const id) const;
    private:
        struct SNavmeshCell
        {
            SNavmeshNode m_node;
            short m_edgeId = -1;
        };
    private:
        SGameGrid2D<SNavmeshCell> m_navCells;
        CVectorInPlace<SNavmeshEdge, MAX_MAP_AREA> m_edges;
    private:

        void BuildNavmesh(SMap const& map);
        void BuildPathToNextNode(SMap const& map, SVec2 const from, SVec2 const to, SNavmeshEdge::Path& path);
        short AddEdge(SNavmeshEdge::Path const& path);
    };
}