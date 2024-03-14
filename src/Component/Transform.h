#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/quaternion.hpp>
#include <spdlog/spdlog.h>
#include <memory> // Include necessary header for std::shared_ptr

class Transform {
public:
    Transform();
    Transform(glm::vec3 initialPosition, glm::quat initialRotation, glm::vec3 initialScale);

    // Setters for position, rotation, and scale
    void SetPosition(glm::vec3 newPosition);
    void SetPosition(float* array);
    void SetPosition(float newPosition, int axis);
    void SetRotation(glm::quat newRotation);
    void SetScale(glm::vec3 newScale);
    void SetScale(float newScale);
    void SetRotation(glm::vec3 eulerRotation);
    void AddRotation(glm::quat newRotation);
    void SetParent(std::shared_ptr<Transform> parent);
    void LookAt(glm::vec3 direction);

    // Getters for position, rotation, and scale
    glm::vec3 GetPosition();
    glm::vec3 GetGlobalPosition();
    glm::quat GetRotation();
    glm::vec3 GetScale();
    glm::mat4 GetCTM();
    glm::mat4 GetGlobalCTM();
    std::shared_ptr<Transform> GetParent();
    bool IsDirty();

    void UpdateGlobalCTM();
    void Debug();
    void UpdateTransform();
    void UpdateCTM();
    glm::mat4 GetMatrix();
    glm::mat4 Combine(glm::mat4 parentCTM);

    static glm::mat4 CalculateTransformMatrix(glm::vec3 pos, glm::quat rotation, glm::vec3 scale);
    static glm::mat4 Origin();

private:
    std::shared_ptr<Transform> _parent = nullptr;
    glm::vec3 _position;
    glm::quat _rotation;
    glm::vec3 _scale;
    glm::mat4 _ctm;
    glm::mat4 _globalCTM;
    bool _isDirty = true;
};