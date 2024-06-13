#include "ImageRenderer.h"
#include "Managers/GameManager.h"

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

void ImageRenderer::Init(const char* file, glm::vec3 initialPosition, bool isAlpha, bool isDynamic) {
    // Load the texture
    Texture2D texture = Texture2D::loadTextureFromFile(file, isAlpha);
    _textureID = texture.ID;

    // Retrieve screen dimensions from GameManager
    _screenWidth = static_cast<float>(GAMEMANAGER._screenWidth);
    _screenHeight = static_cast<float>(GAMEMANAGER._screenHeight);

    // Retrieve texture dimensions
    float textureWidth = static_cast<float>(texture.Width);
    float textureHeight = static_cast<float>(texture.Height);

    // Calculate the relative size of the texture on the screen
    float relativeWidth = textureWidth / _screenWidth;
    float relativeHeight = textureHeight / _screenHeight;

    // Calculate the offset to center the texture on the screen
    float xOffset = relativeWidth / 2.0f;
    float yOffset = relativeHeight / 2.0f;

    // Define vertices in normalized device coordinates (NDC)
    std::array<float, 20> screenVertices = {
        // Positions          // Tex Coords
        xOffset, -yOffset,   1.0f, 1.0f, // Bottom-right (y tex coord from 0.0f to 1.0f)
        xOffset,  yOffset,   1.0f, 0.0f, // Top-right (y tex coord from 1.0f to 0.0f)
       -xOffset,  yOffset,   0.0f, 0.0f, // Top-left (y tex coord from 1.0f to 0.0f)
       -xOffset, -yOffset,   0.0f, 1.0f  // Bottom-left (y tex coord from 0.0f to 1.0f)
    };

    // Convert initialPosition from percentage to screen coordinates
    _pixelPosition = glm::vec2(
        initialPosition.x * 0.01f * _screenWidth,   // X% of the screen width
        initialPosition.y * 0.01f * _screenHeight   // Y% of the screen height
    );

    _rotationAngle = initialPosition.z;

    UpdateModel();

    unsigned int indices[] = {
        0, 1, 2, // first triangle
        0, 2, 3  // second triangle
    };

    glGenVertexArrays(1, &_VAO);
    glGenBuffers(1, &_VBO);
    glGenBuffers(1, &_EBO);

    glBindVertexArray(_VAO);

    glBindBuffer(GL_ARRAY_BUFFER, _VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(screenVertices), screenVertices.data(), isDynamic ? GL_DYNAMIC_DRAW : GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    // Position attribute
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    // Texture coordinate attribute
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)(2 * sizeof(float)));
    glEnableVertexAttribArray(1);

    _shader = RESOURCEMANAGER.GetShaderByName("textureShader");
}



void ImageRenderer::UpdateImage(std::array<float, 32>* vertices) {
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
    //model = glm::translate(model, leftCenter);
    model = glm::rotate(model, glm::radians(_rotationAngle), glm::vec3(0.0f, 0.0f, 1.0f));
    //model = glm::translate(model, -leftCenter);

    glm::vec3 color = glm::vec3(1.0, 1.0, 0.0);
    auto shader = RESOURCEMANAGER.GetShaderByName("textureShader");
    shader->use();
    shader->setBool("useColor", false);
    shader->setVec3("additionalColor", color);
    shader->setMat4("model", model);

    glBindVertexArray(_VAO);
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
}

void ImageRenderer::UpdateImage(std::array<float, 32>* vertices, glm::vec3 additionalColor) {
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
    shader->setBool("useColor", true);
    shader->setVec3("additionalColor", additionalColor);
    shader->setMat4("model", model);

    glBindVertexArray(_VAO);
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
}

void ImageRenderer::Render()
{
    // Activate the shader program
    _shader->use();

    // Pass model matrix to shader
    _shader->setMat4("model", _model);

    // Bind the texture
    glActiveTexture(GL_TEXTURE0); // Activate the texture unit first
    glBindTexture(GL_TEXTURE_2D, _textureID); // Bind the texture ID to the texture unit

    // Bind the Vertex Array Object (VAO)
    glBindVertexArray(_VAO);

    // Draw the textured quad
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

    // Unbind the VAO and texture for safety (optional)
    glBindVertexArray(0);
    glBindTexture(GL_TEXTURE_2D, 0);
}

void ImageRenderer::UpdateModel()
{
    _model = glm::mat4(1.0f);  // Initialize as identity matrix

    // Calculate aspect ratio
    float aspectRatio = _screenWidth / _screenHeight;

    // Translate to initial position and scale
    _model = glm::translate(_model, glm::vec3((_pixelPosition.x*2.0f) / _screenWidth, (_pixelPosition.y*2.0f) / _screenHeight, 0.0f));

    // Apply scaling to counteract squish
    _model = glm::scale(_model, glm::vec3(1.0f, aspectRatio, 1.0f));

    // Apply rotation around Z axis
    _model = glm::rotate(_model, glm::radians(_rotationAngle), glm::vec3(0.0f, 0.0f, 1.0f));

    // Scale back to original aspect ratio
    _model = glm::scale(_model, glm::vec3(1.0f, 1.0f / aspectRatio, 1.0f));
}



void ImageRenderer::AddRotation(float value)
{
    _rotationAngle += value;
    UpdateModel();
}
