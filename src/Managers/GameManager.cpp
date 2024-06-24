#include "GameManager.h"
#include "../Enemies/EnemiesManager.h"
#include "../HUD/PageManager.h"
#include "../Managers/TutorialManager.h"
#include "Managers/NodesManager.h"
#include "UpgradeManager.h"
#include "Turrets/TurretsManager.h"
#include "AudioManager.h"

GameManager::GameManager() {
    // Starting time for digging phase
    float digPhaseStartTime = 60.0f;
    // Percentage separation
    float digPercentage = 0.60f;
    float fightPercentage = 0.40f;

    float sum = 0;
    // Initialize _phaseTimes with calculated values for 10 rounds
    for (int i = 0; i < 10; ++i) {
        float digPhaseTime = digPhaseStartTime + i * 10;  // Dig phase starts at 30 and increases by 10 each round
        float fightPhaseTime = digPhaseTime * fightPercentage / digPercentage;  // Calculate fight phase time based on percentage
        _phaseTimes.push_back({ digPhaseTime, fightPhaseTime });
        //std::cout << "Round " << i + 1 << ": Digging Phase Time = " << digPhaseTime
        //   << " seconds, Fighting Phase Time = " << fightPhaseTime << " seconds" << std::endl;
    }
    _phaseTimes.push_back({ 60.0f, std::numeric_limits<float>::max() });
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
    // Return the phase time based on current phase
    if (_currentPhase == Phase::DIG) {
        return _phaseTimes[_roundNumber].first;
    }
    else { // Phase::DEFEND
        return _phaseTimes[_roundNumber].second;
    }
}

void GameManager::LateGame()
{
    ENEMIESMANAGER.Reset();
    _roundNumber = _phaseTimes.size() - 1;
    _currentPhase = Phase::DIG;
    _playerNode->GetComponent<PlayerAudioController>()->ChangeMusicToDigPhase();
    _currentTime = 0.0f;
    UPGRADEMANAGER.UpgradeMax();
}

float GameManager::GetPhaseProgress()
{
    return _currentTime / GetPhaseTime();
}

void GameManager::SkipPhase()
{
    _currentTime = GetPhaseTime();
}

bool GameManager::IsEndless()
{
    return _roundNumber == _phaseTimes.size() - 1;
}

void GameManager::RoundWon()
{
    _roundWon = true;
    std::cout << "Round WON!" << std::endl;
    RESOURCEMANAGER.GetSoundByName("VictoryMusic")->RiseUp(1, _playerNode);
    _playerNode->GetComponent<PlayerAudioController>()->ChangeMusicToDigPhase();
}

void GameManager::Update() {
    if(INPUT.IsKeyPressed(GLFW_KEY_KP_5)){
        ENEMIESMANAGER.Reset();
    }

    if (TUTORIALMANAGER._isFreePlay) {
        _currentTime += TIME.GetDeltaTime();
        pressToSkipPhase();

        if (_currentTime >= GetPhaseTime()) {
            _currentTime = 0.0f;
            _currentPhase = (_currentPhase == Phase::DIG) ? Phase::DEFEND : Phase::DIG;
            InitPhase();
        }

        if (INPUT.IsKeyPressed(GLFW_KEY_9))
            LateGame();
        if (INPUT.IsKeyPressed(GLFW_KEY_8))
            SkipPhase();
    }

    if(DOMEMANAGER.GetDomeHP() <= 0){
    //    LoseGame();
    }
}

void GameManager::InitPhase() {
    if (_currentPhase == Phase::DIG) {
        _playerNode->GetComponent<PlayerAudioController>()->ChangeMusicToDigPhase();
        _playerNode->GetComponent<PlayerAudioController>()->_isInDigPhase = true;
        if (_roundNumber < _phaseTimes.size() - 1) {
            _roundNumber++;
        }
    }
    else {
        _roundWon = false;
        ENEMIESMANAGER.StartSpawning();
        _playerNode->GetComponent<PlayerAudioController>()->ChangeMusicToBattlePhase();
        _playerNode->GetComponent<PlayerAudioController>()->_isInDigPhase = false;
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
    ResetWorm();
    ResetTopSnap();
}

void GameManager::PlayMenuMusic() {
    if (_playerNode == nullptr) _playerNode = NODESMANAGER.getNodeByName("player");

    if (ma_sound_seek_to_pcm_frame(&RESOURCEMANAGER.GetSoundByName("BackgroundMusic")->_sound, 0) != MA_SUCCESS) {
        std::cerr << "Failed to rewind sound: " << RESOURCEMANAGER.GetSoundByName("BackgroundMusic")->_name << std::endl;
    }
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
    WEATHERMANAGER.Reset();
    DisableMouse();
    AUDIOMANAGER.Reset();
    Unpause();
}

void GameManager::LoseGame() {
    HUD.DisableHUD();
    PAGEMANAGER.CloseAllOtherPages(PAGEMANAGER._restartPage);
    EnableMouse();
}

void GameManager::GoToMainMenu() {
    RESOURCEMANAGER.GetSoundByName("BackgroundMusic")->_isRisingUp = false;
    RESOURCEMANAGER.GetSoundByName("BackgroundMusic")->_isFadingAway = false;
    RestartGame();
    EnableMouse();
    _isInMainMenu = true;
    PAGEMANAGER.CloseAllOtherPages(PAGEMANAGER._mainMenuPage);
    PlayMenuMusic();
}

void GameManager::ResetWorm()
{
    std::shared_ptr<Node> worm = NODESMANAGER.getNodeByName("Worm");
    worm->GetComponent<Rotate>()->Reset();
    worm->GetComponent<Disabler>()->Reset();
    worm->SetEnabled(true);
}

void GameManager::ResetTopSnap()
{
    NODESMANAGER.getNodeByName("Rake")->GetComponent<TopLayerSnap>()->Reset();
    NODESMANAGER.getNodeByName("BigShovel")->GetComponent<TopLayerSnap>()->Reset();
    NODESMANAGER.getNodeByName("Mold1")->GetComponent<TopLayerSnap>()->Reset();
    NODESMANAGER.getNodeByName("Mold3(1)")->GetComponent<TopLayerSnap>()->Reset();
    NODESMANAGER.getNodeByName("Mold3(2)")->GetComponent<TopLayerSnap>()->Reset();
    NODESMANAGER.getNodeByName("Mold7")->GetComponent<TopLayerSnap>()->Reset();
    NODESMANAGER.getNodeByName("Bucket")->GetComponent<TopLayerSnap>()->Reset();
}
