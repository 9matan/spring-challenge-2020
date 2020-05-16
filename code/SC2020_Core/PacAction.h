#pragma once

#include "EPacType.h"

namespace SC2020
{
    enum class EPacActionType : unsigned char
    {
        Wait = 0,
        MoveTo,
        SpeedUp,
        SwitchType,
        Count
    };

    struct SPacAction
    {
        EPacActionType m_actionType;
        EPacType m_pacType;
        SVec2 m_moveToPos;
    };
}