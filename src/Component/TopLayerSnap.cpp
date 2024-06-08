#include "TopLayerSnap.h"

TopLayerSnap::TopLayerSnap():_yOffset(0.0f)
{
	_type = ComponentType::TOPLAYERSNAP;
}

void TopLayerSnap::Deserialize(const nlohmann::json& jsonData)
{
    if (jsonData.contains("yOffset")) {
        _yOffset = jsonData["yOffset"].get<float>();
    }

    Component::Deserialize(jsonData);
}

nlohmann::json TopLayerSnap::Serialize()
{
    nlohmann::json data;
    data["yOffset"] = _yOffset;

    return data;
}

void TopLayerSnap::Init()
{
	_blockManagerRef = COMPONENTSMANAGER.GetComponentByID<BlockManager>(1);
}

void TopLayerSnap::Update()
{
	_ownerTransform->AddPosition(_yOffset, 1);
	_isEnabled = false;
}

