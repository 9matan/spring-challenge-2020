#include "CodingameUtility\CommonDefs.h"

#include <iostream>

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
        data.m_vissiblePacs.resize(vissiblePacCount);
        for (auto& pacData : data.m_vissiblePacs)
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
        data.m_vissiblePellets.resize(vissiblePelletCnt);
        for (auto& pelletData : data.m_vissiblePellets)
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
    ReadInitInData(initInData);
    SC2020::CBot bot(initInData);

    SInputData inData;
    inData.m_vissiblePellets.reserve(MAX_MAP_AREA);
    inData.m_vissiblePacs.reserve(MAX_PLAYERS_CNT * MAX_PACS_CNT_PER_PLAYER);
    ReadInData(inData);
    auto const outData = bot.FirstUpdate(inData);
    PrintOutData(outData);

    while (true)
    {
        ReadInData(inData);
        auto const outData = bot.Update(inData);
        PrintOutData(outData);
    }

    return 0;
}