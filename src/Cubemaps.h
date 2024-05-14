#pragma once
#include <GLFW/glfw3.h>

#include "Common.h"
#include "shader/Shader.h"
#include "Base.h"
#include "model/texture.h"
#include "model/model.h"

float skyboxVertices[] = {
    // positions          
    -1.0f,  1.0f, -1.0f,
    -1.0f, -1.0f, -1.0f,
     1.0f, -1.0f, -1.0f,
     1.0f, -1.0f, -1.0f,
     1.0f,  1.0f, -1.0f,
    -1.0f,  1.0f, -1.0f,

    -1.0f, -1.0f,  1.0f,
    -1.0f, -1.0f, -1.0f,
    -1.0f,  1.0f, -1.0f,
    -1.0f,  1.0f, -1.0f,
    -1.0f,  1.0f,  1.0f,
    -1.0f, -1.0f,  1.0f,

     1.0f, -1.0f, -1.0f,
     1.0f, -1.0f,  1.0f,
     1.0f,  1.0f,  1.0f,
     1.0f,  1.0f,  1.0f,
     1.0f,  1.0f, -1.0f,
     1.0f, -1.0f, -1.0f,

    -1.0f, -1.0f,  1.0f,
    -1.0f,  1.0f,  1.0f,
     1.0f,  1.0f,  1.0f,
     1.0f,  1.0f,  1.0f,
     1.0f, -1.0f,  1.0f,
    -1.0f, -1.0f,  1.0f,

    -1.0f,  1.0f, -1.0f,
     1.0f,  1.0f, -1.0f,
     1.0f,  1.0f,  1.0f,
     1.0f,  1.0f,  1.0f,
    -1.0f,  1.0f,  1.0f,
    -1.0f,  1.0f, -1.0f,

    -1.0f, -1.0f, -1.0f,
    -1.0f, -1.0f,  1.0f,
     1.0f, -1.0f, -1.0f,
     1.0f, -1.0f, -1.0f,
    -1.0f, -1.0f,  1.0f,
     1.0f, -1.0f,  1.0f
};

//
float quadVertices[] = {// vertex attributes for a quad that fills the entire screen in Normalized Device Coordinates.
                        // positions   // texCoords
                        -1.0f, 1.0f, 0.0f, 1.0f,
                        -1.0f, -1.0f, 0.0f, 0.0f,
                        1.0f, -1.0f, 1.0f, 0.0f,

                        -1.0f, 1.0f, 0.0f, 1.0f,
                        1.0f, -1.0f, 1.0f, 0.0f,
                        1.0f, 1.0f, 1.0f, 1.0f};

class Cubemaps : public Base
{
public:
    Cubemaps() = default;

