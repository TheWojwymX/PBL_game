#include "PlayerController.h"
#include "Core/Time.h"
#include "Core/Input.h"

PlayerController::PlayerController(float speed, float gravity, float jumpHeight, float groundLevel, float reach, float width, float height)
    : _speed(speed), _gravity(gravity), _jumpHeight(jumpHeight), _groundLevel(groundLevel), _isGrounded(false), _velocity(glm::vec3(0.0f)),_inputVector(glm::vec2(0.0f)), _reach(reach), _width(width), _height(height) {}

nlohmann::json PlayerController::Serialize() {
    nlohmann::json data;

    data["type"] = "PlayerController";

    return data;
}

void PlayerController::Init() {
    _blockManagerRef->SetCamera(_cameraRef);
}

void PlayerController::Input() {
    MovementInput();
    InteractionInput();
}

void PlayerController::Update() {
    CheckGrounded();
    HandleMovement();
}

void PlayerController::MovementInput() {
    // Get input for movement along the X and Z axes
    float x = (INPUT.GetKeyDown(GLFW_KEY_D) ? 1.0f : 0.0f) - (INPUT.GetKeyDown(GLFW_KEY_A) ? 1.0f : 0.0f);
    float z = (INPUT.GetKeyDown(GLFW_KEY_W) ? 1.0f : 0.0f) - (INPUT.GetKeyDown(GLFW_KEY_S) ? 1.0f : 0.0f);

    _inputVector = glm::vec2(x, z);
    // Check if input vector is non-zero before normalization
    if (x != 0.0f || z != 0.0f)
        _inputVector = glm::normalize(_inputVector);
}

void PlayerController::InteractionInput() {
    if (INPUT.IsMousePressed(GLFW_MOUSE_BUTTON_1)) {
        _blockManagerRef->RayIntersectsBlock(_reach);
    }
}

void PlayerController::CheckGrounded() {
    if (_ownerTransform->GetPosition()[1] <= _groundLevel ) {
        _isGrounded = true;
        _ownerTransform->SetPosition(_groundLevel,1);
    }
    else {
        _isGrounded = false;
    }
}

void PlayerController::HandleMovement() {
    glm::vec3 move = _inputVector.x * _cameraRef->GetRightVector() + _inputVector.y * _cameraRef->GetFrontVector();
    move.y = 0.0f;

    // Only normalize if move vector is not zero
    if (glm::length(move) > 0.0f) {
        move = glm::normalize(move) * _speed;
    }

    // Apply gravity
    if (!_isGrounded)
        _velocity.y += _gravity * TIME.GetDeltaTime();
    else
        _velocity.y = 0.0f;

    // Handle jump
    if (_isGrounded && INPUT.IsKeyPressed(GLFW_KEY_SPACE)) {
        _velocity.y = sqrtf(_jumpHeight * -2.0f * _gravity);
    }

    glm::vec3 movementVector = (move + _velocity) * TIME.GetDeltaTime();

    _ownerTransform->AddPosition(movementVector);
}

