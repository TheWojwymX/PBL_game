#include "ImageRenderer.h"

ImageRenderer::ImageRenderer() {}

void ImageRenderer::Init(const char *file, std::array<float, 32> vertices, bool isAlpha, bool isDynamic) {
    _vertices = vertices;

    unsigned int indices[] = {
            0, 1, 3, // first triangle
            1, 2, 3  // second triangle
    };

    glGenVertexArrays(1, &_VAO);
    glGenBuffers(1, &_VBO);
    glGenBuffers(1, &_EBO);

    glBindVertexArray(_VAO);

    glBindBuffer(GL_ARRAY_BUFFER, _VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(_vertices), _vertices.data(), isDynamic ? GL_DYNAMIC_DRAW : GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
    glEnableVertexAttribArray(2);

    Texture2D texture = Texture2D::loadTextureFromFile(file, isAlpha);
    _textureID = texture.ID;
}

void ImageRenderer::UpdateImage(std::array<float, 32>* vertices, glm::vec3* additionalColor = nullptr) {
    if (!_shouldRender) return;

    glBindTexture(GL_TEXTURE_2D, _textureID);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

    if (vertices) {
        _vertices = *vertices;
        glBindBuffer(GL_ARRAY_BUFFER, _VBO);
        glBufferSubData(GL_ARRAY_BUFFER, 0, 32 * sizeof(float), _vertices.data());
    }

    glm::vec3 leftCenter = glm::vec3(
            _vertices[24],
            (_vertices[25] + _vertices[9]) / 2.0f,
            0.0f
    );

    glm::mat4 model = glm::mat4(1.0f);
    model = glm::translate(model, leftCenter);
    model = glm::rotate(model, glm::radians(_rotationAngle), glm::vec3(0.0f, 0.0f, 1.0f));
    model = glm::translate(model, -leftCenter);

    auto shader = RESOURCEMANAGER.GetShaderByName("textureShader");
    shader->use();

    if (additionalColor) {
        shader->setBool("useColor", true);
        shader->setVec3("additionalColor", *additionalColor);
    } else {
        glm::vec3 defaultColor = glm::vec3(1.0f, 1.0f, 0.0f);
        shader->setBool("useColor", false);
        shader->setVec3("additionalColor", defaultColor);
    }

    shader->setMat4("model", model);

    glBindVertexArray(_VAO);
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
}

