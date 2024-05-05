//
// Created by Jacek on 23.03.2024.
//

#define COMPONENTSMANAGER ComponentsManager::getInstance()

#ifndef OPENGLGP_COMPONENTSMANAGER_H
#define OPENGLGP_COMPONENTSMANAGER_H

#include "Core/Component.h"

class ComponentsManager {
public:
    static ComponentsManager &getInstance();

    ~ComponentsManager() = default;

    ComponentsManager() = default;

    template<typename ComponentType>
    std::shared_ptr<ComponentType> GetComponentByID(int id) {
        const auto &components = GetComponents();
        for (const auto &comp: components) {
            if (comp != nullptr && comp->_id == id) {
                std::shared_ptr<ComponentType> castedComponent = std::dynamic_pointer_cast<ComponentType>(components[id]);
                return castedComponent;
            }
        }
        return nullptr;
    }

    template<typename ComponentType>
    std::shared_ptr<ComponentType> DeserializeComponent(const nlohmann::json &componentJson) {
        auto component = std::make_shared<ComponentType>();
        component->Deserialize(componentJson);
        AddComponentAt(component, component->_id);
        return component;
    }

    template<typename ComponentType>
    void AddComponentAt(std::shared_ptr<ComponentType> component, int position) {
        auto &components = GetComponents();

        if (position >= components.size()) {
            components.resize(position + 1, nullptr);
        }
        components[position] = component;
    }

    template<typename ComponentType>
    void AddComponent(std::shared_ptr<ComponentType> component) {
        GetComponents().push_back(component);
    }

    template<typename ComponentType, typename... Args>
    std::shared_ptr<ComponentType> CreateComponent(Args &&... args) {
        auto component = std::make_shared<ComponentType>(std::forward<Args>(args)...);
        component->_id = _nextComponentID;
        _nextComponentID++;
        AddComponent(component);
        return component;
    }

    void Initiate(){
        const auto &components = GetComponents();
        for (const auto &comp: components) {
            if(comp != nullptr){
                comp->Initiate();
            }
        }
    }

    std::vector<std::shared_ptr<Component>> &GetComponents() {
        return Components;
    }

    std::vector<std::shared_ptr<Component>> Components;

    int _nextComponentID;

    ComponentsManager(const ComponentsManager &) = delete;

    ComponentsManager &operator=(const ComponentsManager &) = delete;
};


#endif //OPENGLGP_COMPONENTSMANAGER_H
