#pragma once

namespace SC2020
{
    enum class ECommandType
    {
        Move = 0,
        SpeedUp,
        SwitchType
    };

    struct SOutputCommand
    {
        ECommandType m_commandType;
        int m_pacId; 
        int m_x;
        int m_y;
        std::string m_pacType;
    };
}