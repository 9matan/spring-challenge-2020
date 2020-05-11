#include "Core_PCH.h"
#include "MapHelper.h"

#include "InputData.h"

using namespace std;

namespace SC2020
{
    array<SVec2, 4> const ADJACENT_MASKS = {
        SVec2(-1, 0), {1, 0}, {0, -1}, {0, 1}
    };

    SMap BuildMap(SInputDataMap const& inDataMap)
    {
        SMap map(inDataMap.m_width, inDataMap.m_height);

        auto const rows = inDataMap.m_rows;
        for (size_t y = 0; y < inDataMap.m_height; ++y)
        {
            for (size_t x = 0; x < inDataMap.m_width; ++x)
            {
                auto& cell = map.GetCell({ x, y });
                cell.m_isFloor = rows[y][x] == ' ';
            
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
}