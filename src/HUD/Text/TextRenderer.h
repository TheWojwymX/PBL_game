//
// Created by Jacek on 03.04.2024.
//

#define TEXTRENDERER TextRenderer::getInstance()

#ifndef OPENGLGP_TEXTRENDERING_H
#define OPENGLGP_TEXTRENDERING_H


#include <freetype/freetype.h>
#include "shader_s.h"
#include "Managers/ResourceManager.h"
#include <glm/gtc/type_ptr.hpp>



class TextRenderer {

public:

    struct Character {
        unsigned int TextureID; // ID handle of the glyph texture
        glm::ivec2   Size;      // Size of glyph
        glm::ivec2   Bearing;   // Offset from baseline to left/top of glyph
        unsigned int Advance;   // Horizontal offset to advance to next glyph
    };

    std::map<GLchar, Character> Characters;

    unsigned int VAO, VBO;

    ~TextRenderer() = default;

    TextRenderer() = default;

    TextRenderer(const TextRenderer &) = delete;

    TextRenderer &operator=(const TextRenderer &) = delete;

    static TextRenderer &getInstance();

    void Init();

    void RenderText(Shader &shader, std::string text, float x, float y, float scale, glm::vec3 color);

    void PrepareShader();
};




#endif //OPENGLGP_TEXTRENDERING_H
