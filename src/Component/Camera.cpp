#include "Camera.h"
#include "Managers/GameManager.h"

Camera::Camera(glm::vec3 offset, glm::vec3 up, float yaw, float pitch)
        : _offset(offset), _position(glm::vec3(0.0f)), _worldUp(up), _yaw(yaw), _pitch(pitch),
          _front(glm::vec3(0.0f, 0.0f, -1.0f)), _movementSpeed(SPEED),
          _mouseSensitivity(SENSITIVITY), _zoom(ZOOM) {

    _type = ComponentType::CAMERA;
    UpdateCameraVectors();
}

nlohmann::json Camera::Serialize() {

    nlohmann::json data = Component::Serialize();

    data["offset"] = {_offset.x, _offset.y, _offset.z};

    return data;
}

void Camera::Deserialize(const nlohmann::json &jsonData) {
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
    if (INPUT.IsKeyPressed(GLFW_KEY_R)) {
        GAMEMANAGER._editMode = !GAMEMANAGER._editMode;
        INPUT.SetCursorMode(GAMEMANAGER._editMode);
    }

    if (!GAMEMANAGER._editMode) {
        ProcessMouseMovement(INPUT.GetMouseOffset().x, INPUT.GetMouseOffset().y);
    }
}


void Camera::Update() {
    _position = _ownerTransform->GetPosition() + _offset;

    _viewProjectionMatrix = GetProjectionMatrix(_screenWidth,_screenHeight) * GetViewMatrix();
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

void Camera::addToInspector(ImguiMain *imguiMain) {
    if (ImGui::TreeNode("Camera"))
    {
        ImGui::Text("Test1:");
        ImGui::Text("Test2:");

        ImGui::TreePop();
    }

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