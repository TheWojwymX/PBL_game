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
    std::shared_ptr<Node> _dialNode;
    std::shared_ptr<Node> _upNode;
    std::shared_ptr<Node> _downNode;

    void SetPosAndRot();

    void RealUpdate();

    float _forwardOffset = 2.0f;  // Dystans przed kamerą
    float _horizontalOffset = -1.0f;  // Dystans w prawo od kamery
    float _verticalOffset = -1.0f;  // Dystans w dół od kamery

    bool _playHideAnim = false;
    bool _playShowAnim = false;
    float _visibilityAnimationTimer = 0;
    float _visibilityAnimationTime = 0.5;
    float _actualVisibilityOffset = 0;
    float _visibilityOffset = 3;
    bool _isHidden = false;

    float _hideY = 0;

    void PlayHideCompass();
    void PlayShowCompass();

    void PlayOpenCompass();
    void PlayCloseCompass();

    bool _playCloseAnim = false;
    bool _playOpenAnim = false;
    float _openingAnimationTimer = 0;
    float _openingAnimationTime = 0.5;
    float _actualOpeningOffset = 0;
    float _openingOffset = 3;
    bool _isOpened = false;

    glm::vec3 _upPosOffset = glm::vec3(0.18, -0.09, -0.05);
    float _upMax = -125;
    float _upActual = 0;

    float _distanceToOpen = 3.2;
};

