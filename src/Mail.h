#pragma once

#include <string>
#include <optional>

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