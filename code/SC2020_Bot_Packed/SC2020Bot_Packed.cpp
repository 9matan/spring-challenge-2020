
#define _CRT_SECURE_NO_WARNINGS
#define _SCL_SECURE_NO_WARNINGS


#include <string>
namespace SC2020{struct SOutputCommand;std::string ToString(SOutputCommand const& cmd);}

#include <algorithm>
#include <assert.h>
template<typename TComponent>struct SVec2Base{static const size_t ms_vecSize = 2;using ValueType = TComponent;template<typename TOtherComponent>explicit SVec2Base(SVec2Base<TOtherComponent> const other): m_x(static_cast<TComponent>(other.m_x)), m_y(static_cast<TComponent>(other.m_y)){}template<typename TOtherComponent>SVec2Base(TOtherComponent const x, TOtherComponent const y): m_x(static_cast<TComponent>(x)), m_y(static_cast<TComponent>(y)){}template<typename TOtherComponent>SVec2Base(std::initializer_list<TOtherComponent> const& data){assert(data.size() <= ms_vecSize);std::transform(data.begin(), data.end(), m_data,[](TOtherComponent const otherComp){return static_cast<TComponent>(otherComp);});}SVec2Base() : m_x(0), m_y(0) {}SVec2Base(TComponent const x, TComponent const y) : m_x(x), m_y(y) {}SVec2Base(std::initializer_list<TComponent> const& data){assert(data.size() <= ms_vecSize);std::copy(data.begin(), data.end(), m_data);}inline SVec2Base& operator=(SVec2Base const other) { m_data[0] = other.m_data[0]; m_data[1] = other.m_data[1]; return *this; }inline bool operator==(SVec2Base const other) const { return other[0] == m_data[0] && other[1] == m_data[1]; }inline bool operator!=(SVec2Base const other) const { return !(*this == other); }inline TComponent operator[](size_t const index) const { return m_data[index]; }inline TComponent& operator[](size_t const index) { return m_data[index]; }inline SVec2Base operator+(SVec2Base const other) const { return SVec2Base(m_data[0] + other.m_data[0], m_data[1] + other.m_data[1]); }inline SVec2Base operator-(SVec2Base const other) const { return SVec2Base(m_data[0] - other.m_data[0], m_data[1] - other.m_data[1]); }inline bool operator<(SVec2Base const other) const { if (m_data[0] == other[0]) { return m_data[1] < other[1]; } return m_data[0] < other[0]; }private:union{struct{TComponent m_x;TComponent m_y;};TComponent m_data[ms_vecSize];};};using SVec2i = SVec2Base<int>;using SVec2u = SVec2Base<unsigned int>;using SVec2si = SVec2Base<short>;using SVec2su = SVec2Base<unsigned short>;using SVec2 = SVec2Base<char>;
template<typename T, size_t TSize>class CVectorInPlace{public:CVectorInPlace() : m_nxtSlotIndex(0) {}CVectorInPlace(size_t const size) { resize(size); }CVectorInPlace(std::initializer_list<T> const& items){reserve(items.size());std::copy(items.begin(), items.end(), begin());}inline void push_back(T const& elem) { assert(m_nxtSlotIndex < TSize); m_data[m_nxtSlotIndex++] = elem; }template<typename ... TArgs>inline T& emplace_back(TArgs&& ... args) { m_data[m_nxtSlotIndex++] = std::move(T(std::forward<TArgs>(args)...)); return back(); }inline void pop_back() { m_nxtSlotIndex--; }inline T& back() { return m_data[m_nxtSlotIndex - 1]; }inline T& front() { return *m_data; }inline size_t size() const { return m_nxtSlotIndex; }inline void resize(size_t const capacity) { m_nxtSlotIndex = capacity; }inline void reserve(size_t const capacity) { assert(capacity <= TSize); }inline size_t max_capacity() const { return TSize; }inline bool is_full() const { return m_nxtSlotIndex >= TSize; }void erase_swap(T* const iter){size_t const index = std::distance(m_data, iter);std::swap(m_data[index], m_data[m_nxtSlotIndex - 1]);pop_back();}inline bool empty() const { return m_nxtSlotIndex == 0; }inline void clear() { m_nxtSlotIndex = 0; }inline T* begin() { return m_data; }inline T* end() { return m_data + m_nxtSlotIndex; }inline T const* begin() const { return m_data; }inline T const* end() const { return m_data + m_nxtSlotIndex; }inline T& operator[](size_t const index) { return m_data[index]; }inline T const& operator[](size_t const index) const { return m_data[index]; }private:size_t m_nxtSlotIndex = 0;T m_data[TSize];};
namespace SC2020{struct SCell{CVectorInPlace<SVec2, 4> m_adjacentCells;float m_pelletScore = 0.0f;bool m_isFloor = false;};}
constexpr int MAX_PLAYERS_CNT = 2;constexpr int MAX_PACS_CNT_PER_PLAYER = 5;constexpr int MAX_MAP_WIDTH = 35;constexpr int MAX_MAP_HEIGHT = 17;constexpr int MAX_MAP_AREA = MAX_MAP_WIDTH * MAX_MAP_HEIGHT;constexpr int MAX_COMMANDS_PER_PAC = 1;constexpr int MAX_OUTPUT_COMMANDS_CNT = MAX_PACS_CNT_PER_PLAYER * MAX_COMMANDS_PER_PAC;
template <typename TElem, size_t TMaxWidth, size_t TMaxHeight>struct SGrid2DInPlace{public:SGrid2DInPlace(): m_width(0), m_height(0){}SGrid2DInPlace(size_t const width, size_t const height): m_width(width), m_height(height){assert(width <= TMaxWidth && height <= TMaxHeight);m_elements.resize(width * height);}inline TElem const& GetElement(SVec2 const pos) const { return m_elements[GetIndex(pos)]; }inline TElem& GetElement(SVec2 const pos) { return m_elements[GetIndex(pos)]; }inline size_t GetWidth() const { return m_width; }inline size_t GetHeight() const { return m_height; }inline size_t GetSize() const { return m_elements.size(); }inline bool IsValid(SVec2 const pos) const { return pos[0] >= 0 && pos[0] < m_width && pos[1] >= 0 && pos[1] < m_height; }inline size_t GetIndex(SVec2 const pos) const { return pos[0] + pos[1] * m_width; }private:CVectorInPlace<TElem, TMaxWidth * TMaxHeight> m_elements;size_t m_width;size_t m_height;};
namespace SC2020{template<typename TElem>using SGameGrid2D = SGrid2DInPlace<TElem, MAX_MAP_WIDTH, MAX_MAP_HEIGHT>;}
namespace SC2020{struct SMap : public SGameGrid2D<SCell>{public:using SGameGrid2D<SCell>::SGameGrid2D;inline SCell const& GetCell(SVec2 const pos) const { return GetElement(pos); }inline SCell& GetCell(SVec2 const pos) { return GetElement(pos); }};}
namespace SC2020{struct SBotData{SMap m_map;};}
namespace SC2020{enum class ECommandType{Move = 0,Speed};struct SOutputCommand{ECommandType m_commandType;int m_pacId;int m_x;int m_y;};}
namespace SC2020{struct SOutputData{CVectorInPlace<SOutputCommand, MAX_OUTPUT_COMMANDS_CNT> m_commands;};}
namespace SC2020{struct SInitInputData;struct SInputData;class CBot{public:CBot(SInitInputData const& initInData);SOutputData FirstUpdate(SInputData const& inData);SOutputData Update(SInputData const& inData);private:CVectorInPlace<SVec2, MAX_MAP_AREA> m_floorCells;SBotData m_data;};}
#include <vector>
namespace SC2020{struct SInputDataPac{int m_pacId;int m_x;int m_y;int m_speedTurnsLeft;int m_abilityCooldown;std::string m_typeId;bool m_isMine;};struct SInputDataPellet{int m_x;int m_y;int m_value;};struct SInputDataMap{int m_width;int m_height;std::vector<std::string> m_rows;};struct SInputData{int m_myScore;int m_opponentScore;std::vector<SInputDataPac> m_vissiblePacs;std::vector<SInputDataPellet> m_vissiblePellets;};struct SInitInputData{SInputDataMap m_map;};}

