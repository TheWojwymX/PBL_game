#include "PlayerController.h"
#include "Core/Time.h"
#include "Core/Input.h"
#include "Core/Node.h"

PlayerController::PlayerController(float speed, float gravity, float jumpHeight, float groundLevel, float reach, int radius, float width, float height, float digPower)
    : _speed(speed), _gravity(gravity), _jumpHeight(jumpHeight), _groundLevel(groundLevel), _isGrounded(false), _velocity(glm::vec3(0.0f)), _inputVector(glm::vec2(0.0f)), _reach(reach), _radius(radius), _width(width), _height(height), _digPower(digPower),
    _interactionCooldown(0.25f), _timeSinceLastInteraction(0.0f) {
    _type = ComponentType::PLAYERCNTROLLER;
}



nlohmann::json PlayerController::Serialize() {
    nlohmann::json data = Component::Serialize();

    if (_blockManagerRef) {
        data["blockManagerRefID"] = _blockManagerRef->_id;
    }

    if (_cameraRef) {
        data["cameraRefID"] = _cameraRef->_id;
    }


    return data;
}

void PlayerController::Deserialize(const nlohmann::json &jsonData) {

    if (jsonData.contains("blockManagerRefID")) {
        _blockManagerRefID = jsonData["blockManagerRefID"].get<int>();
    }

    if (jsonData.contains("cameraRefID")) {
        _cameraRefID = jsonData["cameraRefID"].get<int>();
    }

    Component::Deserialize(jsonData);
}

void PlayerController::Initiate() {
    _blockManagerRef = COMPONENTSMANAGER.GetComponentByID<BlockManager>(_blockManagerRefID);
    _cameraRef = COMPONENTSMANAGER.GetComponentByID<Camera>(_cameraRefID);
    Component::Initiate();
}

void PlayerController::Init() {
    _blockManagerRef->SetCamera(_cameraRef);
}

void PlayerController::Input() {
    MovementInput();
    InteractionInput();
}

void PlayerController::Update() {
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
    _timeSinceLastInteraction += TIME.GetDeltaTime();

    if (_timeSinceLastInteraction >= _interactionCooldown && INPUT.GetMouseButtonState(GLFW_MOUSE_BUTTON_1)) {
        _blockManagerRef->RayIntersectsBlock(_reach, _radius, _digPower);
        _timeSinceLastInteraction = 0.0f;
    }
}


void PlayerController::CheckGrounded(glm::vec3 separationVector) {
    if (separationVector.y > 0) {
        float roundedY = std::round(_ownerTransform->GetPosition().y + 0.5f) - 0.5f;
        _ownerTransform->SetPosition(roundedY, 1);
        _isGrounded = true;
        _velocity.y = 0.0f;
    }
    else if (separationVector.y < 0) {
        _velocity.y = 0.0f;
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
        if(!_ownerNode->GetComponent<PlayerAudioController>()->_isWalking && _isGrounded){ //are PlayerAudioController steps not working at the moment and walking and on ground
            _ownerNode->GetComponent<PlayerAudioController>()->StartSteps();
        }
        else if(!_isGrounded){ //is walking and not grounded
            _ownerNode->GetComponent<PlayerAudioController>()->StopSteps();
        }
        move = glm::normalize(move) * _speed;
    }
    else{
        if(_ownerNode->GetComponent<PlayerAudioController>()->_isWalking){ //are PlayerAudioController steps working at the moment and there isn't any movement
            _ownerNode->GetComponent<PlayerAudioController>()->StopSteps();
        }
    }

    // Apply gravity
    _velocity.y += _gravity * TIME.GetDeltaTime();

    // Handle jump
    if (_isGrounded && INPUT.IsKeyPressed(GLFW_KEY_SPACE)) {
        _ownerNode->GetComponent<PlayerAudioController>()->PlayJumpSound();
        _velocity.y = sqrtf(_jumpHeight * -2.0f * _gravity);
        _jump = true;
    }

    glm::vec3 movementVector = (move + _velocity) * TIME.GetDeltaTime();

    std::pair<glm::vec3, glm::vec3> collisionResult = _blockManagerRef->CheckEntityCollision(_ownerTransform->GetPosition(), movementVector, _width, _height);

    _ownerTransform->AddPosition(collisionResult.first);

    CheckGrounded(collisionResult.second);
}

void PlayerController::addToInspector(ImguiMain *imguiMain) {
    if (ImGui::TreeNode("Player Controller"))
    {
        // Block Gui related things go there
        ImGui::Text("Test1:");
        ImGui::Text("Test2:");

        ImGui::TreePop();
    }

}
