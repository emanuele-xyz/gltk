#include <gltk/Crash.h>

#include <format>
#include <stacktrace>

#if defined(_WIN32)
#include <Windows.h>
#endif

namespace gltk
{
    Crash::Crash(const std::string& file, int line, const std::string message)
        : m_what{ std::format("{}({}): {}\n{}", file, line, message, std::stacktrace::current(1)) }
    {
    }

    void TryDebugBreak()
    {
        #if defined(_WIN32)
        if (IsDebuggerPresent())
        {
            __debugbreak();
        }
        #endif
    }
}
