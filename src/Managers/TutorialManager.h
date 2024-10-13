//
// Created by Jacek on 25.05.2024.
//

#include "HUD/Pages/MessagePage/MessagePage.h"
#include "Core/Node.h"
#include "BatchRandomGenerator.h"

#define TUTORIALMANAGER TutorialManager::getInstance()

#ifndef SANDBOX_TUTORIALMANAGER_H
#define SANDBOX_TUTORIALMANAGER_H

class TutorialManager {
public:
    static TutorialManager &getInstance();
    ~TutorialManager() = default;
    TutorialManager();
    TutorialManager(const TutorialManager &) = delete;
    TutorialManager &operator=(const TutorialManager &) = delete;

    void Init();
    void Update();

    int _actualMessage = 0;
    std::vector<string> _messages{

            "QUICK! TAKE OUT THE RADIO [Q] AND CALL IN SUPPORT [1]!    <<<PRESS [R] TO SKIP THE TUTORIAL>>>",

            "CHOOSE ITS POSITION WITH [LMB].",

            "THERE IS ONE MORE ON THE OTHER SIDE! MOVE THE TURRET WITH [RMB]. YOU LOSE IF YOUR BASE GETS DESTROYED.",

            "GO LOOK FOR MORE MATERIALS IN THE CAVE.",

            "PRESS [RMB] TO THROW A GLOWSTICK. YOU HAVE INFINITE GLOWSTICKS.",

            "DIG UP MATERIALS [HOLD LMB].",

            "GO BACK FOR UPGRADES. HOLD [SPACE] TO USE YOUR JETPACK.",

            "INTERACT WITH UPGRADE STATIONS [E] AND CHECK OUT THE UPGRADES. YOU CAN ALSO UPGRADE TURRETS [E].",

            "ONE OF THE UPGRADES IS FREE. GET IT AND EXIT WITH [ESC] OR [E].",

            "GO GATHER MATERIALS BUT REMEMBER TO COME BACK IN TIME AND DEFEND YOUR BASE.",

            "THE COMPASS [E] AND DEPTH METER WILL HELP YOU GET BACK.",

            "THE COMPASS WILL CLOSE IF YOU ARE EXACTLY BELOW THE CAVE ENTRANCE.",

    };

    std::vector<string> _specialMessages{

            "TURN BACK OR THE TUTORIAL WILL BE CANCELLED!",

            "WATCH OUT FOR THAT BEETLE! YOU NEED A [SNIPER] TO DEAL WITH IT!",

            "LOOK AT THAT WASP! CALL IN A [RIFLEMAN] TO KILL IT!"
    };

    std::vector<glm::vec3> _messagesPositions{
        glm::vec3 (52,288.5,55),
        glm::vec3(49.15, 288.5, 49.8),

    };

    void DisplayAndChangeMessage();

    BatchRandomGenerator _scaleRand;

    shared_ptr<Node> _player;

    void DisplaySpecialMessage(string message);

    bool WarningSystem(int messageNumber);

    bool _isAfterWarning = false;

    bool _isTutorialEnded = false;

    void SpawnTutorialEnemies(int spawnerIndex);

    float _timer = 0.0f;
    bool _isTimerOn = false;

    bool _firstEnemySpawned = false;

    bool _isFreePlay = false;

    void SkipTutorial();

    void ControllHud(bool crosshair, bool materials, bool hp, bool fuel, bool phase, bool depth);

    void Reset();

    shared_ptr<Node> _tutorialEnemy1;
    shared_ptr<Node> _tutorialEnemy2;

    bool _metBeetle = false;
    bool _metWasp = false;

    void CheckEnemiesEncounter();

    bool _isSpecialMessage = false;
    float _specialMessageTimer = 0.0f;

    void SetFalseTutorialNeededAtMoment();

    bool _hasLanded = false;

    bool _isAfterHalfTimeMessage = false;

    bool _isAfterEndGameInfo = false;

    bool _isAfterEndGameActivation = false;
    bool _isAfterEndGameDisplayed = false;
};


#endif //SANDBOX_TUTORIALMANAGER_H
