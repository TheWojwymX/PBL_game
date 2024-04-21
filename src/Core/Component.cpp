#include "Component.h"

#include <utility>
#include "imgui.h"

Component::Component() : _isEnabled(true), _ownerTransform(std::make_shared<Transform>()) {}

Component::~Component() {}

nlohmann::json Component::Serialize() {

    nlohmann::json data;

    data["componentType"] = _type;
    data["componentId"] = _id;

    return data;
}

void Component::Deserialize(const nlohmann::json &jsonData) {
    if (jsonData.contains("componentId")) {
        _id = jsonData["componentId"].get<int>();
    }

    if (jsonData.contains("componentType")) {
        _type = static_cast<ComponentType>(jsonData["componentType"].get<int>());
    }
}

void Component::initiate() {

}

void Component::Init() {}

void Component::Input() {}

void Component::Update() {}

void Component::Render(glm::mat4 parentWorld) {}

void Component::RenderShadows(glm::mat4 parentWorld, Shader *shader) {}

void Component::SetOwnerTransform(std::shared_ptr<Transform> transform) {
    _ownerTransform = std::move(transform);
}

void Component::SetEnabled(bool isEnabled) {
    _isEnabled = isEnabled;
}

bool Component::IsEnabled() const {
    return _isEnabled;
}

void Component::addToInspector(ImguiMain *imguiMain)
{

}

void Component::addToHierarchy() {
    ImGui::Text("Komponent");
}

