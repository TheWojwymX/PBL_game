//
// Created by Jacek on 23.03.2024.
//

#ifndef OPENGLGP_COMPONENTSMANAGER_H
#define OPENGLGP_COMPONENTSMANAGER_H

#include "Core/Component.h"

class ComponentsManager {
public:
    static ComponentsManager &getInstance();

    template <typename ComponentType>
    std::shared_ptr<ComponentType> getComponentByID(size_t id) {
        const auto& components = getComponentList<ComponentType>();
        if (id < components.size()) {
            return components[id];
        }
        return nullptr;
    }

    template<typename ComponentType, typename... Args>
    std::shared_ptr<ComponentType> createComponent(Args&&... args) {
        auto component = std::make_shared<ComponentType>(std::forward<Args>(args)...);
        component->id = getComponentList<ComponentType>().size();
        addComponent(component);
        return component;
    }

    template<typename ComponentType>
    std::shared_ptr<ComponentType> deserializeComponent(const nlohmann::json& componentJson) {
        auto component = std::make_shared<ComponentType>();
        component->Deserialize(componentJson);
        addComponentAt(component, component->id);
        return component;
    }

    ~ComponentsManager() = default;

//private:
    ComponentsManager() = default;

    template <typename ComponentType>
    void addComponent(std::shared_ptr<ComponentType> component) {
        getComponentList<ComponentType>().push_back(component);
    }

    template <typename ComponentType>
    void addComponentAt(std::shared_ptr<ComponentType> component, size_t position) {
        auto& components = getComponentList<ComponentType>();

        if (position >= components.size()) {
            components.resize(position + 1, nullptr);
        }

        components[position] = component;
    }

    template <typename ComponentType>
    std::vector<std::shared_ptr<ComponentType>>& getComponentList() {
        static std::vector<std::shared_ptr<ComponentType>> components;
        return components;
    }

    ComponentsManager(const ComponentsManager&) = delete;
    ComponentsManager& operator=(const ComponentsManager&) = delete;
};


#endif //OPENGLGP_COMPONENTSMANAGER_H
