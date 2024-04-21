#pragma once

#include <glm/glm.hpp>
#include <memory>
#include "Component/Transform.h"
#include "shader_s.h"
#include <../thirdparty/nlohmann/json.hpp>
#include <Core/ComponentTypeEnum.h>

class ImguiMain;

class Component {
public:
    Component();
    virtual ~Component();

    virtual nlohmann::json Serialize();
    virtual void Deserialize(const nlohmann::json& jsonData);
    virtual void initiate();
    virtual void Init();
    virtual void Input();
    virtual void Update();
    virtual void Render(glm::mat4 parentWorld);
    virtual void RenderShadows(glm::mat4 parentWorld, Shader* shader);
    virtual void addToInspector(ImguiMain* imguiMain);
    //virtual void SetUp();
    void addToHierarchy();

    void SetOwnerTransform(std::shared_ptr<Transform> transform);
    void SetEnabled(bool isEnabled);
    bool IsEnabled() const;

    int _id;
    ComponentType _type;
    std::shared_ptr<Transform> _ownerTransform;


protected:
    bool _isEnabled = true;
};
