#pragma once

#include <glm/glm.hpp>
#include <memory>
#include "Component/Transform.h"
#include "shader_s.h"

class Component {
public:
    Component();
    virtual ~Component();

    virtual void Init();
    virtual void Input();
    virtual void Update();
    virtual void Render(glm::mat4 parentWorld);
    virtual void RenderShadows(glm::mat4 parentWorld, Shader* shader);

    void SetOwnerTransform(std::shared_ptr<Transform> transform);
    void SetEnabled(bool isEnabled);
    bool IsEnabled() const;

protected:
    std::shared_ptr<Transform> _ownerTransform;
    bool _isEnabled;
};
