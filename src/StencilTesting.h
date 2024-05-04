#pragma once
#include <GLFW/glfw3.h>

#include "Common.h"
#include "shader/Shader.h"
#include "Base.h"

class StencilTesting : public Base
{
public:
    StencilTesting() = default;

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
        m_shader.CreateShader(c_shaderPath +  "AdvancedOpenGL/StencilTesting.vs",
            c_shaderPath + "AdvancedOpenGL/StencilTesting.fs");
        m_shader.use();
        m_shader.setInt("DiffuseMap", 0);

        m_objectOutliningShader.CreateShader(c_shaderPath +  "AdvancedOpenGL/StencilTesting.vs",
            c_shaderPath + "AdvancedOpenGL/StencilTestingObjectOutlining.fs");
    }

    virtual void Draw() override
    {
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

        m_shader.use();
        glm::mat4 model = glm::mat4(1.0f);
        glm::mat4 view = camera.GetViewMatrix();
        glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
        m_shader.setMat4("view", view);
        m_shader.setMat4("projection", projection);
        
        // 保证我们在绘制地板的时候不会更新模板缓冲
        glStencilMask(0x00);
        // 绘制地板
        glBindVertexArray(planeVAO);
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, floorTexture);
        m_shader.setMat4("model", glm::mat4(1.0f));
        glDrawArrays(GL_TRIANGLES, 0, 6);
        glBindVertexArray(0);

        // 1. 第一遍先正常绘制，并写入模板缓冲
        glStencilFunc(GL_ALWAYS, 1, 0xFF);
        glStencilMask(0xFF);
        // 绘制物体
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
        
        // 2. 第二遍绘制:
        // 此时模板缓冲在箱子被绘制的地方都更新为1了，现在绘制放大的箱子并禁用模板缓冲的写入；
        // 我们将模板函数设置为GL_NOTEQUAL，它会保证我们只绘制箱子上模板值不为1的部分，即只绘制箱子在之前绘制的箱子之外的部分;
        // 注意我们也禁用了深度测试，让放大的箱子，即边框，不会被地板所覆盖。记得要在完成之后重新启用深度缓冲。
        glStencilFunc(GL_NOTEQUAL, 1, 0xFF);
        glStencilMask(0x00);
        glDisable(GL_DEPTH_TEST);
        m_objectOutliningShader.use();
        m_objectOutliningShader.setMat4("view", view);
        m_objectOutliningShader.setMat4("projection", projection);

        float scale = 1.1f;
        glBindVertexArray(cubeVAO);
        //glBindTexture(GL_TEXTURE_2D, cubeTexture);
        model = glm::mat4(1.0f);
        model = glm::translate(model, glm::vec3(-1.0f, 0.0f, -1.0f));
        model = glm::scale(model, glm::vec3(scale, scale, scale));
        m_objectOutliningShader.setMat4("model", model);
        glDrawArrays(GL_TRIANGLES, 0, 36);
        model = glm::mat4(1.0f);
        model = glm::translate(model, glm::vec3(2.0f, 0.0f, 0.0f));
        model = glm::scale(model, glm::vec3(scale, scale, scale));
        m_objectOutliningShader.setMat4("model", model);
        glDrawArrays(GL_TRIANGLES, 0, 36);

        glBindVertexArray(0);
        glStencilMask(0xFF);
        glStencilFunc(GL_ALWAYS, 0, 0xFF);
        glEnable(GL_DEPTH_TEST);
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
        glEnable(GL_STENCIL_TEST);
        glEnable(GL_STENCIL_TEST);
        glStencilFunc(GL_NOTEQUAL, 1, 0xFF);
        glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE);
    }

private:
    Shader m_shader;
    Shader m_objectOutliningShader;
    uint32_t cubeVAO, planeVAO;
    uint32_t cubeVBO, planeVBO;
    uint32_t cubeTexture, floorTexture;
};