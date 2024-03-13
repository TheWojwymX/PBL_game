#pragma once

#include <glm/glm.hpp>
#include <model.h>
#include "Component.h"

class Node {
public:
    Node(Model* model, Shader* shader) : _model(model), _local(new Transform()), _shader(shader) {}
    Node() : _model(NULL), _local(new Transform()), _shader(NULL){}

    Transform* GetTransform() { return _local; }

    void AddChild(Node* child) {
        child->GetTransform()->SetParent(_local);
        _children.push_back(child);
    }

    void AddComponent(Component* component) {
        component->SetOwnerTransform(_local);
        _components.push_back(component);
    }

    void Update() {
        for (Component* component : _components)
            if (component->IsEnabled())
                component->Update();

        for (Node* child : _children)
            child->Update();
    }
   

    void Render(glm::mat4 parentWorld)
    {
        glm::mat4 world = _local->Combine(parentWorld);

        if (_model) RenderModel(_model, world);

        for (Component* component : _components)
            if (component->IsEnabled())
                component->Render(parentWorld);

        for (Node* child : _children)
            child->Render(world);
    }

    void RenderShadows(glm::mat4 parentWorld, Shader* shader)
    {
        glm::mat4 world = _local->Combine(parentWorld);

        if (_model) RenderModel(_model, world);

        for (Component* component : _components)
            if (component->IsEnabled())
                component->Render(parentWorld);

        for (Node* child : _children)
            child->Render(world);
    }

    void RenderModel(Model* model, glm::mat4 ctm)
    {
        _shader->use();
        _shader->setMat4("model", ctm);
        model->Draw(*_shader);
    }

    void UpdateTransforms(glm::mat4 parentWorld) {
        glm::mat4 world = _local->Combine(parentWorld);
        for (Node* child : _children)
            child->UpdateTransforms(world);
    }

    template <typename T>
    T* GetComponent() const {
        for (Component* component : _components) {
            T* derivedComponent = dynamic_cast<T*>(component);
            if (derivedComponent != nullptr) {
                return derivedComponent;
            }
        }
        return nullptr; // Component of type T not found
    }

private:
    Transform* _local;
    Model* _model;
    Shader* _shader;
    
    std::vector<Node*> _children;

    std::vector<Component*> _components;
};