#include <iostream>
using namespace SC2020;using namespace std;void ReadInitInData(SInitInputData& data){cin >> data.m_map.m_width >> data.m_map.m_height; cin.ignore();cerr << data.m_map.m_width << " " << data.m_map.m_height << "\n";data.m_map.m_rows.resize(data.m_map.m_height);for(auto& row: data.m_map.m_rows){getline(cin, row);cerr << row << "\n";}}void ReadInData(SInputData& data){cin >> data.m_myScore >> data.m_opponentScore; cin.ignore();{int vissiblePacCount;cin >> vissiblePacCount; cin.ignore();data.m_vissiblePacs.resize(vissiblePacCount);for (auto& pacData : data.m_vissiblePacs){cin >> pacData.m_pacId>> pacData.m_isMine>> pacData.m_x >> pacData.m_y>> pacData.m_typeId>> pacData.m_speedTurnsLeft>> pacData.m_abilityCooldown;cin.ignore();}}{int vissiblePelletCnt;cin >> vissiblePelletCnt; cin.ignore();data.m_vissiblePellets.resize(vissiblePelletCnt);for (auto& pelletData : data.m_vissiblePellets){cin >> pelletData.m_x >> pelletData.m_y>> pelletData.m_value;cin.ignore();}}}void PrintOutData(SOutputData const& data){static string outputStr;outputStr.clear();for (auto const& cmd : data.m_commands){outputStr += ToString(cmd) + "|";}outputStr.pop_back();printf("%s\n", outputStr.c_str());}int main(){SInitInputData initInData;initInData.m_map.m_rows.reserve(MAX_MAP_HEIGHT);ReadInitInData(initInData);SC2020::CBot bot(initInData);SInputData inData;inData.m_vissiblePellets.reserve(MAX_MAP_AREA);inData.m_vissiblePacs.reserve(MAX_PLAYERS_CNT * MAX_PACS_CNT_PER_PLAYER);ReadInData(inData);auto const outData = bot.FirstUpdate(inData);PrintOutData(outData);while (true){ReadInData(inData);auto const outData = bot.Update(inData);PrintOutData(outData);}return 0;}

