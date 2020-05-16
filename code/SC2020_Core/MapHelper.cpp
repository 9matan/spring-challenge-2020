#include "Core_PCH.h"
#include "MapHelper.h"

#include "InputData.h"

using namespace std;

namespace SC2020
{
    array<SVec2, 4> const ADJACENT_MASKS = {
        SVec2(-1, 0), {1, 0}, {0, -1}, {0, 1}
    };

    namespace
    {
        inline SVec2 AddVector(SVec2 const pos, SVec2 v, size_t const mapWidth)
        {
            SVec2 newPos = pos + v;
            if (newPos[0] < 0) newPos[0] = (SVec2::ValueType)(mapWidth - 1);
            if (newPos[0] >= (SVec2::ValueType)mapWidth) newPos[0] = 0;
            return newPos;
        }
    }

    SMap BuildMap(SInputDataMap const& inDataMap)
    {
        SMap map(inDataMap.m_width, inDataMap.m_height);

        auto const rows = inDataMap.m_rows;
        for (size_t y = 0; y < inDataMap.m_height; ++y)
        {
            for (size_t x = 0; x < inDataMap.m_width; ++x)
            {
                if (inDataMap.m_rows[y][x] != ' ')
                {
                    continue;
                }

                auto& cell = map.ModifyCell({ x, y });
                cell.m_pelletScore = 1.0f;
                for (auto const mask : ADJACENT_MASKS)
                {
                    SVec2 newPos = mask + SVec2(x, y);
                    if (newPos[1] >= 0 && newPos[1] < map.GetHeight())
                    { 
                        if (newPos[0] < 0) newPos[0] += (SVec2::ValueType)map.GetWidth();
                        if (newPos[0] >= map.GetWidth()) newPos[0] -= (SVec2::ValueType)map.GetWidth();
                        if (inDataMap.m_rows[newPos[1]][newPos[0]] == ' ')
                        {
                            cell.m_adjacentCells.push_back(newPos);
                        }
                    }
                }
            }
        }

        return map;
    }

    CVectorInPlace<SVec2, MAX_MAP_AREA> GetVisibleCells(SMap const& map, SVec2 const origin)
    {
        CVectorInPlace<SVec2, MAX_MAP_AREA> result;
        result.push_back(origin);
        for (auto const lineDir : ADJACENT_MASKS)
        {
            auto* curCell = &map.GetCell(origin);
            SVec2 nextPos = AddVector(origin, lineDir, map.GetWidth());

            while (find(curCell->m_adjacentCells.begin(), curCell->m_adjacentCells.end(), nextPos) != curCell->m_adjacentCells.end())
            {
                result.push_back(nextPos);
                curCell = &map.GetCell(nextPos);
                nextPos = AddVector(nextPos, lineDir, map.GetWidth());
            }
        }
        return result;
    }
}