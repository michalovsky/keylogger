#include <string>

#include "Timestamp.h"

namespace keylogger
{
class TimestampFetcher
{
public:
    static std::string getFormattedTimestamp(const std::string& separator = ":");

private:
    static std::string getFormattedDate(const Timestamp&);
    static std::string getFormattedTime(const Timestamp&, const std::string& separator);
};
}