    virtual void LoadModel() override
    {
        // cube VAO
        glGenVertexArrays(1, &cubeVAO);
        glGenBuffers(1, &cubeVBO);
        glBindVertexArray(cubeVAO);
        glBindBuffer(GL_ARRAY_BUFFER, cubeVBO);
        glBufferData(GL_ARRAY_BUFFER, sizeof(cubeVertices), &cubeVertices, GL_STATIC_DRAW);
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void *)0);
        glEnableVertexAttribArray(1);
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void *)(3 * sizeof(float)));
        glEnableVertexAttribArray(2);
        glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void *)(2 * 3 * sizeof(float)));
        glBindVertexArray(0);

        // plane VAO
        glGenVertexArrays(1, &planeVAO);
        glGenBuffers(1, &planeVBO);
        glBindVertexArray(planeVAO);
        glBindBuffer(GL_ARRAY_BUFFER, planeVBO);
        glBufferData(GL_ARRAY_BUFFER, sizeof(planeVertices), &planeVertices, GL_STATIC_DRAW);
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void *)0);
        glEnableVertexAttribArray(1);
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void *)(3 * sizeof(float)));
        glEnableVertexAttribArray(2);
        glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void *)(2 * 3 * sizeof(float)));
        glBindVertexArray(0);

        // skybox
        glGenVertexArrays(1, &skyboxVAO);
        glGenBuffers(1, &skyboxVBO);
        glBindVertexArray(skyboxVAO);
        glBindBuffer(GL_ARRAY_BUFFER, skyboxVBO);
        glBufferData(GL_ARRAY_BUFFER, sizeof(skyboxVertices), &skyboxVertices, GL_STATIC_DRAW);
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void *)0);


        m_model.loadModel(c_assertPath + "objects/nanosuit/nanosuit.obj");
    }

    virtual void LoadTexture() override
    {
        cubeTexture = TextureFromFile("textures/container.jpg");
        floorTexture = TextureFromFile("textures/metal.png");

        std::vector<std::string> faces
        {
            c_assertPath +"textures/skybox/" + "right.jpg",
            c_assertPath +"textures/skybox/" + "left.jpg",
            c_assertPath +"textures/skybox/" + "top.jpg",
            c_assertPath +"textures/skybox/" + "bottom.jpg",
            c_assertPath +"textures/skybox/" + "front.jpg",
            c_assertPath +"textures/skybox/" + "back.jpg"
        };
        cubemapTexture = loadCubemap(faces);
    }

    virtual void CreateShader() override
    {
        m_shader.CreateShader(c_shaderPath + "AdvancedOpenGL/cubemaps.vs",
                              c_shaderPath + "AdvancedOpenGL/cubemaps.fs");
        m_shader.use();
        m_shader.setInt("skybox", 0);

        m_cubemapShader.CreateShader(c_shaderPath + "AdvancedOpenGL/cubemaps_skybox.vs",
                                    c_shaderPath + "AdvancedOpenGL/cubemaps_skybox.fs");
        m_cubemapShader.use();
        m_cubemapShader.setInt("skybox", 0);
    }

    virtual void Draw() override
    {
        glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        glm::mat4 model = glm::mat4(1.0f);
        glm::mat4 view = camera.GetViewMatrix();
        glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);

        m_shader.use();
        m_shader.setMat4("view", view);
        m_shader.setMat4("projection", projection);

        m_shader.setVec3("cameraPos", camera.Position);
        m_shader.setMat4("model", model);
        model = glm::scale(model, glm::vec3(0.2, 0.2, 0.2));
        m_shader.setMat4("model", model);
        // 绘制物体
        // model = glm::translate(model, glm::vec3(-1.0f, 0.0f, -1.0f));
        // m_shader.setMat4("model", model);
        // glBindVertexArray(cubeVAO);
        // glActiveTexture(GL_TEXTURE0);
        // glBindTexture(GL_TEXTURE_CUBE_MAP, cubemapTexture);
        // glDrawArrays(GL_TRIANGLES, 0, 36);
        // // model = glm::mat4(1.0f);
        // // model = glm::translate(model, glm::vec3(2.0f, 0.0f, 0.0f));
        // // m_shader.setMat4("model", model);
        // // glDrawArrays(GL_TRIANGLES, 0, 36);
        // // glBindVertexArray(0);

        // // 绘制地板
        // // glBindVertexArray(planeVAO);
        // // glActiveTexture(GL_TEXTURE0);
        // // glBindTexture(GL_TEXTURE_CUBE_MAP, cubemapTexture);
        // // m_shader.setMat4("model", glm::mat4(1.0f));
        // // glDrawArrays(GL_TRIANGLES, 0, 6);
        // // glBindVertexArray(0);

        m_model.Draw(m_shader);

        //glDisable(GL_DEPTH_TEST);
        glDepthFunc(GL_LEQUAL);
        m_cubemapShader.use();
        m_cubemapShader.setMat4("view",  glm::mat4(glm::mat3(camera.GetViewMatrix())));
        m_cubemapShader.setMat4("projection", projection);
        // m_cubemapShader.setMat4("model", glm::mat4(1.0));
        glBindVertexArray(skyboxVAO);
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_CUBE_MAP, cubemapTexture);
        glDrawArrays(GL_TRIANGLES, 0, 36);
        glBindVertexArray(0);
        //glEnable(GL_DEPTH_TEST);
        glDepthFunc(GL_LESS);
    }

    virtual void Release() override
    {
        glDeleteVertexArrays(1, &cubeVAO);
        glDeleteBuffers(1, &cubeVBO);

        glDeleteVertexArrays(1, &planeVAO);
        glDeleteBuffers(1, &planeVBO);
    }

    virtual void SetGlobalState()
    {
        glEnable(GL_DEPTH_TEST);
    }

private:
    Shader m_shader;
    Shader m_screenShader;
    Shader m_cubemapShader;
    Shader m_objectOutliningShader;
    uint32_t cubeVBO, cubeVAO;
    uint32_t planeVBO, planeVAO;
    uint32_t skyboxVBO, skyboxVAO;
    uint32_t cubeTexture, floorTexture;
    uint32_t cubemapTexture;

    Model m_model;
};