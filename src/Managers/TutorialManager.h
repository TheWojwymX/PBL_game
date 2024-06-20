//
// Created by Jacek on 25.05.2024.
//

#include "HUD/Pages/MessagePage/MessagePage.h"
#include "Core/Node.h"

#define TUTORIALMANAGER TutorialManager::getInstance()

#ifndef SANDBOX_TUTORIALMANAGER_H
#define SANDBOX_TUTORIALMANAGER_H

class TutorialManager {
public:
    static TutorialManager &getInstance();
    ~TutorialManager() = default;
    TutorialManager() = default;
    TutorialManager(const TutorialManager &) = delete;
    TutorialManager &operator=(const TutorialManager &) = delete;

    void Init();
    void Update();

    int _actualMessage = 0;
    std::vector<string> _messages{

            "QUICK! TAKE RADIO [4] AND CALL SUPPORT [1]!",

            "CHOOSE POSITION WITH [LMB].",

            "THERE IS ONE MORE ON ANOTHER SIDE! MOVE TURRET WITH [RMB]. YOU LOSE IF BASE GETS DESTROYED.",

            "GO LOOK FOR MORE MATERIALS IN THE HOLE.",

            "PRESS [RMB] TO THROW A FLARE.",

            "DIG UP MATERIALS [HOLD LMB].",

            "GO BACK FOR UPGRADES. HOLD [SPACE] TO USE JETPACK.",

            "INTERACT WITH UPGRADE STATIONS [E] AND CHECK OUT THE UPGRADES.",

            "EXIT WITH [ESC] OR [E].",

            "GO GATHER MATERIALS BUT REMEMBER TO COME BACK IN TIME AND DEFEND YOUR BASE.",

            "COMPASS [E] AND DEPTH METER WILL HELP YOU GET BACK.",

    };

    std::vector<string> _specialMessages{

            "TURN BACK OR TUTORIAL WILL BE CANCELED!",

            "As you prefer. The training is interrupted. I hope \n"
            "you know what you're doing. Good luck."
    };

    std::vector<glm::vec3> _messagesPositions{
        glm::vec3 (52,288.5,55),
        glm::vec3(49.15, 288.5, 49.8),

    };

    void DisplayAndChangeMessage();

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
};


#endif //SANDBOX_TUTORIALMANAGER_H
