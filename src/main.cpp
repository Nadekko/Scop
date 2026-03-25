#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <math.h>
#include "../include/shader.hpp"

// VAO = vertex array object
// VBO = vertex buffer object
// EBO = element buffer object

void processInput(GLFWwindow *window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
    if (glfwGetKey(window, GLFW_KEY_L) == GLFW_PRESS)
        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    if (glfwGetKey(window, GLFW_KEY_F) == GLFW_PRESS)
        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
}

void framebuffer_size_callback(GLFWwindow *window, int width, int height)
{
    (void)window;
    glViewport(0, 0, width, height); // set the viewport to cover the new window
}

int main(void)
{
    // INIT LIBRARIES AND CREATE WINDOW
    if (!glfwInit())
        return (-1);
    // specify the version of OpenGL to use (3.3 in this case) and the profile (core profile)
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    GLFWwindow *window = glfwCreateWindow(800, 600, "SCOP", NULL, NULL);
    if (!window)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return (-1);
    }
    glfwMakeContextCurrent(window); // make the window's context current
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback); // register the framebuffer size callback function to adjust the viewport when the window is resized

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) // initialize GLAD to load the OpenGL function pointers
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return (-1);
    }

    Shader ourShader("shader/vertex/3.3.shader.vs", "shader/fragment/3.3.shader.fs");

    // CHECK COORD's AND COLORS OF THE VERTICES
    float vertices[] = {
        // coord         // color             //tex coords
        0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 0.0f, 
        -0.5f, -0.5f, 0.0f, 0.0f, 1.0f, 0.0f,
        0.0f, 0.5f, 0.0f, 0.0f, 0.0f, 1.0f, 
    };

    unsigned int VBO, VAO;
    
    glGenBuffers(1, &VBO);
    glGenVertexArrays(1, &VAO);

    // bind the Vertex Array Object first, then bind and set vertex buffer(s), and then configure vertex attributes(s).
    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    // position attribute
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    // color attribute
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);
    
    // RENDER LOOP
    while (!glfwWindowShouldClose(window))
    {
        // input
        processInput(window);
        
        // render
        glClearColor(0.7, 0.6, 0.9, 1.0); // set the clear color
        glClear(GL_COLOR_BUFFER_BIT); // clear the color buffer

        // // no more uniform color,
        // // setting the uniform color to change over time
        // float timeValue = glfwGetTime(); // get current time in seconds since GLFW was initialized
        // float greenValue = (sin(timeValue) / 2.0f + 0.5f);
        // int vertexColorLocation = glGetUniformLocation(shaderProgram, "ourColor");
        // if (vertexColorLocation == -1)
        // {
        //     std::cout << "Could not find uniform variable 'ourColor' in shader program" << std::endl;
        // }
        // glUniform4f(vertexColorLocation, 0.0f, greenValue, 0.0f, 1.0f);
        ourShader.use();
        glBindVertexArray(VAO);
        glDrawArrays(GL_TRIANGLES, 0, 3);

        // check and call events and swap the buffers
        glfwSwapBuffers(window);
        glfwPollEvents();
    }
    
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    
    glfwTerminate();
    return (0);
}