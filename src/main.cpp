
#include <iostream>

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <stb/stb_image.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "shader/Shader.h"
#include "camera/Camera.h"
#include "model/model.h"

namespace
{
    const std::string c_assertPath{"E:/WorkSpace/OpenGL/HelloOpenGL/assert/"};
    const std::string c_shaderPath{c_assertPath + "shaders/"};

    // settings
    const unsigned int SCR_WIDTH = 1600;
    const unsigned int SCR_HEIGHT = 1200;

    // camera
    Camera camera(glm::vec3(0.0f, 0.0f, 6.0f));
    float lastX = SCR_WIDTH / 2.0f;
    float lastY = SCR_HEIGHT / 2.0f;
    bool firstMouse = true;

    // timing
    float deltaTime = 0.0f; // time between current frame and last frame
    float lastFrame = 0.0f;

    // lighting
    glm::vec3 lightPos(1.2f, 1.0f, 2.0f);

    // set up vertex data (and buffer(s)) and configure vertex attributes
    // ------------------------------------------------------------------
    float vertices[] = {
        // positions          // normals           // texture coords
        -0.5f, -0.5f, -0.5f, 0.0f, 0.0f, -1.0f, 0.0f, 0.0f,
        0.5f, -0.5f, -0.5f, 0.0f, 0.0f, -1.0f, 1.0f, 0.0f,
        0.5f, 0.5f, -0.5f, 0.0f, 0.0f, -1.0f, 1.0f, 1.0f,
        0.5f, 0.5f, -0.5f, 0.0f, 0.0f, -1.0f, 1.0f, 1.0f,
        -0.5f, 0.5f, -0.5f, 0.0f, 0.0f, -1.0f, 0.0f, 1.0f,
        -0.5f, -0.5f, -0.5f, 0.0f, 0.0f, -1.0f, 0.0f, 0.0f,

        -0.5f, -0.5f, 0.5f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f,
        0.5f, -0.5f, 0.5f, 0.0f, 0.0f, 1.0f, 1.0f, 0.0f,
        0.5f, 0.5f, 0.5f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f,
        0.5f, 0.5f, 0.5f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f,
        -0.5f, 0.5f, 0.5f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f,
        -0.5f, -0.5f, 0.5f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f,

        -0.5f, 0.5f, 0.5f, -1.0f, 0.0f, 0.0f, 1.0f, 0.0f,
        -0.5f, 0.5f, -0.5f, -1.0f, 0.0f, 0.0f, 1.0f, 1.0f,
        -0.5f, -0.5f, -0.5f, -1.0f, 0.0f, 0.0f, 0.0f, 1.0f,
        -0.5f, -0.5f, -0.5f, -1.0f, 0.0f, 0.0f, 0.0f, 1.0f,
        -0.5f, -0.5f, 0.5f, -1.0f, 0.0f, 0.0f, 0.0f, 0.0f,
        -0.5f, 0.5f, 0.5f, -1.0f, 0.0f, 0.0f, 1.0f, 0.0f,

        0.5f, 0.5f, 0.5f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f,
        0.5f, 0.5f, -0.5f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f,
        0.5f, -0.5f, -0.5f, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f,
        0.5f, -0.5f, -0.5f, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f,
        0.5f, -0.5f, 0.5f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f,
        0.5f, 0.5f, 0.5f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f,

        -0.5f, -0.5f, -0.5f, 0.0f, -1.0f, 0.0f, 0.0f, 1.0f,
        0.5f, -0.5f, -0.5f, 0.0f, -1.0f, 0.0f, 1.0f, 1.0f,
        0.5f, -0.5f, 0.5f, 0.0f, -1.0f, 0.0f, 1.0f, 0.0f,
        0.5f, -0.5f, 0.5f, 0.0f, -1.0f, 0.0f, 1.0f, 0.0f,
        -0.5f, -0.5f, 0.5f, 0.0f, -1.0f, 0.0f, 0.0f, 0.0f,
        -0.5f, -0.5f, -0.5f, 0.0f, -1.0f, 0.0f, 0.0f, 1.0f,

        -0.5f, 0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f,
        0.5f, 0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 1.0f, 1.0f,
        0.5f, 0.5f, 0.5f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f,
        0.5f, 0.5f, 0.5f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f,
        -0.5f, 0.5f, 0.5f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f,
        -0.5f, 0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f};

