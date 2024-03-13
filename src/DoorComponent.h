#pragma once
#ifndef DOORCOMPONENT_H
#define DOORCOMPONENT_H

#include <TimeController.h>
#include "Component.h"
#include <Transform.h>

#define TIME TimeController::Instance()

class DoorComponent : public Component {
public:
    DoorComponent(float rotationSpeed = 45.0f, float maxRotation = 90.0f)
        : _rotationSpeed(rotationSpeed), _maxRotation(glm::radians(maxRotation)), _currentRotation(0.0f), _isDoorOpen(false) {
    }

    // Override the Update method from the base Component class
    void Update() override {
        // Your door-specific update logic here
        if (_isDoorOpen) {
            OpenDoor();
        }
        else {
            CloseDoor();
        }
        
    }

    // Function to open the door
    void OpenDoor() {
        float yawAngle = glm::radians(TIME.GetDeltaTime() * -_rotationSpeed);
        glm::quat yawRotation = glm::angleAxis(yawAngle, glm::vec3(0.0f, 1.0f, 0.0f));
        
        // Check if the total rotation exceeds the limit
        if (_currentRotation + glm::degrees(yawAngle) >= -glm::degrees(_maxRotation)) {
            _ownerTransform->AddRotation(yawRotation);
            _currentRotation += glm::degrees(yawAngle);
        }
        else {
            // Limit the rotation to the maximum allowed
            float remainingRotation = -glm::degrees(_maxRotation) - _currentRotation ;
            _ownerTransform->AddRotation(glm::angleAxis(glm::radians(remainingRotation), glm::vec3(0.0f, 1.0f, 0.0f)));
            _currentRotation += remainingRotation;
        }
    }

    // Function to close the door
    void CloseDoor() {
        float yawAngle = glm::radians(TIME.GetDeltaTime() * _rotationSpeed);
        glm::quat yawRotation = glm::angleAxis(yawAngle, glm::vec3(0.0f, 1.0f, 0.0f));

        // Check if the total rotation exceeds the limit
        if (_currentRotation + glm::degrees(yawAngle) <= 0) {
            _ownerTransform->AddRotation(yawRotation);
            _currentRotation += glm::degrees(yawAngle);
        }
        else {
            // Limit the rotation to the maximum allowed
            float remainingRotation = -_currentRotation;
            _ownerTransform->AddRotation(glm::angleAxis(glm::radians(remainingRotation), glm::vec3(0.0f, 1.0f, 0.0f)));
            _currentRotation += remainingRotation;

        }
    }

    // Function to toggle the door state (open/close)
    void SetState(bool state) {
        _isDoorOpen = state;
    }

private:
    float _rotationSpeed;   // Speed at which the door rotates
    float _maxRotation;     // Maximum allowed rotation in radians
    float _currentRotation; // Current total rotation in degrees
    bool _isDoorOpen;       // Flag to track the door state
};

#endif
