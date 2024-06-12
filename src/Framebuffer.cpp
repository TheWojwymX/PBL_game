#include "Framebuffer.h"


Framebuffer &Framebuffer::getInstance() {
    static Framebuffer instance;
    return instance;
}

void Framebuffer::Init(){
    glGenTextures(1, &framebuffer);
    glBindTexture(GL_TEXTURE_2D, framebuffer);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, nullptr);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glBindTexture(GL_TEXTURE_2D, 0);
}

void Framebuffer::AssignTextureToShader() {
    RESOURCEMANAGER.GetShaderByName("instancedSandShader")->use();
    RESOURCEMANAGER.GetShaderByName("instancedSandShader")->setInt("framebuffer", 2);
}

void Framebuffer::BeginRender() {
    glBindTexture(GL_TEXTURE_2D, framebuffer);
    glCopyTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, 0, 0, width, height, 0);
    glBindTexture(GL_TEXTURE_2D, 0);
}

void Framebuffer::EndRender() {
    // Bind framebuffer texture to texture unit 2
    glActiveTexture(GL_TEXTURE2);
    glBindTexture(GL_TEXTURE_2D, GetFramebufferTexture());
}

GLuint Framebuffer::GetFramebufferTexture() const {
    return framebuffer;
}