//
// Created by Jacek on 09.05.2024.
//

#include "Button.h"

void Button::Init() {
    _backgroundImage.Init(_backgroundImagePath, _buttonVertices, true, false);
    _hoverBackgroundImage.Init(_hoverBackgroundImagePath, _buttonVertices, true, false);
    _clickedBackgroundImage.Init(_clickedBackgroundImagePath, _buttonVertices, true, false);

    TEXTRENDERER.Init();

    _hoverBackgroundImage._shouldRender = false;
    _clickedBackgroundImage._shouldRender = false;
}

void Button::Update() {
    _backgroundImage.UpdateImage(nullptr, nullptr);
    _hoverBackgroundImage.UpdateImage(nullptr, nullptr);
    _clickedBackgroundImage.UpdateImage(nullptr, nullptr);

    TEXTRENDERER.RenderTextCentered(_text, (_buttonVertices[0] + _buttonVertices[16]) / 2.0f, (_buttonVertices[1] + _buttonVertices[17]) / 2.0f, _textSize, glm::vec3(1.0f, 1.0f, 1.0f));

    TimerCount();
    AppareanceManager();
    CheckHover();
    CheckClick();
}

void Button::CheckClick() {
    if(CheckHover() && INPUT.IsMousePressed(0)){
        Onclick();
        _timerOn = true;
    }
}

bool Button::CheckHover() {
    float leftX = _buttonVertices[16];
    float rightX = _buttonVertices[0];
    float bottomY = _buttonVertices[1];
    float topY = _buttonVertices[9];

    glm::vec2 mousePos = INPUT.GetMouseFixedPos();

    if (mousePos.x > leftX && mousePos.x < rightX && mousePos.y > bottomY && mousePos.y < topY) {
        return true;
    } else {
        return false;
    }
}

void Button::AppareanceManager(){
    if(_timerOn){
        _backgroundImage._shouldRender = false;
        _clickedBackgroundImage._shouldRender = true;
        _hoverBackgroundImage._shouldRender = false;
        return;
    }
    else if(CheckHover()){
        _backgroundImage._shouldRender = false;
        _clickedBackgroundImage._shouldRender = false;
        _hoverBackgroundImage._shouldRender = true;
        return;
    }
    else{
        _backgroundImage._shouldRender = true;
        _clickedBackgroundImage._shouldRender = false;
        _hoverBackgroundImage._shouldRender = false;
        return;
    }
}

void Button::TimerCount(){
    if(_timerOn){
        _clickedTimer += TIME.GetDeltaTime();
        if(_clickedTimer >= 0.2){
            _timerOn = false;
            _clickedTimer = 0.0f;
        }
    }
}

void Button::Onclick() {

}

void Button::SetVertices(const array<float, 32> &vertices) {
        _buttonVertices = vertices;
}

void Button::SetText(std::string text, float textSize) {
    _text = text;
    _textSize = textSize;
}

void Button::SetBackgroundImagePath(const char *path) {
    _backgroundImagePath = path;
}

void Button::SetHoverImagePath(const char *path) {
    _hoverBackgroundImagePath = path;
}

void Button::SetClickedImagePath(const char *path) {
    _clickedBackgroundImagePath = path;
}
