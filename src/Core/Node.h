#pragma once

#include <glm/glm.hpp>
#include <model.h>
#include "Core/Component.h"
#include "Component/Transform.h"
#include <memory>
#include "Component/Camera.h"
#include "Component/PlayerController.h"
#include "Component/InstanceRenderer.h"
#include "Component/MeshRenderer.h"
#include "Component/BlockManager.h"
#include "Managers/ComponentsManager.h"
#include "Component/PlayerAudioController.h"
#include "Component/CloudManager.h"
#include "Enemies/Enemy.h"
#include "Component/Animation.h"
#include "Turrets/Turret.h"
#include "ParticleGenerator.h"
#include "Component/RefuelStation.h"

class Node : public std::enable_shared_from_this<Node>{
public:
    Node();
    string _name;
    int _id;

    nlohmann::json Serialize();

    void Deserialize(const nlohmann::json &nodeJson);

    std::shared_ptr<Transform> GetTransform() { return _local; }

    void AddChild(std::shared_ptr<Node> child);
    void AddComponent(std::shared_ptr<Component> component);
    void Init();
    void Input();
    void Update();
    void Render(glm::mat4 parentWorld);
    void RenderShadows(glm::mat4 parentWorld);
    void UpdateTransforms(glm::mat4 parentWorld);
    void addToInspector(ImguiMain* imguiMain);
    void removeChild(std::shared_ptr<Node> child);

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

    const vector<std::shared_ptr<Node>> &getChildren() const;

    const vector<std::shared_ptr<Component>> &getComponents() const;

    template <typename T>
    std::vector<std::shared_ptr<T>> GetAllComponents() const {
        std::vector<std::shared_ptr<T>> matchingComponents;
        for (const auto& component : _components) {
            std::shared_ptr<T> derivedComponent = std::dynamic_pointer_cast<T>(component);
            if (derivedComponent != nullptr) {
                matchingComponents.push_back(derivedComponent);
            }
        }
        return matchingComponents; // Return all components of type T
    }

private:
    std::shared_ptr<Transform> _local;
    std::vector<std::shared_ptr<Node>> _children;
    std::vector<std::shared_ptr<Component>> _components;
};
