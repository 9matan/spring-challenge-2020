#pragma once

#include <string>
#include <vector>

#include "GameConfig.h"

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

    struct SInputDataMap
    {
        int m_width;
        int m_height;
        std::vector<std::string> m_rows;
    };

    struct SInputData
    {
        int m_myScore;
        int m_opponentScore;
        std::vector<SInputDataPac> m_vissiblePacs;
        std::vector<SInputDataPellet> m_vissiblePellets;
    };

    struct SInitInputData
    {
        SInputDataMap m_map;
    };
}