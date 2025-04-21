#pragma once

#include <glad/glad.h>

#include <string>

namespace gltk
{
    class ProgramBuilder
    {
    private:
        static int GetShaderTypeIdx(GLenum shader_type);
        static GLenum GetShaderType(int idx);
    public:
        ProgramBuilder();
        ~ProgramBuilder() noexcept;
        ProgramBuilder(const ProgramBuilder&) = delete;
        ProgramBuilder(ProgramBuilder&&) noexcept = delete;
        ProgramBuilder& operator=(const ProgramBuilder&) = delete;
        ProgramBuilder& operator=(ProgramBuilder&&) noexcept = delete;
    public:
        std::string InfoLog() const noexcept { return m_info_log; }
    public:
        bool Compile(GLenum shader_type, const GLchar* src_str);
        bool Compile(GLenum shader_type, GLsizei count, const GLchar** lines, const GLint* line_lengths);
        GLuint Link();
    public:
        void Delete(GLenum shader_type);
    private:
        constexpr static int INFO_LOG_SIZE{ 1024 };
        constexpr static int MAX_SHADER_INDEX{ 3 };
    private:
        bool m_status;
        char m_info_log[INFO_LOG_SIZE];
        GLuint m_shader[MAX_SHADER_INDEX];
    };
}
