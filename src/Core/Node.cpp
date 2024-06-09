#include "Node.h"
#include "../Managers/NodesManager.h"


Node::Node() : _local(std::make_shared<Transform>()) {}

nlohmann::json Node::Serialize() {
    nlohmann::json nodeJson;

    nodeJson["NodeName"] = _name;
    nodeJson["NodeID"] = _id;

    // Transform
    auto transform = GetTransform();
    if (transform != nullptr) {

        glm::vec3 eulerRotation = glm::degrees(glm::eulerAngles(transform->GetRotation()));

        nodeJson["transform"]["position"] = {transform->GetPosition().x, transform->GetPosition().y, transform->GetPosition().z};
        nodeJson["transform"]["rotation"] = {eulerRotation.x, eulerRotation.y, eulerRotation.z};
        nodeJson["transform"]["scale"] = {transform->GetScale().x, transform->GetScale().y, transform->GetScale().z};
    }

    // Components
    nlohmann::json componentsJson = nlohmann::json::array();
    for (auto &component: _components) {
        componentsJson.push_back(component->Serialize());
    }
    nodeJson["components"] = componentsJson;

    // Child Nodes
    nlohmann::json childrenJson = nlohmann::json::array();
    for (auto &child: _children) {
        childrenJson.push_back(child->Serialize());
    }
    nodeJson["children"] = childrenJson;

    return nodeJson;
}

void Node::Deserialize(const nlohmann::json &nodeJson) {
    if (nodeJson.contains("NodeName")) {
        _name = nodeJson["NodeName"].get<string>();
    }

    if (nodeJson.contains("NodeID")) {
        _id = nodeJson["NodeID"].get<int>();
    }

    NODESMANAGER.addNodeAt(this->shared_from_this(), _id);

    // Transform
    if (nodeJson.contains("transform")) {
        auto transformData = nodeJson["transform"];
        auto position = transformData["position"];
        auto rotation = transformData["rotation"];
        auto scale = transformData["scale"];
        _local->SetPosition(glm::vec3(position[0], position[1], position[2]));
        _local->SetRotation(glm::vec3(rotation[0], rotation[1], rotation[2]));
        _local->SetScale(glm::vec3(scale[0], scale[1], scale[2]));
    }

    // Components
    if (nodeJson.contains("components")) {
        for (auto &componentJson: nodeJson["components"]) {
            if (componentJson.contains("componentType")) {
                ComponentType type = componentJson["componentType"];

                switch (type) {
                    case ComponentType::CAMERA:
                        ComponentsManager::getInstance().DeserializeComponent<Camera>(componentJson);
                        break;
                    case ComponentType::PLAYERCNTROLLER:
                        ComponentsManager::getInstance().DeserializeComponent<PlayerController>(componentJson);
                        break;
                    case ComponentType::INSTANCERENDERER:
                        ComponentsManager::getInstance().DeserializeComponent<InstanceRenderer>(componentJson);
                        break;
                    case ComponentType::BLOCKMANAGER:
                        ComponentsManager::getInstance().DeserializeComponent<BlockManager>(componentJson);
                        break;
                    case ComponentType::AUDIOSOURCE:
                        ComponentsManager::getInstance().DeserializeComponent<PlayerAudioController>(componentJson);
                        break;
                    case ComponentType::MESHRENDERER:
                        ComponentsManager::getInstance().DeserializeComponent<MeshRenderer>(componentJson);
                        break;
                    case ComponentType::ENEMYAI:
                        ComponentsManager::getInstance().DeserializeComponent<Enemy>(componentJson);
                        break;
                    case ComponentType::ANIMATION:
                        ComponentsManager::getInstance().DeserializeComponent<Animation>(componentJson);
                        break;
                    case ComponentType::CLOUDMANAGER:
                        ComponentsManager::getInstance().DeserializeComponent<CloudManager>(componentJson);
                        break;
                    case ComponentType::TURRET:
                        ComponentsManager::getInstance().DeserializeComponent<Turret>(componentJson);
						break;
                    case ComponentType::PARTICLEGENERATOR:
                        ComponentsManager::getInstance().DeserializeComponent<ParticleGenerator>(componentJson);
                        break;
                    case ComponentType::REFUELSTATION:
                        ComponentsManager::getInstance().DeserializeComponent<RefuelStation>(componentJson);
                        break;
                    case ComponentType::SHOVELCONTROLLER:
                        ComponentsManager::getInstance().DeserializeComponent<ShovelController>(componentJson);
                        break;
                    case ComponentType::SHOVELRENDERER:
                        ComponentsManager::getInstance().DeserializeComponent<ShovelRenderer>(componentJson);
                        break;
                    case ComponentType::ROTATE:
                        ComponentsManager::getInstance().DeserializeComponent<Rotate>(componentJson);
                        break;
                    case ComponentType::DISABLER:
                        ComponentsManager::getInstance().DeserializeComponent<Disabler>(componentJson);
                        break;
                    case ComponentType::GLOWSTICKMOVEMENT:
                        ComponentsManager::getInstance().DeserializeComponent<GlowstickMovement>(componentJson);
                        break;
                    case ComponentType::PDACONTROLLER:
                        ComponentsManager::getInstance().DeserializeComponent<PDAController>(componentJson);
                        break;
                    case ComponentType::TOPLAYERSNAP:
                        ComponentsManager::getInstance().DeserializeComponent<TopLayerSnap>(componentJson);
                        break;
                    default:
                        std::cerr << "Unknown component type: " << static_cast<int>(type) << std::endl;
                        break;
                }

                AddComponent(COMPONENTSMANAGER.GetComponentByID<Component>(componentJson["componentId"].get<int>()));
            }
        }
    }

    // Child Nodes
    if (nodeJson.contains("children")) {
        for (auto &childJson: nodeJson["children"]) {
            auto childNode = std::make_shared<Node>();
            childNode->Deserialize(childJson);
            AddChild(childNode);
        }
    }
}

