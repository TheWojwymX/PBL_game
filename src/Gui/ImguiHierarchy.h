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
    void draw(std::shared_ptr<Node>& root, std::shared_ptr<Node> selectedObject, ImguiMain* imguiMain);
    void addGameObject(ImguiMain* imguiMain);
    void DrawGameObjectHierarchy(std::shared_ptr<Node> root, ImguiMain* imguiMain);
    void MoveNodeUp(std::shared_ptr<Node> parent, std::shared_ptr<Node> node);

        private:
    char* _filePath;
    char* _newObjectName;

};


#endif //OPENGLGP_IMGUIHIERARCHY_H
