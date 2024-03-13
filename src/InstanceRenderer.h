#pragma once
#ifndef INSTANCERENDERER_H
#define INSTANCERENDERER_H

#include "Component.h"
#include <model.h>


class InstanceRenderer : public Component {
public:
    InstanceRenderer(Model* model, int amount, glm::mat4* instanceMatrix, Shader* shader):_model(model),
        _amount(amount),_instanceMatrix(instanceMatrix),_shader(shader) {
        CreateMatrixBuffer();
        SetupInstanceModel();
    }

    void Render(glm::mat4 parentWorld) override {
        _shader->use();
        _model->InstanceDraw(*_shader, _amount);
    }

    void RefreshMatrixBuffer()
    {
        glBindBuffer(GL_ARRAY_BUFFER, _instanceBuffer);
        glBufferSubData(GL_ARRAY_BUFFER, 0, _amount * sizeof(glm::mat4), &_instanceMatrix[0]);
    }

private:
    Model* _model;
    int _amount;
    glm::mat4* _instanceMatrix;
    Shader* _shader;
    unsigned int _instanceBuffer;

    void SetupInstanceModel() {
        for (unsigned int i = 0; i < _model->meshes.size(); i++)
        {
            unsigned int VAO = _model->meshes[i].VAO;
            glBindVertexArray(VAO);
            // set attribute pointers for matrix (4 times vec4)
            glEnableVertexAttribArray(3);
            glVertexAttribPointer(3, 4, GL_FLOAT, GL_FALSE, sizeof(glm::mat4), (void*)0);
            glEnableVertexAttribArray(4);
            glVertexAttribPointer(4, 4, GL_FLOAT, GL_FALSE, sizeof(glm::mat4), (void*)(sizeof(glm::vec4)));
            glEnableVertexAttribArray(5);
            glVertexAttribPointer(5, 4, GL_FLOAT, GL_FALSE, sizeof(glm::mat4), (void*)(2 * sizeof(glm::vec4)));
            glEnableVertexAttribArray(6);
            glVertexAttribPointer(6, 4, GL_FLOAT, GL_FALSE, sizeof(glm::mat4), (void*)(3 * sizeof(glm::vec4)));

            glVertexAttribDivisor(3, 1);
            glVertexAttribDivisor(4, 1);
            glVertexAttribDivisor(5, 1);
            glVertexAttribDivisor(6, 1);

            glBindVertexArray(0);
        }
    }

    void CreateMatrixBuffer()
    {
        glGenBuffers(1, &_instanceBuffer);
        glBindBuffer(GL_ARRAY_BUFFER, _instanceBuffer);
        glBufferData(GL_ARRAY_BUFFER, _amount * sizeof(glm::mat4), &_instanceMatrix[0], GL_STATIC_DRAW);
    }
};

#endif