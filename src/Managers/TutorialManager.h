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

    shared_ptr<MessagePage> _dialogPage;

    void Init();
    void Update();

    int _actualMessage = 0;
    std::vector<string> _messages{

        "Quick! Call reinforcements [L] using \n"
        "materials!\n",

        "Choose position with [RMB]. \n",

        "There is one more on another side! \n"
        "Relocate the turret with [RMB]. \n"
        "You lose if base gets destroyed. \n",

        "Go look for more materials in the \n"
        "hole. \n",

        "Press [RMB] to throw a flare. \n",

        "Dig up materials using [LMB]. \n",

        "Go back for upgrades. Get Jetpack \n"
        "fuel from materials [R]. Hold \n"
        "[SPACE] to use jetpack. \n",

        "Interact with upgrade stations [F] \n"
        "and check out the upgrades. \n",

        "Exit with [ESC] or [F]. \n",

        "Go and gather materials but \n"
        "remember to come back in time \n"
        "and prepare for defense. \n",

        "Depth and arrow pointing to the \n"
        "hole will help you get back. \n",

    };

    std::vector<string> _specialMessages{

            "Come back, private! You must follow my instructions \n"
            "exactly, otherwise the training will be interrupted \n"
            "and you will be on your own!",

            "As you prefer. The training is interrupted. I hope \n"
            "you know what you're doing. Good luck."
    };

    float _tutorialTimer = 0;

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

    bool _firstWaveSpawned = false;
    bool _secondWaveSpawned = false;

    float _timer = 0.0f;
    bool _isTimerOn = false;

    bool _firstEnemySpawned = false;

    bool _isFreePlay = false;

    void SkipTutorial();
};


#endif //SANDBOX_TUTORIALMANAGER_H
