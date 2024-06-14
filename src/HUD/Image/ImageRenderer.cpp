#include "ImageRenderer.h"
#include "Managers/GameManager.h"

ImageRenderer::ImageRenderer() {}

void ImageRenderer::Init(const char* file, glm::vec2 initialPosition, float initiatRotation, bool isAlpha, bool isDynamic) {
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
    _vertices = {
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

    _rotationAngle = initiatRotation;

    UpdateModel();

    SetupBuffers(isDynamic);

    _shader = RESOURCEMANAGER.GetShaderByName("textureShader");
}

void ImageRenderer::Init(const char* file, glm::vec2 downLeftCorner, glm::vec2 topRightCorner, bool isAlpha, bool isDynamic) {
    // Load the texture
    Texture2D texture = Texture2D::loadTextureFromFile(file, isAlpha);
    _textureID = texture.ID;

    // Retrieve screen dimensions from GameManager
    _screenWidth = static_cast<float>(GAMEMANAGER._screenWidth);
    _screenHeight = static_cast<float>(GAMEMANAGER._screenHeight);

    // Calculate positions in screen coordinates (-1.0f to 1.0f range)
    float x0 = (downLeftCorner.x * 0.02f * _screenWidth) / _screenWidth;
    float y0 = (downLeftCorner.y * 0.02f * _screenHeight) / _screenHeight;
    float x1 = (topRightCorner.x * 0.02f * _screenWidth) / _screenWidth;
    float y1 = (topRightCorner.y * 0.02f * _screenHeight) / _screenHeight;

    // Define vertices in normalized device coordinates (NDC) with appropriate texture coordinates
    _vertices = {
        // Positions          // Tex Coords
        x1, y0,              1.0f, 1.0f, // Bottom-right (y tex coord from 0.0f to 1.0f)
        x1, y1,              1.0f, 0.0f, // Top-right (y tex coord from 1.0f to 0.0f)
        x0, y1,              0.0f, 0.0f, // Top-left (y tex coord from 1.0f to 0.0f)
        x0, y0,              0.0f, 1.0f  // Bottom-left (y tex coord from 0.0f to 1.0f)
    };

    _pixelPosition = glm::vec2(0.0f);
    _rotationAngle = 0.0f;

    // Update model without scaling and rotation for simplicity
    UpdateModel();

    SetupBuffers(isDynamic);

    _shader = RESOURCEMANAGER.GetShaderByName("textureShader");
}

void ImageRenderer::Render()
{
    if (!_shouldRender) return;

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

void ImageRenderer::SetupBuffers(bool isDynamic) {
    unsigned int indices[] = {
    0, 1, 2, // first triangle
    0, 2, 3  // second triangle
    };

    glGenVertexArrays(1, &_VAO);
    glGenBuffers(1, &_VBO);
    glGenBuffers(1, &_EBO);

    glBindVertexArray(_VAO);

    glBindBuffer(GL_ARRAY_BUFFER, _VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(_vertices), _vertices.data(), isDynamic ? GL_DYNAMIC_DRAW : GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    // Position attribute (2D position)
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    // Texture coordinate attribute (2D texture coordinates)
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)(2 * sizeof(float)));
    glEnableVertexAttribArray(1);
}

void ImageRenderer::AddRotation(float value)
{
    _rotationAngle += value;
    UpdateModel();
}

glm::vec2 ImageRenderer::CalculateCenter() {
    // Define vertices in NDC after transformation by _model
    glm::vec4 transformedVertices[4];
    for (int i = 0; i < 4; ++i) {
        glm::vec4 vertex = glm::vec4(_vertices[i * 4], _vertices[i * 4 + 1], 0.0f, 1.0f); // Assuming only x, y coordinates for simplicity
        transformedVertices[i] = _model * vertex;
    }

    // Extract x and y positions from transformed vertices
    float x0 = transformedVertices[0].x; // Bottom-right x
    float y0 = transformedVertices[0].y; // Bottom-right y
    float x1 = transformedVertices[1].x; // Top-right x
    float y1 = transformedVertices[1].y; // Top-right y
    float x2 = transformedVertices[2].x; // Top-left x
    float y2 = transformedVertices[2].y; // Top-left y
    float x3 = transformedVertices[3].x; // Bottom-left x
    float y3 = transformedVertices[3].y; // Bottom-left y

    // Calculate the center
    float centerX = (x0 + x1 + x2 + x3) / 4.0f;
    float centerY = (y0 + y1 + y2 + y3) / 4.0f;

    return glm::vec2(centerX, centerY);
}


std::pair<glm::vec2, glm::vec2> ImageRenderer::GetCorners() {
    // Apply model matrix to vertices
    glm::vec4 transformedVertices[4];
    for (int i = 0; i < 4; ++i) {
        glm::vec4 vertex = glm::vec4(_vertices[i * 4], _vertices[i * 4 + 1], 0.0f, 1.0f); // Assuming only x, y coordinates for simplicity
        transformedVertices[i] = _model * vertex;
    }

    // Calculate bottom-left and top-right corners in screen coordinates
    glm::vec2 downLeftCorner = glm::vec2(transformedVertices[3].x, transformedVertices[3].y); // Bottom-left
    glm::vec2 topRightCorner = glm::vec2(transformedVertices[1].x, transformedVertices[1].y); // Top-right

    // Convert NDC to screen coordinates (optional if already in screen space)
    // Ensure to map to actual screen pixels if needed

    return std::make_pair(downLeftCorner, topRightCorner);
}

void ImageRenderer::SetRotationAngle(float rotationAngle) {
    _rotationAngle = rotationAngle;
    UpdateModel();
}
