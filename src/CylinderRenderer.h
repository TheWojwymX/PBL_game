#pragma once
#ifndef CYLINDERRENDERER_H
#define CYLINDERRENDERER_H

#include <glm/glm.hpp>
#include <Transform.h>
#include <Node.h>

class CylinderRenderer {
public:
    CylinderRenderer(int sides)
        :_sides(sides) {
        UpdateCylinder();
    }

    void SetCylinderSides(int sides) {
        _sides = sides;
        UpdateCylinder();
    }

    void Render(glm::mat4 parentWorld, Shader& shader) {
        RenderCylinder(parentWorld, shader);
    }

private:
    float _sides;
    unsigned int _cylinder;
    std::vector<unsigned int> _indices;


    void UpdateCylinder() {
        std::vector<glm::vec3> vertices;
        _indices.clear();

        int numVertices = _sides; // Number of vertices in the circles
        float radius = 0.5f;  // Fixed radius
        float height = 1.0f;  // Fixed height

        // Generate top circle vertices and the center vertex
        for (int i = 0; i < numVertices; i++) {
            float angle = 2 * glm::pi<float>() * i / numVertices;
            float x = radius * cos(angle);
            float z = radius * sin(angle);
            vertices.push_back(glm::vec3(x, height * 0.5f, z));
        }

        // Generate bottom circle vertices and the center vertex
        for (int i = 0; i < numVertices; i++) {
            float angle = 2 * glm::pi<float>() * i / numVertices;
            float x = radius * cos(angle);
            float z = radius * sin(angle);
            vertices.push_back(glm::vec3(x, -height * 0.5f, z));
        }

        // Add the center of the top circle
        vertices.push_back(glm::vec3(0.0f, height * 0.5f, 0.0f));
        // Add the center of the bottom circle
        vertices.push_back(glm::vec3(0.0f, -height * 0.5f, 0.0f));

        for (int i = 0; i < numVertices; i++) {
            int next = (i + 1) % numVertices;
            //side rectangle
            _indices.push_back(i);
            _indices.push_back(i + numVertices);
            _indices.push_back(next);
            _indices.push_back(i + numVertices);
            _indices.push_back(next + numVertices);
            _indices.push_back(next);

            //top triangle
            _indices.push_back(i);
            _indices.push_back(next);
            _indices.push_back(numVertices*2);

            //bottom triangle
            _indices.push_back(i + numVertices);
            _indices.push_back(next + numVertices);
            _indices.push_back(numVertices * 2 + 1);
        }


        unsigned int VBO, EBO;
        glGenVertexArrays(1, &_cylinder);
        glGenBuffers(1, &VBO);
        glGenBuffers(1, &EBO);

        // Bind the VAO to set up the following configuration
        glBindVertexArray(_cylinder);

        // Bind the VBO and allocate memory for the vertices
        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(glm::vec3), vertices.data(), GL_STATIC_DRAW);

        // Bind the EBO and allocate memory for the indices
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, _indices.size() * sizeof(unsigned int), _indices.data(), GL_STATIC_DRAW);

        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(glm::vec3), (void*)0);
        glEnableVertexAttribArray(0);

        glBindBuffer(GL_ARRAY_BUFFER, 0);

        glBindVertexArray(0);
    }

    void RenderCylinder(glm::mat4 parentWorld, Shader& shader) {
        shader.use();
        shader.setMat4("model", parentWorld);
        
        glBindVertexArray(_cylinder);
        glDrawElements(GL_TRIANGLES, _indices.size(), GL_UNSIGNED_INT, 0);
    }
};
#endif