#include <cmath>
#include <cstdio>
#include <iostream>
#include <ostream>
#include <string>
// clang-format off
#include "glad/glad.h"
#include "GLFW/glfw3.h"
#include "glm/ext/matrix_clip_space.hpp"
#include "glm/ext/matrix_transform.hpp"
#include "glm/ext/vector_float3.hpp"
#include "glm/trigonometric.hpp"
// clang-format on
#define STB_IMAGE_IMPLEMENTATION
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"
#include "stb/stb_image.h"

#include "shader.h"

// clang-format off
float vertices[] = {
    -0.5f, -0.5f, -0.5f,  0.0f, 0.0f,
     0.5f, -0.5f, -0.5f,  1.0f, 0.0f,
     0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
     0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
    -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
    -0.5f, -0.5f, -0.5f,  0.0f, 0.0f,

    -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
     0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
     0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
     0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
    -0.5f,  0.5f,  0.5f,  0.0f, 1.0f,
    -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,

    -0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
    -0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
    -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
    -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
    -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
    -0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

     0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
     0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
     0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
     0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
     0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
     0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

    -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
     0.5f, -0.5f, -0.5f,  1.0f, 1.0f,
     0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
     0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
    -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
    -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,

    -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
     0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
     0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
     0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
    -0.5f,  0.5f,  0.5f,  0.0f, 0.0f,
    -0.5f,  0.5f, -0.5f,  0.0f, 1.0f
};

unsigned int indices[]{
    0, 1, 3,
    3, 2, 1,
};

glm::vec3 cubePositions[] = {
    glm::vec3( 0.0f,  0.0f,  0.0f), 
    glm::vec3( 2.0f,  5.0f, -15.0f), 
    glm::vec3(-1.5f, -2.2f, -2.5f),  
    glm::vec3(-3.8f, -2.0f, -12.3f),  
    glm::vec3( 2.4f, -0.4f, -3.5f),  
    glm::vec3(-1.7f,  3.0f, -7.5f),  
    glm::vec3( 1.3f, -2.0f, -2.5f),  
    glm::vec3( 1.5f,  2.0f, -2.5f), 
    glm::vec3( 1.5f,  0.2f, -1.5f), 
    glm::vec3(-1.3f,  1.0f, -1.5f)  
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

    float screenWidth{800.0f}, screenHeight{600.0f};
    GLFWwindow* window =
        glfwCreateWindow(screenWidth, screenHeight, "learngl", NULL, NULL);
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

    glEnable(GL_DEPTH_TEST);

    while (!glfwWindowShouldClose(window))
    {
        processInput(window);

        float time = glfwGetTime();

        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, boxTexture);
        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, epicTexture);

        // inverse transformation of where we want the camera
        glm::mat4 viewMatrix{1.0f};
        viewMatrix = glm::rotate(viewMatrix,
                                 glm::radians(-40.0f),
                                 glm::vec3(1.0f, 0.0f, 0.0f));
        viewMatrix = glm::translate(viewMatrix, glm::vec3(0.0f, 3.0f, -3.0f));

        glm::mat4 projectionMatrix{1.0f};
        projectionMatrix =
            glm::perspective(glm::radians(71.0f), 16.0f / 9.0f, 0.1f, 100.0f);

        shader.use();
        shader.setMat4("view", viewMatrix);
        shader.setMat4("projection", projectionMatrix);

        glBindVertexArray(vao);
        for (int i = 0; i < 10; ++i)
        {
            glm::mat4 modelMatrix{1.0f};
            modelMatrix = glm::translate(modelMatrix, cubePositions[i]);
            if (i % 3 == 0)
            {
                modelMatrix = glm::rotate(modelMatrix,
                                          time * glm::radians(-55.0f),
                                          glm::vec3(1.0f, 1.0f, 0.0f));
            }
            shader.setMat4("model", modelMatrix);
            glDrawArrays(GL_TRIANGLES, 0, 36);
        }

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