void Node::AddChild(std::shared_ptr<Node> child) {
    child->GetTransform()->SetParent(_local);
    _children.push_back(child);
}

void Node::AddComponent(std::shared_ptr<Component> component) {
    component->SetOwnerTransform(_local);
    std::shared_ptr<Node> self = shared_from_this();
    component->SetOwnerNode(self);
    _components.push_back(component);
}

void Node::SetEnabled(bool enabled) {
    _enabled = enabled;
}

void Node::Init() {
    if (!_enabled) return; // Check if node is enabled

    for (auto& component : _components)
        if (component->IsEnabled())
            component->Init();

    for (auto& child : _children)
        child->Init();
}

void Node::Input() {
    if (!_enabled) return; // Check if node is enabled

    for (auto& component : _components)
        if (component->IsEnabled())
            component->Input();

    for (auto& child : _children)
        child->Input();
}

void Node::Update() {
    if (!_enabled) return; // Check if node is enabled

    for (auto& component : _components)
        if (component->IsEnabled())
            component->Update();

    for (auto& child : _children)
        if (child != nullptr) child->Update();
}

void Node::Render(glm::mat4 parentWorld) {
    if (!_enabled) return; // Check if node is enabled

    glm::mat4 world = _local->Combine(parentWorld);

    for (auto& component : _components)
        if (component->IsEnabled())
            component->Render(parentWorld);

    for (auto& child : _children)
        if (child != nullptr) child->Render(world);
}

void Node::RenderShadows(glm::mat4 parentWorld) {
    if (!_enabled) return; // Check if node is enabled

    glm::mat4 world = _local->Combine(parentWorld);

    for (auto& component : _components)
        if (component->IsEnabled())
            component->RenderShadows(parentWorld);

    for (auto& child : _children)
        child->RenderShadows(world);
}

void Node::UpdateTransforms(glm::mat4 parentWorld) {
    if (!_enabled) return; // Check if node is enabled

    glm::mat4 world = _local->Combine(parentWorld);
    for (auto& child : _children)
        child->UpdateTransforms(world);
}

const vector<std::shared_ptr<Node>> &Node::getChildren() const {
    return _children;
}

const std::shared_ptr<Node> &Node::getFirstChild() const {
    return _children[0];
}

const vector<std::shared_ptr<Component>> &Node::getComponents() const {
    return _components;
}

void Node::addToInspector(ImguiMain *imguiMain) {
    _local->addToInspector(imguiMain);

    for (auto &component : _components)
    {
        component->addToInspector(imguiMain);
    }
}

void Node::removeChild(std::shared_ptr<Node> child)
{
    auto it = std::find(_children.begin(), _children.end(), child);
    if (it != _children.end())
    {
        _children.erase(it);
    }
    else
    {
        // If the child is not found at this level, recursively search in each child node
        for (auto& node : _children)
        {
            node->removeChild(child);
        }
    }
}

