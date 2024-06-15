#include "CompassController.h"

CompassController::CompassController() {
    _type = ComponentType::COMPASSCONTROLLER;
}

nlohmann::json CompassController::Serialize() {
    return Component::Serialize();
}

void CompassController::Deserialize(const nlohmann::json &jsonData) {
    Component::Deserialize(jsonData);
}

void CompassController::Init() {
    _playerNode = NODESMANAGER.getNodeByName("player");
    _shovelNode = NODESMANAGER.getNodeByName("CompassNode");
    _hideY = GAMEMANAGER._groundLevel - 0.8f;
    Component::Init();
}

void CompassController::Update() {
    Component::Update();
}

void CompassController::RealUpdate() {

    float playerY = _playerNode->GetTransform()->GetPosition().y;

    if (playerY >= _hideY && !_isHidden && !_playShowAnim) {
        _playHideAnim = true;
    }

    if(playerY <= _hideY) {
        if (!_isHidden && !_playShowAnim && INPUT.IsKeyPressed(69)) {
            _playHideAnim = true;
        } else if (_isHidden && !_playHideAnim && INPUT.IsKeyPressed(69)) {
            _playShowAnim = true;
        }
    }

    if (_playHideAnim) {
        PlayHideShovel();
    }
    if (_playShowAnim) {
        PlayShowShovel();
    }

    SetPosAndRot();
}

void CompassController::PlayHideShovel() {
    if (_visibilityAnimationTimer < _visibilityAnimationTime) {
        _visibilityAnimationTimer += TIME.GetDeltaTime();
        if (_visibilityAnimationTimer < _visibilityAnimationTime) {
            float t = glm::clamp(_visibilityAnimationTimer / _visibilityAnimationTime, 0.0f, 1.0f);
            _actualVisibilityOffset = glm::mix(_actualVisibilityOffset, _visibilityOffset, t);
        } else {
            _actualVisibilityOffset = _visibilityOffset;
            _visibilityAnimationTimer = 0;
            _playHideAnim = false;
            _isHidden = true;
        }
    }
}

void CompassController::PlayShowShovel() {
    if (_visibilityAnimationTimer < _visibilityAnimationTime) {
        _visibilityAnimationTimer += TIME.GetDeltaTime();
        if (_visibilityAnimationTimer < _visibilityAnimationTime) {
            float t = glm::clamp(_visibilityAnimationTimer / _visibilityAnimationTime, 0.0f, 1.0f);
            _actualVisibilityOffset = glm::mix(_actualVisibilityOffset, 0.0f, t);
        } else {
            _actualVisibilityOffset = 0;
            _visibilityAnimationTimer = 0;
            _playShowAnim = false;
            _isHidden = false;
        }
    }
}

void CompassController::SetPosAndRot() {
    // Get camera vectors
    glm::vec3 forwardVector = glm::normalize(_playerNode->GetComponent<Camera>()->GetFrontVector());
    glm::vec3 rightVector = glm::normalize(_playerNode->GetComponent<Camera>()->GetRightVector());
    glm::vec3 upVector = glm::normalize(_playerNode->GetComponent<Camera>()->GetUpVector());
    auto cameraPos = _playerNode->GetComponent<Camera>()->GetPosition();

    // Set shovel position based on offsets
    glm::vec3 shovelPosition = cameraPos + forwardVector * _forwardOffset + rightVector * _horizontalOffset + upVector * _verticalOffset;
    glm::vec3 additionalOffset = upVector * -_actualVisibilityOffset;
    shovelPosition += additionalOffset;
    _shovelNode->GetTransform()->SetPosition(shovelPosition);

    // Set initial shovel rotation based on camera's orientation
    glm::mat4 rotationMatrix(1.0f);
    rotationMatrix[0] = glm::vec4(rightVector, 0.0f);  // x-axis
    rotationMatrix[1] = glm::vec4(upVector, 0.0f);     // y-axis
    rotationMatrix[2] = glm::vec4(-forwardVector, 0.0f); // z-axis (negative because camera looks towards -z)
    glm::quat initialRotation = glm::quat_cast(rotationMatrix);
    _shovelNode->GetTransform()->SetRotation(initialRotation);

    // Get the owner's euler rotation and apply the +90.0f to the X-axis
    glm::vec3 eulerRotation = glm::degrees(glm::eulerAngles(_ownerTransform->GetRotation()));
    glm::quat ownerRotation = glm::quat(glm::vec3(glm::radians(eulerRotation.x + 90.0f), glm::radians(eulerRotation.y), glm::radians(eulerRotation.z)));
    _shovelNode->GetTransform()->SetRotation(ownerRotation);

    // Calculate the direction from shovel to the target point
    glm::vec3 directionToTarget = glm::normalize(glm::vec3(49.0f,shovelPosition.y,49.0f) - shovelPosition);

    // Transform the direction to the local coordinate system of the shovel
    glm::vec3 localDirection = glm::normalize(glm::inverse(glm::mat3(rotationMatrix)) * directionToTarget);

    // Calculate the angle between the current forward vector and the target direction on the YZ plane
    float angleY = glm::atan(localDirection.x, localDirection.z);

    // Create the rotation quaternion around the local Y-axis
    glm::quat rotationAroundY = glm::angleAxis(angleY, glm::vec3(0.0f, 1.0f, 0.0f));

    // Apply the additional rotation to the existing rotation
    glm::quat finalRotation = ownerRotation * rotationAroundY;
    _shovelNode->GetTransform()->SetRotation(finalRotation);
}