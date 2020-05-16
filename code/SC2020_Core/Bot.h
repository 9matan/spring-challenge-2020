#pragma once

#include <memory>

#include "OutputData.h"

namespace SC2020
{
    class CBotImpl;
    struct SInitInputData;
    struct SInputData;

    class CBot
    {
    public:
        CBot();
        ~CBot();

        SOutputData FirstUpdate(SInitInputData const& initInData, SInputData const& inData);
        SOutputData Update(SInputData const& inData);

    private:
        std::unique_ptr<CBotImpl> m_impl;
    };
}