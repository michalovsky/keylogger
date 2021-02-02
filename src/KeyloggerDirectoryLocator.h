#pragma once

#include <string>

namespace keylogger
{
class KeyloggerDirectoryLocator
{
public:
    static std::string getAppDataKeyloggerPath(bool appendSeparator = false);
};
}