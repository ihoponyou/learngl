// clang-format off
#include <cmath>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
// clang-format on
#include <iostream>
#include <ostream>
#include <string>

#include "shader.h"

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);
}

bool wireframeMode{false};
void processInput(GLFWwindow* window)
{
    if (glfwGetKey(window, GLFW_KEY_Q))
    {
        glfwSetWindowShouldClose(window, true);
    }
    if (glfwGetKey(window, GLFW_KEY_SPACE))
    {
        wireframeMode = !wireframeMode;
        glPolygonMode(GL_FRONT_AND_BACK, wireframeMode ? GL_LINE : GL_FILL);
    }
}

// clang-format off
float vertices[]{
    // position             // color
    -0.5f,  -0.5f,  0.0f,   1.0f, 0.0f, 0.0f, // bottom left
    0.5f,   -0.5f,  0.0f,   0.0f, 1.0f, 0.0f, // bottom right
    0.0f,   0.5f,   0.0f,   0.0f, 0.0f, 1.0f, // top middle
};

unsigned int indices[]{
    0, 1, 2,
    2, 3, 1,
};
// clang-format on

int main(int argc, char* argv[])
{
    std::string pathToExe(argv[0]);
    std::string pathToExeDir = pathToExe.substr(0, pathToExe.find_last_of('/'));
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    GLFWwindow* window = glfwCreateWindow(800, 600, "learngl", NULL, NULL);
    if (window == NULL)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "failed to initialize GLAD" << std::endl;
        return -1;
    }

    glViewport(0, 0, 800, 600);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    unsigned int vao;
    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);

    unsigned int vbo;
    glGenBuffers(1, &vbo);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof vertices, vertices, GL_STATIC_DRAW);

    unsigned int ebo;
    glGenBuffers(1, &ebo);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER,
                 sizeof indices,
                 indices,
                 GL_STATIC_DRAW);

    // works since cmake copies shaders/ into build/
    Shader shader("shaders/shader.vs", "shaders/shader.fs");

    glVertexAttribPointer(0,
                          3,
                          GL_FLOAT,
                          GL_FALSE,
                          6 * sizeof(float),
                          (void*)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1,
                          3,
                          GL_FLOAT,
                          GL_FALSE,
                          6 * sizeof(float),
                          (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    // wireframe mode
    // glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

    while (!glfwWindowShouldClose(window))
    {
        processInput(window);

        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        shader.use();
        shader.setFloat("xOffset", 0.5f);

        glBindVertexArray(vao);
        glDrawArrays(GL_TRIANGLES, 0, 3);
        glBindVertexArray(0);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwTerminate();
    return 0;
}
