#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include "Shaders/Shader.h"

void windowResizeCallback(GLFWwindow* window, int height, int width)
{
    glViewport(0, 0, height, width);
}

void processInput(GLFWwindow* window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
        glfwSetWindowShouldClose(window, true);
    };
}

int main(void)
{
    GLFWwindow* window;

    /* Initialize the library */
    if (!glfwInit())
        return -1;

    /* Create a windowed mode window and its OpenGL context */
    window = glfwCreateWindow(640, 480, "Hello World", NULL, NULL);
    if (!window)
    {
        glfwTerminate();
        return -1;
    }

    /* Make the window's context current */
    glfwMakeContextCurrent(window);

    if (glewInit() != GLEW_OK)
        std::cout << "Glew init error." << std::endl;

    float positions[] = {
        0.5f,  0.5f, 0.0f, 1.0f, 0.0f, 0.0f,   // top right
        0.5f, -0.5f, 0.0f, 0.0f, 1.0f, 0.0f,  // bottom right
       -0.5f, -0.5f, 0.0f, 0.0f, 0.0f, 1.0f,  // bottom left
       -0.5f,  0.5f, 0.0f, 0.5f, 0.5f, 0.5f, // top left 
    };

    unsigned int indices[] = {
        0, 1, 3,
        1, 2, 3
    };

    unsigned int VAO;
    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);

    unsigned int buffer;
    glGenBuffers(1, &buffer);
    glBindBuffer(GL_ARRAY_BUFFER, buffer);
    glBufferData(GL_ARRAY_BUFFER, sizeof(positions), positions, GL_STATIC_DRAW);

    unsigned int EBO;
    glGenBuffers(1, &EBO);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);


    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 6, 0);
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 6, (void*)(sizeof(float) * 3));
    glEnableVertexAttribArray(1);


    Shader myShader("src/Shaders/Source/vertex.glsl", "src/Shaders/Source/fragment.glsl");

    /* Loop until the user closes the window */
    while (!glfwWindowShouldClose(window))
    {
        /* Render here */
        processInput(window);
        glfwSetFramebufferSizeCallback(window, windowResizeCallback);

        glClear(GL_COLOR_BUFFER_BIT);

        /*float time = glfwGetTime();
        float rgb[] = {
            (sin(time) + 1.0f) / 2.0f,
            (sin(time + 2.0f) + 1.0f) / 2.0f,
            (sin(time + 4.0f) + 1.0f) / 2.0f
        };

        int uniformLocation = glGetUniformLocation(shader, "dynamicColor");

        glUniform4f(uniformLocation, rgb[0], rgb[1], rgb[2], 1.0f);*/

        myShader.UseProgram();

        glBindVertexArray(VAO);

        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

        /* Swap front and back buffers */
        glfwSwapBuffers(window);

        /* Poll for and process events */
        glfwPollEvents();
    }

    myShader.DeleteProgram();

    glfwTerminate();
    return 0;
}