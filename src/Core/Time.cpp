#include "Core/Time.h"

Time& Time::Instance() {
    static Time instance;
    return instance;
}

void Time::Update() {
    float currentFrame = static_cast<float>(glfwGetTime());
    deltaTime = currentFrame - lastFrame;
    lastFrame = currentFrame;
    if (deltaTime > 0.06) deltaTime = 0.06f; // TYMCZASOWE PILNOWANIE KROKU UPDATU
}

float Time::GetDeltaTime() {
    return deltaTime;
}

float Time::GetTime() {
    return static_cast<float>(glfwGetTime());
}

Time::Time() : deltaTime(0.0f), lastFrame(0.0f) {}
