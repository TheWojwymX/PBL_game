#pragma once

#include "Managers/GameManager.h"

class ShadowMap {
public:
    ShadowMap(unsigned int shadowWidth = 2560, unsigned int shadowHeight = 1440);
    ~ShadowMap();

    // Initialize the shadow map
    void Init();

    void AssignShadowMapToShader();

    // Begin rendering to the shadow map
    void BeginRender();

    void RenderMap();

    void AssignLightSpaceMatrixToShader();

    // End rendering to the shadow map
    void EndRender();

    // Get the depth map texture
    GLuint GetDepthMapTexture() const;

    // Get the light space matrix
    glm::mat4 GetLightSpaceMatrix() const;

    // Set the light projection and view matrices
    void SetLightProjection(const glm::mat4& projection);
    void SetLightView(const glm::mat4& view);

private:
    GLuint depthMapFBO;   // Framebuffer object for the shadow map
    GLuint depthMap;      // Depth texture for the shadow map
    unsigned int shadowWidth;
    unsigned int shadowHeight;
    glm::mat4 lightProjection;
    glm::mat4 lightView;
};
