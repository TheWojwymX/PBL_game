#include "Disabler.h"
#include "Core/Time.h"
#include "Core/Node.h"

Disabler::Disabler() : _delay(1.0f)
{
    _type = ComponentType::DISABLER;
}

void Disabler::Deserialize(const nlohmann::json& jsonData)
{
    if (jsonData.contains("delay")) {
        _delay = jsonData["delay"].get<float>();
    }

    Component::Deserialize(jsonData);
}

nlohmann::json Disabler::Serialize()
{
    nlohmann::json data;
    data["delay"] = _delay;
    return data;
}

void Disabler::Update()
{
    if(!GAMEMANAGER._paused)
        DisableAfterDelay();
}

void Disabler::Reset()
{
    _timer = 0.0f;
}

void Disabler::DisableAfterDelay()
{
    _timer += TIME.GetDeltaTime();
    if (_timer >= _delay) {
        _ownerNode->SetEnabled(false);
    }
}
