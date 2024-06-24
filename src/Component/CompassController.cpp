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
    _dialNode = NODESMANAGER.getNodeByName("CompassNode");
    _upNode = NODESMANAGER.getNodeByName("CompassUp");
    _downNode = NODESMANAGER.getNodeByName("CompassDown");

    _hideY = GAMEMANAGER._groundLevel - 0.8f;
}

bool CompassController::IsWithinHoleRange(){
    if (glm::distance(glm::vec3(49.5, _playerNode->GetTransform()->GetPosition().y, 49.5), _playerNode->GetTransform()->GetPosition()) > _distanceToOpen){
        return false;
    }
    else{
        return true;
    }
}

bool CompassController::IsDuringAnyAnim(){
    if(_playShowAnim || _playHideAnim || _playOpenAnim || _playCloseAnim){
        return true;
    }
    else{
        return false;
    }
}

void CompassController::RealUpdate() {

    float playerY = _playerNode->GetTransform()->GetPosition().y;

    if (playerY >= _hideY && !_isHidden && !IsDuringAnyAnim()) {
        _playHideAnim = true;
    }

    if (playerY <= _hideY) {
        if (!_isHidden && !_playShowAnim && INPUT.IsKeyPressed(69)) {
            _playHideAnim = true;
        } else if (_isHidden && !_playHideAnim && INPUT.IsKeyPressed(69)) {
            _playShowAnim = true;
        }
    }

    if(!IsWithinHoleRange() && !_isHidden && !IsDuringAnyAnim() && !_isOpened){
        _playOpenAnim = true;
    }else if(IsWithinHoleRange() && !_isHidden && !IsDuringAnyAnim() && _isOpened){
        _playCloseAnim = true;
    }

    if (_playHideAnim) {
        PlayHideCompass();
    }else if (_playShowAnim) {
        PlayShowCompass();
    }

    if (_playOpenAnim) {
        PlayOpenCompass();
    }else if (_playCloseAnim) {
        PlayCloseCompass();
    }

    //std::cout << "_isOpened: " << _isOpened << " _playCloseAnim: " << _playCloseAnim << " _playOpenAnim: " << _playOpenAnim << " _isHidden: " << _isHidden << std::endl;
    //std::cout << "is within hole: " << IsWithinHoleRange() << std::endl;
    SetPosAndRot();
}

void CompassController::PlayHideCompass() {
    if (_isOpened) {
        _playCloseAnim = true;
    } else if (_visibilityAnimationTimer < _visibilityAnimationTime) {
        _visibilityAnimationTimer += TIME.GetDeltaTime();
        if (_visibilityAnimationTimer < _visibilityAnimationTime) {
            float t = glm::clamp(_visibilityAnimationTimer / _visibilityAnimationTime, 0.0f, 1.0f);
            _actualVisibilityOffset = glm::mix(_actualVisibilityOffset, _visibilityOffset, t);
        } else {
            _actualVisibilityOffset = _visibilityOffset;
            _visibilityAnimationTimer = 0;
            _playHideAnim = false;
            _isHidden = true;
            _upActual = 0.0f;
            _openingAnimationTimer = 0;
            _isOpened = false;
        }
    }
}

