#include "Camera.h"
#include "Managers/GameManager.h"
#include "HUD/PageManager.h"

Camera::Camera(glm::vec3 offset, glm::vec3 up, float yaw, float pitch)
    : _offset(offset), _position(glm::vec3(0.0f)), _worldUp(up), _yaw(yaw), _pitch(pitch),
    _front(glm::vec3(0.0f, 0.0f, -1.0f)),
    _mouseSensitivity(SENSITIVITY), _zoom(ZOOM),
    _bobAmount(0.06f), _bobFrequency(6.0f), _bobTime(0.0f) {

    _type = ComponentType::CAMERA;
    UpdateCameraVectors();
}

nlohmann::json Camera::Serialize() {
    nlohmann::json data = Component::Serialize();
    data["offset"] = { _offset.x, _offset.y, _offset.z };
    return data;
}

void Camera::Deserialize(const nlohmann::json& jsonData) {
    if (jsonData.contains("offset")) {
        _offset = glm::vec3(
            jsonData["offset"][0].get<float>(),
            jsonData["offset"][1].get<float>(),
            jsonData["offset"][2].get<float>()
        );
    }
    Component::Deserialize(jsonData);
}

void Camera::Initiate() {
    Component::Initiate();
}

void Camera::Input() {
    if (INPUT.IsKeyPressed(GLFW_KEY_E)) {
        GAMEMANAGER._editMode = !GAMEMANAGER._editMode;
        INPUT.SetCursorMode(GAMEMANAGER._editMode);
    }

    if (!GAMEMANAGER._editMode) {
        ProcessMouseMovement(INPUT.GetMouseOffset().x, INPUT.GetMouseOffset().y);
    }
}

void Camera::Update() {
    if (_playerRef == nullptr) _playerRef = _ownerNode->GetComponent<PlayerController>();
    _position = _ownerTransform->GetPosition() + _offset;

    // Figure-8 head bob effect
    if (_playerRef->IsWalking() && !PAGEMANAGER._isInPage && !GAMEMANAGER._paused) {
        _bobTime += TIME.GetDeltaTime(); // Adjust multiplier to control speed of bob effect
        float horizontalBobOffset = _horizontalBobScale * _bobAmount * sin(_bobFrequency * _bobTime);

        // Apply a phase shift of π/2 to the cosine to start at 0
        float verticalBobOffset = _bobAmount * cos(2.0f * _bobFrequency * _bobTime - glm::half_pi<float>());

        // Apply the bob to the camera's horizontal and vertical position
        _currentHorizontalOffset = horizontalBobOffset;
        _currentVerticalOffset = verticalBobOffset;
    }
    else {
        // Smoothly transition bob offsets back to 0
        float lerpSpeed = 5.0f; // Adjust speed of the smoothing effect
        _currentHorizontalOffset = glm::mix(_currentHorizontalOffset, 0.0f, lerpSpeed * TIME.GetDeltaTime());
        _currentVerticalOffset = glm::mix(_currentVerticalOffset, 0.0f, lerpSpeed * TIME.GetDeltaTime());

        _bobTime = 0.0f; // Reset bobbing time
    }

    _position += _right * _currentHorizontalOffset;
    _position.y += _currentVerticalOffset;

    _viewProjectionMatrix = GetProjectionMatrix(_screenWidth, _screenHeight) * GetViewMatrix();
    _frustumPlanes = FrustumCulling::extractFrustumPlanes(_viewProjectionMatrix);
}

glm::mat4 Camera::GetViewMatrix() {
    return glm::lookAt(_position, _position + _front, _up);
}

void Camera::UpdateCameraVectors() {
    glm::vec3 front;
    front.x = cos(glm::radians(_yaw)) * cos(glm::radians(_pitch));
    front.y = sin(glm::radians(_pitch));
    front.z = sin(glm::radians(_yaw)) * cos(glm::radians(_pitch));
    _front = glm::normalize(front);
    _right = glm::normalize(glm::cross(_front, _worldUp));
    _up = glm::normalize(glm::cross(_right, _front));
}

void Camera::ProcessMouseMovement(float xoffset, float yoffset, GLboolean constrainPitch) {
    xoffset *= _mouseSensitivity;
    yoffset *= _mouseSensitivity;

    _yaw += xoffset;
    _pitch += yoffset;

    if (constrainPitch) {
        if (_pitch > 89.0f)
            _pitch = 89.0f;
        if (_pitch < -89.0f)
            _pitch = -89.0f;
    }

    UpdateCameraVectors();
}

glm::vec3 Camera::LerpPosition(glm::vec3 currentPosition) {
    float lerpValue = 0.25f;
    currentPosition = lerp(currentPosition, _position, lerpValue);
    return currentPosition;
}

glm::vec3 Camera::LerpDirection(glm::vec3 currentDirection) {
    float lerpValue = 0.3f;
    currentDirection = lerp(currentDirection, _front, lerpValue);
    return currentDirection;
}

glm::vec2 Camera::GetRotation() const {
    return glm::vec2(_yaw, _pitch);
}

void Camera::setRotation(glm::vec2 rotation) {
    _yaw = rotation.x;
    _pitch = rotation.y;
}

void Camera::setPosition(glm::vec3 position) {
    _position = position;
}