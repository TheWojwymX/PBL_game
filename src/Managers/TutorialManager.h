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

        "So, private, you decided to call for help, huh? \n"
        "Okay, let's start with the basics. Press [F] to\n"
        "turn on flashlight. See that tunnel? Go to it \n"
        "[WSAD and spacebar] and continue digging [LMB] \n"
        "for resources. When you find some, dig them out too!",

        "Great, so you've mined your first resources. \n"
        "This should be enough for now. Follow the glow \n"
        "sticks to find the exit.",

        "So you're at the exit, but you can't jump \n"
        "that high? Rest assured, your jetpack should \n"
        "have enough fuel to get you out of here. Hold \n"
        "[space], fly out and land on the surface! \n"
        "But watch out for fuel - if you're downstairs \n"
        "with an empty tank, I won't be able to help you!",

        "Congratulations, you've made it to the surface! \n"
        "Just don't get lazy here! See those exclamation \n"
        "marks? They symbolize that an attack will come \n"
        "soon! Quick, you need to buy support with mined \n"
        "resources! Press [L], set the target position \n"
        "and press [RMB] to confirm! You can also move \n"
        "your turret after landing with [RMB]!",

        "Watch out, it's not over yet! They're coming from\n"
        "the other side! Move the turret with [RMB]. You \n"
        "can also upgrade them using the menu available \n"
        "by pressing [F] on the turret.",

        "Good job! What now you ask? Well, the training is \n"
        "almost complete. I must also inform you that in \n"
        "the upgrade station you can improve your skills, \n"
        "and in the doma station you can buy more life \n"
        "and repair damage. Try to improve your speed \n"
        "now. This time it will be at the company's expense.",

        "Congratulations, you have just completed your training! \n"
        "Your next task is to mine, improve the base and fend \n"
        "off attacks (the time for which is marked in the upper \n"
        "left corner). And one more thing, before going down, \n"
        "remember to refuel at the refuel station. Just stand \n"
        "next to it and wait a few seconds until it is full. \n"
        "However, if you ever end up in the caves without fuel, \n"
        "don't worry - you can constantly convert raw materials \n"
        "into fuel."


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
        glm::vec3 (0,0,0),
        glm::vec3 (1,0,0),
        glm::vec3 (1,0,0),
        glm::vec3 (1,0,0),
    };

    void DisplayAndChangeMessage();

    shared_ptr<Node> _player;

    void DisplaySpecialMessage(string message);

    void WarningSystem();

    bool _isAfterWarning = false;

    bool _isTutorialEnded = true;

    void SpawnTutorialEnemies(int spawnerIndex);

    bool _firstWaveSpawned = false;
    bool _secondWaveSpawned = false;

    float _timer = 0.0f;
    bool _isTimerOn = false;
};


#endif //SANDBOX_TUTORIALMANAGER_H
