#ifndef OPENGLGP_SKYBOX_H
#define OPENGLGP_SKYBOX_H

#include <glad/glad.h>
#include "stb_image.h"

#include <iostream>
#include <vector>
#include <string>

class Skybox {
public:
    Skybox();
    ~Skybox();
    void init();
    void draw() const;

private:
    unsigned int skyboxVAO, skyboxVBO;
    unsigned int cubemapTexture;
    std::vector<std::string> faces;
    static unsigned int loadCubemap(std::vector<std::string> faces);
};

#endif //OPENGLGP_SKYBOX_H
