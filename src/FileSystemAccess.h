#pragma once

#include <string>

namespace keylogger
{
class FileSystemAccess
{
public:
    virtual ~FileSystemAccess() = default;

    virtual void tryToWrite(const std::string& absolutePath, const std::string& content) const = 0;
    virtual void tryToAppend(const std::string& absolutePath, const std::string& content) const = 0;
    virtual std::string tryToReadContent(const std::string& absolutePath) const = 0;
    virtual void createDirectory(const std::string& absolutePath) const = 0;
    virtual bool exists(const std::string& absolutePath) const = 0;
};
}