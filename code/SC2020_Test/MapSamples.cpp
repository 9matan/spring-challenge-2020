#include "MapSamples.h"

using namespace SC2020;

static std::vector<SInputDataMap> CreateSamples()
{
    std::vector<SInputDataMap> samples;

    // codingame seed = -6378816199984322600
    SInputDataMap map1;
    map1.m_width = 35;
    map1.m_height = 14;
    map1.m_rows = {
        "###################################",
        "      #   # #   # #   # #   #      ",
        "##### # # # # # # # # # # # # #####",
        "#       #   # #     # #   #       #",
        "# # # # ### # # # # # # ### # # # #",
        "# # # # #     # # # #     # # # # #",
        "### # # # # ### ### ### # # # # ###",
        "    #     #             #     #    ",
        "### ### # ### ### ### ### # ### ###",
        "###     #     #     #     #     ###",
        "### ### # # # # ### # # # # ### ###",
        "      # #   # #     # #   # #      ",
        "### # # ### # # ### # # ### # # ###",
        "###################################"
    };
    samples.emplace_back(std::move(map1));

    return samples;
}

std::vector<SInputDataMap> g_mapSamples = CreateSamples();