#include "CodingameUtility\CommonDefs.h"

#include <iostream>

#include "CodingameUtility\Profiler.h"

#include "SC2020_Core\CommandHelper.h"
#include "SC2020_Core\Bot.h"
#include "SC2020_Core\InputData.h"
#include "SC2020_Core\OutputData.h"

using namespace SC2020;
using namespace std;

void ReadInitInData(SInitInputData& data)
{
    cin >> data.m_map.m_width >> data.m_map.m_height; cin.ignore();
    cerr << data.m_map.m_width << " " << data.m_map.m_height << "\n";
    data.m_map.m_rows.resize(data.m_map.m_height);
    for(auto& row: data.m_map.m_rows)
    {
        getline(cin, row);
        cerr << row << "\n";
    }
}

void ReadInData(SInputData& data)
{
    cin >> data.m_myScore >> data.m_opponentScore; cin.ignore();
    {
        int vissiblePacCount;
        cin >> vissiblePacCount; cin.ignore();
        data.m_visiblePacs.resize(vissiblePacCount);
        for (auto& pacData : data.m_visiblePacs)
        {
            cin >> pacData.m_pacId
                >> pacData.m_isMine
                >> pacData.m_x >> pacData.m_y
                >> pacData.m_typeId
                >> pacData.m_speedTurnsLeft
                >> pacData.m_abilityCooldown;
            cin.ignore();
        }
    }
    {
        int vissiblePelletCnt;
        cin >> vissiblePelletCnt; cin.ignore();
        data.m_visiblePellets.resize(vissiblePelletCnt);
        for (auto& pelletData : data.m_visiblePellets)
        {
            cin >> pelletData.m_x >> pelletData.m_y
                >> pelletData.m_value;
            cin.ignore();
        }
    }
}

void PrintOutData(SOutputData const& data)
{
    static string outputStr;
    outputStr.clear();
    for (auto const& cmd : data.m_commands)
    {
        outputStr += ToString(cmd) + "|";
    }
    outputStr.pop_back();
    printf("%s\n", outputStr.c_str());
}

int main()
{

    SInitInputData initInData;
    initInData.m_map.m_rows.reserve(MAX_MAP_HEIGHT);
    SInputData inData;
    inData.m_visiblePellets.reserve(MAX_MAP_AREA);
    inData.m_visiblePacs.reserve(MAX_PLAYERS_CNT * MAX_PACS_CNT_PER_PLAYER);

    
    SC2020::CBot bot;
    {
        PROFILE_TIME("First update");
        ReadInitInData(initInData);
        ReadInData(inData);
        auto const outData = bot.FirstUpdate(initInData, inData);
        PrintOutData(outData);
    }

    while (true)
    {
        PROFILE_TIME("Update");
        ReadInData(inData);
        auto const outData = bot.Update(inData);
        PrintOutData(outData);
    }

    return 0;
}