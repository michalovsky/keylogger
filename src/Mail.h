#pragma once

#include <optional>
#include <string>

namespace keylogger::mail
{
struct Mail
{
    std::string emailSourceAddress;
    std::string emailTargetAddress;
    std::string subject;
    std::string body;
    std::optional<std::string> attachment;
};
}