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
    DisableAfterDelay();
}

void Disabler::DisableAfterDelay()
{
    if (TIME.GetTime() >= _delay) {
        _ownerNode->SetEnabled(false);
    }
}
