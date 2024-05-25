//
// Created by Jacek on 25.05.2024.
//

#ifndef SANDBOX_MESSAGEPAGE_H
#define SANDBOX_MESSAGEPAGE_H

#include "HUD/Pages/Page.h"
#include "HUD/Text/TextRenderer.h"
#include "HUD/Buttons/MessagePage/ContinueMessageButton.h"

class MessagePage : public Page  {
public:

    std::array<float, 32> _backgroundVertices{
            // positions          // colors           // texture coords
            0.9f,  -0.2f, 0.0f,   1.0f, 0.0f, 0.0f,   1.0f, 1.0f, // top right
            0.9f, -0.9f, 0.0f,   0.0f, 1.0f, 0.0f,   1.0f, 0.0f, // bottom right
            -0.6f, -0.9f, 0.0f,   0.0f, 0.0f, 1.0f,   0.0f, 0.0f, // bottom left
            -0.6f,  -0.2f, 0.0f,   1.0f, 1.0f, 0.0f,   0.0f, 1.0f  // top left
    };

    array<float, 32> _generalImageVertices{
            // positions          // colors           // texture coords
            -0.5f,  -0.9f, 0.0f,   1.0f, 0.0f, 0.0f,   1.0f, 1.0f, // top right
            -0.5f, -0.2f, 0.0f,   0.0f, 1.0f, 0.0f,   1.0f, 0.0f, // bottom right
            -0.8f, -0.2f, 0.0f,   0.0f, 0.0f, 1.0f,   0.0f, 0.0f, // bottom left
            -0.8f,  -0.9f, 0.0f,   1.0f, 1.0f, 0.0f,   0.0f, 1.0f  // top left
    };
    const char *_generalImagePath = "../../res/Images/placeholderGeneral.png";
    ImageRenderer _generalImage;

    ContinueMessageButton _continueDialogButton;

    void Init() override;

    void Update() override;

    void SetVertices(const array<float, 32> &vertices) override;

    string _actualText = "default";

};


#endif //SANDBOX_MESSAGEPAGE_H
