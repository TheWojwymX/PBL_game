#pragma once
#include "Core/Component.h"
#include <glm/glm.hpp>
#include <model.h>
#include "Managers/ResourceManager.h"
#include "Component/Camera.h"
#include "Managers/ComponentsManager.h"

struct FrustumPlane {
    glm::vec3 normal;
    float distance; // Distance from origin

    // Evaluate the plane equation for a point
    float distanceToPoint(const glm::vec3& point) const {
        return glm::dot(normal, point) + distance;
    }
};

class MeshRenderer : public Component {
public:
    MeshRenderer();

    MeshRenderer(Model* model, Shader* shader);

    void Render(glm::mat4 parentWorld) override;

    void RenderShadows(glm::mat4 parentWorld) override;

    void addToInspector(ImguiMain *imguiMain) override;

    nlohmann::json Serialize() override;

    void Deserialize(const nlohmann::json &jsonData) override;

    void initiate() override;

    void SetShader(shared_ptr<Shader> newShader) {
        _shader = newShader;
    }

    void Update() override;

    bool IsInFrustum(const glm::mat4& viewProjectionMatrix);
    void RenderBoundingBox(const glm::mat4& viewProjectionMatrix, glm::vec3 min, glm::vec3 max);
    std::vector<FrustumPlane> extractFrustumPlanes(const glm::mat4& viewProjectionMatrix);
    bool isBoxInFrustum(const std::vector<FrustumPlane>& planes, const glm::vec3& min, const glm::vec3& max);
    glm::vec3 GetWorldMinBoundingBox(const std::shared_ptr<Model>& model, const std::shared_ptr<Transform>& transform);
    glm::vec3 GetWorldMaxBoundingBox(const std::shared_ptr<Model>& model, const std::shared_ptr<Transform>& transform);

    bool _shouldRenderOutline = false;
    bool _renderWireframeBB = false;

    shared_ptr<Model> _model; // Raw pointer to the model
    shared_ptr<Shader> _shader; // Raw pointer to the shader


private:

    shared_ptr<Shader> _outlineShader; // Raw pointer to the outline shader
    std::shared_ptr<Camera> _cameraRef;
    int framesRendered;

    void RenderModel(shared_ptr<Model> model, glm::mat4 ctm);
};
