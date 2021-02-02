#pragma once

#include <string>
#include <vector>

namespace keylogger::mail
{
struct Mail
{
    std::string emailSourceAddress;
    std::string emailTargetAddress;
    std::string subject;
    std::string body;
    std::vector<std::string> attachments;
};
}