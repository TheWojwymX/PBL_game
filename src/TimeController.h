#pragma once
#ifndef TIMECONTROLLER_H
#define TIMECONTROLLER_H

#include <GLFW/glfw3.h>

class TimeController {
public:
    static TimeController& Instance() {
        static TimeController instance;  // This creates a single instance of the Time class
        return instance;
    }

    void Update() {
        float currentFrame = static_cast<float>(glfwGetTime());
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;
    }

    float GetDeltaTime() {
        return deltaTime;
    }

    float GetTime() {
        return static_cast<float>(glfwGetTime());
    }

private:
    float deltaTime;
    float lastFrame;

    TimeController() : deltaTime(0.0f), lastFrame(0.0f) {}
};
#endif