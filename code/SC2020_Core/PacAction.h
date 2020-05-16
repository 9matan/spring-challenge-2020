#pragma once

#include "EPacType.h"

namespace SC2020
{
    enum class EPacActionType : unsigned char
    {
        MoveTo = 0,
        SpeedUp,
        SwitchType
    };

    struct SPacAction
    {
        SPacAction()
            : m_type(EPacActionType::MoveTo)
            , m_moveToPos(-1, -1)
        {}

        EPacActionType m_type;
        union
        {
            EPacType m_pacType;
            SVec2 m_moveToPos;
        };
    };
}