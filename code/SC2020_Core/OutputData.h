#pragma once

#include "CodingameUtility\VectorInPlace.h"

#include "GameConfig.h"
#include "OutputCommand.h"

namespace SC2020
{
    struct SOutputData
    {
        CVectorInPlace<SOutputCommand, MAX_OUTPUT_COMMANDS_CNT> m_commands;
    };
}