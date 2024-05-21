#pragma once

#include <glm/glm.hpp>
#include <memory>
#include "Component/Transform.h"
#include "shader_s.h"
#include <../thirdparty/nlohmann/json.hpp>
#include <Core/ComponentTypeEnum.h>

class ImguiMain;
class Node;

class Component {
public:
    Component();
    virtual ~Component();

    virtual nlohmann::json Serialize();
    virtual void Deserialize(const nlohmann::json& jsonData);
    virtual void Initiate();
    virtual void Init();
    virtual void Input();
    virtual void Update();
    virtual void Render(glm::mat4 parentWorld);
    virtual void RenderShadows(glm::mat4 parentWorld);
    virtual void addToInspector(ImguiMain* imguiMain);
    //virtual void SetUp();
    void addToHierarchy();

    void SetOwnerTransform(std::shared_ptr<Transform> transform);
    void SetEnabled(bool isEnabled);
    bool IsEnabled() const;

    int _id;
    ComponentType _type;
    void SetOwnerNode(std::shared_ptr<Node> node);
    std::shared_ptr<Node> GetOwnerNode() {return _ownerNode;}

    glm::vec3 GetOwnerPosition();

    glm::quat GetOwnerRotation();

    glm::quat GetOwnerScale();

    std::shared_ptr<Node> _ownerNode;
protected:
    std::shared_ptr<Transform> _ownerTransform;
    bool _isEnabled = true;
};
