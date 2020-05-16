#pragma once

#include "PacAction.h"
#include "PacGoal.h"

namespace SC2020
{
    struct SPacEntity
    {
        SPacGoal m_goal;
        SPacAction m_action;
        SVec2 m_lastVisiblePos = SVec2(-1, -1);
        SVec2 m_pos = SVec2(-1, -1);
        EPacType m_pacType;
        bool m_isMine = false;
        bool m_isAlive = true;
        unsigned char m_speedTurnsLeft = 0;
        unsigned char m_abilityCooldown = 0;
        unsigned char m_lastVisibleTurn = 0;
        unsigned char m_id;
    };
}