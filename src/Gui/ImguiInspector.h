//
// Created by TheWojwymX on 27.03.2024.
//

#ifndef OPENGLGP_IMGUIINSPECTOR_H
#define OPENGLGP_IMGUIINSPECTOR_H

enum component_type {
    BLOCK, BLOCK_MANAGER,
    CAMERA, INSTANCE_RENDERER, MESH_RENDERER,
    PLAYER_CONTROLLER, STATIC_BLOCK_CONTROLLER
};

class Node;
class ImguiMain;

class ImguiInspector {
public:
    ImguiInspector();
    void draw(std::shared_ptr<Node> selectedObject, ImguiMain* imguiMain);

private:
    void addComponent(std::shared_ptr<Node> selectedObject, component_type componentType);
    int newComponent;


};


#endif //OPENGLGP_IMGUIINSPECTOR_H
