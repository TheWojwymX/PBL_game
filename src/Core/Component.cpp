#include "Component.h"

Component::Component() : _isEnabled(true), _ownerTransform(nullptr) {}

Component::~Component() {}

nlohmann::json Component::Serialize() {
    return nlohmann::json();
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
