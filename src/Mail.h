#pragma once

#include <string>
#include <vector>

namespace keylogger
{
struct Mail
{
    std::string emailSourceAddress;
    std::string emailCopyToAddress;
    std::string emailTargetAddress;
    std::string subject;
    std::string body;
    std::vector<std::string> attachments;
};
}