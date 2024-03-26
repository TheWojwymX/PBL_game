//
// Created by TheWojwymX on 26.03.2024.
//

#ifndef OPENGLGP_IMGUIHIERARCHY_H
#define OPENGLGP_IMGUIHIERARCHY_H

#include <memory>
#include "Core/Node.h"

class ImguiMain;

class ImguiHierarchy {
public:
    ImguiHierarchy();
    void draw(std::shared_ptr<Node> root, int nextNodeId, std::shared_ptr<Node> selectedObject, ImguiMain* imguiMain);
    void addGameObject(ImguiMain* imguiMain);

private:
    char* filePath;
    char* newObjectName;

};


#endif //OPENGLGP_IMGUIHIERARCHY_H
