//
// Created by TheWojwymX on 26.03.2024.
//

#ifndef OPENGLGP_IMGUIMAIN_H
#define OPENGLGP_IMGUIMAIN_H

#include "GLFW/glfw3.h"
#include "ImguiMain.h"
#include "ImguiHierarchy.h"
#include "imgui.h"
#include "../imgui_impl/imgui_impl_glfw.h"
#include "../imgui_impl/imgui_impl_opengl3.h"
#include "memory"

class ImguiMain {
public:
    ImguiMain(GLFWwindow* window, const char* glsl_version);
    void draw(int nextNodeId);
    void endDraw();
    void destroy();
    void SetSelectedObject(std::shared_ptr<Node> GameObject);
    std::shared_ptr<Node> GetSelectedObject();
    void SetRoot(std::shared_ptr<Node> GameObject);

private:
    std::shared_ptr<ImguiHierarchy> hierarchy;
    std::shared_ptr<Node> rootObject;
    std::shared_ptr<Node> selectedObject;



};


#endif //OPENGLGP_IMGUIMAIN_H
