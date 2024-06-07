#include "Skybox.h"

Skybox::Skybox() {
    faces = {
            "res/Skybox/right.png",
            "res/Skybox/left.png",
            "res/Skybox/top.png",
            "res/Skybox/bottom.png",
            "res/Skybox/front.png",
            "res/Skybox/back.png"
    };
}

Skybox::~Skybox() {
    glDeleteVertexArrays(1, &skyboxVAO); // Delete VAO
    glDeleteBuffers(1, &skyboxVBO); // Delete VBO
}

void Skybox::init() {
    // Skybox vertices
    float skyboxVertices[] = {
            // positions
            -1.0f,  1.0f, -1.0f,
            -1.0f, -1.0f, -1.0f,
            1.0f, -1.0f, -1.0f,
            1.0f, -1.0f, -1.0f,
            1.0f,  1.0f, -1.0f,
            -1.0f,  1.0f, -1.0f,

            -1.0f, -1.0f,  1.0f,
            -1.0f, -1.0f, -1.0f,
            -1.0f,  1.0f, -1.0f,
            -1.0f,  1.0f, -1.0f,
            -1.0f,  1.0f,  1.0f,
            -1.0f, -1.0f,  1.0f,

            1.0f, -1.0f, -1.0f,
            1.0f, -1.0f,  1.0f,
            1.0f,  1.0f,  1.0f,
            1.0f,  1.0f,  1.0f,
            1.0f,  1.0f, -1.0f,
            1.0f, -1.0f, -1.0f,

            -1.0f, -1.0f,  1.0f,
            -1.0f,  1.0f,  1.0f,
            1.0f,  1.0f,  1.0f,
            1.0f,  1.0f,  1.0f,
            1.0f, -1.0f,  1.0f,
            -1.0f, -1.0f,  1.0f,

            -1.0f,  1.0f, -1.0f,
            1.0f,  1.0f, -1.0f,
            1.0f,  1.0f,  1.0f,
            1.0f,  1.0f,  1.0f,
            -1.0f,  1.0f,  1.0f,
            -1.0f,  1.0f, -1.0f,

            -1.0f, -1.0f, -1.0f,
            -1.0f, -1.0f,  1.0f,
            1.0f, -1.0f, -1.0f,
            1.0f, -1.0f, -1.0f,
            -1.0f, -1.0f,  1.0f,
            1.0f, -1.0f,  1.0f
    };

    // skybox VAO and VBO
    glGenVertexArrays(1, &skyboxVAO); // Generate VAO
    glGenBuffers(1, &skyboxVBO); // Generate VBO
    glBindVertexArray(skyboxVAO); // Bind VAO
    glBindBuffer(GL_ARRAY_BUFFER, skyboxVBO); // Bind VBO
    glBufferData(GL_ARRAY_BUFFER, sizeof(skyboxVertices), &skyboxVertices, GL_STATIC_DRAW); // Copy vertices to VBO
    glEnableVertexAttribArray(0); // Set vertex attribute pointers
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*) nullptr);

    // Load skybox texture
    cubemapTexture = loadCubemap(faces);
}

void Skybox::draw() const {
    glDepthFunc(GL_LEQUAL);
    // Skybox cube
    glBindVertexArray(skyboxVAO);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_CUBE_MAP, cubemapTexture);
    glDrawArrays(GL_TRIANGLES, 0, 36);
    glBindVertexArray(0);
    glDepthFunc(GL_LESS);
}

unsigned int Skybox::loadCubemap(std::vector<std::string> faces) {
    unsigned int textureID;
    glGenTextures(1, &textureID); // Generate texture ID
    glBindTexture(GL_TEXTURE_CUBE_MAP, textureID); // Bind texture ID

    int width, height, nrChannels;
    for (unsigned int i = 0; i < faces.size(); i++) {
        unsigned char *data = stbi_load(faces[i].c_str(), &width, &height, &nrChannels, 0); // Load image
        if (data) {
            glTexImage2D // Generate texture
                    (
                            GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, // Texture target
                            0, // Mipmap level
                            GL_RGBA, // Format
                            width, // Width
                            height, // Height
                            0, // Always 0
                            GL_RGBA, // Format
                            GL_UNSIGNED_BYTE, // Data type
                            data // Image data
                    );
            stbi_image_free(data); // Free image data
        } else {
            std::cout << "Failed to load cubemap texture: " << faces[i]
                      << std::endl;
            stbi_image_free(data); // Free image data
        }
    }

    // Set texture parameters
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR); // Minification filter
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR); // Magnification filter
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE); // Wrap parameter S
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE); // Wrap parameter T
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE); // Wrap parameter R

    return textureID;
}