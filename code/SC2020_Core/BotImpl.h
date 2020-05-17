#pragma once

#include "InputData.h"
#include "OutputData.h"
#include "Map.h"
#include "Navmesh.h"
#include "PacEntity.h"
#include "Pathfinding.h"
#include "Strategy.h"
#include "StrategyEvalResult.h"

namespace SC2020
{
    class CBotImpl
    {
    public:
        CBotImpl(SInitInputData const& initInData, SInputData const& inData);

        SOutputData FirstUpdate(SInputData const& inData);
        SOutputData Update(SInputData const& inData);

        inline SPacEntity const& GetPacEntity(unsigned char const pacId, bool const isMine) const { return const_cast<CBotImpl*>(this)->GetPacEntity(pacId, isMine); }
        inline SMap const& GetMap() const { return m_map; }
        inline CNavmesh const& GetNavmesh() const { return m_navmesh; }
        inline CPathfinding const& GetPathfinding() const { return m_pathfinding; }
        inline std::vector<SVec2> const& GetInitSuperPelletPos() const { return m_initSuperPelletPos; }
        inline CVectorInPlace<SPacEntity, MAX_PACS_CNT_PER_PLAYER* MAX_PLAYERS_CNT> const& GetPacs() const { return m_pacs; }
        inline unsigned char GetTurnIndex() const { return m_turnIndex; }

    private:
        SMap m_map;
        CNavmesh m_navmesh;
        CPathfinding m_pathfinding;
        std::vector<SVec2> m_initSuperPelletPos;
        CVectorInPlace<SPacEntity, MAX_PACS_CNT_PER_PLAYER* MAX_PLAYERS_CNT> m_pacs;
        CVectorInPlace<SVec2, MAX_MAP_AREA> m_floorCells;
        unsigned char m_turnIndex = 0;

    private:
        void UpdatePellets(SInputData const& inData);
        void UpdatePacs(SInputData const& inData);
        void ValidateGoals();
        SStrategy SelectStrategy();
        SStrategyEvalResult EvaluateStrategy(SStrategy const& strategy);
        void ApplyStrategyEvaluationResult(SStrategyEvalResult const& strategyEvaluationResult);
        void ResolveActions();

        CVectorInPlace<SVec2, MAX_PACS_CNT_PER_PLAYER* MAX_PLAYERS_CNT> CollectPacPositions() const;
        SOutputData CreateOutputData() const;
        SPacEntity& GetPacEntity(unsigned char const pacId, bool const isMine);
    };
}