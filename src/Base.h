#pragma once

class Base{
public:
    virtual void Init(){
        SetGlobalState();
        CreateShader();
        LoadModel();
        LoadTexture();
    }

    virtual void SetGlobalState(){
        glEnable(GL_DEPTH_TEST);
    }

    virtual void LoadModel() = 0;
    virtual void LoadTexture() = 0;

    virtual void CreateShader() = 0;

    virtual void Draw() = 0;

    virtual void Release() = 0;
};