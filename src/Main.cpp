#include <glad/glad.h> // NOTE: before glfw
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>
#include <stb_image.h>
#include <tiny_gltf.h>

#include <gltk/GLTK.h>

#include <iostream>
#include <format>

constexpr const char* WINDOW_TITLE{ "gltk" };
constexpr int WINDOW_W{ 1280 };
constexpr int WINDOW_H{ 720 };

static void OnGLFWError(int error, const char* description)
{
    std::cerr << std::format("[GLFW({})]: {}\n", error, description);
}
static void OnGLFWResize(GLFWwindow*, int width, int height)
{
    gltk_GLCheck(glViewport(0, 0, width, height));
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
        // test glm
        {
            glm::vec3 vec{};
        }

        // test stb
        {
            int w{};
            int h{};
            int channels{};
            stbi_load("wow", &w, &h, &channels, 4);
        }

        // test tinygltf
        {
            tinygltf::Scene scene{};
        }

        glfwSetErrorCallback(OnGLFWError);

        glfwInit();
        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
        glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
        #if !defined(NDEBUG)
        glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, true);
        #endif

        #ifdef __APPLE__
        glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
        #endif

        GLFWwindow* window{ glfwCreateWindow(WINDOW_W, WINDOW_H, WINDOW_TITLE, nullptr, nullptr) };
        if (!window)
        {
            std::cout << "Failed to create GLFW window" << std::endl;
            glfwTerminate();
            return -1;
        }
        glfwMakeContextCurrent(window);

        if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
        {
            std::cout << "Failed to initialize GLAD" << std::endl;
            return -1;
        }

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

        glfwSwapInterval(1); // enable vsync
        glfwSetFramebufferSizeCallback(window, OnGLFWResize);

        // Setup Dear ImGui context
        IMGUI_CHECKVERSION();
        ImGui::CreateContext();

        // Setup Dear ImGui style
        ImGui::StyleColorsDark(); // or ImGui::StyleColorsLight();

        // Setup Platform/Renderer backends
        {
            constexpr const char* IMGUI_GLSL_VERSION{ "#version 130" };
            ImGui_ImplGlfw_InitForOpenGL(window, true);
            ImGui_ImplOpenGL3_Init(IMGUI_GLSL_VERSION);
        }

        // Load Fonts
        // - If no fonts are loaded, dear imgui will use the default font. You can also load multiple fonts and use ImGui::PushFont()/PopFont() to select them.
        // - AddFontFromFileTTF() will return the ImFont* so you can store it if you need to select the font among multiple.
        // - If the file cannot be loaded, the function will return a nullptr. Please handle those errors in your application (e.g. use an assertion, or display an error and quit).
        // - The fonts will be rasterized at a given size (w/ oversampling) and stored into a texture when calling ImFontAtlas::Build()/GetTexDataAsXXXX(), which ImGui_ImplXXXX_NewFrame below will call.
        // - Use '#define IMGUI_ENABLE_FREETYPE' in your imconfig file to use Freetype for higher quality font rendering.
        // - Read 'docs/FONTS.md' for more instructions and details.
        // - Remember that in C/C++ if you want to include a backslash \ in a string literal you need to write a double backslash \\ !
        // - Our Emscripten build process allows embedding fonts to be accessible at runtime from the "fonts/" folder. See Makefile.emscripten for details.
        // io.Fonts->AddFontDefault();
        // io.Fonts->AddFontFromFileTTF("c:\\Windows\\Fonts\\segoeui.ttf", 18.0f);
        // io.Fonts->AddFontFromFileTTF("../../misc/fonts/DroidSans.ttf", 16.0f);
        // io.Fonts->AddFontFromFileTTF("../../misc/fonts/Roboto-Medium.ttf", 16.0f);
        // io.Fonts->AddFontFromFileTTF("../../misc/fonts/Cousine-Regular.ttf", 15.0f);
        // ImFont* font = io.Fonts->AddFontFromFileTTF("c:\\Windows\\Fonts\\ArialUni.ttf", 18.0f, nullptr, io.Fonts->GetGlyphRangesJapanese());
        // IM_ASSERT(font != nullptr);

        // ui state
        bool show_demo_window{ true };
        bool show_another_window{ false };
        ImVec4 clear_color{ 0.45f, 0.55f, 0.60f, 1.00f };

        while (!glfwWindowShouldClose(window))
        {
            glfwPollEvents();

            // process input
            {
                if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
                {
                    glfwSetWindowShouldClose(window, true);
                }
            }

            // render
            {
                gltk_GLCheck(glClearColor(clear_color.x * clear_color.w, clear_color.y * clear_color.w, clear_color.z * clear_color.w, clear_color.w));
                gltk_GLCheck(glClear(GL_COLOR_BUFFER_BIT));
            }

            // render ui
            {
                // Start the Dear ImGui frame
                ImGui_ImplOpenGL3_NewFrame();
                ImGui_ImplGlfw_NewFrame();
                ImGui::NewFrame();

                // 1. Show the big demo window (Most of the sample code is in ImGui::ShowDemoWindow()! You can browse its code to learn more about Dear ImGui!).
                if (show_demo_window)
                {
                    ImGui::ShowDemoWindow(&show_demo_window);
                }

                // 2. Show a simple window that we create ourselves. We use a Begin/End pair to create a named window.
                {
                    static float f{};
                    static int counter{};

                    ImGui::Begin("Hello, world!"); // Create a window called "Hello, world!" and append into it.

                    ImGui::Text("This is some useful text.");          // Display some text (you can use a format strings too)
                    ImGui::Checkbox("Demo Window", &show_demo_window); // Edit bools storing our window open/close state
                    ImGui::Checkbox("Another Window", &show_another_window);

                    ImGui::SliderFloat("float", &f, 0.0f, 1.0f);             // Edit 1 float using a slider from 0.0f to 1.0f
                    ImGui::ColorEdit3("clear color", (float*)&clear_color); // Edit 3 floats representing a color

                    if (ImGui::Button("Button")) // Buttons return true when clicked (most widgets return true when edited/activated)
                    {
                        counter++;
                    }
                    ImGui::SameLine();
                    ImGui::Text("counter = %d", counter);

                    ImGui::End();
                }

                // 3. Show another simple window.
                if (show_another_window)
                {
                    ImGui::Begin("Another Window", &show_another_window); // Pass a pointer to our bool variable (the window will have a closing button that will clear the bool when clicked)
                    ImGui::Text("Hello from another window!");
                    if (ImGui::Button("Close Me"))
                    {
                        show_another_window = false;
                    }
                    ImGui::End();
                }

                // Rendering
                ImGui::Render();
                int display_w{}, display_h{};
                glfwGetFramebufferSize(window, &display_w, &display_h);
                glViewport(0, 0, display_w, display_h);
                ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
            }

            // present
            {
                glfwSwapBuffers(window);
            }
        }

        // cleanup
        {
            ImGui_ImplOpenGL3_Shutdown();
            ImGui_ImplGlfw_Shutdown();
            ImGui::DestroyContext();

            glfwDestroyWindow(window);

            glfwTerminate();
        }
    }
    catch (const gltk::Crash& e)
    {
        std::cerr << e.What() << '\n';
    }

    return 0;
}
