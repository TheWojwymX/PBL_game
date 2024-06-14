#include "Button.h"

Button::Button(const char* backgroundImagePath, const char* hoverBackgroundImagePath, const char* clickedBackgroundImagePath,
    glm::vec2 position, std::function<void()> onClickFunction)
    : _backgroundImagePath(backgroundImagePath),
    _hoverBackgroundImagePath(hoverBackgroundImagePath),
    _clickedBackgroundImagePath(clickedBackgroundImagePath),
    _position(position),
    _onClickFunction(onClickFunction) {}

void Button::Init() {
    _backgroundImage.Init(_backgroundImagePath, _position, 0, true, false);
    _hoverBackgroundImage.Init(_hoverBackgroundImagePath, _position, 0, true, false);
    _clickedBackgroundImage.Init(_clickedBackgroundImagePath, _position, 0, true, false);

    TEXTRENDERER.Init();

    _hoverBackgroundImage._shouldRender = false;
    _clickedBackgroundImage._shouldRender = false;
}

void Button::Update() {
    _backgroundImage.Render();
    _hoverBackgroundImage.Render();
    _clickedBackgroundImage.Render();

    //glm::vec2 buttonCenter = _backgroundImage.CalculateCenter();
    //TEXTRENDERER.RenderTextCentered(_text, buttonCenter.x, buttonCenter.y, _textSize, glm::vec3(1.0f, 1.0f, 1.0f));

    TimerCount();
    AppareanceManager();
    CheckHover();
    CheckClick();
}

void Button::CheckClick() {
    if (CheckHover() && INPUT.IsMousePressed(0)) {
        Onclick();
        _timerOn = true;
    }
}

bool Button::CheckHover() {
    auto corners = _backgroundImage.GetCorners();
    glm::vec2 downLeftCorner = corners.first;
    glm::vec2 topRightCorner = corners.second;

    glm::vec2 mousePos = INPUT.GetMouseFixedPos();

    if (mousePos.x > downLeftCorner.x && mousePos.x < topRightCorner.x &&
        mousePos.y > downLeftCorner.y && mousePos.y < topRightCorner.y) {
        return true;
    }
    else {
        return false;
    }
}

void Button::AppareanceManager() {
    if (_timerOn) {
        _backgroundImage._shouldRender = false;
        _clickedBackgroundImage._shouldRender = true;
        _hoverBackgroundImage._shouldRender = false;
        return;
    }
    else if (CheckHover()) {
        _backgroundImage._shouldRender = false;
        _clickedBackgroundImage._shouldRender = false;
        _hoverBackgroundImage._shouldRender = true;
        return;
    }
    else {
        _backgroundImage._shouldRender = true;
        _clickedBackgroundImage._shouldRender = false;
        _hoverBackgroundImage._shouldRender = false;
        return;
    }
}

void Button::TimerCount() {
    if (_timerOn) {
        _clickedTimer += TIME.GetDeltaTime();
        if (_clickedTimer >= 0.2) {
            _timerOn = false;
            _clickedTimer = 0.0f;
        }
    }
}

void Button::Onclick() {
    if (_onClickFunction) {
        _onClickFunction();
    }
}

void Button::SetText(std::string text, float textSize) {
    _text = text;
    _textSize = textSize;
}

void Button::SetBackgroundImagePath(const char* path) {
    _backgroundImagePath = path;
}

void Button::SetHoverImagePath(const char* path) {
    _hoverBackgroundImagePath = path;
}

void Button::SetClickedImagePath(const char* path) {
    _clickedBackgroundImagePath = path;
}

void Button::SetButtonPosition(glm::vec2 position) {
    _position = position;
}
