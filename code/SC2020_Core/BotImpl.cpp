#include "Core_PCH.h"
#include "BotImpl.h"

#include "CodingameUtility\Random.h"
#include "CodingameUtility\Vec2Helper.h"

#include "MapHelper.h"
#include "PacHelper.h"

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

        CVectorInPlace<SPacEntity, MAX_PACS_CNT_PER_PLAYER* MAX_PLAYERS_CNT> CreatePacEntities(SInputData const& inData)
        {
            int const maxPacId = max_element(inData.m_visiblePacs.begin(), inData.m_visiblePacs.end(),
                [](SInputDataPac const& lhs, SInputDataPac const& rhs)
                {
                    return lhs.m_pacId < rhs.m_pacId;
                })->m_pacId;

            int const pacsCnt = (maxPacId + 1) * MAX_PLAYERS_CNT;
            CVectorInPlace<SPacEntity, MAX_PACS_CNT_PER_PLAYER* MAX_PLAYERS_CNT> pacs;
            pacs.resize(pacsCnt);
            for (int i = 0; i < pacsCnt; ++i)
            {
                pacs[i].m_isMine = (i <= maxPacId);
                pacs[i].m_id = ((pacs[i].m_isMine) ? (i) : (i - maxPacId - 1));
            }
            return pacs;
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

        SPacAction CreateMoveToAction(SVec2 const moveTo)
        {
            SPacAction action;
            action.m_actionType = EPacActionType::MoveTo;
            action.m_moveToPos = moveTo;
            return action;
        }

        SOutputCommand CreateMoveToCommand(SVec2 const moveTo)
        {
            SOutputCommand cmd;
            cmd.m_x = moveTo[0];
            cmd.m_y = moveTo[1];
            cmd.m_commandType = ECommandType::Move;
            return cmd;
        }

        SOutputCommand CreateSpeedUpCommand()
        {
            SOutputCommand cmd;
            cmd.m_commandType = ECommandType::SpeedUp;
            return cmd;
        }

        SOutputCommand CreateSwitchTypeCommand(EPacType const pacType)
        {
            SOutputCommand cmd;
            cmd.m_commandType = ECommandType::SwitchType;
            cmd.m_pacType = ToString(pacType);
            return cmd;
        }
    }

    CBotImpl::CBotImpl(SInitInputData const& initInData, SInputData const& inData)
        : m_map(BuildMap(initInData.m_map))
        , m_navmesh(m_map)
        , m_pacs(CreatePacEntities(inData))
        , m_turnIndex(0)
        , m_floorCells(CollectFloorCells(initInData.m_map))
        , m_initSuperPelletPos(GetSuperPelletPos(inData.m_visiblePellets))
    {
    }

    SOutputData CBotImpl::FirstUpdate(SInputData const& inData)
    {
        random_shuffle(RNG(m_floorCells));
        return Update(inData);
    }

    SOutputData CBotImpl::Update(SInputData const& inData)
    {
        ++m_turnIndex;
        UpdatePacs(inData);
        UpdatePellets(inData);

        // behaviour
        {
            CVectorInPlace<SVec2, MAX_MAP_AREA> superPellets;
            CVectorInPlace<SVec2, MAX_MAP_AREA> pellets;
            for (auto const& pellet : inData.m_visiblePellets)
            {
                if (pellet.m_value > 1)
                {
                    superPellets.push_back({ pellet.m_x , pellet.m_y });
                }
            }
            for (size_t y = 0; y < m_map.GetHeight(); ++y)
            {
                for (size_t x = 0; x < m_map.GetWidth(); ++x)
                {
                    if (m_map.GetElement({ x, y }).m_pelletScore > 0.001f)
                    {
                        pellets.push_back({ x, y });
                    }
                }
            }

            for (auto& pac : m_pacs)
            {
                if (!pac.m_isMine || !pac.m_isAlive) continue;

                SVec2 moveTo(0, 0);
                if (!pellets.empty() || !superPellets.empty())
                {
                    SVec2* moveToIter = !superPellets.empty()
                        ? FindClosestPos(RNG(superPellets), pac.m_pos)
                        : FindClosestPos(RNG(pellets), pac.m_pos);

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
                pac.m_action = CreateMoveToAction(moveTo);
            }
        }

        return CreateOutputData();
    }

    void CBotImpl::UpdatePellets(SInputData const& inData)
    {
        // clear super pellets pos
        for (auto const pos : m_initSuperPelletPos)
        {
            m_map.GetCell(pos).m_pelletScore = 0.0f;
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
                auto const visiblePos = GetVisibleCells(m_map, pacPos);

                for (auto const vPos : visiblePos)
                {
                    m_map.GetCell(vPos).m_pelletScore = 0.0f;
                }
            }
        }
        // update from visible
        for (auto const& pellet : inData.m_visiblePellets)
        {
            SVec2 const pelletPos(pellet.m_x, pellet.m_y);
            m_map.GetCell(pelletPos).m_pelletScore = (float)pellet.m_value;
        }

        /*
        for (size_t y = 0; y < m_map.GetHeight(); ++y)
        {
            for (size_t x = 0; x < m_map.GetWidth(); ++x)
            {
                cerr << (int)(m_map.GetElement({ x, y }).m_pelletScore + 0.0001f) << " ";
            }
            cerr << "\n";
        }
        */
    }

    void CBotImpl::UpdatePacs(SInputData const& inData)
    {
        assert(inData.m_visiblePacs.size() <= m_pacs.size());

        CVectorInPlace<bool, MAX_PACS_CNT_PER_PLAYER> isPacVisible[2];
        auto const pacSize = (m_pacs.size() >> 1);
        for (auto& isVisible : isPacVisible)
        {
            isVisible.resize(pacSize);
            fill(RNG(isVisible), false);
        }

        for (auto& inDataPac : inData.m_visiblePacs)
        {
            isPacVisible[inDataPac.m_isMine ? 1 : 0][inDataPac.m_pacId] = true;

            auto& pacEntity = GetPacEntity(inDataPac.m_pacId, inDataPac.m_isMine);
            if (m_map.IsValidPos(pacEntity.m_pos))
            {
                pacEntity.m_lastVisiblePos = pacEntity.m_pos;
                pacEntity.m_lastVisibleTurn = m_turnIndex - 1;
            }
            pacEntity.m_pos = SVec2(inDataPac.m_x, inDataPac.m_y);
            pacEntity.m_abilityCooldown = inDataPac.m_abilityCooldown;
            pacEntity.m_speedTurnsLeft = inDataPac.m_speedTurnsLeft;
            pacEntity.m_pacType = ToPacType(inDataPac.m_typeId.c_str());
        }

        // update invisible enemies
        // TODO: check wether an enemy pac is alive
        auto const& isEnemyVisible = isPacVisible[0];
        for (size_t i = 0; i < isEnemyVisible.size(); ++i)
        {
            if (isEnemyVisible[i])
            {
                continue;
            }
            auto& pacEntity = GetPacEntity((unsigned int)i, false);
            if (m_map.IsValidPos(pacEntity.m_pos))
            {
                pacEntity.m_lastVisiblePos = pacEntity.m_pos;
                pacEntity.m_lastVisibleTurn = m_turnIndex - 1;
            }
            pacEntity.m_pos = SVec2(-1, -1);
        }

        // update invisible own (dead)
        auto const& isOwnVisible = isPacVisible[1];
        for (size_t i = 0; i < isOwnVisible.size(); ++i)
        {
            if (isOwnVisible[i])
            {
                continue;
            }
            GetPacEntity((unsigned int)i, true).m_isAlive = false;
        }
    }

    SOutputData CBotImpl::CreateOutputData() const
    {
        SOutputData output;
        for (auto const& pac : m_pacs)
        {
            if (!pac.m_isMine || !pac.m_isAlive) continue;

            SOutputCommand cmd;
            switch (pac.m_action.m_actionType)
            {
            case EPacActionType::Wait:
                cmd = CreateMoveToCommand(pac.m_pos);
                break;
            case EPacActionType::MoveTo:
                assert(m_map.IsValidPos(pac.m_action.m_moveToPos));
                cmd = CreateMoveToCommand(pac.m_action.m_moveToPos);
                break;
            case EPacActionType::SpeedUp:
                cmd = CreateSpeedUpCommand();
                break;
            case EPacActionType::SwitchType:
                cmd = CreateSwitchTypeCommand(pac.m_action.m_pacType);
                break;
            }
            cmd.m_pacId = pac.m_id;
            output.m_commands.push_back(cmd);
        }
        return output;
    }

    SPacEntity& CBotImpl::GetPacEntity(unsigned int const pacId, bool const isMine)
    {
        return *std::find_if(m_pacs.begin(), m_pacs.end(),
            [pacId, isMine](SPacEntity const& pac)
            {
                return pac.m_id == (unsigned char)pacId && pac.m_isMine == isMine;
            });
    }
}