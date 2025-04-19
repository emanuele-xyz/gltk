#pragma once

#include <string>

#define gltk_Crash(msg) do { ::gltk::TryDebugBreak(); throw ::gltk::Crash{ __FILE__, __LINE__, msg }; } while (false)

namespace gltk
{
    class Crash
    {
    public:
        Crash(const std::string& file, int line, const std::string message);
    public:
        const std::string& What() const noexcept { return m_what; }
    private:
        std::string m_what;
    };

    void TryDebugBreak();
}
