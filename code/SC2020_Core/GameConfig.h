#pragma once

constexpr int MAX_PLAYERS_CNT = 2;
constexpr int MAX_PACS_CNT_PER_PLAYER = 5;

constexpr int MAX_MAP_WIDTH = 50;
constexpr int MAX_MAP_HEIGHT = 30;
constexpr int MAX_MAP_AREA = MAX_MAP_WIDTH * MAX_MAP_HEIGHT;

constexpr int MAX_COMMANDS_PER_PAC = 1;
constexpr int MAX_OUTPUT_COMMANDS_CNT = MAX_PACS_CNT_PER_PLAYER * MAX_COMMANDS_PER_PAC;