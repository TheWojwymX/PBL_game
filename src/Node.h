#pragma once

#include <glm/glm.hpp>
#include <model.h>
#include "Component.h"
#include "Transform.h"
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
    std::shared_ptr<T> GetComponent() const {
        for (const auto& component : _components) {
            std::shared_ptr<T> derivedComponent = std::dynamic_pointer_cast<T>(component);
            if (derivedComponent != nullptr) {
                return derivedComponent;
            }
        }
        return nullptr; // Component of type T not found
    }

private:
    std::shared_ptr<Transform> _local;
    Model* _model;
    Shader* _shader;

    std::vector<std::shared_ptr<Node>> _children;
    std::vector<std::shared_ptr<Component>> _components;

    void RenderModel(Model* model, glm::mat4 ctm);
};
