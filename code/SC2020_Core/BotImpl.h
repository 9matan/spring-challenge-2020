#pragma once

#include "InputData.h"
#include "OutputData.h"
#include "Map.h"
#include "Navmesh.h"
#include "PacEntity.h"

namespace SC2020
{
    class CBotImpl
    {
    public:
        CBotImpl(SInitInputData const& initInData, SInputData const& inData);

        SOutputData FirstUpdate(SInputData const& inData);
        SOutputData Update(SInputData const& inData);
    
    private:
        unsigned int m_turnIndex = 0;
        SMap m_map;
        CNavmesh m_navmesh;
        std::vector<SVec2> m_initSuperPelletPos;
        CVectorInPlace<SPacEntity, MAX_PACS_CNT_PER_PLAYER* MAX_PLAYERS_CNT> m_pacs;
        CVectorInPlace<SVec2, MAX_MAP_AREA> m_floorCells;

    private:
        void UpdatePellets(SInputData const& inData);
        void UpdatePacs(SInputData const& inData);

        SPacEntity& GetPacEntity(unsigned int const pacId, bool const isMine);
    };
}