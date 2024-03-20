#pragma once

#include <glm/glm.hpp>
#include <model.h>
#include "Core/Component.h"
#include "Component/Transform.h"
#include <memory>

class Node {
public:
    Node();

    std::shared_ptr<Transform> GetTransform() { return _local; }

    void AddChild(std::shared_ptr<Node> child);
    void AddComponent(std::shared_ptr<Component> component);
    void Init();
    void Input();
    void Update();
    void Render(glm::mat4 parentWorld);
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

    std::vector<std::shared_ptr<Node>> _children;
    std::vector<std::shared_ptr<Component>> _components;
};
