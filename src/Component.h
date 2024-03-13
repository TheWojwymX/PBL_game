#pragma once

#include <glm/glm.hpp>
#include "Transform.h" 
#include "shader_s.h"

class Component {
public:
    Component():_isEnabled(true) {}
    virtual ~Component() {}

    virtual void Initialize() {};
    virtual void Update() {};
    virtual void Render(glm::mat4 parentWorld) {};
    virtual void RenderShadows(glm::mat4 parentWorld, Shader* shader) {};

    void SetOwnerTransform(Transform* transform) {
        _ownerTransform = transform;
    }

    void SetEnabled(bool isEnabled) {
        _isEnabled = isEnabled;
    }

    bool IsEnabled() const {
        return _isEnabled;
    }


protected:
    Transform* _ownerTransform;
    bool _isEnabled;
};