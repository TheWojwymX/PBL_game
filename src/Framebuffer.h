#pragma once

#define FRAMEBUFFER Framebuffer::getInstance()

#include "Managers/GameManager.h"

class Framebuffer{
public:
    static Framebuffer &getInstance();
    ~Framebuffer() = default;
    Framebuffer() = default;
    Framebuffer(const Framebuffer &) = delete;
    Framebuffer &operator=(const Framebuffer &) = delete;

    void Init();

    void AssignTextureToShader();
    void BeginRender();
    void EndRender();
    GLuint GetFramebufferTexture() const;
private:
    GLuint framebuffer;

    int width = GAMEMANAGER._screenWidth;
    int height = GAMEMANAGER._screenHeight;
};