    // world space positions of our cubes
    glm::vec3 cubePositions[] = {
        glm::vec3(0.0f, 0.0f, 0.0f),
        glm::vec3(2.0f, 5.0f, -15.0f),
        glm::vec3(-1.5f, -2.2f, -2.5f),
        glm::vec3(-3.8f, -2.0f, -12.3f),
        glm::vec3(2.4f, -0.4f, -3.5f),
        glm::vec3(-1.7f, 3.0f, -7.5f),
        glm::vec3(1.3f, -2.0f, -2.5f),
        glm::vec3(1.5f, 2.0f, -2.5f),
        glm::vec3(1.5f, 0.2f, -1.5f),
        glm::vec3(-1.3f, 1.0f, -1.5f)};

    // positions of the point lights
    glm::vec3 pointLightPositions[] = {
        glm::vec3(0.7f, 0.2f, 2.0f),
        glm::vec3(2.3f, -3.3f, -4.0f),
        glm::vec3(-4.0f, 2.0f, -12.0f),
        glm::vec3(0.0f, 0.0f, -3.0f)};
}

void framebuffer_size_callback(GLFWwindow *window, int width, int height);
void mouse_callback(GLFWwindow *window, double xpos, double ypos);
void scroll_callback(GLFWwindow *window, double xoffset, double yoffset);
void processInput(GLFWwindow *window);


void setLightUniformForShader(Shader &objectShader);
void drawObject(uint32_t vao, Shader &objectShader, const glm::mat4 &view, const glm::mat4 &projection, uint32_t texture1, uint32_t texture2);
void drawLight(uint32_t vao, Shader &lightShader, const glm::mat4 &view, const glm::mat4 &projection);


int main()
{
    // glfw: initialize and configure
    // ------------------------------
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

    // glfw window creation
    // --------------------
    GLFWwindow *window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "LearnOpenGL", NULL, NULL);
    if (window == NULL)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    glfwSetCursorPosCallback(window, mouse_callback);
    glfwSetScrollCallback(window, scroll_callback);

    // tell GLFW to capture our mouse
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    // glad: load all OpenGL function pointers
    // ---------------------------------------
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }

    // configure global opengl state
    // -----------------------------
    glEnable(GL_DEPTH_TEST);

    // build and compile our shader zprogram
    // ------------------------------------
    // Shader lightShader((c_shaderPath + "Lightning/" + "1.Light.vs").c_str(), (c_shaderPath + "Lightning/" + "1.Light.fs").c_str());
    // Shader objectShader((c_shaderPath + "Lightning/" + "1.Object.vs").c_str(), (c_shaderPath + "Lightning/" + "1.Object.fs").c_str());

    //
    unsigned int VAO;
    unsigned int VBO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    // position attribute
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void *)0);
    glEnableVertexAttribArray(0);
    // normal attribute
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void *)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);
    // texture coords attribute
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void *)(6 * sizeof(float)));
    glEnableVertexAttribArray(2);

    // light VAO
    unsigned int lightVAO;
    glGenVertexArrays(1, &lightVAO);
    glBindVertexArray(lightVAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    // position attribute
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void *)0);
    glEnableVertexAttribArray(0);
    // glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
    // glEnableVertexAttribArray(1);

    // load and create a texture
    // -------------------------
    unsigned int texture1, texture2;
    texture1 = TextureFromFile("textures/container2.png", c_assertPath);
    texture2 = TextureFromFile("textures/container2_specular.png", c_assertPath);

    // Model
    Model ourModel((c_assertPath + "objects/nanosuit/nanosuit.obj"));

    // shader
    std::string shaderPath = c_shaderPath + "Model/";
    Shader modelShader((shaderPath + "model.vs").c_str(), (shaderPath + "model.fs").c_str());

    // render loop
    // -----------
    while (!glfwWindowShouldClose(window))
    {
        // per-frame time logic
        // --------------------
        float currentFrame = static_cast<float>(glfwGetTime());
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        // input
        // -----
        processInput(window);

        // render
        // ------
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
        glm::mat4 view = camera.GetViewMatrix();

        // lightPos.x = 1.0f + sin(glfwGetTime()) * 2.0f;
        // lightPos.y = sin(glfwGetTime() / 2.0f) * 1.0f;
        //  光源
        // drawLight(lightVAO, lightShader, view, projection);

        // // 物体
        // drawObject(VAO, objectShader, view, projection, texture1, texture2);

        modelShader.use();
        modelShader.setMat4("projection", projection);
        modelShader.setMat4("view", view);

        // render the loaded model
        glm::mat4 model = glm::mat4(1.0f);
        model = glm::translate(model, glm::vec3(0.0f, 0.0f, 0.0f)); // translate it down so it's at the center of the scene
        model = glm::scale(model, glm::vec3(0.2f, 0.2f, 0.2f));     // it's a bit too big for our scene, so scale it down
        modelShader.setMat4("model", model);

        modelShader.setVec3("viewPos", camera.Position);
        modelShader.setFloat("material.shininess", 32.0f);

        setLightUniformForShader(modelShader);

        ourModel.Draw(modelShader);

        // glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
        // -------------------------------------------------------------------------------
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    // optional: de-allocate all resources once they've outlived their purpose:
    // ------------------------------------------------------------------------
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);

    // glfw: terminate, clearing all previously allocated GLFW resources.
    // ------------------------------------------------------------------
    glfwTerminate();
    return 0;
}

