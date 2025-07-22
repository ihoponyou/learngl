#include <cstdio>
#include <iostream>
#include <ostream>
#include <string>
// clang-format off
#include "glad/glad.h"
#include "GLFW/glfw3.h"
#include "glm/ext/matrix_transform.hpp"
// clang-format on
#define STB_IMAGE_IMPLEMENTATION
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"
#include "stb/stb_image.h"

#include "shader.h"

// clang-format off
float vertices[]{
    // position             // texture
    0.5f,   0.5f,   0.0f,   1.0f, 1.0f, // top right
    0.5f,   -0.5f,  0.0f,   1.0f, 0.0f, // bottom right
    -0.5f,  -0.5f,  0.0f,   0.0f, 0.0f, // bottom left
    -0.5f,   0.5f,  0.0f,   0.0f, 1.0f, // top left
};

unsigned int indices[]{
    0, 1, 3,
    3, 2, 1,
};
// clang-format on

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

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);
}

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

    // position
    glVertexAttribPointer(0,
                          3,
                          GL_FLOAT,
                          GL_FALSE,
                          5 * sizeof(float),
                          (void*)0);
    glEnableVertexAttribArray(0);
    // texture coordinates
    glVertexAttribPointer(1,
                          2,
                          GL_FLOAT,
                          GL_FALSE,
                          5 * sizeof(float),
                          (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    // wireframe mode
    // glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

    unsigned int boxTexture;
    glGenTextures(1, &boxTexture);
    glBindTexture(GL_TEXTURE_2D, boxTexture);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    int textureWidth, textureHeight, textureColorChannels;
    stbi_set_flip_vertically_on_load(true);

    unsigned char* textureData = stbi_load("assets/container.jpg",
                                           &textureWidth,
                                           &textureHeight,
                                           &textureColorChannels,
                                           0);
    if (textureData)
    {
        glTexImage2D(GL_TEXTURE_2D,
                     0,
                     GL_RGB,
                     textureWidth,
                     textureHeight,
                     0,
                     GL_RGB,
                     GL_UNSIGNED_BYTE,
                     textureData);
        glGenerateMipmap(GL_TEXTURE_2D);
    }
    else
    {
        std::cout << "error: failed to load box texture" << std::endl;
    }
    stbi_image_free(textureData);

    unsigned int epicTexture;
    glGenTextures(1, &epicTexture);
    glBindTexture(GL_TEXTURE_2D, epicTexture);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    textureData = stbi_load("assets/awesomeface.png",
                            &textureWidth,
                            &textureHeight,
                            &textureColorChannels,
                            0);
    if (textureData)
    {
        glTexImage2D(GL_TEXTURE_2D,
                     0,
                     GL_RGBA,
                     textureWidth,
                     textureHeight,
                     0,
                     GL_RGBA,
                     GL_UNSIGNED_BYTE,
                     textureData);
        glGenerateMipmap(GL_TEXTURE_2D);
    }
    else
    {
        std::cout << "error: failed to load epic texture" << std::endl;
    }
    stbi_image_free(textureData);

    shader.use();
    shader.setInt("boxTexture", 0);
    shader.setInt("epicTexture", 1);

    while (!glfwWindowShouldClose(window))
    {
        processInput(window);

        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, boxTexture);
        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, epicTexture);

        glm::mat4 translationMatrix{1.0f};
        translationMatrix =
            glm::translate(translationMatrix, glm::vec3(0.5f, -0.5f, 0.0f));
        translationMatrix = glm::rotate(translationMatrix,
                                        (float)glfwGetTime(),
                                        glm::vec3(0.0f, 0.0f, 1.0f));

        shader.use();
        unsigned int transformUniformLocation =
            glGetUniformLocation(shader.id, "transform");
        shader.setMat4("transform", translationMatrix);

        glBindVertexArray(vao);
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

        glBindVertexArray(0);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glDeleteVertexArrays(1, &vao);
    glDeleteBuffers(1, &vbo);
    glDeleteBuffers(1, &ebo);

    glfwTerminate();
    return 0;
}
