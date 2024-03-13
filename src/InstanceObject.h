#pragma once
#ifndef INSTANCEOBJECT_H
#define INSTANCEOBJECT_H

#include "Component.h"


class InstanceObject : public Component {
public:
    InstanceObject(int id, glm::mat4* instanceMatrix) : _ID(id), _instanceMatrix(instanceMatrix) {}

    void UpdateInstanceMatrix() {
        _instanceMatrix[_ID] = _ownerTransform->GetGlobalCTM();
    }

private:
    int _ID;
    glm::mat4* _instanceMatrix;
};

#endif