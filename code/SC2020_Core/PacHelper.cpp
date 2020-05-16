#include "Core_PCH.h"
#include "PacHelper.h"

#include <assert.h>
#include <cstring>

namespace SC2020
{
    char const* ToString(EPacType const pacType)
    {
        switch (pacType)
        {
        case EPacType::Paper: return "PAPER";
        case EPacType::Rock: return "ROCK";
        case EPacType::Scissors: return "SCISSORS";
        }
        assert(false);
        return "ERROR";
    }

    EPacType ToPacType(char const* const str)
    {
        if (strcmp(str, "PAPER") == 0) return EPacType::Paper;
        if (strcmp(str, "ROCK") == 0) return EPacType::Rock;
        if (strcmp(str, "SCISSORS") == 0) return EPacType::Scissors;
        assert(false);
        return EPacType::Count;
    }
}