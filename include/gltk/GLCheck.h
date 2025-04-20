#pragma once

#define gltk_GLCheck(call) (call); ::gltk::PrintGLErrorsIfAny(__FILE__, __LINE__)

namespace gltk
{
    void PrintGLErrorsIfAny(const char* file, int line);
}
