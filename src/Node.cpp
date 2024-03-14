#include "Node.h"

Node::Node(Model* model, Shader* shader) : _model(model), _local(std::make_shared<Transform>()), _shader(shader) {}
Node::Node() : _model(nullptr), _local(std::make_shared<Transform>()), _shader(nullptr) {}

void Node::AddChild(std::shared_ptr<Node> child) {
    child->GetTransform()->SetParent(_local);
    _children.push_back(child);
}

void Node::AddComponent(std::shared_ptr<Component> component) {
    component->SetOwnerTransform(_local);
    _components.push_back(component);
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

    if (_model) RenderModel(_model, world);

    for (auto& component : _components)
        if (component->IsEnabled())
            component->Render(parentWorld);

    for (auto& child : _children)
        child->Render(world);
}

void Node::RenderShadows(glm::mat4 parentWorld, Shader* shader) {
    glm::mat4 world = _local->Combine(parentWorld);

    if (_model) RenderModel(_model, world);

    for (auto& component : _components)
        if (component->IsEnabled())
            component->Render(parentWorld);

    for (auto& child : _children)
        child->Render(world);
}

void Node::RenderModel(Model* model, glm::mat4 ctm) {
    _shader->use();
    _shader->setMat4("model", ctm);
    model->Draw(*_shader);
}

void Node::UpdateTransforms(glm::mat4 parentWorld) {
    glm::mat4 world = _local->Combine(parentWorld);
    for (auto& child : _children)
        child->UpdateTransforms(world);
}