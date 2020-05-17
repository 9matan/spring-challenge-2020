#include "Core_PCH.h"
#include "Navmesh.h"

#include "CodingameUtility\Vec2Helper.h"

#include "Map.h"

using namespace std;

namespace SC2020
{
    namespace
    {
        bool IsMapCellNode(SCell const& cell)
        {
            return cell.m_adjacentCells.size() != 2 && cell.m_adjacentCells.size() != 0;
        }
    }

    CNavmesh::CNavmesh(SMap const& map)
        : m_navCells(map.GetWidth(), map.GetHeight())
    {
        BuildNavmesh(map);
    }

    CVectorInPlace<SNavmeshEdge::Path, 4> CNavmesh::GetPathsToAdjacentNodes(SVec2 const pos) const
    {
        CVectorInPlace<SNavmeshEdge::Path, 4> paths;
        auto const& navCell = m_navCells.GetElement(pos);
        auto const& edgeIds = navCell.m_node.m_edgeIds;
        if (!edgeIds.empty())
        {
            for (auto const edgeId : edgeIds)
            {
                auto const& edge = m_edges[edgeId];
                auto const& edgePath = edge.m_path;
                if (edge.GetAPos() == pos)
                {
                    paths.emplace_back(edgePath.begin() + 1, edgePath.end());
                }
                else
                {
                    paths.emplace_back(edgePath.rbegin() + 1, edgePath.rend());
                }
            }
        }
        else if (navCell.m_edgeId >= 0)
        {
            auto const& edge = m_edges[navCell.m_edgeId];
            auto const& edgePath = edge.m_path;
            auto const posIter = find(edgePath.begin(), edgePath.end(), pos);

            SNavmeshEdge::Path edgePath1(edgePath.begin(), posIter);
            reverse(edgePath1.begin(), edgePath1.end());
            SNavmeshEdge::Path const edgePath2(posIter + 1, edgePath.end());
            paths.push_back(edgePath1);
            paths.push_back(edgePath2);
        }
        return paths;
    }

    CVectorInPlace<SVec2, 4> CNavmesh::GetAdjacentNodes(SVec2 const pos) const
    {
        CVectorInPlace<SVec2, 4> nodes;
        auto const& navCell = m_navCells.GetElement(pos);
        auto const& edgeIds = navCell.m_node.m_edgeIds;
        if (!edgeIds.empty())
        {
            for (auto const edgeId : edgeIds)
            {
                auto const& edge = m_edges[edgeId];
                if (edge.GetAPos() == pos)
                {
                    nodes.push_back(edge.GetBPos());
                }
                else
                {
                    nodes.push_back(edge.GetAPos());
                }
            }
        }
        else if (navCell.m_edgeId >= 0)
        {
            auto const& edge = m_edges[navCell.m_edgeId];
            nodes.push_back(edge.GetAPos());
            nodes.push_back(edge.GetBPos());
        }
        return nodes;
    }

    /*bool CNavmesh::IsNode(SVec2 const pos) const
    {
        assert(m_navCells.IsValid(pos));

        auto const& navCell = m_navCells.GetElement(pos);
        return !navCell.m_node.m_edgeIds.empty();
    }

    SNavmeshEdge const& CNavmesh::GetEdge(short const id) const
    {
        return m_edges[id];
    }*/

    void CNavmesh::BuildNavmesh(SMap const& map)
    {
        SNavmeshEdge::Path path;
        path.reserve(MAX_MAP_AREA);
        for(int x = 0; x < map.GetWidth(); ++x)
        {
            for (int y = 0; y < map.GetHeight(); ++y)
            {
                auto const pos = SVec2(x, y);
                auto const& mapCell = map.GetCell({ x, y });
                if (!IsMapCellNode(mapCell))
                {
                    continue;
                }

                for (auto const nextPos : mapCell.m_adjacentCells)
                {
                    if (m_navCells.GetElement(nextPos).m_edgeId < 0)
                    {
                        path.clear();
                        path.push_back(pos);
                        BuildPathToNextNode(map, pos, nextPos, path);
                        AddEdge(path);
                    }
                }
            }
        }
    }

    void CNavmesh::BuildPathToNextNode(SMap const& map, SVec2 const from, SVec2 const to, SNavmeshEdge::Path& path)
    {
        path.push_back(to);
        auto const& mapCell = map.GetElement(to);
        if (IsMapCellNode(mapCell))
        {
            return;
        }

        for (auto const nextPos : mapCell.m_adjacentCells)
        {
            if (nextPos != from)
            {
                BuildPathToNextNode(map, to, nextPos, path);
            }
        }
    }

    short CNavmesh::AddEdge(SNavmeshEdge::Path const& path)
    {
        assert(path.size() > 1);

        for (auto const edgeId : m_navCells.GetElement(path.front()).m_node.m_edgeIds)
        {
            auto const& nextEdgeIds = m_navCells.GetElement(path.back()).m_node.m_edgeIds;
            if (find(nextEdgeIds.begin(), nextEdgeIds.end(), edgeId) != nextEdgeIds.end())
            {
                return edgeId;
            }
        }

        short const edgeId = (short)m_edges.size();
        auto& edge = m_edges.emplace_back();
        for(auto iter = path.begin() + 1; iter != (path.end() - 1); ++iter)
        {
            auto& cell = m_navCells.ModifyElement(*iter);
            cell.m_edgeId = edgeId;
        }

        edge.m_path = path;
        m_navCells.ModifyElement(edge.GetAPos()).m_node.m_edgeIds.push_back(edgeId);
        m_navCells.ModifyElement(edge.GetBPos()).m_node.m_edgeIds.push_back(edgeId);
        //cerr << "Edge added: " << (SVec2i)edge.GetAPos() << " " << (SVec2i)edge.GetBPos() << "\n";
        return edgeId;
    }
}