
#pragma once

#include <glm/glm.hpp>
#include <model.h>
#include "Core/Component.h"
#include "Component/Transform.h"
#include <memory>
#include <algorithm>
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
#include "Component/ShovelController.h"
#include "Component/PDAController.h"
#include "Component/ShovelRenderer.h"
#include "Component/Rotate.h"
#include "Component/Disabler.h"
#include "Component/GlowstickMovement.h"
#include "Component/TopLayerSnap.h"
#include "Component/CompassController.h"

class Node : public std::enable_shared_from_this<Node> {
public:
    Node();
    string _name;
    int _id;

    nlohmann::json Serialize();

    void Deserialize(const nlohmann::json& nodeJson);

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
    std::shared_ptr<Node> GetParent() const { return _parent.lock(); }

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

    const vector<std::shared_ptr<Node>>& getChildren() const;

    const std::shared_ptr<Node>& getFirstChild() const;

    const vector<std::shared_ptr<Component>>& getComponents() const;

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

    void SetEnabled(bool enabled);
    void MoveChildToEnd(std::shared_ptr<Node> child);
private:
    bool _enabled = true; 
    std::shared_ptr<Transform> _local;
    std::vector<std::shared_ptr<Node>> _children;
    std::vector<std::shared_ptr<Component>> _components;
    std::weak_ptr<Node> _parent;
};