#pragma once

#define TRANSPARENT_RENDERER TransparentRenderer::Instance()

#include <vector>
#include <memory>
#include "Component/MeshRenderer.h"
#include "Component/Camera.h"
#include <glm/gtx/norm.hpp>

struct ModelData {
    std::shared_ptr<MeshRenderer> meshRenderer;
    glm::mat4 matrix;
    float distance;

    // Constructor
    ModelData(std::shared_ptr<MeshRenderer> meshRenderer = nullptr,
        const glm::mat4& matrix = glm::mat4(1.0f),
        float distance = 0.0f)
        : meshRenderer(meshRenderer), matrix(matrix), distance(distance) {}
};

class TransparentRenderer {
public:
    static TransparentRenderer& Instance();

    void Render();

    void AddModel(std::shared_ptr<MeshRenderer> renderer, glm::mat4 model);

private:
    std::vector<ModelData> _models;
    std::shared_ptr<Camera> _cameraRef = nullptr;

    TransparentRenderer(); 
};
