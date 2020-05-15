#pragma once

#include <string>
#include <vector>

#include "GameConfig.h"
#include "InputDataMap.h"

namespace SC2020
{
    struct SInputDataPac
    {
        int m_pacId;
        int m_x;
        int m_y;
        int m_speedTurnsLeft;
        int m_abilityCooldown;
        std::string m_typeId;
        bool m_isMine;
    };

    struct SInputDataPellet
    {
        int m_x;
        int m_y;
        int m_value;
    };

    struct SInputData
    {
        int m_myScore;
        int m_opponentScore;
        std::vector<SInputDataPac> m_visiblePacs;
        std::vector<SInputDataPellet> m_visiblePellets;
    };

    struct SInitInputData
    {
        SInputDataMap m_map;
    };
}