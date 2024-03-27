//
// Created by TheWojwymX on 27.03.2024.
//

#ifndef OPENGLGP_IMGUIINSPECTOR_H
#define OPENGLGP_IMGUIINSPECTOR_H

enum component_type {
    BLOCK, BLOCK_DATA, BLOCK_MANAGER,
    CAMERA, INSTANCE_RENDERER, MESH_RENDERER,
    PLAYER_CONTROLLER, STATIC_BLOCK_CONTROLLER
};

class ImguiInspector {
public:
    ImguiInspector();
    void draw();

private:
    void addComponent();
    int newComponent;


};


#endif //OPENGLGP_IMGUIINSPECTOR_H