// process all input: query GLFW whether relevant keys are pressed/released this frame and react accordingly
// ---------------------------------------------------------------------------------------------------------
void processInput(GLFWwindow *window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);

    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        camera.ProcessKeyboard(FORWARD, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        camera.ProcessKeyboard(BACKWARD, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        camera.ProcessKeyboard(LEFT, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        camera.ProcessKeyboard(RIGHT, deltaTime);
}

// glfw: whenever the window size changed (by OS or user resize) this callback function executes
// ---------------------------------------------------------------------------------------------
void framebuffer_size_callback(GLFWwindow *window, int width, int height)
{
    // make sure the viewport matches the new window dimensions; note that width and
    // height will be significantly larger than specified on retina displays.
    glViewport(0, 0, width, height);
}

// glfw: whenever the mouse moves, this callback is called
// -------------------------------------------------------
void mouse_callback(GLFWwindow *window, double xposIn, double yposIn)
{
    float xpos = static_cast<float>(xposIn);
    float ypos = static_cast<float>(yposIn);

    if (firstMouse)
    {
        lastX = xpos;
        lastY = ypos;
        firstMouse = false;
    }

    float xoffset = xpos - lastX;
    float yoffset = lastY - ypos; // reversed since y-coordinates go from bottom to top

    lastX = xpos;
    lastY = ypos;

    camera.ProcessMouseMovement(xoffset, yoffset);
}

// glfw: whenever the mouse scroll wheel scrolls, this callback is called
// ----------------------------------------------------------------------
void scroll_callback(GLFWwindow *window, double xoffset, double yoffset)
{
    camera.ProcessMouseScroll(static_cast<float>(yoffset));
}



void setLightUniformForShader(Shader &objectShader)
{
    // directional light
    objectShader.setVec3("dirLight.direction", -0.2f, -1.0f, -0.3f);
    objectShader.setVec3("dirLight.ambient", 0.05f, 0.05f, 0.05f);
    objectShader.setVec3("dirLight.diffuse", 0.4f, 0.4f, 0.4f);
    objectShader.setVec3("dirLight.specular", 0.5f, 0.5f, 0.5f);
    // point light 1
    objectShader.setVec3("pointLights[0].position", pointLightPositions[0]);
    objectShader.setVec3("pointLights[0].ambient", 0.05f, 0.05f, 0.05f);
    objectShader.setVec3("pointLights[0].diffuse", 0.8f, 0.8f, 0.8f);
    objectShader.setVec3("pointLights[0].specular", 1.0f, 1.0f, 1.0f);
    objectShader.setFloat("pointLights[0].constant", 1.0f);
    objectShader.setFloat("pointLights[0].linear", 0.09f);
    objectShader.setFloat("pointLights[0].quadratic", 0.032f);
    // point light 2
    objectShader.setVec3("pointLights[1].position", pointLightPositions[1]);
    objectShader.setVec3("pointLights[1].ambient", 0.05f, 0.05f, 0.05f);
    objectShader.setVec3("pointLights[1].diffuse", 0.8f, 0.8f, 0.8f);
    objectShader.setVec3("pointLights[1].specular", 1.0f, 1.0f, 1.0f);
    objectShader.setFloat("pointLights[1].constant", 1.0f);
    objectShader.setFloat("pointLights[1].linear", 0.09f);
    objectShader.setFloat("pointLights[1].quadratic", 0.032f);
    // point light 3
    objectShader.setVec3("pointLights[2].position", pointLightPositions[2]);
    objectShader.setVec3("pointLights[2].ambient", 0.05f, 0.05f, 0.05f);
    objectShader.setVec3("pointLights[2].diffuse", 0.8f, 0.8f, 0.8f);
    objectShader.setVec3("pointLights[2].specular", 1.0f, 1.0f, 1.0f);
    objectShader.setFloat("pointLights[2].constant", 1.0f);
    objectShader.setFloat("pointLights[2].linear", 0.09f);
    objectShader.setFloat("pointLights[2].quadratic", 0.032f);
    // point light 4
    objectShader.setVec3("pointLights[3].position", pointLightPositions[3]);
    objectShader.setVec3("pointLights[3].ambient", 0.05f, 0.05f, 0.05f);
    objectShader.setVec3("pointLights[3].diffuse", 0.8f, 0.8f, 0.8f);
    objectShader.setVec3("pointLights[3].specular", 1.0f, 1.0f, 1.0f);
    objectShader.setFloat("pointLights[3].constant", 1.0f);
    objectShader.setFloat("pointLights[3].linear", 0.09f);
    objectShader.setFloat("pointLights[3].quadratic", 0.032f);
    // spotLight
    objectShader.setVec3("spotLight.position", camera.Position);
    objectShader.setVec3("spotLight.direction", camera.Front);
    objectShader.setVec3("spotLight.ambient", 0.0f, 0.0f, 0.0f);
    objectShader.setVec3("spotLight.diffuse", 1.0f, 1.0f, 1.0f);
    objectShader.setVec3("spotLight.specular", 1.0f, 1.0f, 1.0f);
    objectShader.setFloat("spotLight.constant", 1.0f);
    objectShader.setFloat("spotLight.linear", 0.09f);
    objectShader.setFloat("spotLight.quadratic", 0.032f);
    objectShader.setFloat("spotLight.cutOff", glm::cos(glm::radians(12.5f)));
    objectShader.setFloat("spotLight.outerCutOff", glm::cos(glm::radians(15.0f)));
}

void drawObject(uint32_t vao, Shader &objectShader, const glm::mat4 &view, const glm::mat4 &projection, uint32_t texture1, uint32_t texture2)
{
    objectShader.use();

    objectShader.setVec3("viewPos", camera.Position);
    {
        // 材质
        objectShader.setInt("material.diffuseMap", 0);
        objectShader.setInt("material.specularMap", 1);
        objectShader.setFloat("material.shininess", 32.0f);
    }

    {
        // 灯光
        // glm::vec3 lightColor;
        // lightColor.x = sin(glfwGetTime() * 2.0f);
        // lightColor.y = sin(glfwGetTime() * 0.7f);
        // lightColor.z = sin(glfwGetTime() * 1.3f);
        setLightUniformForShader(objectShader);
    }

    // pass projection matrix to shader (note that in this case it could change every frame)
    objectShader.setMat4("projection", projection);
    // camera/view transformation
    objectShader.setMat4("view", view);
    // world transformation
    glBindVertexArray(vao);

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, texture1);
    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, texture2);

    for (unsigned int i = 0; i < 10; i++)
    {
        // calculate the model matrix for each object and pass it to shader before drawing
        glm::mat4 model = glm::mat4(1.0f); // make sure to initialize matrix to identity matrix first
        model = glm::translate(model, cubePositions[i]);
        float angle = 20.0f * i;
        model = glm::rotate(model, glm::radians(angle), glm::vec3(1.0f, 0.3f, 0.7f));
        objectShader.setMat4("model", model);

        glDrawArrays(GL_TRIANGLES, 0, 36);
    }
}

void drawLight(uint32_t vao, Shader &lightShader, const glm::mat4 &view, const glm::mat4 &projection)
{
    //  光源
    // activate shader
    lightShader.use();
    // pass projection matrix to shader (note that in this case it could change every frame)
    lightShader.setMat4("projection", projection);
    // camera/view transformation
    lightShader.setMat4("view", view);
    glm::mat4 lightModel = glm::mat4(1.0f);
    lightModel = glm::translate(lightModel, lightPos);
    lightModel = glm::scale(lightModel, glm::vec3(0.2f));
    lightShader.setMat4("model", lightModel);
    // render boxes
    glBindVertexArray(vao);
    for (unsigned int i = 0; i < 4; i++)
    {
        glm::mat4 model = glm::mat4(1.0f);
        model = glm::translate(model, pointLightPositions[i]);
        model = glm::scale(model, glm::vec3(0.2f)); // Make it a smaller cube
        lightShader.setMat4("model", model);
        glDrawArrays(GL_TRIANGLES, 0, 36);
    }
}

