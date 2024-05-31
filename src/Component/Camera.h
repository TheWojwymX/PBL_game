#pragma once

#include <iostream>
#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "Core/Component.h"
#include "Core/Input.h"
#include "Component/FrustumCulling.h"

#include <vector>

class FrustumPlane;
// Default camera values
const float YAW = -90.0f;
const float PITCH = 0.0f;
const float SPEED = 30.0f;
const float SENSITIVITY = 0.1f;
const float ZOOM = 60.0f;

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

    nlohmann::json Serialize() override;
    void Deserialize(const nlohmann::json& jsonData) override;

    void Initiate() override;

    void Input() override;
    void Update() override;

    // returns the view matrix calculated using Euler Angles and the LookAt Matrix
    glm::mat4 GetViewMatrix();

    // Getter for position attribute
    glm::vec3 GetPosition() const { return _position; }
    glm::vec3 GetDirection() const { return _front; }

    // Getter for zoom attribute
    float GetZoom() const { return _zoom; }
    glm::vec3 GetRightVector() const { return _right; }
    glm::vec3 GetFrontVector() const { return _front; }
    glm::vec3 GetUpVector() const { return _up; }
    glm::mat4 GetProjectionMatrix(float screenWidth, float screenHeight) {
        return glm::perspective(glm::radians(_zoom), screenWidth / screenHeight, 0.1f, 1000.0f);
    }

    glm::vec3 LerpPosition(glm::vec3 currentPosition);
    glm::vec3 LerpDirection(glm::vec3 currentPosition);

    glm::vec3 lerp(const glm::vec3 &start, const glm::vec3 &end, float t) {
        return start + t * (end - start);
    }

    vector<FrustumPlane> getFrustumPlanes(){
        return _frustumPlanes;
    }

    glm::mat4 getViewProjectionMatrix(){
        return _viewProjectionMatrix;
    }

    void addToInspector(ImguiMain *imguiMain) override;

    void setScreenWidth(unsigned int screenWidth) {_screenWidth = screenWidth;}
    void setScreenHeight(unsigned int screenHeight) {_screenHeight = screenHeight;}
    unsigned int getScreenWidth() {return _screenWidth;}
    unsigned int getScreenHeight() {return _screenHeight;}

    glm::vec2 GetRotation() const;
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

    vector<FrustumPlane> _frustumPlanes;
    glm::mat4 _viewProjectionMatrix;

    unsigned int _screenWidth;
    unsigned int _screenHeight;

    // calculates the front vector from the Camera's (updated) Euler Angles
    void UpdateCameraVectors();

    // processes input received from a mouse input system. Expects the offset value in both the x and y direction.
    void ProcessMouseMovement(float xoffset, float yoffset, GLboolean constrainPitch = true);

};