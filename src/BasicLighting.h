#pragma once
#include <GLFW/glfw3.h>
#include <memory>

#include "Common.h"
#include "shader/Shader.h"
#include "model/model.h"
#include "Base.h"

class BasicLighting : public Base
{
public:
    BasicLighting() = default;

    virtual void LoadModel() override{
        m_model.loadModel(c_assertPath + "objects/nanosuit/nanosuit.obj");
    }
    
    virtual void LoadTexture() override{

    }
    virtual void CreateShader() override{
        // shader
        std::string shaderPath = c_shaderPath + "Model/";
        m_shader.CreateShader(shaderPath + "model.vs", shaderPath + "model.fs");
    }

    virtual void Draw() override{
        glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
        glm::mat4 view = camera.GetViewMatrix();

        m_shader.use();
        m_shader.setMat4("projection", projection);
        m_shader.setMat4("view", view);

        // render the loaded model
        glm::mat4 model = glm::mat4(1.0f);
        model = glm::translate(model, glm::vec3(0.0f, 0.0f, 0.0f)); // translate it down so it's at the center of the scene
        model = glm::scale(model, glm::vec3(0.2f, 0.2f, 0.2f));     // it's a bit too big for our scene, so scale it down
        m_shader.setMat4("model", model);

        m_shader.setVec3("viewPos", camera.Position);
        m_shader.setFloat("material.shininess", 32.0f);

        setLightUniformForShader();

        m_model.Draw(m_shader);
    }
    
    virtual void Release() override
    {
    }

private:
    void setLightUniformForShader()
    {
        // directional light
        m_shader.setVec3("dirLight.direction", -0.2f, -1.0f, -0.3f);
        m_shader.setVec3("dirLight.ambient", 0.05f, 0.05f, 0.05f);
        m_shader.setVec3("dirLight.diffuse", 0.4f, 0.4f, 0.4f);
        m_shader.setVec3("dirLight.specular", 0.5f, 0.5f, 0.5f);
        // point light 1
        m_shader.setVec3("pointLights[0].position", pointLightPositions[0]);
        m_shader.setVec3("pointLights[0].ambient", 0.05f, 0.05f, 0.05f);
        m_shader.setVec3("pointLights[0].diffuse", 0.8f, 0.8f, 0.8f);
        m_shader.setVec3("pointLights[0].specular", 1.0f, 1.0f, 1.0f);
        m_shader.setFloat("pointLights[0].constant", 1.0f);
        m_shader.setFloat("pointLights[0].linear", 0.09f);
        m_shader.setFloat("pointLights[0].quadratic", 0.032f);
        // point light 2
        m_shader.setVec3("pointLights[1].position", pointLightPositions[1]);
        m_shader.setVec3("pointLights[1].ambient", 0.05f, 0.05f, 0.05f);
        m_shader.setVec3("pointLights[1].diffuse", 0.8f, 0.8f, 0.8f);
        m_shader.setVec3("pointLights[1].specular", 1.0f, 1.0f, 1.0f);
        m_shader.setFloat("pointLights[1].constant", 1.0f);
        m_shader.setFloat("pointLights[1].linear", 0.09f);
        m_shader.setFloat("pointLights[1].quadratic", 0.032f);
        // point light 3
        m_shader.setVec3("pointLights[2].position", pointLightPositions[2]);
        m_shader.setVec3("pointLights[2].ambient", 0.05f, 0.05f, 0.05f);
        m_shader.setVec3("pointLights[2].diffuse", 0.8f, 0.8f, 0.8f);
        m_shader.setVec3("pointLights[2].specular", 1.0f, 1.0f, 1.0f);
        m_shader.setFloat("pointLights[2].constant", 1.0f);
        m_shader.setFloat("pointLights[2].linear", 0.09f);
        m_shader.setFloat("pointLights[2].quadratic", 0.032f);
        // point light 4
        m_shader.setVec3("pointLights[3].position", pointLightPositions[3]);
        m_shader.setVec3("pointLights[3].ambient", 0.05f, 0.05f, 0.05f);
        m_shader.setVec3("pointLights[3].diffuse", 0.8f, 0.8f, 0.8f);
        m_shader.setVec3("pointLights[3].specular", 1.0f, 1.0f, 1.0f);
        m_shader.setFloat("pointLights[3].constant", 1.0f);
        m_shader.setFloat("pointLights[3].linear", 0.09f);
        m_shader.setFloat("pointLights[3].quadratic", 0.032f);
        // spotLight
        m_shader.setVec3("spotLight.position", camera.Position);
        m_shader.setVec3("spotLight.direction", camera.Front);
        m_shader.setVec3("spotLight.ambient", 0.0f, 0.0f, 0.0f);
        m_shader.setVec3("spotLight.diffuse", 1.0f, 1.0f, 1.0f);
        m_shader.setVec3("spotLight.specular", 1.0f, 1.0f, 1.0f);
        m_shader.setFloat("spotLight.constant", 1.0f);
        m_shader.setFloat("spotLight.linear", 0.09f);
        m_shader.setFloat("spotLight.quadratic", 0.032f);
        m_shader.setFloat("spotLight.cutOff", glm::cos(glm::radians(12.5f)));
        m_shader.setFloat("spotLight.outerCutOff", glm::cos(glm::radians(15.0f)));
    }

private:
    Shader m_shader;
    Model m_model;
};