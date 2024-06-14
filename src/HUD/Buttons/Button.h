#pragma once

#include "../Image/ImageRenderer.h"
#include "../Text/TextRenderer.h"
#include "Core/Input.h"
#include <functional>

class Button {
public:
    Button(const char* backgroundImagePath, const char* hoverBackgroundImagePath, const char* clickedBackgroundImagePath,
        glm::vec2 position, std::function<void()> onClickFunction);

    void Init();
    void Update();
    void CheckClick();
    bool CheckHover();
    void Onclick();

    void SetBackgroundImagePath(const char* path);
    void SetHoverImagePath(const char* path);
    void SetClickedImagePath(const char* path);

    void SetButtonPosition(glm::vec2 position);
    void SetText(std::string text, float textSize);

protected:
    float _clickedTimer = 0.0f;
    bool _timerOn = false;

    void AppareanceManager();
    void TimerCount();

private:
    glm::vec2 _position;
    const char* _backgroundImagePath;
    const char* _hoverBackgroundImagePath;
    const char* _clickedBackgroundImagePath;

    ImageRenderer _backgroundImage;
    ImageRenderer _hoverBackgroundImage;
    ImageRenderer _clickedBackgroundImage;

    std::string _text = "default";
    float _textSize = 1;

    std::function<void()> _onClickFunction;
};
