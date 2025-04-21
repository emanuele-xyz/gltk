#include <glad/glad.h> // NOTE: before glfw
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>
#include <scope_guard.hpp>
#include <stb_image.h>
#include <tiny_gltf.h>

#include <gltk/GLTK.h>

#include <iostream>
#include <format>

constexpr const char* WINDOW_TITLE{ "gltk" };
constexpr int WINDOW_W{ 1280 };
constexpr int WINDOW_H{ 720 };
constexpr const char* IMGUI_GLSL_VERSION{ "#version 130" };

// TODO: load these from files
constexpr const char* VERTEX_SHADER_SOURCE{
    "#version 330 core\n"
    "layout (location = 0) in vec3 aPos;\n"
    "void main()\n"
    "{\n"
    "   gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
    "}\0"
};
constexpr const char* FRAGMENT_SHADER_SOURCE{
    "#version 330 core\n"
    "out vec4 FragColor;\n"
    "void main()\n"
    "{\n"
    "   FragColor = vec4(1.0f, 0.5f, 0.2f, 1.0f);\n"
    "}\n\0"
};

static void OnGLFWError(int error, const char* description)
{
    std::cerr << std::format("[GLFW({})]: {}\n", error, description);
}

static void GLAPIENTRY OnOpenGLDebugMessage(GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei /*length*/, const GLchar* message, const void* /*user*/)
{
    // ignore non-significant error/warning codes
    if (id == 131185)
    {
        return;
    }

    const char* source_str{};
    switch (source)
    {
    case GL_DEBUG_SOURCE_API: { source_str = "SOURCE_API"; } break;
    case GL_DEBUG_SOURCE_WINDOW_SYSTEM: { source_str = "SOURCE_WINDOW_SYSTEM"; } break;
    case GL_DEBUG_SOURCE_SHADER_COMPILER: { source_str = "SOURCE_SHADER_COMPILER"; } break;
    case GL_DEBUG_SOURCE_THIRD_PARTY: { source_str = "SOURCE_THIRD_PARTY"; } break;
    case GL_DEBUG_SOURCE_APPLICATION: { source_str = "SOURCE_APPLICATION"; } break;
    case GL_DEBUG_SOURCE_OTHER: { source_str = "SOURCE_OTHER"; } break;
    }
    const char* type_str{};
    switch (type)
    {
    case GL_DEBUG_TYPE_ERROR: { type_str = "TYPE_ERROR"; } break;
    case GL_DEBUG_TYPE_DEPRECATED_BEHAVIOR: { type_str = "TYPE_DEPRECATED_BEHAVIOR"; } break;
    case GL_DEBUG_TYPE_UNDEFINED_BEHAVIOR: { type_str = "TYPE_UNDEFINED_BEHAVIOR"; } break;
    case GL_DEBUG_TYPE_PORTABILITY: { type_str = "TYPE_PORTABILITY"; } break;
    case GL_DEBUG_TYPE_PERFORMANCE: { type_str = "TYPE_PERFORMANCE"; } break;
    case GL_DEBUG_TYPE_MARKER: { type_str = "TYPE_MARKER"; } break;
    case GL_DEBUG_TYPE_PUSH_GROUP: { type_str = "TYPE_PUSH_GROUP"; } break;
    case GL_DEBUG_TYPE_POP_GROUP: { type_str = "TYPE_POP_GROUP"; } break;
    case GL_DEBUG_TYPE_OTHER: { type_str = "TYPE_OTHER"; } break;
    }
    const char* severity_str{};
    switch (severity)
    {
    case GL_DEBUG_SEVERITY_HIGH: { severity_str = "SEVERITY_HIGH"; } break;
    case GL_DEBUG_SEVERITY_MEDIUM: { severity_str = "SEVERITY_MEDIUM"; } break;
    case GL_DEBUG_SEVERITY_LOW: { severity_str = "SEVERITY_LOW"; } break;
    case GL_DEBUG_SEVERITY_NOTIFICATION: { severity_str = "SEVERITY_NOTIFICATION"; } break;
    }

    std::cerr << std::format("[GL({})]:{}:{}:{}: {}\n", id, source_str, type_str, severity_str, message);
}

