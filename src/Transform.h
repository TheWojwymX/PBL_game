#pragma once
#ifndef TRANSFORM_H
#define TRANSFORM_H

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/quaternion.hpp>
#include <spdlog/spdlog.h>

class Transform {
public:
    Transform() : _position(0.0f), _rotation(1.0f, 0.0f, 0.0f, 0.0f), _scale(1.0f), _ctm(1.0f) {
        UpdateCTM();
    }
    Transform(glm::vec3 initialPosition, glm::quat initialRotation, glm::vec3 initialScale): _position(initialPosition), _rotation(initialRotation), _scale(initialScale), _ctm(1.0f) {
        UpdateCTM();
    }

    // Setters for position, rotation, and scale
    void SetPosition(glm::vec3 newPosition) { _isDirty = true; _position = newPosition; }
    void SetPosition(float* array) { _isDirty = true; _position = glm::vec3(array[0],array[1],array[2]); }
    void SetPosition(float newPosition, int axis) {
        switch (axis) {
        case 0: // Set position along the X-axis
            _position.x = newPosition;
            break;
        case 1: // Set position along the Y-axis
            _position.y = newPosition;
            break;
        case 2: // Set position along the Z-axis
            _position.z = newPosition;
            break;
        }
        _isDirty = true;
    }
    void SetRotation(glm::quat newRotation) { _isDirty = true; _rotation = newRotation; }
    void SetScale(glm::vec3 newScale) { _isDirty = true; _scale = newScale; }
    void SetScale(float newScale) { _isDirty = true; _scale = glm::vec3(newScale); }
    void SetRotation(glm::vec3 eulerRotation) { _isDirty = true; _rotation = glm::quat(glm::radians(eulerRotation)); }
    void AddRotation(glm::quat newRotation) { _isDirty = true; _rotation *= newRotation;}
    void SetParent(Transform* parent) {
        _parent = parent;
    }
    void LookAt(glm::vec3 direction) {
        glm::vec3 localForward(0.0f, 0.0f, 1.0f);

        // Calculate the rotation axis using cross product
        glm::vec3 rotationAxis = glm::cross(localForward, -direction);

        // Calculate the rotation angle using dot product
        float dotProduct = glm::dot(localForward, -direction);
        float rotationAngle = acos(dotProduct);

        // Create a quaternion representing the rotation
        glm::quat rotationQuaternion = glm::angleAxis(rotationAngle, glm::normalize(rotationAxis));

        _rotation = rotationQuaternion;

        _isDirty = true;
    }

    // Getters for position, rotation, and scale
    glm::vec3 GetPosition() { return _position; }
    glm::vec3 GetGlobalPosition() {return glm::vec3(_globalCTM[3]);}
    glm::quat GetRotation() { return _rotation; }
    glm::vec3 GetScale() { return _scale; }
    glm::mat4 GetCTM() {return _ctm;}
    glm::mat4 GetGlobalCTM() { return _globalCTM; }
    Transform* GetParent() {
        return _parent;
    }
    bool IsDirty() {
        return _isDirty;
    }

    void UpdateGlobalCTM() {
        UpdateCTM();
        _globalCTM = _parent->GetCTM() * _ctm;
    }

    void Debug() {
        spdlog::info("Logging position: ({},{},{})", _position.x, _position.y, _position.z);
        spdlog::info("Logging scale: ({},{},{})", _scale.x, _scale.y, _scale.z);
        glm::vec3 tmpRotation = glm::degrees(glm::eulerAngles(_rotation));
        spdlog::info("Logging rotation: ({},{},{})", tmpRotation.x, tmpRotation.y, tmpRotation.z);

    }

    void UpdateTransform() {
        if (_isDirty) {
            if (_parent != NULL) {
                _parent->UpdateTransform();
            }
            UpdateCTM();
            Combine(_parent ? _parent->GetCTM() : glm::mat4(1.0f));
        }
    }

    void UpdateCTM() {
        if (!_isDirty) return;
        glm::mat4 translationMatrix = glm::translate(glm::mat4(1.0f), _position);
        glm::mat4 rotationMatrix = glm::mat4_cast(_rotation);
        glm::mat4 scaleMatrix = glm::scale(glm::mat4(1.0f), _scale);
        _ctm = translationMatrix * rotationMatrix * scaleMatrix;
        _isDirty = false;
    }

    glm::mat4 GetMatrix() {
        glm::mat4 translationMatrix = glm::translate(glm::mat4(1.0f), _position);
        glm::mat4 rotationMatrix = glm::mat4_cast(_rotation);
        glm::mat4 scaleMatrix = glm::scale(glm::mat4(1.0f), _scale);
        return translationMatrix * rotationMatrix * scaleMatrix;
    }

    glm::mat4 Combine(glm::mat4 parentCTM) {
        UpdateCTM();
        _globalCTM = parentCTM * _ctm;
        return _globalCTM;
    }

    static glm::mat4 CalculateTransformMatrix(glm::vec3 pos, glm::quat rotation ,glm::vec3 scale) {
        glm::mat4 translationMatrix = glm::translate(glm::mat4(1.0f), pos);
        glm::mat4 rotationMatrix = glm::mat4_cast(rotation);
        glm::mat4 scaleMatrix = glm::scale(glm::mat4(1.0f), scale);
        return translationMatrix * rotationMatrix * scaleMatrix;
    }

    static glm::mat4 Origin() {
        glm::mat4 model = glm::mat4(1.0f);
        model = glm::translate(model, glm::vec3(0.0f, 0.0f, 0.0f)); // translate it down so it's at the center of the scene
        model = glm::scale(model, glm::vec3(1.0f, 1.0f, 1.0f));	// it's a bit too big for our scene, so scale it down
        return model;
    }

private:
    Transform* _parent = NULL;
    glm::vec3 _position;
    glm::quat _rotation;
    glm::vec3 _scale;
    glm::mat4 _ctm;
    glm::mat4 _globalCTM;
    bool _isDirty = true;
};
#endif