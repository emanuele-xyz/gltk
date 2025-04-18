#include <glad/glad.h> // NOTE: before glfw
#include <GLFW/glfw3.h>

#include <iostream>

#include <gltk/Crash.h>

constexpr const char *WINDOW_TITLE{"gltk"};
constexpr int WINDOW_W{1280};
constexpr int WINDOW_H{720};

static void FramebufferSizeCallback(GLFWwindow *, int width, int height)
{
    glViewport(0, 0, width, height);
}

static void ProcessInput(GLFWwindow *window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
    {
        glfwSetWindowShouldClose(window, true);
    }
}

int main()
{
    try
    {
        throw gltk::Error{__FILE__, __LINE__, "Wow"};

        glfwInit();
        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
        glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef __APPLE__
        glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

        GLFWwindow *window{glfwCreateWindow(WINDOW_W, WINDOW_H, WINDOW_TITLE, nullptr, nullptr)};
        if (!window)
        {
            std::cout << "Failed to create GLFW window" << std::endl;
            glfwTerminate();
            return -1;
        }
        glfwMakeContextCurrent(window);
        glfwSetFramebufferSizeCallback(window, FramebufferSizeCallback);

        if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
        {
            std::cout << "Failed to initialize GLAD" << std::endl;
            return -1;
        }

        while (!glfwWindowShouldClose(window))
        {
            glfwPollEvents();
            ProcessInput(window);

            glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
            glClear(GL_COLOR_BUFFER_BIT);

            glfwSwapBuffers(window);
        }

        glfwTerminate();
    }
    catch (const gltk::Error &e)
    {
        std::cerr << e.What() << '\n';
    }

    return 0;
}