void CompassController::PlayShowCompass() {
    if (_visibilityAnimationTimer < _visibilityAnimationTime) {
        _visibilityAnimationTimer += TIME.GetDeltaTime();
        if (_visibilityAnimationTimer < _visibilityAnimationTime) {
            float t = glm::clamp(_visibilityAnimationTimer / _visibilityAnimationTime, 0.0f, 1.0f);
            _actualVisibilityOffset = glm::mix(_actualVisibilityOffset, 0.0f, t);
            if (_visibilityAnimationTimer >= _visibilityAnimationTime / 2) {
                if (glm::distance(glm::vec3(49.5, _playerNode->GetTransform()->GetPosition().y, 49.5), _playerNode->GetTransform()->GetPosition()) >
                    _distanceToOpen) {
                    _playOpenAnim = true;
                }
            }
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

    // Set object position based on offsets
    glm::vec3 objectPosition = cameraPos + forwardVector * _forwardOffset + rightVector * _horizontalOffset + upVector * _verticalOffset;
    glm::vec3 additionalOffset = upVector * -_actualVisibilityOffset;
    objectPosition += additionalOffset;
    _dialNode->GetTransform()->SetPosition(objectPosition);
    _downNode->GetTransform()->SetPosition(objectPosition);

    // Set initial object rotation based on camera's orientation
    glm::mat4 rotationMatrix(1.0f);
    rotationMatrix[0] = glm::vec4(rightVector, 0.0f);  // x-axis
    rotationMatrix[1] = glm::vec4(upVector, 0.0f);     // y-axis
    rotationMatrix[2] = glm::vec4(-forwardVector, 0.0f); // z-axis (negative because camera looks towards -z)
    glm::quat initialRotation = glm::quat_cast(rotationMatrix);
    _dialNode->GetTransform()->SetRotation(initialRotation);
    _upNode->GetTransform()->SetRotation(initialRotation);
    _downNode->GetTransform()->SetRotation(initialRotation);

    //UP
    glm::quat currentRotation = _upNode->GetTransform()->GetRotation();
    glm::quat rotationOffsetY = glm::angleAxis(glm::radians(105.0f), glm::vec3(0.0f, 1.0f, 0.0f));
    glm::quat rotationOffsetZ = glm::angleAxis(glm::radians(_upActual), glm::vec3(0.0f, 0.0f, 1.0f));
    glm::quat ownerRotation = currentRotation * rotationOffsetY * rotationOffsetZ;
    _upNode->GetTransform()->SetRotation(ownerRotation);

    glm::vec3 upOffset = forwardVector * _upPosOffset.x + rightVector * _upPosOffset.z + upVector * _upPosOffset.y;
    glm::vec3 upPosition = objectPosition + upOffset;
    _upNode->GetTransform()->SetPosition(upPosition);


    //DOWN
    currentRotation = _downNode->GetTransform()->GetRotation();
    rotationOffsetY = glm::angleAxis(glm::radians(105.0f), glm::vec3(0.0f, 1.0f, 0.0f));
    ownerRotation = currentRotation * rotationOffsetY;
    _downNode->GetTransform()->SetRotation(ownerRotation);


    //MID
    // Get the owner's euler rotation and apply the +90.0f to the X-axis
    glm::vec3 eulerRotation = glm::degrees(glm::eulerAngles(_ownerTransform->GetRotation()));
    ownerRotation = glm::quat(glm::vec3(glm::radians(eulerRotation.x), glm::radians(eulerRotation.y), glm::radians(eulerRotation.z)));
    _dialNode->GetTransform()->SetRotation(ownerRotation);

    // Calculate the direction from object to the target point
    glm::vec3 directionToTarget = glm::normalize(glm::vec3(49.5f, objectPosition.y, 49.5f) - objectPosition);

    // Transform the direction to the local coordinate system of the object
    glm::vec3 localDirection = glm::normalize(glm::inverse(glm::mat3(rotationMatrix)) * directionToTarget);

    // Calculate the angle between the current forward vector and the target direction on the YZ plane
    float angleY = glm::atan(localDirection.x, localDirection.z) + 90;
    // Create the rotation quaternion around the local Y-axis
    glm::quat rotationAroundY = glm::angleAxis(angleY, glm::vec3(0.0f, 1.0f, 0.0f));

    // Apply the additional rotation to the existing rotation
    glm::quat finalRotation = ownerRotation * rotationAroundY;
    _dialNode->GetTransform()->SetRotation(finalRotation);
}

void CompassController::PlayOpenCompass() {
    if (_openingAnimationTimer < _openingAnimationTime) {
        _openingAnimationTimer += TIME.GetDeltaTime();
        if (_openingAnimationTimer < _openingAnimationTime) {
            float t = glm::clamp(_openingAnimationTimer / _openingAnimationTime, 0.0f, 1.0f);
            _upActual = glm::mix(0.0f, _upMax, t);
        } else {
            _upActual = _upMax;
            _openingAnimationTimer = 0;
            _playOpenAnim = false;
            _isOpened = true;
        }
    }
}

void CompassController::PlayCloseCompass() {
    if (_openingAnimationTimer < _openingAnimationTime) {
        _openingAnimationTimer += TIME.GetDeltaTime();
        if (_openingAnimationTimer < _openingAnimationTime) {
            float t = glm::clamp(_openingAnimationTimer / _openingAnimationTime, 0.0f, 1.0f);
            _upActual = glm::mix(_upMax, 0.0f, t);
        } else {
            _upActual = 0.0f;
            _openingAnimationTimer = 0;
            _playCloseAnim = false;
            _isOpened = false;
        }
    }
}
