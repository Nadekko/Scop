#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <math.h>
#include "../include/shader.hpp"
#include "../include/texture.hpp"
#include "../include/mesh.hpp"

// VAO = vertex array object
// VBO = vertex buffer object
// EBO = element buffer object

void processInput(GLFWwindow *window, float &mixValue, bool &textureMode)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
    if (glfwGetKey(window, GLFW_KEY_L) == GLFW_PRESS)
        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    if (glfwGetKey(window, GLFW_KEY_F) == GLFW_PRESS)
        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    
    static bool tWasPressed = false;
    bool tIsPressed = glfwGetKey(window, GLFW_KEY_T) == GLFW_PRESS;
    if (tIsPressed && !tWasPressed)
        textureMode = !textureMode; // toggle
    tWasPressed = tIsPressed;

    // transition
    if (textureMode && mixValue < 1.0f)
        mixValue += 0.01f;
    else if (!textureMode && mixValue > 0.0f)
        mixValue -= 0.01f;
    if (mixValue < 0.0f)
        mixValue = 0.0f;
    else if (mixValue > 1.0f)
        mixValue = 1.0f;
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
        std::cerr << "ERROR::WINDOW::FAILED_TO_CREATE" << std::endl;
        glfwTerminate();
        return (-1);
    }
    glfwMakeContextCurrent(window); // make the window's context current
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback); // register the framebuffer size callback function to adjust the viewport when the window is resized

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) // initialize GLAD to load the OpenGL function pointers
    {
        std::cerr << "ERROR::GLAD::FAILED_TO_INITIALIZE" << std::endl;
        return (-1);
    }

    {
        Shader ourShader("shader/vertex/3.3.shader.vs", "shader/fragment/3.3.shader.fs");
        Texture ourTexture("texture/cat_gray.bmp");
        Mesh ourMesh("resources/42.obj");

        float mixValue = 0.0f;
        bool textureMode = false;

        // RENDER LOOP
        while (!glfwWindowShouldClose(window))
        {
            // input
            processInput(window, mixValue, textureMode);
            
            // render
            glClearColor(0.7, 0.6, 0.9, 1.0); // set the clear color
            glClear(GL_COLOR_BUFFER_BIT); // clear the color buffer

            ourShader.use();
            ourShader.setInt("ourTexture", 0); // set the texture uniform to use texture unit 0
            ourShader.setFloat("mixValue", mixValue); // set the mix value uniform to 0.5 for blending
            glActiveTexture(GL_TEXTURE0); // activate the texture unit first before binding the texture
            ourTexture.bind();
            
            ourMesh.draw();

            // check and call events and swap the buffers
            glfwSwapBuffers(window);
            glfwPollEvents();
        }
    }
    
    glfwTerminate();
    return (0);
}


        // // CHECK COORD's AND COLORS OF THE VERTICES
        // float vertices[] = {
        //     // position          // color           // UV
        //     0.5f, -0.5f, 0.0f,  1.0f, 0.0f, 0.0f,   0.0f, 0.0f, // bottom left
        //     -0.5f, -0.5f, 0.0f, 0.0f, 1.0f, 0.0f,   1.0f, 0.0f, // bottom right
        //     0.0f,  0.5f, 0.0f,  0.0f, 0.0f, 1.0f,   0.5f, 1.0f// top center
        // };

        // unsigned int VBO, VAO;
        
        // glGenBuffers(1, &VBO);
        // glGenVertexArrays(1, &VAO);

        // // bind the Vertex Array Object first, then bind and set vertex buffer(s), and then configure vertex attributes(s).
        // glBindVertexArray(VAO);

        // glBindBuffer(GL_ARRAY_BUFFER, VBO);
        // glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

        // // [x][y][z] | [r][g][b] | [u][v]
        // // location 0| location 1| location 2
        // // offset 0  | offset 3  | offset 6
        // // position attribute 
        // glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
        // glEnableVertexAttribArray(0);

        // // color attribute
        // glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
        // glEnableVertexAttribArray(1);

        // // texture attribute
        // glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
        // glEnableVertexAttribArray(2);