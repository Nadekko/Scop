#include "../include/config.hpp"

// VAO = vertex array object
// VBO = vertex buffer object

// deltaTime = seconds elapsed since the previous frame.
// Multiplying every movement by deltaTime makes it frame-rate independent:
// values are expressed "per second" instead of "per frame".
// Key: L (line), F (fill), T (texture), A/D/W/S/Q/E (translation)
// UP/DOWN (X rotation), LEFT/RIGHT (Y rotation), X/Z (Z rotation)
// ESC (quit), SPACE (play/pause rotation)


// Everything the user can change
struct State
{
    Vec3        position;                   // translation
    Vec3        rotation;                   // angle in radians around X, Y and Z
    bool        autoRotate = true;          // automatic spin around Y (Space)
    bool        textureMode = false;        // color (false), texture (true)
    float       mixValue = 0.0f;            // 0 = color, 1 = texture
    bool        tWasPressed = false;        
    bool        spaceWasPressed = false;
};

// true only on the frame the key goes from released to pressed,
// so holding the key down does not toggle it every frame
static bool keyPressedOnce(GLFWwindow *window, int key, bool &wasPressed)
{
    bool isPressed = glfwGetKey(window, key) == GLFW_PRESS;
    bool pressedNow = isPressed && !wasPressed;
    wasPressed = isPressed;
    return pressedNow;
}

void processInput(GLFWwindow *window, float deltaTime, State &state)
{
    float speed = 2.0f * deltaTime; // 2 units per second
    float rotSpeed = 1.5f * deltaTime; // 1.5 radians per second

    // ESC, L, F
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
    if (glfwGetKey(window, GLFW_KEY_L) == GLFW_PRESS)
        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    if (glfwGetKey(window, GLFW_KEY_F) == GLFW_PRESS)
        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    

    // toggles
    if (keyPressedOnce(window, GLFW_KEY_T, state.tWasPressed))
        state.textureMode = !state.textureMode;
    if (keyPressedOnce(window, GLFW_KEY_SPACE, state.spaceWasPressed))
        state.autoRotate = !state.autoRotate;

    // transition: mixValue goes from 0 (colors) to 1 (texture) in 1 second,
    // the fragment shader blends both with mix()
    if (state.textureMode && state.mixValue < 1.0f)
        state.mixValue += deltaTime;
    else if (!state.textureMode && state.mixValue > 0.0f)
        state.mixValue -= deltaTime;
    if (state.mixValue < 0.0f)
        state.mixValue = 0.0f;
    else if (state.mixValue > 1.0f)
        state.mixValue = 1.0f;
    
    // translation on the 3 axes (GLFW keys are named after a QWERTY layout)
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        state.position.x -= speed;
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        state.position.x += speed;
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        state.position.y -= speed;
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        state.position.y += speed;
    if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS)
        state.position.z -= speed;
    if (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS)
        state.position.z += speed;

    // rotation angles
    if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS)
        state.rotation.x -= rotSpeed;
    if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS)
        state.rotation.x += rotSpeed;
    if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS)
        state.rotation.y -= rotSpeed;
    if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS)
        state.rotation.y += rotSpeed;
    if (glfwGetKey(window, GLFW_KEY_X) == GLFW_PRESS)
        state.rotation.z -= rotSpeed;
    if (glfwGetKey(window, GLFW_KEY_C) == GLFW_PRESS)
        state.rotation.z += rotSpeed;
    
    // automatic spin
    if (state.autoRotate)
        state.rotation.y += 1.0f * deltaTime;
}

void framebuffer_size_callback(GLFWwindow *window, int width, int height)
{
    (void)window;
    glViewport(0, 0, width, height); // set the viewport to cover the new window
}

int main(int ac, char **av)
{
    if (ac != 3) {
        std::cerr << "Usage: ./scop <file.obj> <texture.bmp>" << std::endl;
        return (1);
    }
    // INIT LIBRARIES AND CREATE WINDOW
    if (!glfwInit()) {
        std::cerr << "Failed to initialize glfw" << std::endl;
        return (1);
    }
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

        Mat4 view       = Mat4::translation(0.0f, 0.0f, -3.0f);
        
        State state;
        float   lastFrame = glfwGetTime();
        // RENDER LOOP
        while (!glfwWindowShouldClose(window))
        {
            // real size of the drawable area, in pixels (changes when the window is resized)
            int fbWidth, fbHeight;
            glfwGetFramebufferSize(window, &fbWidth, &fbHeight);
            if (fbHeight == 0)      // minimized window: avoid a division by 0
                fbHeight = 1;
            Mat4 projection = Mat4::perspective(45.0f * M_PI / 180.0f, (float)fbWidth / (float)fbHeight, 0.1f, 100.0f);
            
            float   now = glfwGetTime();
            float   deltaTime = now - lastFrame;
            lastFrame = now;

            // input
            processInput(window, deltaTime, state);
            
            // render
            glClearColor(0.7, 0.6, 0.9, 1.0); // set the clear color
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); // clear the color buffer

            // model matrix, applied to each vertex from RIGHT to LEFT:
            // 1. translation(-center): move the object's center to the origin
            // 2. scale: resize the object so its largest side is 2 units
            // 3. rotation_y: spin on its own vertical axis (auto spin + LEFT/RIGHT)
            // 4. rotation_z, rotation_x: tilt the spinning object (Z/X, UP/DOWN)
            // 5. translation(position): move it where the user put it
            // the user translation comes after the rotation, so the object
            // keeps spinning on itself wherever it is moved
            Vec3 c = ourMesh.getCenter();
            Mat4 model = Mat4::translation(state.position.x, state.position.y, state.position.z)
                        * Mat4::rotation_x(state.rotation.x)
                        * Mat4::rotation_z(state.rotation.z)
                        * Mat4::rotation_y(state.rotation.y)
                        * Mat4::scale(ourMesh.getScale())
                        * Mat4::translation(-c.x, -c.y, -c.z);

            ourShader.use();
            
            glUniformMatrix4fv(glGetUniformLocation(ourShader.ID, "model"),      1, GL_FALSE, model.m);
            glUniformMatrix4fv(glGetUniformLocation(ourShader.ID, "view"),       1, GL_FALSE, view.m);
            glUniformMatrix4fv(glGetUniformLocation(ourShader.ID, "projection"), 1, GL_FALSE, projection.m);
            
            ourShader.setInt("ourTexture", 0); // set the texture uniform to use texture unit 0
            ourShader.setFloat("mixValue", state.mixValue); // 0 = colors / 1 = texture
            glActiveTexture(GL_TEXTURE0); // activate the texture unit first before binding the texture
            ourTexture.bind();
            ourMesh.draw();
            
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