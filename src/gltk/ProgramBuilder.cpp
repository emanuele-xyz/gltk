#include <gltk/ProgramBuilder.h>
#include <gltk/Check.h>
#include <gltk/GLCheck.h>

#include <scope_guard.hpp>

namespace gltk
{
    int ProgramBuilder::GetShaderTypeIdx(GLenum shader_type)
    {
        int idx{ -1 };
        switch (shader_type)
        {
        case GL_VERTEX_SHADER: { idx = 0; } break;
        case GL_GEOMETRY_SHADER: { idx = 1; } break;
        case GL_FRAGMENT_SHADER: { idx = 2; } break;
        default: { gltk_Unreachable(); } break;
        }
        gltk_Check(0 <= idx && idx <= MAX_SHADER_INDEX);
        return idx;
    }
    GLenum ProgramBuilder::GetShaderType(int idx)
    {
        gltk_Check(0 <= idx && idx <= MAX_SHADER_INDEX);
        GLenum shader_type{};
        switch (idx)
        {
        case 0: { shader_type = GL_VERTEX_SHADER; } break;
        case 1: { shader_type = GL_GEOMETRY_SHADER; } break;
        case 2: { shader_type = GL_FRAGMENT_SHADER; } break;
        default: { gltk_Unreachable(); } break;
        }
        return shader_type;
    }
    ProgramBuilder::ProgramBuilder()
        : m_status{}
        , m_info_log{}
        , m_shader{}
    {
    }
    ProgramBuilder::~ProgramBuilder()
    {
        for (int i{}; i < MAX_SHADER_INDEX; i++)
        {
            GLenum shader_type{ GetShaderType(i) };
            Delete(shader_type);
        }
    }
    bool ProgramBuilder::Compile(GLenum shader_type, const GLchar* src_str)
    {
        return Compile(shader_type, 1, &src_str, nullptr);
    }
    bool ProgramBuilder::Compile(GLenum shader_type, GLsizei count, const GLchar** lines, const GLint* line_lengths)
    {
        int shader_idx{ GetShaderTypeIdx(shader_type) };

        if (!m_shader[shader_idx])
        {
            gltk_GLCheck(m_shader[shader_idx] = glCreateShader(shader_type));
        }

        gltk_GLCheck(glShaderSource(m_shader[shader_idx], count, lines, line_lengths));
        gltk_GLCheck(glCompileShader(m_shader[shader_idx]));

        int success{ true };
        gltk_GLCheck(glGetShaderiv(m_shader[shader_idx], GL_COMPILE_STATUS, &success));
        if (!success)
        {
            gltk_GLCheck(glGetShaderInfoLog(m_shader[shader_idx], INFO_LOG_SIZE, nullptr, m_info_log));
        }
        return success;
    }
    GLuint ProgramBuilder::Link()
    {
        GLuint program{};
        gltk_GLCheck(program = glCreateProgram());
        auto delete_program_on_exit{ sg::make_scope_guard([=]() { glDeleteProgram(program); }) };

        for (int i{}; i < MAX_SHADER_INDEX; i++)
        {
            if (m_shader[i])
            {
                gltk_GLCheck(glAttachShader(program, m_shader[i]));
            }
        }
        gltk_GLCheck(glLinkProgram(program));

        int success{ false };
        gltk_GLCheck(glGetProgramiv(program, GL_LINK_STATUS, &success));
        if (success)
        {
            delete_program_on_exit.dismiss();
            return program;
        }
        else
        {
            gltk_GLCheck(glGetProgramInfoLog(program, INFO_LOG_SIZE, nullptr, m_info_log));
            return 0;
        }
    }
    void ProgramBuilder::Delete(GLenum shader_type)
    {
        int shader_idx{ GetShaderTypeIdx(shader_type) };
        if (m_shader[shader_idx])
        {
            gltk_GLCheck(glDeleteShader(m_shader[shader_idx]));
            m_shader[shader_idx] = 0;
        }
    }
}
