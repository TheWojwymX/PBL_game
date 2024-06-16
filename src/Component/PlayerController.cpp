#include "PlayerController.h"
#include "Core/Time.h"
#include "Core/Input.h"
#include "Core/Node.h"
#include "Managers/GameManager.h"
#include "ShovelController.h"
#include "HUD/PageManager.h"

PlayerController::PlayerController(float speed, float gravity, float jumpHeight, float groundLevel, float reach, int radius, float width, float height, float digPower)
    : _speed(speed), _gravity(gravity), _jumpHeight(jumpHeight), _groundLevel(groundLevel), _isGrounded(false), _velocity(glm::vec3(0.0f)), _inputVector(glm::vec2(0.0f)), _reach(reach), _radius(radius), _width(width), _height(height), _digPower(digPower),
    _interactionCooldown(0.25f), _timeSinceLastInteraction(0.0f),
    _jetpackCapacityLevel(0), _miningSpeedLevel(0), _miningReachLevel(0), _miningRadiusLevel(0) 
{
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
    _shovelController = NODESMANAGER.getNodeByName("Shovel")->GetComponent<ShovelController>();
    Component::Initiate();
}

void PlayerController::Init() {
    _blockManagerRef->SetCamera(_cameraRef);
}

void PlayerController::Input() {
    MovementInput();
    InteractionInput();
    JetpackInput();
}

