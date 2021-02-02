#pragma once

#include <stdexcept>

namespace keylogger::exceptions
{
struct FileNotFound : std::runtime_error
{
    using std::runtime_error::runtime_error;
};
}