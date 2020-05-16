#include "Core_PCH.h"
#include "CommandHelper.h"

#include "OutputCommand.h"

namespace SC2020
{
    std::string ToString(SOutputCommand const& cmd)
    {
        char buff[256];
        switch (cmd.m_commandType)
        {
        case ECommandType::Move:
            sprintf(buff, "MOVE %i %i %i", cmd.m_pacId, cmd.m_x, cmd.m_y);
            break;
        case ECommandType::SpeedUp:
            sprintf(buff, "SPEED %i", cmd.m_pacId);
            break;
        case ECommandType::SwitchType:
            sprintf(buff, "SWITCH %i %s", cmd.m_pacId, cmd.m_pacType.c_str());
            break;
        default:
            assert(false);
        }

        return buff;
    }
}