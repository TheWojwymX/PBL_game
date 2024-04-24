#ifndef SHADOW_MAP_H
#define SHADOW_MAP_H

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "glad/glad.h"

#include "Managers/ComponentsManager.h"
#include "MeshRenderer.h"
#include "InstanceRenderer.h"
#include "Managers/GameManager.h"


class ShadowMap {
public:
    ShadowMap(unsigned int shadowWidth = 4096, unsigned int shadowHeight = 4096);
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

#endif  // SHADOW_MAP_H
