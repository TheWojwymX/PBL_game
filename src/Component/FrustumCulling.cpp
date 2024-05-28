//
// Created by mbola on 5/8/2024.
//
#include "FrustumCulling.h"

std::vector<FrustumPlane> FrustumCulling::extractFrustumPlanes(const glm::mat4& viewProjectionMatrix) {
    static glm::mat4 lastMatrix;
    static std::vector<FrustumPlane> planes(6);

    if (lastMatrix == viewProjectionMatrix) {
        return planes;
    }

    lastMatrix = viewProjectionMatrix;

    // Extract planes
    for (int i = 0; i < 3; ++i) {
        // Left/right planes
        planes[2 * i].normal = glm::vec3(viewProjectionMatrix[0][3] + viewProjectionMatrix[0][i],
                                         viewProjectionMatrix[1][3] + viewProjectionMatrix[1][i],
                                         viewProjectionMatrix[2][3] + viewProjectionMatrix[2][i]);
        planes[2 * i].distance = viewProjectionMatrix[3][3] + viewProjectionMatrix[3][i];

        planes[2 * i + 1].normal = glm::vec3(viewProjectionMatrix[0][3] - viewProjectionMatrix[0][i],
                                             viewProjectionMatrix[1][3] - viewProjectionMatrix[1][i],
                                             viewProjectionMatrix[2][3] - viewProjectionMatrix[2][i]);
        planes[2 * i + 1].distance = viewProjectionMatrix[3][3] - viewProjectionMatrix[3][i];
    }

    // Normalize the plane normals
    for (auto& plane : planes) {
        float length = glm::length(plane.normal);
        plane.normal /= length;
        plane.distance /= length;
    }

    return planes;
}

bool FrustumCulling::_renderWireframeBB;
bool FrustumCulling::IsInFrustum(const glm::mat4& viewProjectionMatrix, glm::mat4 ctm, const std::vector<FrustumPlane>& frustumPlanes, const shared_ptr<Model>& _model)
{
    glm::vec3 min = GetWorldMinBoundingBox(_model, ctm);
    glm::vec3 max = GetWorldMaxBoundingBox(_model, ctm);

    if(_renderWireframeBB) {RenderBoundingBox(viewProjectionMatrix, min, max);} //Wireframe bounding boxes

    // Check if the bounding box intersects the frustum
    return isBoxInFrustum(frustumPlanes, min, max);
}

void FrustumCulling::RenderBoundingBox(const glm::mat4& viewProjectionMatrix, glm::vec3 min, glm::vec3 max) {
    glm::vec4 corners[8];

    corners[0] = glm::vec4(min.x, min.y, min.z, 1.0f);
    corners[1] = glm::vec4(max.x, min.y, min.z, 1.0f);
    corners[2] = glm::vec4(min.x, max.y, min.z, 1.0f);
    corners[3] = glm::vec4(max.x, max.y, min.z, 1.0f);
    corners[4] = glm::vec4(min.x, min.y, max.z, 1.0f);
    corners[5] = glm::vec4(max.x, min.y, max.z, 1.0f);
    corners[6] = glm::vec4(min.x, max.y, max.z, 1.0f);
    corners[7] = glm::vec4(max.x, max.y, max.z, 1.0f);

    // Indices for lines connecting the corners to form the bounding box
    std::vector<int> indices = {
            0, 1, 1, 3, 3, 2, 2, 0,  // bottom rectangle
            4, 5, 5, 7, 7, 6, 6, 4,  // top rectangle
            0, 4, 1, 5, 2, 6, 3, 7   // vertical lines
    };

    std::vector<glm::vec3> vertices;
    for (auto index : indices) {
        vertices.push_back(corners[index]);
    }

    GLuint vao, vbo;
    glGenVertexArrays(1, &vao);
    glGenBuffers(1, &vbo);

    glBindVertexArray(vao);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(glm::vec3), vertices.data(), GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(glm::vec3), (void*)0);
    glEnableVertexAttribArray(0);

    // Set up your wireframe shader and render the lines
    RESOURCEMANAGER.GetShaderByName("wireframeShader")->use();
    RESOURCEMANAGER.GetShaderByName("wireframeShader")->setMat4("viewProjection", viewProjectionMatrix);

    glDrawArrays(GL_LINES, 0, vertices.size());

    glBindVertexArray(0);
    glDeleteVertexArrays(1, &vao);
    glDeleteBuffers(1, &vbo);
}

