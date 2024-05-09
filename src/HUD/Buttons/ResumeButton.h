//
// Created by Jacek on 09.05.2024.
//

#ifndef SANDBOX_RESUMEBUTTON_H
#define SANDBOX_RESUMEBUTTON_H


#include "Button.h"

class ResumeButton : public Button {
public:
    void Init() override;

    void Update() override;

    void OnClick() override;

    void OnHoover() override;

    void OnReleaseClick() override;
};


#endif //SANDBOX_RESUMEBUTTON_H
