#include "Transform.h"

Transform::Transform() : _position(0.0f), _rotation(1.0f, 0.0f, 0.0f, 0.0f), _scale(1.0f), _ctm(1.0f) {
    UpdateCTM();
}

Transform::Transform(glm::vec3 initialPosition, glm::quat initialRotation, glm::vec3 initialScale)
    : _position(initialPosition), _rotation(initialRotation), _scale(initialScale), _ctm(1.0f) {
    UpdateCTM();
}

void Transform::SetPosition(glm::vec3 newPosition) {
    _isDirty = true;
    _position = newPosition;
}

void Transform::SetPosition(float* array) {
    _isDirty = true;
    _position = glm::vec3(array[0], array[1], array[2]);
}

void Transform::AddPosition(glm::vec3 offset) {
    _position += offset;
    _isDirty = true;
}

void Transform::AddPosition(float offset, int axis)
{
    switch (axis) {
    case 0: // Set position along the X-axis
        _position.x += offset;
        break;
    case 1: // Set position along the Y-axis
        _position.y += offset;
        break;
    case 2: // Set position along the Z-axis
        _position.z += offset;
        break;
    }
    _isDirty = true;
}

void Transform::SetPosition(float newPosition, int axis) {
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

void Transform::SetRotation(glm::quat newRotation) {
    _isDirty = true;
    _rotation = newRotation;
}

void Transform::SetScale(glm::vec3 newScale) {
    _isDirty = true;
    _scale = newScale;
}

void Transform::SetScale(float newScale) {
    _isDirty = true;
    _scale = glm::vec3(newScale);
}

void Transform::SetRotation(glm::vec3 eulerRotation) {
    _isDirty = true;
    _rotation = glm::quat(glm::radians(eulerRotation));
}

void Transform::AddRotation(glm::quat newRotation) {
    _isDirty = true;
    _rotation *= newRotation;
}

void Transform::SetParent(std::shared_ptr<Transform> parent) {
    _parent = parent;
    _isDirty = true;
}

void Transform::LookAt(glm::vec3 direction) {
    glm::vec3 localForward(0.0f, 0.0f, 1.0f);

    if (glm::length(direction) < FLT_EPSILON) {
        // direction is too small to provide meaningful direction
        return;
    }

    // Normalize direction
    direction = glm::normalize(direction);

    if (glm::all(glm::epsilonEqual(localForward, -direction, FLT_EPSILON))) {
        // The direction is exactly opposite to localForward
        _rotation = glm::quat(glm::vec3(0.0f, glm::radians(180.0f), 0.0f));
        return;
    } else if (glm::all(glm::epsilonEqual(localForward, direction, FLT_EPSILON))) {
        // The direction is already where we want to look
        _rotation = glm::quat(); // No rotation needed
        return;
    }

    // Calculate the rotation axis using cross product
    glm::vec3 rotationAxis = glm::cross(localForward, direction);

    // Calculate the rotation angle using dot product
    float dotProduct = glm::dot(localForward, direction);
    float rotationAngle = acos(glm::clamp(dotProduct, -1.0f, 1.0f));

    // Create a quaternion representing the rotation
    _rotation = glm::angleAxis(rotationAngle, glm::normalize(rotationAxis));

    _isDirty = true;
}

void Transform::LookAtPosition(glm::vec3 position) {

    glm::vec3 direction = position - GetPosition();

    glm::vec3 localForward(0.0f, 0.0f, 1.0f);

    if (glm::length(direction) < FLT_EPSILON) {
        // direction is too small to provide meaningful direction
        return;
    }

    // Normalize direction
    direction = glm::normalize(direction);

    if (glm::all(glm::epsilonEqual(localForward, -direction, FLT_EPSILON))) {
        // The direction is exactly opposite to localForward
        _rotation = glm::quat(glm::vec3(0.0f, glm::radians(180.0f), 0.0f));
        return;
    } else if (glm::all(glm::epsilonEqual(localForward, direction, FLT_EPSILON))) {
        // The direction is already where we want to look
        _rotation = glm::quat(); // No rotation needed
        return;
    }

    // Calculate the rotation axis using cross product
    glm::vec3 rotationAxis = glm::cross(localForward, direction);

    // Calculate the rotation angle using dot product
    float dotProduct = glm::dot(localForward, direction);
    float rotationAngle = acos(glm::clamp(dotProduct, -1.0f, 1.0f));

    // Create a quaternion representing the rotation
    _rotation = glm::angleAxis(rotationAngle, glm::normalize(rotationAxis));

    _isDirty = true;
}


glm::vec3 Transform::GetPosition() {
    return _position;
}

glm::vec3 Transform::GetGlobalPosition() {
    return glm::vec3(_globalCTM[3]);
}

glm::quat Transform::GetRotation() {
    return _rotation;
}

glm::vec3 Transform::GetScale() {
    return _scale;
}

glm::mat4 Transform::GetCTM() {
    return _ctm;
}

glm::mat4 Transform::GetGlobalCTM() {
    return _globalCTM;
}

std::shared_ptr<Transform> Transform::GetParent() {
    return _parent;
}

bool Transform::IsDirty() {
    return _isDirty;
}

void Transform::UpdateGlobalCTM() {
    UpdateCTM();
    _globalCTM = _parent->GetCTM() * _ctm;
}

void Transform::Debug() {
    spdlog::info("Logging position: ({},{},{})", _position.x, _position.y, _position.z);
    spdlog::info("Logging scale: ({},{},{})", _scale.x, _scale.y, _scale.z);
    glm::vec3 tmpRotation = glm::degrees(glm::eulerAngles(_rotation));
    spdlog::info("Logging rotation: ({},{},{})", tmpRotation.x, tmpRotation.y, tmpRotation.z);
}

void Transform::UpdateTransform() {
    if (_isDirty) {
        if (_parent != nullptr) {
            _parent->UpdateTransform();
        }
        UpdateCTM();
        Combine(_parent ? _parent->GetCTM() : glm::mat4(1.0f));
    }
}

void Transform::UpdateCTM() {
    if (!_isDirty) return;
    glm::mat4 translationMatrix = glm::translate(glm::mat4(1.0f), _position);
    glm::mat4 rotationMatrix = glm::mat4_cast(_rotation);
    glm::mat4 scaleMatrix = glm::scale(glm::mat4(1.0f), _scale);
    _ctm = translationMatrix * rotationMatrix * scaleMatrix;
    _isDirty = false;
}

glm::mat4 Transform::GetMatrix() {
    glm::mat4 translationMatrix = glm::translate(glm::mat4(1.0f), _position);
    glm::mat4 rotationMatrix = glm::mat4_cast(_rotation);
    glm::mat4 scaleMatrix = glm::scale(glm::mat4(1.0f), _scale);
    return translationMatrix * rotationMatrix * scaleMatrix;
}

glm::mat4 Transform::Combine(glm::mat4 parentCTM) {
    UpdateCTM();
    _globalCTM = parentCTM * _ctm;
    return _globalCTM;
}

std::vector<glm::vec3> Transform::ApplyTransformation(std::vector<glm::vec3>& vertices, glm::mat4& transform)
{
    std::vector<glm::vec3> transformedVertices;
    transformedVertices.reserve(vertices.size());

    for (const auto& vertex : vertices)
    {
        // Convert glm::vec3 to glm::vec4
        glm::vec4 transformedVertex = transform * glm::vec4(vertex, 1.0f);

        // Convert back to glm::vec3
        transformedVertices.push_back(glm::vec3(transformedVertex));
    }

    return transformedVertices;
}

glm::mat4 Transform::CalculateTransformMatrix(glm::vec3 pos, glm::quat rotation, glm::vec3 scale) {
    glm::mat4 translationMatrix = glm::translate(glm::mat4(1.0f), pos);
    glm::mat4 rotationMatrix = glm::mat4_cast(rotation);
    glm::mat4 scaleMatrix = glm::scale(glm::mat4(1.0f), scale);
    return translationMatrix * rotationMatrix * scaleMatrix;
}

glm::mat4 Transform::Origin() {
    glm::mat4 model = glm::mat4(1.0f);
    model = glm::translate(model, glm::vec3(0.0f, 0.0f, 0.0f)); // translate it down so it's at the center of the scene
    model = glm::scale(model, glm::vec3(1.0f, 1.0f, 1.0f));    // it's a bit too big for our scene, so scale it down
    return model;
}

glm::vec3 Transform::MoveTowards(glm::vec3 current, glm::vec3 target, float maxDistanceDelta)
{
    glm::vec3 a = target - current;
    float magnitude = glm::length(a);
    if (magnitude <= maxDistanceDelta || magnitude == 0.0f)
    {
        return target;
    }
    return current + a / magnitude * maxDistanceDelta;
}
