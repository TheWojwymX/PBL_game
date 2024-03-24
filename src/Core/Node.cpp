#include "Node.h"

Node::Node() : _local(std::make_shared<Transform>()) {}

nlohmann::json Node::Serialize() {
    nlohmann::json nodeJson;

    nodeJson["NodeID"] = id;

    // Transform
    auto transform = GetTransform();
    if (transform != nullptr) {
        nodeJson["transform"] = {
                {"position", {transform->GetPosition().x, transform->GetPosition().y, transform->GetPosition().z}},
                {"rotation", {transform->GetRotation().x, transform->GetRotation().y, transform->GetRotation().z}},
                {"scale", {transform->GetScale().x, transform->GetScale().y, transform->GetScale().z}}
        };
    }

    // Components
    nlohmann::json componentsJson = nlohmann::json::array();
    for (auto& component : _components) {
        componentsJson.push_back(component->Serialize());
    }
    nodeJson["components"] = componentsJson;

    // Child Nodes
    nlohmann::json childrenJson = nlohmann::json::array();
    for (auto& child : _children) {
        childrenJson.push_back(child->Serialize());
    }
    nodeJson["children"] = childrenJson;

    return nodeJson;
}

void Node::AddChild(std::shared_ptr<Node> child) {
    child->GetTransform()->SetParent(_local);
    _children.push_back(child);
}

void Node::AddComponent(std::shared_ptr<Component> component) {
    component->SetOwnerTransform(_local);
    _components.push_back(component);
}

void Node::Init() {
    for (auto& component : _components)
        if (component->IsEnabled())
            component->Init();

    for (auto& child : _children)
        child->Init();
}

void Node::Input() {
    for (auto& component : _components)
        if (component->IsEnabled())
            component->Input();

    for (auto& child : _children)
        child->Input();
}

void Node::Update() {
    for (auto& component : _components)
        if (component->IsEnabled())
            component->Update();

    for (auto& child : _children)
        child->Update();
}

void Node::Render(glm::mat4 parentWorld) {
    glm::mat4 world = _local->Combine(parentWorld);

    for (auto& component : _components)
        if (component->IsEnabled())
            component->Render(parentWorld);

    for (auto& child : _children)
        child->Render(world);
}

void Node::UpdateTransforms(glm::mat4 parentWorld) {
    glm::mat4 world = _local->Combine(parentWorld);
    for (auto& child : _children)
        child->UpdateTransforms(world);
}