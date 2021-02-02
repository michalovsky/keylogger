#pragma once

#include <ctime>

namespace keylogger
{
struct Timestamp
{
    Timestamp()
    {
        time_t ms;
        time(&ms);

        struct tm* timeInfo = localtime(&ms);

        year = 1900 + timeInfo->tm_year;
        month = timeInfo->tm_mon + 1;
        day = timeInfo->tm_mday;
        hour = timeInfo->tm_hour;
        minutes = timeInfo->tm_min;
        seconds = timeInfo->tm_sec;
    }

    int day, month, year, hour, minutes, seconds;
};

}
