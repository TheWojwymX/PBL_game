#include "Button.h"
#include "Managers/NodesManager.h"

Button::Button(const char* backgroundImagePath, const char* hoverBackgroundImagePath, const char* clickedBackgroundImagePath,
    glm::vec2 position, std::function<void()> onClickFunction)
    : _backgroundImagePath(backgroundImagePath),
    _hoverBackgroundImagePath(hoverBackgroundImagePath),
    _clickedBackgroundImagePath(clickedBackgroundImagePath),
    _position(position),
    _usePosition(true), // Use position
    _onClickFunction(onClickFunction) {}

Button::Button(const char* backgroundImagePath, const char* hoverBackgroundImagePath, const char* clickedBackgroundImagePath,
    glm::vec2 downLeftCorner, glm::vec2 topRightCorner, std::function<void()> onClickFunction)
    : _backgroundImagePath(backgroundImagePath),
    _hoverBackgroundImagePath(hoverBackgroundImagePath),
    _clickedBackgroundImagePath(clickedBackgroundImagePath),
    _corners(downLeftCorner, topRightCorner),
    _usePosition(false), // Use corners
    _onClickFunction(onClickFunction) {}

void Button::Init() {
    if (_usePosition) {
        _backgroundImage.Init(_backgroundImagePath, _position, 0, true, false);
        _hoverBackgroundImage.Init(_hoverBackgroundImagePath, _position, 0, true, false);
        _clickedBackgroundImage.Init(_clickedBackgroundImagePath, _position, 0, true, false);
    }
    else {
        _backgroundImage.Init(_backgroundImagePath, _corners.first, _corners.second, true, false);
        _hoverBackgroundImage.Init(_hoverBackgroundImagePath, _corners.first, _corners.second, true, false);
        _clickedBackgroundImage.Init(_clickedBackgroundImagePath, _corners.first, _corners.second, true, false);
    }

    TEXTRENDERER.Init();

    _hoverBackgroundImage._shouldRender = false;
    _clickedBackgroundImage._shouldRender = false;

    _player = NODESMANAGER.getNodeByName("player");
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
    glm::vec2 mousePos = INPUT.GetMouseFixedPos();
    glm::vec2 downLeftCorner, topRightCorner;

    auto corners = _backgroundImage.GetCorners();
    downLeftCorner = corners.first;
    topRightCorner = corners.second;

    return mousePos.x > downLeftCorner.x && mousePos.x < topRightCorner.x &&
        mousePos.y > downLeftCorner.y && mousePos.y < topRightCorner.y;
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
        PlayHoverSound();
        return;
    }
    else {
        _backgroundImage._shouldRender = true;
        _clickedBackgroundImage._shouldRender = false;
        _hoverBackgroundImage._shouldRender = false;
        _hoverSoundPlayed = false;
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
    PlayClickSound();

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
    _usePosition = true; 
}

void Button::SetCorners(glm::vec2 downLeftCorner, glm::vec2 topRightCorner) {
    _corners = std::make_pair(downLeftCorner, topRightCorner);
    _usePosition = false; 
}

void Button::PlayHoverSound(){
    if(!_hoverSoundPlayed){
        RESOURCEMANAGER.GetSoundByName("Hover")->PlaySoundSim(_player);
        _hoverSoundPlayed = true;
    }
}

void Button::PlayClickSound(){
    RESOURCEMANAGER.GetSoundByName("Click")->PlaySoundSim(_player);
}
