#include "Core_PCH.h"
#include "Bot.h"

#include "CodingameUtility\Random.h"
#include "CodingameUtility\Vec2.h"
#include "CodingameUtility\Vec2Helper.h"

#include "InputData.h"
#include "Map.h"
#include "MapHelper.h"
#include "Navmesh.h"

#define RNG(container)  container.begin(), container.end()

using namespace std;

namespace SC2020
{

    namespace
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
            for (auto const& row : map.m_rows)
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

        inline bool IsSuperPellet(float const score) { return score > 1.0f; }

        vector<SVec2> GetSuperPelletPos(vector<SInputDataPellet> const& pellets)
        {
            vector<SVec2> res;
            res.reserve(pellets.size());
            for (auto& pellet : pellets)
            {
                if (IsSuperPellet((float)pellet.m_value))
                {
                    res.push_back({ pellet.m_x, pellet.m_y });
                }
            }
            return res;
        }

        // updates
        void UpdatePellets(SBotData& botData, SInputData const& inData)
        {
            // clear super pellets pos
            for (auto const pos : botData.m_superPelletPos)
            {
                botData.m_map->GetCell(pos).m_pelletScore = 0.0f;
            }
            // clear visible pos
            {
                for (auto const pacData : inData.m_visiblePacs)
                {
                    if (!pacData.m_isMine)
                    {
                        continue;
                    }
                    SVec2 const pacPos(pacData.m_x, pacData.m_y);
                    auto const visiblePos = GetVisibleCells(*botData.m_map, pacPos);

                    for (auto const vPos : visiblePos)
                    {
                        botData.m_map->GetCell(vPos).m_pelletScore = 0.0f;
                    }
                }
            }
            // update from visible
            for (auto const& pellet : inData.m_visiblePellets)
            {
                SVec2 const pelletPos(pellet.m_x, pellet.m_y);
                botData.m_map->GetCell(pelletPos).m_pelletScore = (float)pellet.m_value;
            }

            for (size_t y = 0; y < botData.m_map->GetHeight(); ++y)
            {
                for (size_t x = 0; x < botData.m_map->GetWidth(); ++x)
                {
                    cerr << (int)(botData.m_map->GetElement({ x, y }).m_pelletScore + 0.0001f) << " ";
                }
                cerr << "\n";
            }
        }
    }

    CBot::CBot(SInitInputData const& initInData)
    {
        auto const seed = InitializeRandom();
        cerr << "Seed: " << seed << "\n";

        m_data.m_map = make_unique<SMap>(BuildMap(initInData.m_map));
        m_data.m_navmesh = make_unique<CNavmesh>(*m_data.m_map);

        m_floorCells = CollectFloorCells(initInData.m_map);
        random_shuffle(RNG(m_floorCells));
    }

    CBot::~CBot() {}

    SOutputData CBot::FirstUpdate(SInputData const& inData)
    {
        m_data.m_superPelletPos = GetSuperPelletPos(inData.m_visiblePellets);
        return Update(inData);
    }

    SOutputData CBot::Update(SInputData const& inData)
    {
        SOutputData output;

        UpdatePellets(m_data, inData);

        CVectorInPlace<SVec2, MAX_MAP_AREA> superPellets;
        CVectorInPlace<SVec2, MAX_MAP_AREA> pellets;
        for (auto const& pellet : inData.m_visiblePellets)
        {
            if (pellet.m_value > 1)
            {
                superPellets.push_back({ pellet.m_x , pellet.m_y });
            }
        }
        for (size_t y = 0; y < m_data.m_map->GetHeight(); ++y)
        {
            for (size_t x = 0; x < m_data.m_map->GetWidth(); ++x)
            {
                if (m_data.m_map->GetElement({x, y}).m_pelletScore > 0.001f)
                {
                    pellets.push_back({ x, y });
                }
            }
        }
        //random_shuffle(RNG(superPellets));
        //random_shuffle(RNG(pellets));

        for (auto const& pac : inData.m_visiblePacs)
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