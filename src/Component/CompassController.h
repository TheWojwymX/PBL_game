#pragma once

#include "Core/Component.h"
#include "Managers/NodesManager.h"

class CompassController : public Component {

public:

    CompassController();

    nlohmann::json Serialize() override;

    void Deserialize(const nlohmann::json &jsonData) override;

    void Init() override;

    void Update() override;

    std::shared_ptr<Node> _playerNode;
    std::shared_ptr<Node> _shovelNode;

    void SetPosAndRot();

    void RealUpdate();

    float _forwardOffset = 2.0f;  // Dystans przed kamerą
    float _horizontalOffset = -1.0f;  // Dystans w prawo od kamery
    float _verticalOffset = -1.0f;  // Dystans w dół od kamery

    bool _playDigAnim = false;
    float _digAnimationTimer = 0;
    float _digAnimationTime = 0.2;
    float _actualDegree = 0;
    float _maxDegree = 60;
    float _minDegree = 0;

    bool _playHideAnim = false;
    bool _playShowAnim = false;
    float _visibilityAnimationTimer = 0;
    float _visibilityAnimationTime = 0.5;
    float _actualVisibilityOffset = 0;
    float _visibilityOffset = 3;
    bool _isHidden = false;

    float _hideY = 0;

    void PlayHideShovel();
    void PlayShowShovel();
};

