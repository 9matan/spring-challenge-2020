#pragma once

#include "EPacType.h"

namespace SC2020
{
    char const* ToString(EPacType const pacType);
    EPacType ToPacType(char const* const str);
}