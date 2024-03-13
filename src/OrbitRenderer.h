#pragma once
#ifndef ORBITRENDERER_H
#define ORBITRENDERER_H

#include <glm/glm.hpp>
#include <Transform.h>
#include <Node.h>

class OrbitRenderer {
public:
    OrbitRenderer(float radius)
        :_radius(radius), _orbitVerticesNum(50){
        GenerateOrbit();
    }

    void Render(glm::mat4 parentWorld, Shader& shader) {
        RenderOrbit(parentWorld, shader);
    }

private:
    float _radius;
    int _orbitVerticesNum;
    unsigned int _orbit;


    void GenerateOrbit() {
        int orbitVerticesNum = 50;
        std::vector<glm::vec3> vertices;
        for (int i = 0; i < orbitVerticesNum; ++i) {
            float angle = 2 * glm::pi<float>() * i / orbitVerticesNum;
            float x = _radius * std::cos(angle);
            float z = _radius * std::sin(angle);
            vertices.push_back(glm::vec3(x, 0, z));
        }

        unsigned int VBO;
        glGenVertexArrays(1, &_orbit);
        glGenBuffers(1, &VBO);

        glBindVertexArray(_orbit);

        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(glm::vec3), vertices.data(), GL_STATIC_DRAW);

        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(glm::vec3), (void*)0);
        glEnableVertexAttribArray(0);
     
        glBindBuffer(GL_ARRAY_BUFFER, 0);

        glBindVertexArray(0);
    }

    void RenderOrbit(glm::mat4 parentWorld, Shader& shader) {
        shader.use();
        shader.setMat4("model", parentWorld);

        glBindVertexArray(_orbit);
        glDrawArrays(GL_LINE_LOOP, 0, _orbitVerticesNum);
        glBindVertexArray(0);
    }
};
#endif