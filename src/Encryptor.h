#pragma once

#include <string>

namespace keylogger
{
class Encryptor
{
public:
    virtual ~Encryptor() = default;

    virtual std::string encrypt(const std::string&) const = 0;
};
}