#pragma once
#include <GLFW/glfw3.h>
#include <memory>

#include "Common.h"
#include "shader/Shader.h"
#include "Base.h"

class DepthTesting : public Base
{
public:
    DepthTesting() = default;

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
        glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void *)(2*3 * sizeof(float)));
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
        glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void *)(2*3 * sizeof(float)));
        glBindVertexArray(0);
    }

    virtual void LoadTexture() override
    {
        cubeTexture = TextureFromFile("textures/marble.jpg");
        floorTexture = TextureFromFile("textures/metal.png");
    }

    virtual void CreateShader() override
    {
        m_shader.CreateShader(c_shaderPath +  "AdvancedOpenGL/DepthTesting.vs", c_shaderPath + "AdvancedOpenGL/DepthTesting.fs");
         m_shader.use();
        m_shader.setInt("DiffuseMap", 0);
    }

    virtual void Draw() override
    {
        m_shader.use();
        glm::mat4 model = glm::mat4(1.0f);
        glm::mat4 view = camera.GetViewMatrix();
        glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
        m_shader.setMat4("view", view);
        m_shader.setMat4("projection", projection);
        // cubes
        glBindVertexArray(cubeVAO);
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, cubeTexture); 	
        model = glm::translate(model, glm::vec3(-1.0f, 0.0f, -1.0f));
        m_shader.setMat4("model", model);
        glDrawArrays(GL_TRIANGLES, 0, 36);
        model = glm::mat4(1.0f);
        model = glm::translate(model, glm::vec3(2.0f, 0.0f, 0.0f));
        m_shader.setMat4("model", model);
        glDrawArrays(GL_TRIANGLES, 0, 36);
        // floor
        glBindVertexArray(planeVAO);
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, floorTexture);
        m_shader.setMat4("model", glm::mat4(1.0f));
        glDrawArrays(GL_TRIANGLES, 0, 6);
        glBindVertexArray(0);

    }

    virtual void Release() override
    {
        glDeleteVertexArrays(1, &cubeVAO);
        glDeleteVertexArrays(1, &planeVAO);
        glDeleteBuffers(1, &cubeVBO);
        glDeleteBuffers(1, &planeVBO);
    }
    
    virtual void SetGlobalState(){
        glEnable(GL_DEPTH_TEST);
        //glDepthFunc(GL_LESS);
    }

private:
    Shader m_shader;
    uint32_t cubeVAO, planeVAO;
    uint32_t cubeVBO, planeVBO;
    uint32_t cubeTexture, floorTexture;
};