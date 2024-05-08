//
// Created by mbola on 5/8/2024.
//
#pragma once
#include "Managers/ResourceManager.h"
#include "Managers/ComponentsManager.h"

struct FrustumPlane {
    glm::vec3 normal;
    float distance; // Distance from origin

    // Evaluate the plane equation for a point
    float distanceToPoint(const glm::vec3& point) const {
        return glm::dot(normal, point) + distance;
    }
};

class FrustumCulling{
public:
    static std::vector<FrustumPlane> extractFrustumPlanes(const glm::mat4& viewProjectionMatrix);
    static bool IsInFrustum(const glm::mat4& viewProjectionMatrix, glm::mat4 ctm, const std::vector<FrustumPlane>& frustumPlanes, const shared_ptr<Model>& _model);
    static void RenderBoundingBox(const glm::mat4& viewProjectionMatrix, glm::vec3 min, glm::vec3 max);
    static bool isBoxInFrustum(const std::vector<FrustumPlane>& planes, const glm::vec3& min, const glm::vec3& max);
    static glm::vec3 GetWorldMinBoundingBox(const std::shared_ptr<Model>& model, const glm::mat4& ctm);
    static glm::vec3 GetWorldMaxBoundingBox(const std::shared_ptr<Model>& model, const glm::mat4& ctm);

    static bool _renderWireframeBB;
private:
};