void PlayerController::Update() {
    if(GAMEMANAGER._paused || PAGEMANAGER._isInUpgradeMenu){
        _ownerNode->GetComponent<PlayerAudioController>()->StopSteps();
        HandleGravity();
    }else{
        HandleMovement();
        HandleGlowstick();
    }
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

    if (_timeSinceLastInteraction >= _interactionCooldown && INPUT.GetMouseButtonState(GLFW_MOUSE_BUTTON_1)
        && !_shovelController->_isHidden && !_shovelController->_playHideAnim) {
        if(_blockManagerRef->RayIntersectsBlock(_reach, _radius, _digPower))
            RESOURCEMANAGER.GetSoundByID(17)->PlaySoundSim(_ownerNode);
            _shovelController->_playDigAnim = true;
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

void PlayerController::HandleGravity() {
    _velocity.y += _gravity * TIME.GetDeltaTime();
    glm::vec3 movementVector = _velocity * TIME.GetDeltaTime();
    movementVector = glm::clamp(movementVector, -glm::vec3(0.999f), glm::vec3(0.999f));
    std::pair<glm::vec3, glm::vec3> collisionResult = _blockManagerRef->CheckEntityCollision(_ownerTransform->GetPosition(), movementVector, _width, _height);
    _ownerTransform->AddPosition(collisionResult.first);
    CheckGrounded(collisionResult.second);
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

    HandleJetpack();

    glm::vec3 movementVector = (move + _velocity) * TIME.GetDeltaTime();
    movementVector = glm::clamp(movementVector, -glm::vec3(0.999f), glm::vec3(0.999f));

    if(CheckIsOutsideBase(_ownerTransform->GetPosition(), GAMEMANAGER._domePosition, GAMEMANAGER._domeRadius)){
        movementVector = CircleCollision(_ownerTransform->GetPosition(), movementVector, GAMEMANAGER._domePosition, GAMEMANAGER._domeRadius, true);
    }

    if(_activeMineEntranceCollision){
        if(CheckIfPlayerIsAtEntranceToMine()){
            movementVector = CircleCollision(_ownerTransform->GetPosition(), movementVector, GAMEMANAGER._domePosition, GAMEMANAGER._mineEntranceRadius, false);
        }
    }

    std::pair<glm::vec3, glm::vec3> collisionResult = _blockManagerRef->CheckEntityCollision(_ownerTransform->GetPosition(), movementVector, _width, _height);

    _ownerTransform->AddPosition(collisionResult.first);

    CheckGrounded(collisionResult.second);
}

void PlayerController::HandleJetpack()
{
    // Get delta time
    float deltaTime = TIME.GetDeltaTime();

    if (_isUsingJetpack) {
        if (_jetpackFuel > 0 && _ownerTransform->GetPosition().y < GAMEMANAGER._groundLevel) {
            // Playing jetpack sound
            RESOURCEMANAGER.GetSoundByID(16)->PlaySound(_ownerNode, 0.5f);

            // Applying upward force
            _velocity.y += _jetpackStrength * deltaTime;

            // Consuming fuel
            _jetpackFuel -= _jetpackFuelConsumption * deltaTime;

            // Ensure fuel doesn't go below zero
            if (_jetpackFuel <= 0) {
                _jetpackFuel = 0;
                _isUsingJetpack = false;
                StartJetpackRegenTimer();
                RESOURCEMANAGER.GetSoundByID(16)->StopSound();
            }

            // Jetpack particle effects
            _ownerNode->GetComponent<ParticleGenerator>()->jumpOffPoint = glm::vec3(0.0f, _blockManagerRef->GetCaveFloor(_ownerTransform->GetPosition(), 10.0f), 0.0f);
            _ownerNode->GetComponent<ParticleGenerator>()->SpawnParticles();
        }
    }
    else {
        // Check if delay is active
        if (_fuelRegenerationDelayActive) {
            // Update the delay timer
            _fuelRegenerationDelayTimer += deltaTime;
            if (_fuelRegenerationDelayTimer >= _fuelRegenerationDelay) {
                _fuelRegenerationDelayActive = false;  // End delay
            }
        }
        else {
            // Recover fuel only if delay is not active
            _jetpackFuel += _fuelRegeneration * _maxJetpackFuel * deltaTime;

            // Ensure fuel doesn't exceed max capacity
            if (_jetpackFuel > _maxJetpackFuel) _jetpackFuel = _maxJetpackFuel;
        }
    }
}

void PlayerController::JetpackInput() {
    // Get the current time
    auto now = std::chrono::steady_clock::now();

    // Check if the space key is currently held down
    if (INPUT.GetKeyDown(GLFW_KEY_SPACE)) {
        if (!_spaceKeyWasPressed) {
            // If the space key was not pressed before, update the last press time
            _lastSpacePressTime = now;
            _spaceKeyWasPressed = true;
        }

        // Calculate the elapsed time since the space key was last pressed
        auto elapsed = std::chrono::duration_cast<std::chrono::duration<float>>(now - _lastSpacePressTime).count();

        if (elapsed >= _jetpackActivationDelay) {
            if (_jetpackFuel > 0.0f) {
                _isUsingJetpack = true;
                StartJetpackRegenTimer();
            }
            else {
                StartJetpackRegenTimer();
            }
        }
    }
    else {
        // If the space key is not held down, reset the state
        _spaceKeyWasPressed = false;
        _isUsingJetpack = false;
    }
}

void PlayerController::StartJetpackRegenTimer()
{
    _fuelRegenerationDelayActive = true;
    _fuelRegenerationDelayTimer = 0.0f;
}

bool PlayerController::CheckIfPlayerIsAtEntranceToMine(){
    auto playerPos = _ownerNode->GetTransform()->GetPosition();
    playerPos.y = GAMEMANAGER._groundLevel;

    if(glm::distance(playerPos, glm::vec3(GAMEMANAGER._domePosition.x, GAMEMANAGER._groundLevel, GAMEMANAGER._domePosition.y)) <= GAMEMANAGER._mineEntranceRadius){
        return true;
    }else{
        return false;
    }
}

glm::vec3 PlayerController::CircleCollision(glm::vec3 playerPos, glm::vec3 movementVec, glm::vec2 circleCenterPos, float circleRadius, bool isInsideCircle) {
    glm::vec3 newPos = playerPos + movementVec;
    glm::vec2 newPos2D = glm::vec2(newPos.x, newPos.z);
    glm::vec2 playerPos2D = glm::vec2(playerPos.x, playerPos.z);
    glm::vec2 direction = glm::normalize(newPos2D - circleCenterPos);
    glm::vec2 perpDirection = glm::vec2(-direction.y, direction.x);

    if (isInsideCircle) {
        if (glm::distance(newPos2D, circleCenterPos) > circleRadius) {
            glm::vec2 correctedMovement2D = glm::dot(glm::vec2(movementVec.x, movementVec.z), perpDirection) * perpDirection;
            return glm::vec3(correctedMovement2D.x, movementVec.y, correctedMovement2D.y);
        }
    } else {
        if (glm::distance(newPos2D, circleCenterPos) < circleRadius) {
            glm::vec2 tangentMovement = glm::dot(glm::vec2(movementVec.x, movementVec.z), perpDirection) * perpDirection;
            glm::vec2 radialMovement = direction * (circleRadius - glm::distance(playerPos2D, circleCenterPos));
            float interpolationFactor = 0.5f;
            glm::vec2 correctedMovement2D = glm::mix(tangentMovement, tangentMovement + radialMovement, interpolationFactor);
            return glm::vec3(correctedMovement2D.x, movementVec.y, correctedMovement2D.y);
        }
    }

    return movementVec;
}


bool PlayerController::CheckIsOutsideBase(glm::vec3 playerPos, glm::vec2 domePos, float domeRadius){

    glm::vec2 playerPos2D = glm::vec2(playerPos.x, playerPos.z);

    if(glm::distance(playerPos2D, domePos) + 4.5f > domeRadius && playerPos.y >= GAMEMANAGER._groundLevel){
        return true;
    } else{
        return false;
    }
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

void PlayerController::HandleGlowstick() {
    // Get delta time in seconds
    float deltaTime = TIME.GetDeltaTime();

    // Update the cooldown timer
    _glowstickCooldownTimer += deltaTime;

    // Check if cooldown has elapsed
    if (_glowstickCooldownTimer >= _glowstickCooldown) {
        if (Input::Instance().IsMousePressed(1) && NODESMANAGER.getNodeByName("player")->GetTransform()->GetPosition().y < GAMEMANAGER._groundLevel - 1.0) {
            LIGHTSMANAGER.AddGlowstick();
            _glowstickCooldownTimer = 0.0f; 
        }
    }
}

void PlayerController::SetGravity(float gravity) {
    _gravity = gravity;
}
