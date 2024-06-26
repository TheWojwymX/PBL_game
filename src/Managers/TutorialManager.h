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

            "QUICK! TAKE RADIO [Q] AND CALL SUPPORT [1]!    <<<PRESS [R] TO SKIP TUTORIAL>>>",

            "CHOOSE POSITION WITH [LMB].",

            "THERE IS ONE MORE ON ANOTHER SIDE! MOVE TURRET WITH [RMB]. YOU LOSE IF BASE GETS DESTROYED.",

            "GO LOOK FOR MORE MATERIALS IN THE HOLE.",

            "PRESS [RMB] TO THROW A FLARE.",

            "DIG UP MATERIALS [HOLD LMB].",

            "GO BACK FOR UPGRADES. HOLD [SPACE] TO USE JETPACK.",

            "INTERACT WITH UPGRADE STATIONS [E] AND CHECK OUT THE UPGRADES. YOU CAN ALSO UPGRADE TURRETS [E].",

            "EXIT WITH [ESC] OR [E].",

            "GO GATHER MATERIALS BUT REMEMBER TO COME BACK IN TIME AND DEFEND YOUR BASE.",

            "COMPASS [E] AND DEPTH METER WILL HELP YOU GET BACK.",

            "COMPASS WILL CLOSE IF YOU ARE EXACTLY BELOW HOLE.",

    };

    std::vector<string> _specialMessages{

            "TURN BACK OR TUTORIAL WILL BE CANCELED!",

            "WATCH OUT THAT BEETLE! YOU NEED SNIPER TO DEAL WITH IT!",

            "LOOK AT THAT WASP! CALL RIFLEMAN TO KILL IT!"
    };

    std::vector<glm::vec3> _messagesPositions{
        glm::vec3 (52,288.5,55),
        glm::vec3(49.15, 288.5, 49.8),

    };

    void DisplayAndChangeMessage();

    BatchRandomGenerator _scaleRand;

    shared_ptr<Node> _player;

    shared_ptr<Node> _paratrooper;

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
