#include "Rotate.h"
#include "Core/Time.h"

Rotate::Rotate() : _axis(glm::vec3(0.0f, 1.0f, 0.0f)), _speed(1.0f)
{
    _type = ComponentType::ROTATE;
}

void Rotate::Deserialize(const nlohmann::json& jsonData)
{
    if (jsonData.contains("axis")) {
        _axis.x = jsonData["axis"].get<float>();
    }

    if (jsonData.contains("speed")) {
        _speed = jsonData["speed"].get<float>();
    }

    Component::Deserialize(jsonData);
}

nlohmann::json Rotate::Serialize()
{
    nlohmann::json data;

    data["axis"] = _axis.x;
    data["speed"] = _speed;

    return data;
}

void Rotate::Update()
{
    ApplyRotation();
}

void Rotate::ApplyRotation()
{
    // Calculate the rotation angle based on the speed and delta time
    float angle = _speed * TIME.GetDeltaTime();

    // Calculate the rotation quaternion based on the axis and angle
    glm::quat rotation = glm::angleAxis(glm::radians(angle), glm::normalize(_axis));

    // Combine the new rotation with the existing rotation
    _ownerTransform->SetRotation(rotation * _ownerTransform->GetRotation());
}
