#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include "Shaders/Shader.h"
#include "Textures/stb_image.h"

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
        // position         // color           // texture coordinates
        0.5f,  0.5f, 1.0f,  1.0f, 0.0f, 0.0f,  1.0f, 1.0f,   // top right
        0.5f, -0.5f, 1.0f,  0.0f, 1.0f, 0.0f,  1.0f, 0.0f,   // bottom right
       -0.5f, -0.5f, 1.0f,  0.0f, 0.0f, 1.0f,  0.0f, 0.0f,   // bottom left
       -0.5f,  0.5f, 1.0f,  0.5f, 0.5f, 0.5f,  0.0f, 1.0f,   // top left 
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


    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 8, 0);
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 8, (void*)(sizeof(float) * 3));
    glEnableVertexAttribArray(1);

    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 8, (void*)(sizeof(float) * 6));
    glEnableVertexAttribArray(2);

    

    unsigned int texture0, texture1;
    glGenTextures(1, &texture0); // generate textures
    glBindTexture(GL_TEXTURE_2D, texture0);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);  // we set that the texture bound to GL_TEXTURE_2D should repeat on the S and T axis of the texture (X, Y)
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);  // when the texture needs to be downscaled for a smaller object we tell it to apply the mipmap that we generate later
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR); // when upscaling we just tell it to use linear filtering
    

    int width, heigth, colorChannels;
    unsigned char* textureData = stbi_load("src/Textures/tex_files/wooden_container.jpg", &width, &heigth, &colorChannels, 0); // load texture file

    if (textureData)
    {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, heigth, 0, GL_RGB, GL_UNSIGNED_BYTE, textureData); // we generate the texture based on the data we loaded from the texture file
        glGenerateMipmap(GL_TEXTURE_2D);
    }
    else
    {
        std::cout << "Failed to load texture" << std::endl;
    }

    stbi_image_free(textureData);

    //-----------------------------------------------

    glGenTextures(1, &texture1); // generate textures
    glBindTexture(GL_TEXTURE_2D, texture1);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);  // we set that the texture bound to GL_TEXTURE_2D should repeat on the S and T axis of the texture (X, Y)
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);  // when the texture needs to be downscaled for a smaller object we tell it to apply the mipmap that we generate later
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR); // when upscaling we just tell it to use linear filtering


    stbi_set_flip_vertically_on_load(true);
    textureData = stbi_load("src/Textures/tex_files/awesomeface.png", &width, &heigth, &colorChannels, 0); // load texture file

    if (textureData)
    {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, heigth, 0, GL_RGBA, GL_UNSIGNED_BYTE, textureData); // we generate the texture based on the data we loaded from the texture file
        glGenerateMipmap(GL_TEXTURE_2D);
    }
    else
    {
        std::cout << "Failed to load texture" << std::endl;
    }

    stbi_image_free(textureData);

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, texture0);
    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, texture1);


    Shader myShader("src/Shaders/Source/vertex.glsl", "src/Shaders/Source/fragment.glsl");

    myShader.UseProgram();

    myShader.SetUniformInt("texture0", 0);
    myShader.SetUniformInt("texture1", 1);

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