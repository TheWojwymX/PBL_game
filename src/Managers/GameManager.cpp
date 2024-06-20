#include "GameManager.h"
#include "../Enemies/EnemiesManager.h"
#include "../HUD/PageManager.h"
#include "../Managers/TutorialManager.h"
#include "Managers/NodesManager.h"
#include "UpgradeManager.h"
#include "Turrets/TurretsManager.h"

GameManager::GameManager() {
    // Starting time for digging phase
    float digPhaseStartTime = 30.0f;
    // Percentage separation
    float digPercentage = 0.60f;
    float fightPercentage = 0.40f;

    // Initialize _phaseTimes with calculated values for 10 rounds
    for (int i = 0; i < 10; ++i) {
        float digPhaseTime = digPhaseStartTime + i * 10;  // Dig phase starts at 30 and increases by 10 each round
        float fightPhaseTime = digPhaseTime * fightPercentage / digPercentage;  // Calculate fight phase time based on percentage
        _phaseTimes.push_back({ digPhaseTime, fightPhaseTime });
        //std::cout << "Round " << i + 1 << ": Digging Phase Time = " << digPhaseTime
        //   << " seconds, Fighting Phase Time = " << fightPhaseTime << " seconds" << std::endl;
    }
}

GameManager& GameManager::GetInstance() {
    static GameManager instance;
    return instance;
}

void GameManager::pressToSkipPhase() {
    if (INPUT.IsKeyPressed(GLFW_KEY_O)) {
        _currentTime = 31;
    }
}

void GameManager::AddPlastic(int amount) {
    HUD._isPlasticInAnim = true;
    _plastic += amount;
}

bool GameManager::HasPlastic(int amount) {
    return amount <= _plastic;
}

void GameManager::RemovePlastic(int amount)
{
    _plastic -= amount;
    HUD._isPlasticInAnim = true;
}

void GameManager::StartGame() {
    _isInMainMenu = false;
    GAMEMANAGER._editMode = false;
    INPUT.SetCursorMode(false);
    StopMenuMusic();
    PAGEMANAGER.CloseAllPages();
}

void GameManager::Evacuate() {
    std::cout << "Evacuation in GAMEMANAGER" << std::endl;
}

bool GameManager::IsUnderground() {
    if (_playerNode == nullptr) _playerNode = NODESMANAGER.getNodeByName("player");

    float playerY = _playerNode->GetTransform()->GetPosition().y;
    return playerY < _groundLevel;
}

float GameManager::GetPhaseTime()
{
    // Ensure _currentRound is within valid range
    int roundIndex = _roundNumber % _phaseTimes.size();

    // Return the phase time based on current phase
    if (_currentPhase == Phase::DIG) {
        return _phaseTimes[roundIndex].first;
    }
    else { // Phase::DEFEND
        return _phaseTimes[roundIndex].second;
    }
}

void GameManager::Update() {
    if (TUTORIALMANAGER._isFreePlay) {
        _currentTime += TIME.GetDeltaTime();
        pressToSkipPhase();

        if (_currentTime >= GetPhaseTime()) {
            _currentTime = 0.0f;
            _currentPhase = (_currentPhase == Phase::DIG) ? Phase::DEFEND : Phase::DIG;
            InitPhase();
        }
    }

    if(DOMEMANAGER.GetDomeHP() <= 0){
    //    LoseGame();
    }
}

void GameManager::InitPhase() {
    if (_currentPhase == Phase::DIG) {
        _roundNumber++;
    }
    else { // Phase::DEFEND
        ENEMIESMANAGER.SpawnEnemiesForRound(_roundNumber);
    }
}

void GameManager::Init() {
    if (_isFullscreen) {
        _screenWidth = 1920;
        _screenHeight = 1080;
        _window = glfwCreateWindow(_screenWidth, _screenHeight, "SandBOX", glfwGetPrimaryMonitor(), NULL);
    }
    else {
        _window = glfwCreateWindow(_screenWidth, _screenHeight, "SandBOX", NULL, NULL);
    }

    if (!_window) {
        std::cerr << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return;
    }

    // Make the OpenGL context current for the new window
    glfwMakeContextCurrent(_window);

    // Retrieve the actual dimensions of the framebuffer
    int actualWidth, actualHeight;
    glfwGetFramebufferSize(_window, &actualWidth, &actualHeight);

    // Update _screenWidth and _screenHeight with the actual dimensions
    _screenWidth = actualWidth;
    _screenHeight = actualHeight;
}

void GameManager::EnableMouse() {
    INPUT.SetCursorMode(true);
    _editMode = true;
}

void GameManager::DisableMouse() {
    INPUT.SetCursorMode(false);
    _editMode = false;
}

void GameManager::Pause() {
    _paused = true;
    PAGEMANAGER._pauseMenuPage->_shouldRender = true;
    HUD.DisableHUD();
    EnableMouse();
}

void GameManager::Unpause() {
    _paused = false;
    PAGEMANAGER._pauseMenuPage->_shouldRender = false;
    HUD.EnableHUD();
    DisableMouse();
}

void GameManager::Reset() {
    _roundNumber = 0;
    _currentPhase = Phase::DIG;
    _plastic = 5;
    _currentTime = 0.0f;
    DisableMouse();
}

void GameManager::PlayMenuMusic() {
    if (_playerNode == nullptr) _playerNode = NODESMANAGER.getNodeByName("player");
    RESOURCEMANAGER.GetSoundByName("BackgroundMusic")->SetLooping(true);
    RESOURCEMANAGER.GetSoundByName("BackgroundMusic")->RiseUp(3, _playerNode);
}

void GameManager::StopMenuMusic() {
    RESOURCEMANAGER.GetSoundByName("BackgroundMusic")->FadeAway(4);
}

void GameManager::RestartGame() {
    NODESMANAGER.getNodeByName("blockManager")->GetComponent<BlockManager>()->Reset();
    LIGHTSMANAGER.Reset();
    TURRETSMANAGER.Reset();
    ENEMIESMANAGER.Reset();
    Reset();
    TUTORIALMANAGER.Reset();
    DOMEMANAGER.Reset();
    UPGRADEMANAGER.Reset();
    NODESMANAGER.getNodeByName("player")->GetComponent<PlayerController>()->Reset();
    PAGEMANAGER.Reset();
    DisableMouse();
}

void GameManager::LoseGame() {
    HUD.DisableHUD();
    PAGEMANAGER.CloseAllOtherPages(PAGEMANAGER._restartPage);
    EnableMouse();
}
