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

    void PlayDigAnimation();

    float _forwardOffset = 2.0f;  // Dystans przed kamerą
    float _horizontalOffset = 1.3f;  // Dystans w prawo od kamery
    float _verticalOffset = -1.5f;  // Dystans w dół od kamery

    bool _playAnim = false;
    float _animationTimer = 0;
    float _animationTime = 0.2;
    float _actualDegree = 0;
    float _maxDegree = 60;
    float _minDegree = 0;
};

#endif //SANDBOX_SHOVELCONTROLLER_H
