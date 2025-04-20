#include <gltk/GLCheck.h>

#include <glad/glad.h>

#include <format>
#include <iostream>

namespace gltk
{
    void PrintGLErrorsIfAny(const char* file, int line)
    {
        GLenum error_code{};
        while ((error_code = glGetError()) != GL_NO_ERROR)
        {
            std::string error{};
            switch (error_code)
            {
            case GL_INVALID_ENUM: { error = "GL_INVALID_ENUM"; } break;
            case GL_INVALID_VALUE: { error = "GL_INVALID_VALUE"; } break;
            case GL_INVALID_OPERATION: { error = "GL_INVALID_OPERATION"; } break;
            case GL_OUT_OF_MEMORY: { error = "GL_OUT_OF_MEMORY"; } break;
            case GL_INVALID_FRAMEBUFFER_OPERATION: { error = "GL_INVALID_FRAMEBUFFER_OPERATION"; } break;
            }
            std::cerr << std::format("[GL]:{}({}): {}\n", file, line, error);
        }
    }
}
