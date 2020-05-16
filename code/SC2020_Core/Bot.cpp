#include "Core_PCH.h"
#include "Bot.h"

#include "CodingameUtility\Random.h"

#include "BotImpl.h"

using namespace std;

namespace SC2020
{
    CBot::CBot() : m_impl(nullptr) {}
    CBot::~CBot() {}

    SOutputData CBot::FirstUpdate(SInitInputData const& initInData, SInputData const& inData)
    {
        auto const seed = InitializeRandom();
        cerr << "Seed: " << seed << "\n";

        m_impl = make_unique<CBotImpl>(initInData, inData);
        return m_impl->FirstUpdate(inData);
    }

    SOutputData CBot::Update(SInputData const& inData)
    {
        return m_impl->Update(inData);
    }
}