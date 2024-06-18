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

    if(INPUT.IsKeyPressed(GLFW_KEY_T))
    {
        StartShake(1.0f, 0.15f, glm::vec3(30, 300, 50));
    }

    if (!GAMEMANAGER._editMode) {
        ProcessMouseMovement(INPUT.GetMouseOffset().x, INPUT.GetMouseOffset().y);
    }
}


void Camera::Update() {
    if (_shakeTimer > 0.0f)
    {
        _shakeTimer -= TIME.GetDeltaTime();
        float shakeFactor = _shakeIntensity * (_shakeTimer / _shakeDuration);
        ApplyShakeEffect(_enemyPosition, shakeFactor);
    }
    else if (_isShaking) {
        _yaw = Lerp(_yaw, _originalYaw, 0.7f);
        _pitch = Lerp(_pitch, _originalPitch, 0.7f);
        if (fabs(_yaw - _originalYaw) < 0.01f && fabs(_pitch - _originalPitch) < 0.01f) {
            _yaw = _originalYaw;
            _pitch = _originalPitch;
            _isShaking = false;
        }
    }

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

void Camera::ApplyShakeEffect(glm::vec3 enemyPosition, float shakeFactor) {
    // Calculate the direction vector from the camera to the enemy
    glm::vec3 direction = enemyPosition - _position;
    direction.y = 0; // Ignore the y value
    direction = glm::normalize(direction);

    // Calculate the shake offset based on the direction and shake factor
    float shakeOffset = 3 * shakeFactor * glm::dot(direction, _right);
    float shakeOffsetPitch = shakeFactor * GetRandomFloat(-3.0, 3.0);

    // Apply the shake offset to the yaw
    _yaw += shakeOffset;
    _pitch += shakeOffsetPitch;
}

void Camera::StartShake(float intensity, float duration, glm::vec3 enemyPosition)
{
    _shakeIntensity = intensity;
    _shakeDuration = duration;
    _shakeTimer = duration;
    _enemyPosition = enemyPosition;

    _originalYaw = _yaw;
    _originalPitch = _pitch;
    _isShaking = true;
}

float Camera::GetRandomFloat(float min, float max) {
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution<> dis(min, max);
    return dis(gen);
}

float Camera::Lerp(float a, float b, float f) {
    return a + f * (b - a);
}