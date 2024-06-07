//
// Created by Jacek on 06.04.2024.
//

#include "ImageRenderer.h"

ImageRenderer::ImageRenderer() {}

void ImageRenderer::Init(const char *file, array<float, 32> vertices, bool isAlpha, bool isDynamic) {

    _vertices = vertices;

    unsigned int indices[] = {
            0, 1, 3, // first triangle
            1, 2, 3  // second triangle
    };

    // set up vertex data (and buffer(s)) and configure vertex attributes
    // ------------------------------------------------------------------
    glGenVertexArrays(1, &_VAO);
    glGenBuffers(1, &_VBO);
    glGenBuffers(1, &_EBO);

    glBindVertexArray(_VAO);

    glBindBuffer(GL_ARRAY_BUFFER, _VBO);
    if(!isDynamic){
        glBufferData(GL_ARRAY_BUFFER, sizeof(_vertices), _vertices.data(), GL_STATIC_DRAW);
    }
    else{
        glBufferData(GL_ARRAY_BUFFER, sizeof(_vertices), _vertices.data(), GL_DYNAMIC_DRAW);
    }

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    // position attribute
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    // color attribute
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);
    // texture coord attribute
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
    glEnableVertexAttribArray(2);

    Texture2D texture = Texture2D::loadTextureFromFile(file, isAlpha);
    _textureID = texture.ID;
}

void ImageRenderer::UpdateImage(std::array<float, 32>* vertices) {

    if(!_shouldRender) return;

    // bind Texture
    glBindTexture(GL_TEXTURE_2D, _textureID);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

    if(vertices){
        _vertices = *vertices;
        glBindBuffer(GL_ARRAY_BUFFER, _VBO);
        glBufferSubData(GL_ARRAY_BUFFER, 0, 32 * sizeof(float), _vertices.data());
    }

    // render container
    glm::vec3 color = glm::vec3(1.0,1.0,0.0);
    RESOURCEMANAGER.GetShaderByName("textureShader")->use();
    RESOURCEMANAGER.GetShaderByName("textureShader")->setBool("useColor", false);
    RESOURCEMANAGER.GetShaderByName("textureShader")->setVec3("additionalColor", color);

    glBindVertexArray(_VAO);
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
}

void ImageRenderer::UpdateImage(std::array<float, 32>* vertices, glm::vec3 additionalColor) {

    if(!_shouldRender) return;

    // bind Texture
    glBindTexture(GL_TEXTURE_2D, _textureID);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

    if(vertices){
        _vertices = *vertices;
        glBindBuffer(GL_ARRAY_BUFFER, _VBO);
        glBufferSubData(GL_ARRAY_BUFFER, 0, 32 * sizeof(float), _vertices.data());
    }

    // render container
    glm::vec3 color = glm::vec3(1.0,1.0,0.0);
    RESOURCEMANAGER.GetShaderByName("textureShader")->use();
    RESOURCEMANAGER.GetShaderByName("textureShader")->setBool("useColor", true);
    RESOURCEMANAGER.GetShaderByName("textureShader")->setVec3("additionalColor", additionalColor);

    glBindVertexArray(_VAO);
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
}