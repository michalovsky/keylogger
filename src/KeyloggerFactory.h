#pragma once

#include "Keylogger.h"

namespace keylogger
{
class KeyloggerFactory
{
public:
    static std::unique_ptr<Keylogger> createKeylogger();
};
}