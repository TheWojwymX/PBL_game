//
// Created by Jacek on 09.05.2024.
//

#include "Button.h"

void Button::Init() {
    _backgroundImage.Init(_backgroundImagePath, _buttonVertices, true, false);
    _hooverBackgroundImage.Init(_hooverBackgroundImagePath, _buttonVertices, true, false);
    _clickedBackgroundImage.Init(_clickedBackgroundImagePath, _buttonVertices, true, false);

    _textRenderer.Init();
}

void Button::Update() {
    _backgroundImage.UpdateImage();
    _hooverBackgroundImage.UpdateImage();
    _clickedBackgroundImage.UpdateImage();
}

void Button::OnClick() {

}

void Button::OnHoover() {

}

void Button::OnReleaseClick() {

}
