#include <gltk/Crash.h>

#include <format>
#include <stacktrace>

namespace gltk
{
    Error::Error(const std::string& file, int line, const std::string message)
        : m_what{ std::format("{}({}): {}\n{}", file, line, message, std::stacktrace::current(1)) }
    {
    }
}