#define _CRT_SECURE_NO_WARNINGS

#include <array>
unsigned int InitializeRandom();unsigned int GetRandomUInt(unsigned int const minVal, unsigned int const maxVal);template<typename TIter>TIter GetRandomItem(TIter first, TIter last){size_t const rangeSize = static_cast<size_t>(last - first);return first + GetRandomUInt(0, static_cast<unsigned int>(rangeSize));}template<typename TEnum>TEnum GetRandomEnumValue(){return static_cast<TEnum>(GetRandomUInt(0, static_cast<unsigned int>(TEnum::Count)));}

#include <istream>
#include <ostream>
template <typename TVec>inline int GetDistanceL1(TVec const lhs, TVec const rhs) { return abs(static_cast<int>(lhs[0] - rhs[0])) + abs(static_cast<int>(lhs[1] - rhs[1])); }char const* ToString(SVec2i const vec2);std::ostream& operator<<(std::ostream& out, SVec2i const& vec);std::istream& operator>>(std::istream& in, SVec2i& vec);
namespace SC2020{struct SInputDataMap;SMap BuildMap(SInputDataMap const& inDataMap);}

#define RNG(container)  container.begin(), container.end()
using namespace std;namespace SC2020{template<typename TIter>TIter FindClosestPos(TIter b, TIter e, SVec2 const origin){TIter curMin = b;for (TIter cur = b; cur != e; ++cur){if (GetDistanceL1(origin, *cur) < GetDistanceL1(origin, *curMin)){curMin = cur;}}return curMin;}CVectorInPlace<SVec2, MAX_MAP_AREA> CollectFloorCells(SInputDataMap const& map){CVectorInPlace<SVec2, MAX_MAP_AREA> cells;int rIndx = 0;int cIndx = 0;for (auto const& row: map.m_rows){cIndx = 0;for (auto const cell : row){if (cell == ' '){cells.push_back({ cIndx, rIndx });}++cIndx;}++rIndx;}return cells;}CBot::CBot(SInitInputData const& initInData){auto const seed = InitializeRandom();cerr << "Seed: " << seed << "\n";m_data.m_map = BuildMap(initInData.m_map);m_floorCells = CollectFloorCells(initInData.m_map);random_shuffle(RNG(m_floorCells));}SOutputData CBot::FirstUpdate(SInputData const& inData){return Update(inData);}SOutputData CBot::Update(SInputData const& inData){SOutputData output;CVectorInPlace<SVec2, MAX_MAP_AREA> superPellets;CVectorInPlace<SVec2, MAX_MAP_AREA> pellets;for (auto const& pellet : inData.m_vissiblePellets){if (pellet.m_value > 1){superPellets.push_back({ pellet.m_x , pellet.m_y });}else{pellets.push_back({ pellet.m_x , pellet.m_y });}}for (auto const& pac : inData.m_vissiblePacs){if (!pac.m_isMine){continue;}SVec2 const pacPos(pac.m_x, pac.m_y);SVec2 moveTo(0, 0);if (!pellets.empty() || !superPellets.empty()){SVec2* moveToIter = !superPellets.empty()? FindClosestPos(RNG(superPellets), pacPos): FindClosestPos(RNG(pellets), pacPos);if (!superPellets.empty()){superPellets.erase_swap(moveToIter);}else{pellets.erase_swap(moveToIter);}moveTo = *moveToIter;}else{cerr << "Selecting random for " << pac.m_pacId << "\n";cerr << "Adjacent size: " << m_data.m_map.GetCell(pacPos).m_adjacentCells.size() << "\n";moveTo = *GetRandomItem(RNG(m_data.m_map.GetCell(pacPos).m_adjacentCells));}SOutputCommand cmd;cmd.m_commandType = ECommandType::Move;cmd.m_pacId = pac.m_pacId;cmd.m_x = moveTo[0];cmd.m_y = moveTo[1];output.m_commands.push_back(cmd);}return output;}}
using namespace std;namespace SC2020{array<SVec2, 4> const ADJACENT_MASKS = {SVec2(-1, 0), {1, 0}, {0, -1}, {0, 1}};SMap BuildMap(SInputDataMap const& inDataMap){SMap map(inDataMap.m_width, inDataMap.m_height);auto const rows = inDataMap.m_rows;for (size_t y = 0; y < inDataMap.m_height; ++y){for (size_t x = 0; x < inDataMap.m_width; ++x){auto& cell = map.GetCell({ x, y });cell.m_isFloor = rows[y][x] == ' ';for (auto const mask : ADJACENT_MASKS){SVec2 newPos = mask + SVec2(x, y);if (newPos[1] >= 0 && newPos[1] < map.GetHeight()){if (newPos[0] < 0) newPos[0] += (SVec2::ValueType)map.GetWidth();if (newPos[0] >= map.GetWidth()) newPos[0] -= (SVec2::ValueType)map.GetWidth();if (inDataMap.m_rows[newPos[1]][newPos[0]] == ' '){cell.m_adjacentCells.push_back(newPos);}}}}}return map;}}
char const* ToString(SVec2i const vec2){static char buff[32];sprintf(buff, "{%i:%i}", vec2[0], vec2[1]);return buff;}std::ostream& operator<<(std::ostream& out, SVec2i const& vec){out << vec[0] << " " << vec[1];return out;}std::istream& operator>>(std::istream& in, SVec2i& vec){in >> vec[0] >> vec[1];return in;}
#include <random>
#include <time.h>
unsigned int InitializeRandom(){unsigned int const seed = (unsigned int)time(nullptr);srand(seed);return seed;}unsigned int GetRandomUInt(unsigned int const minVal, unsigned int const maxVal){assert(minVal < maxVal);unsigned int const range = maxVal - minVal;return rand() % range + minVal;}
namespace SC2020{std::string ToString(SOutputCommand const& cmd){char buff[256];switch (cmd.m_commandType){case ECommandType::Move:sprintf(buff, "MOVE %i %i %i", cmd.m_pacId, cmd.m_x, cmd.m_y);break;case ECommandType::Speed:sprintf(buff, "SPEED %i", cmd.m_pacId);break;default:assert(false);}return buff;}}
