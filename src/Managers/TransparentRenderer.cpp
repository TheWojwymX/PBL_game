#include "TransparentRenderer.h"


TransparentRenderer::TransparentRenderer() {  
}

TransparentRenderer& TransparentRenderer::Instance() {
    static TransparentRenderer instance;
    return instance;
}

void TransparentRenderer::Render() {
    for (const auto& modelData : _models) {
        if (modelData.meshRenderer) {
            modelData.meshRenderer->RenderModel(modelData.matrix);
        }
    }
    _models.clear();
}

void TransparentRenderer::AddModel(std::shared_ptr<MeshRenderer> renderer, glm::mat4 model) {
    if (!_cameraRef) {
        _cameraRef = COMPONENTSMANAGER.GetComponentByID<Camera>(2);
    }

    glm::vec3 cameraPos = _cameraRef->GetPosition();

    // Extract the translation component (position) from the model's matrix
    glm::vec3 modelPos = glm::vec3(model[3]);

    // Calculate distance from camera to model
    float distance = glm::distance(cameraPos, modelPos);

    // Create ModelData object
    ModelData newModelData(renderer, model, distance);

    // Find the correct position to insert the new model based on distance
    auto insertPos = std::lower_bound(_models.begin(), _models.end(), newModelData,
        [](const ModelData& a, const ModelData& b) {
            return a.distance > b.distance; // Sort in descending order
        });

    // Insert the model data into the vector
    _models.insert(insertPos, newModelData);
}