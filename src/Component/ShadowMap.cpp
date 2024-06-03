#include "ShadowMap.h"

ShadowMap::ShadowMap(unsigned int shadowWidth, unsigned int shadowHeight)
        : shadowWidth(shadowWidth), shadowHeight(shadowHeight), depthMapFBO(0), depthMap(0) {
}

ShadowMap::~ShadowMap() {
    if (depthMapFBO != 0) {
        glDeleteFramebuffers(1, &depthMapFBO);
    }
    if (depthMap != 0) {
        glDeleteTextures(1, &depthMap);
    }
}

void ShadowMap::Init() {
    // Create framebuffer
    glGenFramebuffers(1, &depthMapFBO);
    glBindFramebuffer(GL_FRAMEBUFFER, depthMapFBO);

    // Create depth texture
    glGenTextures(1, &depthMap);
    glBindTexture(GL_TEXTURE_2D, depthMap);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, shadowWidth, shadowHeight, 0, GL_DEPTH_COMPONENT, GL_FLOAT, nullptr);

    // Set texture parameters
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
    float borderColor[] = {1.0f, 1.0f, 1.0f, 1.0f};
    glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, borderColor);

    // Bind the texture to the framebuffer
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, depthMap, 0);
    glDrawBuffer(GL_NONE);
    glReadBuffer(GL_NONE);

    // Check if the framebuffer is complete
    if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
        std::cerr << "Error: Shadow framebuffer is not complete!\n";
    }

    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void ShadowMap::AssignShadowMapToShader() {
    RESOURCEMANAGER.GetShaderByName("modelShader")->use();
    RESOURCEMANAGER.GetShaderByName("modelShader")->setInt("shadowMap", 1);

    RESOURCEMANAGER.GetShaderByName("instancedSandShader")->use();
    RESOURCEMANAGER.GetShaderByName("instancedSandShader")->setInt("shadowMap", 1);

    RESOURCEMANAGER.GetShaderByName("instancedMetalShader")->use();
    RESOURCEMANAGER.GetShaderByName("instancedMetalShader")->setInt("shadowMap", 1);
}

void ShadowMap::BeginRender() {
    glViewport(0, 0, shadowWidth, shadowHeight);
    glBindFramebuffer(GL_FRAMEBUFFER, depthMapFBO);
    glClear(GL_DEPTH_BUFFER_BIT);
}

void ShadowMap::RenderMap()
{
    //glCullFace(GL_FRONT);
    GAMEMANAGER.root->RenderShadows(Transform::Origin());
    //glCullFace(GL_BACK);
}

void ShadowMap::AssignLightSpaceMatrixToShader() {
    RESOURCEMANAGER.GetShaderByName("shadowShader")->use();
    RESOURCEMANAGER.GetShaderByName("shadowShader")->setMat4("lightSpaceMatrix", GetLightSpaceMatrix());

    RESOURCEMANAGER.GetShaderByName("shadowInstanceShader")->use();
    RESOURCEMANAGER.GetShaderByName("shadowInstanceShader")->setMat4("lightSpaceMatrix", GetLightSpaceMatrix());
}

void ShadowMap::EndRender() {
    glBindFramebuffer(GL_FRAMEBUFFER, 0);

    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, GetDepthMapTexture());
}

GLuint ShadowMap::GetDepthMapTexture() const {
    return depthMap;
}

glm::mat4 ShadowMap::GetLightSpaceMatrix() const {
    return lightProjection * lightView;
}

void ShadowMap::SetLightProjection(const glm::mat4& projection) {
    lightProjection = projection;
}

void ShadowMap::SetLightView(const glm::mat4& view) {
    lightView = view;
}
