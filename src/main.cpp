#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>

const char *vertexShaderSource =
    "#version 330 core\n"
    "layout (location = 0) in vec3 aPos;\n"
    "void main()\n"
    "{\n"
    "   gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
    "}\0";

const char *fragmentShaderSource =
    "#version 330 core\n"
    "out vec4 FragColor;\n"
    "void main()\n"
    "{\n"
    "   FragColor = vec4(1.0f, 0.5f, 0.2f, 1.0f);\n"
    "}\n\0";

void processInput(GLFWwindow *window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
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
    
    unsigned int vertexShader = glCreateShader(GL_VERTEX_SHADER); // create a vertex shader object
    glShaderSource(vertexShader, 1, &vertexShaderSource, NULL); // attach the vertex shader source code to the shader object
    glCompileShader(vertexShader); // compile the vertex shader

    unsigned int fragmentShader = glCreateShader(GL_FRAGMENT_SHADER); // create a fragment shader object
    glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL); // attach the fragment shader source code to the shader object
    glCompileShader(fragmentShader); // compile the fragment shader

    unsigned int shaderProgram = glCreateProgram(); // create a shader program object
    glAttachShader(shaderProgram, vertexShader); // attach the vertex shader to the shader program
    glAttachShader(shaderProgram, fragmentShader); // attach the fragment shader to the shader program
    glLinkProgram(shaderProgram); // link the shader program


    // checking for shader compile errors
    int success;
    char infoLog[512];
    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
    }
    glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << std::endl;
    }
    glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
    if (!success)
    {
        glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
    }

    glUseProgram(shaderProgram);

    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);

    // RENDER LOOP
    while (!glfwWindowShouldClose(window))
    {
        // input (e.g., keyboard, mouse)
        processInput(window);

        // rendering commands here
        glClearColor(0.7, 0.6, 0.9, 1.0); // set the clear color
        glClear(GL_COLOR_BUFFER_BIT); // clear the color buffer

        // check and call events and swap the buffers
        glfwSwapBuffers(window);
        glfwPollEvents();
    }
    glfwTerminate();
    return (0);
}