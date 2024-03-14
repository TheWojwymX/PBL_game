#pragma once

#include <glm/glm.hpp>
#include <model.h>
#include "Component.h"
#include <memory> // Include necessary header for std::shared_ptr

class Node {
public:
    Node(Model* model, Shader* shader);
    Node();

    std::shared_ptr<Transform> GetTransform() { return _local; }

    void AddChild(std::shared_ptr<Node> child);
    void AddComponent(std::shared_ptr<Component> component);
    void Update();
    void Render(glm::mat4 parentWorld);
    void RenderShadows(glm::mat4 parentWorld, Shader* shader);
    void UpdateTransforms(glm::mat4 parentWorld);

    template <typename T>
    std::shared_ptr<T> GetComponent() const;

private:
    std::shared_ptr<Transform> _local;
    Model* _model;
    Shader* _shader;

    std::vector<std::shared_ptr<Node>> _children;
    std::vector<std::shared_ptr<Component>> _components;

    void RenderModel(Model* model, glm::mat4 ctm);
};