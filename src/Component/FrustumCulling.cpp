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
    std::vector<glm::vec3> worldCorners = GetRange(_model, ctm);

    if(_renderWireframeBB)
    {
        RenderBoundingBox(viewProjectionMatrix, worldCorners);
    }

    return isBoxInFrustum(frustumPlanes, worldCorners);
}

void FrustumCulling::RenderBoundingBox(const glm::mat4& viewProjectionMatrix, std::vector<glm::vec3> worldCorners) {
    glm::vec4 corners[8];

    corners[0] = glm::vec4(worldCorners[0],1.0f);
    corners[1] = glm::vec4(worldCorners[1],1.0f);
    corners[2] = glm::vec4(worldCorners[2],1.0f);
    corners[3] = glm::vec4(worldCorners[4],1.0f);
    corners[4] = glm::vec4(worldCorners[3],1.0f);
    corners[5] = glm::vec4(worldCorners[5],1.0f);
    corners[6] = glm::vec4(worldCorners[6],1.0f);
    corners[7] = glm::vec4(worldCorners[7],1.0f);

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

bool FrustumCulling::isBoxInFrustum(const std::vector<FrustumPlane>& planes, std::vector<glm::vec3> worldCorners) {
    glm::vec3 min = worldCorners[0];
    glm::vec3 max = worldCorners[7];

    for (const glm::vec3 &point: worldCorners) {
        max.x = std::max(max.x, point.x);
        max.y = std::max(max.y, point.y);
        max.z = std::max(max.z, point.z);

        min.x = std::min(min.x, point.x);
        min.y = std::min(min.y, point.y);
        min.z = std::min(min.z, point.z);
    }

    for (const auto& plane : planes) {
        glm::vec3 positiveVertex = {
                plane.normal.x >= 0 ? max.x : min.x,
                plane.normal.y >= 0 ? max.y : min.y,
                plane.normal.z >= 0 ? max.z : min.z,
        };

        if (plane.distanceToPoint(positiveVertex) < 0) {
            return false;
        }
    }

    return true;
}

std::vector<glm::vec3> FrustumCulling::GetRange(const std::shared_ptr<Model>& model, const glm::mat4& ctm) {
    glm::vec3 localMin = model->GetMinBoundingBox();
    glm::vec3 localMax = model->GetMaxBoundingBox();

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

    std::vector<glm::vec3> worldCorners;
    for (const glm::vec3& corner : localCorners) {
        glm::vec4 transformedCorner = ctm * glm::vec4(corner, 1.0f); // Transform to world space
        worldCorners.push_back(glm::vec3(transformedCorner)); // Convert back to vec3
    }

    return worldCorners;
}