#include "Rotate.h"
#include "Core/Time.h"
#include "Managers/GameManager.h"

Rotate::Rotate() : _axis(glm::vec3(0.0f, 1.0f, 0.0f)), _speed(1.0f)
{
    _type = ComponentType::ROTATE;
}

void Rotate::Deserialize(const nlohmann::json& jsonData)
{
    if (jsonData.contains("axis")) {
        _axis.x = jsonData["axis"]["x"].get<float>();
        _axis.y = jsonData["axis"]["y"].get<float>();
        _axis.z = jsonData["axis"]["z"].get<float>();
    }

    if (jsonData.contains("speed")) {
        _speed = jsonData["speed"].get<float>();
    }

    Component::Deserialize(jsonData);
}

nlohmann::json Rotate::Serialize()
{
    nlohmann::json data;
    data["axis"] = {
            {"x", _axis.x},
            {"y", _axis.y},
            {"z", _axis.z}
    };
    data["speed"] = _speed;

    return data;
}

void Rotate::Update()
{
    if(!GAMEMANAGER._paused)
        ApplyRotation();
}

void Rotate::Reset()
{
    _ownerTransform->SetRotation(glm::vec3(0.0f, -60.0f, 0.0f));
}

void Rotate::ApplyRotation()
{
    // Calculate the rotation angle based on the speed and delta time
    float angle = _speed * TIME.GetDeltaTime();

    // Get the current rotation quaternion
    glm::quat currentRotation = _ownerTransform->GetRotation();

    // Transform the rotation axis by the current rotation
    glm::vec3 transformedAxis = currentRotation * _axis;

    // Calculate the rotation quaternion based on the transformed axis and angle
    glm::quat rotation = glm::angleAxis(glm::radians(angle), glm::normalize(transformedAxis));

    // Combine the new rotation with the existing rotation
    _ownerTransform->SetRotation(rotation * currentRotation);
}
