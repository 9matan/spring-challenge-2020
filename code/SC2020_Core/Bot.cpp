#include "Core_PCH.h"
#include "Bot.h"

#include "CodingameUtility\Random.h"
#include "CodingameUtility\Vec2.h"
#include "CodingameUtility\Vec2Helper.h"

#include "InputData.h"

#define RNG(container)  container.begin(), container.end()

using namespace std;

namespace SC2020
{

    template<typename TIter>
    TIter FindClosestPos(TIter b, TIter e, SVec2 const origin)
    {
        TIter curMin = b;
        for (TIter cur = b; cur != e; ++cur)
        {
            if (GetDistanceL1(origin, *cur) < GetDistanceL1(origin, *curMin))
            {
                curMin = cur;
            }
        }
        return curMin;
    }

    CVectorInPlace<SVec2, MAX_MAP_AREA> CollectFloorCells(SInputDataMap const& map)
    {
        CVectorInPlace<SVec2, MAX_MAP_AREA> cells;
        int rIndx = 0;
        int cIndx = 0;
        for (auto const& row: map.m_rows)
        {
            cIndx = 0;
            for (auto const cell : row)
            {
                if (cell == ' ')
                {
                    cells.push_back({ cIndx, rIndx });
                }
                ++cIndx;
            }
            ++rIndx;
        }
        return cells;
    }

    SOutputData CBot::FirstUpdate(SInitInputData const& initInData, SInputData const& inData)
    {
        auto const seed = InitializeRandom();
        cerr << "Seed: " << seed << "\n";

        m_floorCells = CollectFloorCells(initInData.m_map);
        random_shuffle(RNG(m_floorCells));

        return Update(inData);
    }

    SOutputData CBot::Update(SInputData const& inData)
    {
        SOutputData output;

        CVectorInPlace<SVec2, MAX_MAP_AREA> superPellets;
        CVectorInPlace<SVec2, MAX_MAP_AREA> pellets;
        for (auto const& pellet : inData.m_vissiblePellets)
        {
            if (pellet.m_value > 1)
            {
                superPellets.push_back({ pellet.m_x , pellet.m_y });
            }
            else
            {
                pellets.push_back({ pellet.m_x , pellet.m_y });
            }
        }
        //random_shuffle(RNG(superPellets));
        //random_shuffle(RNG(pellets));

        for (auto const& pac : inData.m_vissiblePacs)
        {
            if (!pac.m_isMine)
            {
                continue;
            }

            /*if (pac.m_abilityCooldown == 0)
            {
                SOutputCommand cmd;
                cmd.m_commandType = ECommandType::Speed;
                cmd.m_pacId = pac.m_pacId;
                output.m_commands.push_back(cmd);
                continue;
            }*/

            SVec2 const pacPos(pac.m_x, pac.m_y);

            SVec2 moveTo(0, 0);
            if (!pellets.empty() || !superPellets.empty())
            {

                SVec2* moveToIter = !superPellets.empty()
                    ? FindClosestPos(RNG(superPellets), pacPos)
                    : FindClosestPos(RNG(pellets), pacPos);

                if (!superPellets.empty())
                {
                    superPellets.erase_swap(moveToIter);
                }
                else
                {
                    pellets.erase_swap(moveToIter);
                }

                moveTo = *moveToIter;
            }
            else
            {
                moveTo = *GetRandomItem(RNG(m_floorCells));
            }

            SOutputCommand cmd;
            cmd.m_commandType = ECommandType::Move;
            cmd.m_pacId = pac.m_pacId;
            cmd.m_x = moveTo[0];
            cmd.m_y = moveTo[1];
            output.m_commands.push_back(cmd);
        }

        return output;
    }
}