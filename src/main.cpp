#include "../include/config.hpp"

// VAO = vertex array object
// VBO = vertex buffer object
// EBO = element buffer object

// deltaTime = seconds elapsed since the previous frame.
// Multiplying every movement by deltaTime makes it frame-rate independent:
// values are expressed "per second" instead of "per frame".
void processInput(GLFWwindow *window, float deltaTime, Vec3 &position, float &mixValue, bool &textureMode)
{
    float speed = 2.0f * deltaTime; // 2 units per second
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
    if (glfwGetKey(window, GLFW_KEY_L) == GLFW_PRESS)
        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    if (glfwGetKey(window, GLFW_KEY_F) == GLFW_PRESS)
        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    
    // edge detection: toggle only on the frame the key goes from released to pressed,
    // otherwise holding T would flip textureMode every frame
    static bool tWasPressed = false;
    bool tIsPressed = glfwGetKey(window, GLFW_KEY_T) == GLFW_PRESS;
    if (tIsPressed && !tWasPressed)
        textureMode = !textureMode; // toggle
    tWasPressed = tIsPressed;

    // transition: mixValue goes from 0 (colors) to 1 (texture) in 1 second,
    // the fragment shader blends both with mix()
    if (textureMode && mixValue < 1.0f)
        mixValue += deltaTime;
    else if (!textureMode && mixValue > 0.0f)
        mixValue -= deltaTime;
    if (mixValue < 0.0f)
        mixValue = 0.0f;
    else if (mixValue > 1.0f)
        mixValue = 1.0f;
    
    // translation on the 3 axes (GLFW keys are named after a QWERTY layout)
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        position.x -= speed;
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        position.x += speed;
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        position.y -= speed;
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        position.y += speed;
    if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS)
        position.z -= speed;
    if (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS)
        position.z += speed;
}

void framebuffer_size_callback(GLFWwindow *window, int width, int height)
{
    (void)window;
    glViewport(0, 0, width, height); // set the viewport to cover the new window
}

int main(int ac, char **av)
{
    if (ac < 3) {
        std::cerr << "Usage: ./scop <file.obj> <texture.bmp>" << std::endl;
        return (1);
    }
    // INIT LIBRARIES AND CREATE WINDOW
    if (!glfwInit())
        return (1);
    // specify the version of OpenGL to use (3.3 in this case) and the profile (core profile)
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    GLFWwindow *window = glfwCreateWindow(WIDTH, HEIGHT, "SCOP", NULL, NULL);
    if (!window)
    {
        std::cerr << "ERROR::WINDOW::FAILED_TO_CREATE" << std::endl;
        glfwTerminate();
        return (1);
    }
    glfwMakeContextCurrent(window); // make the window's context current
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback); // register the framebuffer size callback function to adjust the viewport when the window is resized

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) // initialize GLAD to load the OpenGL function pointers
    {
        std::cerr << "ERROR::GLAD::FAILED_TO_INITIALIZE" << std::endl;
        glfwTerminate();
        return (1);
    }
    glEnable(GL_DEPTH_TEST);
    int status = 0;
    try {

        Shader ourShader("shader/vertex/3.3.shader.vs", "shader/fragment/3.3.shader.fs");
        Texture ourTexture(av[2]);
        Mesh ourMesh(av[1]);

        float mixValue = 0.0f;
        bool textureMode = false;

        Mat4 projection = Mat4::perspective(45.0f * M_PI / 180.0f,(float)WIDTH / (float)HEIGHT, 0.1f, 100.0f);
        Mat4 view       = Mat4::translation(0.0f, 0.0f, -3.0f);
        
        Vec3    position(0.0f, 0.0f, 0.0f);
        float   lastFrame = glfwGetTime();
        // RENDER LOOP
        while (!glfwWindowShouldClose(window))
        {
            float   now = glfwGetTime();
            float   deltaTime = now - lastFrame;
            lastFrame = now;

            // input
            processInput(window, deltaTime, position, mixValue, textureMode);
            
            // render
            glClearColor(0.7, 0.6, 0.9, 1.0); // set the clear color
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); // clear the color buffer

            // model matrix, applied to each vertex from RIGHT to LEFT:
            // 1. translation(-center): move the object's center to the origin
            // 2. scale: resize the object so its largest side is 2 units
            // 3. rotation_y: spin around the origin, which is now the object's center
            // 4. translation(position): move it where the user put it
            // the user translation comes after the rotation, so the object
            // keeps spinning on itself wherever it is moved
            Vec3 c = ourMesh.getCenter();
            Mat4 model = Mat4::translation(position.x, position.y, position.z)
                        * Mat4::rotation_y(now)
                        * Mat4::scale(ourMesh.getScale())
                        * Mat4::translation(-c.x, -c.y, -c.z);

            ourShader.use();
            
            glUniformMatrix4fv(glGetUniformLocation(ourShader.ID, "model"),      1, GL_FALSE, model.m);
            glUniformMatrix4fv(glGetUniformLocation(ourShader.ID, "view"),       1, GL_FALSE, view.m);
            glUniformMatrix4fv(glGetUniformLocation(ourShader.ID, "projection"), 1, GL_FALSE, projection.m);
            
            ourShader.setInt("ourTexture", 0); // set the texture uniform to use texture unit 0
            ourShader.setFloat("mixValue", mixValue); // set the mix value uniform to 0.5 for blending
            glActiveTexture(GL_TEXTURE0); // activate the texture unit first before binding the texture
            ourTexture.bind();
            ourMesh.draw(ourShader.ID);
            
            // check and call events and swap the buffers
            glfwSwapBuffers(window);
            glfwPollEvents();
        }
    }
    catch (const std::exception &e) {
        std::cerr << e.what() << std::endl;
        status = 1;
    }
    glfwTerminate();
    return (status);
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