#pragma once
#ifndef ANIMATED_TEXTURE_H
#define ANIMATED_TEXTURE_H

#include <vector>
#include <shader_s.h>
#include <texture.h>
#include <Transform.h>

using namespace std;

class AnimatedTexture {
public:
    AnimatedTexture(Shader* shader, vector<Texture2D> textures, Texture2D offTexture, float timeStep, glm::vec3 position, glm::quat rotation, glm::vec3 scale)
        : _shader(shader), _textures(textures), _offTexture(offTexture), _timeStep(timeStep), _position(position), _rotation(rotation), _scale(scale) {
        Init();
    }

    void Update(float dt) {
        if (_animate) {
            _elapsedTime += dt;

            // Check if it's time to switch to the next texture
            if (_elapsedTime >= _timeStep) {
                _elapsedTime -= _timeStep;

                // Switch to the next texture
                int nextTextureIndex = (_currentIndex++) % _textures.size();
                _currentTexture = _textures[nextTextureIndex];
            }
        }
    }

    void Render()
    {
        _shader->use();
        _shader->setMat4("model", _model);
        if (_animate) {
            _currentTexture.Bind();
        }
        else {
            _offTexture.Bind();
        }

        glBindVertexArray(VAO);
        glDrawArrays(GL_TRIANGLES, 0, 6);
        glBindVertexArray(0);
    }

    void SetAnimState(bool state) {
        _animate = state;
    }

private:
    glm::vec3 _position;
    glm::quat _rotation;
    glm::vec3 _scale;
    glm::mat4 _model;
    float _elapsedTime = 0.0f;
    Shader* _shader;
    vector<Texture2D> _textures;
    Texture2D _offTexture;
    Texture2D _currentTexture;
    int _currentIndex;
    float _timeStep;
    bool _animate;
    unsigned int VAO;

    void Init()
    {
        _currentTexture = _textures[0];
        _currentIndex = 0;
        _animate = true;

        _model = Transform::CalculateTransformMatrix(_position, _rotation, _scale);

        // set up mesh and attribute properties
        unsigned int VBO;
        float texture_quad[] = {
            -0.5f, 0.5f, 0.0f, 1.0f,
            0.5f, -0.5f, 1.0f, 0.0f,
            -0.5f, -0.5f, 0.0f, 0.0f,

            -0.5f, 0.5f, 0.0f, 1.0f,
            0.5f, 0.5f, 1.0f, 1.0f,
            0.5f, -0.5f, 1.0f, 0.0f
        };
        glGenVertexArrays(1, &VAO);
        glGenBuffers(1, &VBO);
        glBindVertexArray(VAO);
        // fill mesh buffer
        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glBufferData(GL_ARRAY_BUFFER, sizeof(texture_quad), texture_quad, GL_STATIC_DRAW);
        // set mesh attributes
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0);
        glBindVertexArray(0);
    }
};
#endif