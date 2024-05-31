//
// Created by Jacek on 30.05.2024.
//

#ifndef SANDBOX_SHOVELCONTROLLER_H
#define SANDBOX_SHOVELCONTROLLER_H

#include "Core/Component.h"
#include "Managers/NodesManager.h"

class ShovelController : public Component {

public:

    ShovelController();

    nlohmann::json Serialize() override;

    void Deserialize(const nlohmann::json &jsonData) override;

    void Init() override;

    void Update() override;

    std::shared_ptr<Node> _playerNode;
    std::shared_ptr<Node> _shovelNode;

    void SetPosAndRot();

    void RealUpdate();
};

#endif //SANDBOX_SHOVELCONTROLLER_H
