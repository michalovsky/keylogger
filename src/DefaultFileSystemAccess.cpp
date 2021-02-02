#include "DefaultFileSystemAccess.h"

#include <experimental/filesystem>
#include <filesystem>
#include <fstream>
#include <sstream>

#include "exceptions/FileNotFound.h"

namespace keylogger
{
namespace
{
enum class Result
{
    Success,
    Failure
};

Result write(std::ofstream& fileStream, const std::string& data);
}

namespace fs = std::experimental::filesystem;

void DefaultFileSystemAccess::tryToWrite(const std::string& absolutePath, const std::string& content) const
{
    std::ofstream fileStream{absolutePath};

    if (write(fileStream, content) == Result::Failure)
    {
        throw exceptions::FileNotFound("File not found: " + absolutePath);
    }
}

void DefaultFileSystemAccess::tryToAppend(const std::string& absolutePath, const std::string& content) const
{
    std::ofstream fileStream{absolutePath, std::ofstream::app};

    if (write(fileStream, content) == Result::Failure)
    {
        throw exceptions::FileNotFound("File not found: " + absolutePath);
    }
}

std::string DefaultFileSystemAccess::tryToReadContent(const std::string& absolutePath) const
{
    std::ifstream fileStream{absolutePath};
    std::stringstream buffer;

    if (fileStream.is_open())
    {
        buffer << fileStream.rdbuf();
    }
    else
    {
        throw exceptions::FileNotFound("File not found: " + absolutePath);
    }

    return buffer.str();
}

void DefaultFileSystemAccess::createDirectory(const std::string& absolutePath) const
{
    fs::create_directory(absolutePath);
}

namespace
{
Result write(std::ofstream& fileStream, const std::string& data)
{
    if (fileStream.is_open())
    {
        fileStream << data;
        return Result::Success;
    }
    return Result::Failure;
}
}
}