#pragma once

#include <iostream>
#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "Core/Component.h"
#include "Core/InputManager.h"

#include <vector>

// Default camera values
const float YAW = -90.0f;
const float PITCH = 0.0f;
const float SPEED = 30.0f;
const float SENSITIVITY = 0.1f;
const float ZOOM = 45.0f;

// Defines several possible options for camera movement. Used as abstraction to stay away from window-system specific input methods
enum CameraMovement {
    FORWARD,
    BACKWARD,
    LEFT,
    RIGHT
};

// An abstract camera class that processes input and calculates the corresponding Euler Angles, Vectors and Matrices for use in OpenGL
class Camera : public Component {
public:
    // constructor with vectors
    Camera(glm::vec3 offset = glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f), float yaw = YAW, float pitch = PITCH);

    void Input() override;
    void Update() override;

    // returns the view matrix calculated using Euler Angles and the LookAt Matrix
    glm::mat4 GetViewMatrix();

    // Getter for position attribute
    glm::vec3 GetPosition() const { return _position; }

    // Getter for zoom attribute
    float GetZoom() const { return _zoom; }
    glm::vec3 GetRightVector() const { return _right; }
    glm::vec3 GetFrontVector() const { return _front; }

private:
    // camera attributes
    glm::vec3 _offset;
    glm::vec3 _position;
    glm::vec3 _front;
    glm::vec3 _up;
    glm::vec3 _right;
    glm::vec3 _worldUp;
    // euler angles
    float _yaw;
    float _pitch;
    // camera options
    float _movementSpeed;
    float _mouseSensitivity;
    float _zoom;
    bool _editMode;

    // calculates the front vector from the Camera's (updated) Euler Angles
    void UpdateCameraVectors();

    // processes input received from a mouse input system. Expects the offset value in both the x and y direction.
    void ProcessMouseMovement(float xoffset, float yoffset, GLboolean constrainPitch = true);
};