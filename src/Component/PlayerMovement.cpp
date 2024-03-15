#include "PlayerMovement.h"
#include "Core/Time.h"
#include "Core/InputManager.h"

PlayerMovement::PlayerMovement(float speed, float gravity, float jumpHeight, float groundLevel)
    : _speed(speed), _gravity(gravity), _jumpHeight(jumpHeight), _groundLevel(groundLevel), _isGrounded(false), _velocity(glm::vec3(0.0f)),_inputVector(glm::vec2(0.0f)) {}

void PlayerMovement::Input() {
    // Get input for movement along the X and Z axes
    float x = (INPUT.GetKeyDown(GLFW_KEY_D) ? 1.0f : 0.0f) - (INPUT.GetKeyDown(GLFW_KEY_A) ? 1.0f : 0.0f);
    float z = (INPUT.GetKeyDown(GLFW_KEY_S) ? 1.0f : 0.0f) - (INPUT.GetKeyDown(GLFW_KEY_W) ? 1.0f : 0.0f);

    _inputVector = glm::vec2(x, z);
    // Check if input vector is non-zero before normalization
    if (x != 0.0f || z != 0.0f) 
        _inputVector = glm::normalize(_inputVector);
}

void PlayerMovement::Update() {
    CheckGrounded();
    HandleMovement();
}

void PlayerMovement::CheckGrounded() {
    if (_ownerTransform->GetPosition()[1] <= _groundLevel ) {
        _isGrounded = true;
        _ownerTransform->SetPosition(_groundLevel,1);
    }
    else {
        _isGrounded = false;
    }
}

void PlayerMovement::HandleMovement() {
    glm::vec3 move = glm::vec3(_inputVector.x, 0.0f, _inputVector.y);

    // Only normalize if move vector is not zero
    if (glm::length(move) > 0.0f) {
        move = glm::normalize(move) * _speed;
    }

    // Apply gravity
    if (!_isGrounded)
        _velocity.y += _gravity * TIME.GetDeltaTime();
    else
        _velocity.y = 0.0f;

    _ownerTransform->AddPosition(move * TIME.GetDeltaTime());

    // Handle jump
    if (_isGrounded && INPUT.IsKeyPressed(GLFW_KEY_SPACE)) {
        _velocity.y = sqrtf(_jumpHeight * -2.0f * _gravity);
    }

    _ownerTransform->AddPosition(_velocity * TIME.GetDeltaTime());
}