bool FrustumCulling::isBoxInFrustum(const std::vector<FrustumPlane>& planes, const glm::vec3& min, const glm::vec3& max) {
    // Check each frustum plane
    for (const auto& plane : planes) {
        // Calculate the box's farthest point along the plane's normal
        glm::vec3 positiveVertex = {
                plane.normal.x >= 0 ? max.x : min.x,
                plane.normal.y >= 0 ? max.y : min.y,
                plane.normal.z >= 0 ? max.z : min.z,
        };

        // If the farthest point is outside the plane, the box is outside the frustum
        if (plane.distanceToPoint(positiveVertex) < 0) {
            return false;
        }
    }

    // If the box is not outside any plane, it's in the frustum
    return true;
}

glm::vec3 FrustumCulling::GetWorldMinBoundingBox(const std::shared_ptr<Model>& model, const glm::mat4& ctm) {
    glm::vec3 localMin = model->GetMinBoundingBox(); // Local min bounding box
    glm::vec3 localMax = model->GetMaxBoundingBox(); // Local max bounding box

    // Define all eight corners of the bounding box in local space
    std::vector<glm::vec3> localCorners = {
            localMin,
            glm::vec3(localMax.x, localMin.y, localMin.z),
            glm::vec3(localMin.x, localMax.y, localMin.z),
            glm::vec3(localMin.x, localMin.y, localMax.z),
            glm::vec3(localMax.x, localMax.y, localMin.z),
            glm::vec3(localMax.x, localMin.y, localMax.z),
            glm::vec3(localMin.x, localMax.y, localMax.z),
            localMax
    };

    // Transform all corners to world space
    std::vector<glm::vec3> worldCorners;
    for (const glm::vec3& corner : localCorners) {
        glm::vec4 transformedCorner = ctm * glm::vec4(corner, 1.0f); // Transform to world space
        worldCorners.push_back(glm::vec3(transformedCorner)); // Convert back to vec3
    }

    // Find the minimum point among transformed corners
    glm::vec3 worldMin = worldCorners[0];
    for (const glm::vec3& point : worldCorners) {
        worldMin.x = std::min(worldMin.x, point.x);
        worldMin.y = std::min(worldMin.y, point.y);
        worldMin.z = std::min(worldMin.z, point.z);
    }

    return worldMin;
}

glm::vec3 FrustumCulling::GetWorldMaxBoundingBox(const std::shared_ptr<Model>& model, const glm::mat4& ctm) {
    glm::vec3 localMin = model->GetMinBoundingBox(); // Local min bounding box
    glm::vec3 localMax = model->GetMaxBoundingBox(); // Local max bounding box

    // Define all eight corners in local space
    std::vector<glm::vec3> localCorners = {
            localMin,
            glm::vec3(localMax.x, localMin.y, localMin.z),
            glm::vec3(localMin.x, localMax.y, localMin.z),
            glm::vec3(localMin.x, localMin.y, localMax.z),
            glm::vec3(localMax.x, localMax.y, localMin.z),
            glm::vec3(localMax.x, localMin.y, localMax.z),
            glm::vec3(localMin.x, localMax.y, localMax.z),
            localMax
    };

    // Transform all corners to world space
    std::vector<glm::vec3> worldCorners;
    for (const glm::vec3 &corner: localCorners) {
        glm::vec4 transformedCorner = ctm * glm::vec4(corner, 1.0f); // Transform to world space
        worldCorners.push_back(glm::vec3(transformedCorner)); // Convert back to vec3
    }

    // Find the maximum point among transformed corners
    glm::vec3 worldMax = worldCorners[0];
    for (const glm::vec3 &point: worldCorners) {
        worldMax.x = std::max(worldMax.x, point.x);
        worldMax.y = std::max(worldMax.y, point.y);
        worldMax.z = std::max(worldMax.z, point.z);
    }

    return worldMax;
}

std::vector<glm::vec3> FrustumCulling::GetRange(const std::shared_ptr<Model>& model, const glm::mat4& ctm) {
    glm::vec3 localMin = model->GetMinBoundingBox(); // Local min bounding box
    glm::vec3 localMax = model->GetMaxBoundingBox(); // Local max bounding box

    // Define all eight corners of the bounding box in local space
    std::vector<glm::vec3> localCorners = {
            localMin,
            glm::vec3(localMax.x, localMin.y, localMin.z),
            glm::vec3(localMin.x, localMax.y, localMin.z),
            glm::vec3(localMin.x, localMin.y, localMax.z),
            glm::vec3(localMax.x, localMax.y, localMin.z),
            glm::vec3(localMax.x, localMin.y, localMax.z),
            glm::vec3(localMin.x, localMax.y, localMax.z),
            localMax
    };

    // Transform all corners to world space
    std::vector<glm::vec3> worldCorners;
    for (const glm::vec3& corner : localCorners) {
        glm::vec4 transformedCorner = ctm * glm::vec4(corner, 1.0f); // Transform to world space
        worldCorners.push_back(glm::vec3(transformedCorner)); // Convert back to vec3
    }

    return worldCorners;
}