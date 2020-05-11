#pragma once

namespace SC2020
{
    enum class ECellTypeFlag : unsigned short
    {
        // phys
        Empty = 1,
        Wall = Empty << 1,
        // contains
        Pellet = Wall << 1,
        Pacman = Pellet << 1
    };
}