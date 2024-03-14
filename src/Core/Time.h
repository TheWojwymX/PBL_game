#pragma once

#define TIME Time::Instance()

#include <GLFW/glfw3.h>

class Time {
public:
    static Time& Instance();

    void Update();

    float GetDeltaTime();

    float GetTime();

private:
    Time(); 

    float deltaTime;
    float lastFrame;
};
