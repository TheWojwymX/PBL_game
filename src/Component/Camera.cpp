#include "Camera.h"

Camera::Camera(glm::vec3 offset, glm::vec3 up, float yaw, float pitch)
    : _offset(offset), _position(glm::vec3(0.0f)), _worldUp(up), _yaw(yaw), _pitch(pitch),
    _front(glm::vec3(0.0f, 0.0f, -1.0f)), _movementSpeed(SPEED),
    _mouseSensitivity(SENSITIVITY), _zoom(ZOOM), _editMode(true) {
    UpdateCameraVectors();
}

nlohmann::json Camera::Serialize() const {
    return {
            {"type", "Camera"},
            {"offset", {_offset.x, _offset.y, _offset.z}},
            {"position", {_position.x, _position.y, _position.z}},
            {"front", {_front.x, _front.y, _front.z}},
            {"up", {_up.x, _up.y, _up.z}},
            {"right", {_right.x, _right.y, _right.z}},
            {"worldUp", {_worldUp.x, _worldUp.y, _worldUp.z}},
            {"yaw", _yaw},
            {"pitch", _pitch},
            {"movementSpeed", _movementSpeed},
            {"mouseSensitivity", _mouseSensitivity},
            {"zoom", _zoom},
            {"editMode", _editMode}
    };
}

void Camera::Input() {
    if (INPUT.IsKeyPressed(GLFW_KEY_E)) {
        _editMode = !_editMode;
        INPUT.SetCursorMode(_editMode);
    }

    if (!_editMode) {
        ProcessMouseMovement(INPUT.GetMouseOffset().x, INPUT.GetMouseOffset().y);
    }
}

void Camera::Update() {
    _position = _ownerTransform->GetPosition() + _offset;
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
