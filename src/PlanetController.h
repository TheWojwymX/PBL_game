#pragma once
#ifndef PLANETCONTROLLER_H
#define PLANETCONTROLLER_H

#include <glm/glm.hpp>
#include <TimeController.h>
#include <Node.h>
#include <spdlog/spdlog.h>


#define TIME TimeController::Instance()


class PlanetController {
public:
    PlanetController(glm::vec3 tilt, float rotationSpeed)
        :_tilt(tilt), _rotationSpeed(rotationSpeed) {
    }

    void SetOwnerTransform(Transform* ownerTransform) {
        _ownerTransform = ownerTransform;
        _ownerTransform->SetRotation(_tilt);
    }

    void Update() {
        UpdateRotation();
    }

private:
    Transform* _ownerTransform;
    glm::vec3 _tilt;
    float _rotationSpeed;
    
    void UpdateRotation() {
        if (_rotationSpeed > 0)
        {
            float yawAngle = glm::radians(TIME.GetDeltaTime() * _rotationSpeed);
            glm::quat yawRotation = glm::angleAxis(yawAngle, glm::vec3(0.0f, 1.0f, 0.0f));
            _ownerTransform->AddRotation(yawRotation);         
        }
    }
};
#endif