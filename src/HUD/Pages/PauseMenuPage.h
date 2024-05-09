//
// Created by Jacek on 09.05.2024.
//

#ifndef SANDBOX_PAUSEMENUPAGE_H
#define SANDBOX_PAUSEMENUPAGE_H


#include "Page.h"
#include "../Buttons/ResumeButton.h"

class PauseMenuPage : public Page {

public:

    ResumeButton _resumeButton;

    void Init() override;

    void Update() override;

};


#endif //SANDBOX_PAUSEMENUPAGE_H
