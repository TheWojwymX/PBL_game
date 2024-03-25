#include "Component.h"

Component::Component() : _isEnabled(true), _ownerTransform(nullptr) {}

Component::~Component() {}

nlohmann::json Component::Serialize() {

    nlohmann::json data;

    data["componentId"] = id;

    return data;
}

void Component::Deserialize(const nlohmann::json &jsonData) {
    if (jsonData.contains("componentId")) {
        id = jsonData["componentId"].get<int>();
    }
}

void Component::Init() {}

void Component::Input() {}

void Component::Update() {}

void Component::Render(glm::mat4 parentWorld) {}

void Component::RenderShadows(glm::mat4 parentWorld, Shader* shader) {}

void Component::SetOwnerTransform(std::shared_ptr<Transform> transform) {
    _ownerTransform = transform;
}

void Component::SetEnabled(bool isEnabled) {
    _isEnabled = isEnabled;
}

bool Component::IsEnabled() const {
    return _isEnabled;
}
