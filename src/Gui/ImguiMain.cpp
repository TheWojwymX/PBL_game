//
// Created by TheWojwymX on 26.03.2024.
//

#include "ImguiMain.h"
#include "imgui.h"
#include "../imgui_impl/imgui_impl_glfw.h"
#include "../imgui_impl/imgui_impl_opengl3.h"

ImguiMain::ImguiMain(GLFWwindow *window, const char *glsl_version)
{
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;
    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init(glsl_version);

    _hierarchy = std::make_shared<ImguiHierarchy>();
    _component = std::make_shared<ImguiInspector>();
}

void ImguiMain::draw()
{
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();

    ImGui::Begin("Info:");
    ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
    ImGui::End();

    //_hierarchy->draw(_rootObject, _selectedObject, this);
    //_component->draw(_selectedObject, this);

}

void ImguiMain::destroy()
{
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();
}

void ImguiMain::SetSelectedObject(std::shared_ptr<Node> GameObject)
{
    _selectedObject = GameObject;
}

std::shared_ptr<Node> ImguiMain::GetSelectedObject()
{
    return _selectedObject;
}

void ImguiMain::SetRoot(std::shared_ptr<Node>& GameObject)
{
    _rootObject = GameObject;

}

void ImguiMain::endDraw()
{
    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}

std::shared_ptr<Node> ImguiMain::GetRoot()
{
    return _rootObject;
}