int main()
{
    try
    {
        glfwSetErrorCallback(OnGLFWError);

        glfwInit();
        auto glfw_terminate_on_exit{ sg::make_scope_guard([]() { glfwTerminate(); }) };

        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
        glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
        #if !defined(NDEBUG)
        glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, true);
        #endif

        #ifdef __APPLE__
        glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
        #endif

        // create window
        GLFWwindow* window{};
        gltk_Check(window = glfwCreateWindow(WINDOW_W, WINDOW_H, WINDOW_TITLE, nullptr, nullptr));
        auto destroy_window_on_exit{ sg::make_scope_guard([=]() { glfwDestroyWindow(window); }) };

        // make window OpenGL context current on the calling thread
        glfwMakeContextCurrent(window);

        // load OpenGL functions
        gltk_Check(gladLoadGLLoader((GLADloadproc)glfwGetProcAddress));

        // check whether we got a debug context
        #if !defined(NDEBUG)
        {
            int flags{};
            gltk_GLCheck(glGetIntegerv(GL_CONTEXT_FLAGS, &flags));
            if (!(flags & GL_CONTEXT_FLAG_DEBUG_BIT))
            {
                std::cerr << "Failed to create OpenGL debug context.\n";
            }

            if (GLAD_GL_KHR_debug)
            {
                gltk_GLCheck(glEnable(GL_DEBUG_OUTPUT));
                gltk_GLCheck(glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS));
                gltk_GLCheck(glDebugMessageCallback(OnOpenGLDebugMessage, nullptr));
                gltk_GLCheck(glDebugMessageControl(GL_DONT_CARE, GL_DONT_CARE, GL_DONT_CARE, 0, nullptr, true));
            }
            else if (GLAD_GL_ARB_debug_output)
            {
                gltk_GLCheck(glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS_ARB));
                gltk_GLCheck(glDebugMessageCallbackARB(OnOpenGLDebugMessage, nullptr));
                gltk_GLCheck(glDebugMessageControlARB(GL_DONT_CARE, GL_DONT_CARE, GL_DONT_CARE, 0, nullptr, true));
            }
            else
            {
                std::cerr << "No supported OpenGL debug output extension available.\n";
            }
        }
        #endif

        // configure context
        glfwSwapInterval(1); // enable vsync

        // create imgui context
        IMGUI_CHECKVERSION();
        gltk_Check(ImGui::CreateContext());
        auto destroy_imgui_context_on_exit{ sg::make_scope_guard([]() { ImGui::DestroyContext(); }) };

        // set imgui style
        ImGui::StyleColorsDark(); // or ImGui::StyleColorsLight();

        // setup imgui platform and renderer backends
        gltk_Check(ImGui_ImplGlfw_InitForOpenGL(window, true));
        auto shutdown_imgui_glfw_impl_on_exit{ sg::make_scope_guard([]() { ImGui_ImplGlfw_Shutdown();}) };
        gltk_Check(ImGui_ImplOpenGL3_Init(IMGUI_GLSL_VERSION));
        auto shutdown_imgui_opengl_impl_on_exit{ sg::make_scope_guard([]() { ImGui_ImplOpenGL3_Shutdown(); }) };

        // build shaders
        gltk::ProgramBuilder program_builder{};
        if (!program_builder.Compile(GL_VERTEX_SHADER, VERTEX_SHADER_SOURCE))
        {
            std::cerr << std::format("[GLTK]: {}\n", program_builder.InfoLog());
        }
        if (!program_builder.Compile(GL_FRAGMENT_SHADER, FRAGMENT_SHADER_SOURCE))
        {
            std::cerr << std::format("[GLTK]: {}\n", program_builder.InfoLog());
        }
        GLuint shader_program{ program_builder.Link() };
        if (!shader_program)
        {
            std::cerr << std::format("[GLTK]: {}\n", program_builder.InfoLog());
        }

        while (!glfwWindowShouldClose(window))
        {
            // poll input events
            glfwPollEvents();

            // process input
            {
                if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
                {
                    glfwSetWindowShouldClose(window, true);
                }
            }

            // update viewport
            {
                int w{};
                int h{};
                glfwGetFramebufferSize(window, &w, &h);
                gltk_GLCheck(glViewport(0, 0, w, h));
            }

            // render scene
            {
                // clear
                gltk_GLCheck(glClearColor(0.2f, 0.3f, 0.3f, 1.0f));
                gltk_GLCheck(glClear(GL_COLOR_BUFFER_BIT));
            }

            // imgui
            {
                // start frame
                ImGui_ImplOpenGL3_NewFrame();
                ImGui_ImplGlfw_NewFrame();
                ImGui::NewFrame();

                // imgui logic
                {
                }

                // render
                ImGui::Render();
                ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
            }

            // present
            {
                glfwSwapBuffers(window);
            }
        }
    }
    catch (const gltk::Crash& e)
    {
        std::cerr << e.What() << '\n';
    }

    return 0;
}
