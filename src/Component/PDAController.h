//
// Created by Jacek on 08.06.2024.
//

#ifndef SANDBOX_PDACONTROLLER_H
#define SANDBOX_PDACONTROLLER_H

#include "Core/Component.h"
#include "Managers/NodesManager.h"
#include "HUD/HUDMain.h"

class Page;

class PDAController : public Component {

public:

    PDAController();

    nlohmann::json Serialize() override;

    void Deserialize(const nlohmann::json &jsonData) override;

    void Init() override;

    void Update() override;

    std::shared_ptr<Node> _playerNode;
    std::shared_ptr<Node> _PDANode;
    std::shared_ptr<Node> _PDAAntennaNode;

    void SetPosAndRot();

    void RealUpdate();

    float _forwardOffset = 2.0f;  // Dystans przed kamerą
    float _horizontalOffset = 0;  // Dystans w prawo od kamery
    float _verticalOffset = -1.15f;  // Dystans w dół od kamery

    bool _playHideAnim = false;
    bool _playShowAnim = false;
    float _visibilityAnimationTimer = 0;
    float _visibilityAnimationTime = 0.5;
    float _actualVisibilityOffset = 0;
    float _visibilityOffset = 3;
    bool _isHidden = false;

    void PlayHidePDA();
    void PlayShowPDA();

    float _antennaYoffset = -1;
    float _antennaActualYoffset = 0;
    bool _playShowAntennaAnim = false;

    void PlayShowAntenna();

    void HideImmediately();
};


#endif //SANDBOX_PDACONTROLLER_H
