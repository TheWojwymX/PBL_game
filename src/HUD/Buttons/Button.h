#pragma once

#include "../Image/ImageRenderer.h"
#include "../Text/TextRenderer.h"
#include "Core/Input.h"
#include <functional>

class Button {
public:
    // Constructor using position
    Button(const char* backgroundImagePath, const char* hoverBackgroundImagePath, const char* clickedBackgroundImagePath,
        glm::vec2 position, std::function<void()> onClickFunction);

    // Constructor using corners
    Button(const char* backgroundImagePath, const char* hoverBackgroundImagePath, const char* clickedBackgroundImagePath,
        glm::vec2 downLeftCorner, glm::vec2 topRightCorner, std::function<void()> onClickFunction);

    void Init();
    void Update();
    void CheckClick();
    bool CheckHover();
    void Onclick();

    void SetBackgroundImagePath(const char* path);
    void SetHoverImagePath(const char* path);
    void SetClickedImagePath(const char* path);

    void SetButtonPosition(glm::vec2 position);
    void SetCorners(glm::vec2 downLeftCorner, glm::vec2 topRightCorner);
    void SetText(std::string text, float textSize);

protected:
    float _clickedTimer = 0.0f;
    bool _timerOn = false;

    void AppareanceManager();
    void TimerCount();

private:
    glm::vec2 _position;
    std::pair<glm::vec2, glm::vec2> _corners;
    bool _usePosition;

    const char* _backgroundImagePath;
    const char* _hoverBackgroundImagePath;
    const char* _clickedBackgroundImagePath;

    ImageRenderer _backgroundImage;
    ImageRenderer _hoverBackgroundImage;
    ImageRenderer _clickedBackgroundImage;

    std::string _text = "default";
    float _textSize = 1;

    std::function<void()> _onClickFunction;

    bool _hoverSoundPlayed = false;

    void PlayHoverSound();

    void PlayClickSound();

    shared_ptr<Node> _player;
};
