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

        "So, private, you decided to call for help, huh?\n"
        " Okay, let's start with the basics. See that \n"
        "exclamation point? The threat of an attack coming \n"
        "soon. Approach the sandbags [WSAD and spacebar] \n"
        "and call for backup from [L]. which you need to \n"
        "[RMB] to throw a flare that will summon a \n"
        "paratrooper. Remember that you must connect the \n"
        "soldier in such a way that his barrel is outside \n"
        "the base!",

        "Good job! But watch out, it's not over yet, they're \n"
        "coming from the other side! Approach the gunner's \n"
        "unit and use [RMB] to move the turret to the right \n"
        "place. Additionally, after placing it, click \"F\" to \n"
        "open the upgrade menu. You can upgrade damage - you \n"
        "should be equipped with basic resources.",

        "Great! You repelled the attack! But your mission is \n"
        "only just beginning. See the hole in the middle of \n"
        "the base? It's a mine, jump in and find some \n"
        "resources!",

        "Okay, you're in a cave. Look for materials - metal \n"
        "caps, plastic caps - these are all materials that \n"
        "will be necessary to upgrade your base and buy \n"
        "more turrets. Dig them out by holding [LMB].",

        "That should be enough for now. \n"
        "Return to the hole and climb to the surface.",

        "Well, you can't jump that high. But don't worry, \n"
        "you can use some of the materials you get to turn \n"
        "them into jetpack fuel. Press [] and then hold \n"
        "spacebar to fly out.",

        "Okay, you're on the surface. Now you can use the \n"
        "refueling station to have a full supply in your \n"
        "jetpack just in case. Just stand next to it and \n"
        "wait, the process is completely automatic.",

        "This is practically the end of the training. \n"
        "Oh, I almost forgot - in the upgrade station \n"
        "you can improve your skills, and in the doma \n"
        "station you can buy more life and repair damage. \n"
        "Try to improve your speed now. This time it will \n"
        "be at the company's expense.",

        "Congratulations, you have just completed your training! \n"
        "Your next task is to mine, improve the base and fend \n"
        "off attacks (the time for which is marked in the upper \n"
        "left corner)."
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

    void DisplaySpecialMessage(string message);

    bool WarningSystem(int messageNumber);

    bool _isAfterWarning = false;

    bool _isTutorialEnded = false;

    void SpawnTutorialEnemies(int spawnerIndex);

    bool _firstWaveSpawned = false;
    bool _secondWaveSpawned = false;

    float _timer = 0.0f;
    bool _isTimerOn = false;
};


#endif //SANDBOX_TUTORIALMANAGER_H
