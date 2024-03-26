//
// Created by TheWojwymX on 26.03.2024.
//

#include "ImguiHierarchy.h"
#include "imgui.h"
#include "../Managers/SceneManager.h"
#include "../imgui_impl/imgui_impl_glfw.h"
#include "../imgui_impl/imgui_impl_opengl3.h"

ImguiHierarchy::ImguiHierarchy()
{
    newObjectName = new char[100]{"Name"};
    filePath = new char[100]{ "..\\..\\scenes\\test3.json" };
}

void ImguiHierarchy::draw(std::shared_ptr<Node> root, int nextNodeId, std::shared_ptr<Node> selectedObject, ImguiMain *imguiMain)
{
    ImGui::Begin("Hierarchy");
    ImGui::InputText("scene Path", filePath, 128);
    if (ImGui::Button("SAVE") == true)
    {
        nlohmann::json jsonData = SceneManager::SerializeRoot(root, nextNodeId);
        SceneManager::SaveToJsonFile(jsonData, filePath);
    }
    ImGui::SameLine();
    if (ImGui::Button("LOAD") == true)
    {
        SceneManager::LoadFromJsonFile(filePath);
    }

    ImGui::InputText("new Object Name", newObjectName, 128);
    ImGui::PushStyleColor(ImGuiCol_Button, ImVec4{ 0.1f, 0.8f, 0.1f, 1.0f });
    if (ImGui::Button("add object") == true)
    {
        //Adding object implementation
    }
    ImGui::PopStyleColor();

    ImGui::SameLine();
    ImGui::PushStyleColor(ImGuiCol_Button, ImVec4{ 0.7f, 0.1f, 0.1f, 1.0f });
    if (ImGui::Button("remove object") == true)
    {
        // Deleting object goes here
        selectedObject = root;
    }
    ImGui::PopStyleColor();

    ImGui::SameLine();
    ImGui::PushStyleColor(ImGuiCol_Button, ImVec4{ 0.9f, 0.5f, 0.0f, 1.0f });
    if (ImGui::Button("move object  ") == true)
    {
        // Moving object implementation goes here
    }
    ImGui::PopStyleColor();

    ImGui::Text("=================");
    ImGui::End();

}

void ImguiHierarchy::addGameObject(ImguiMain *imguiMain)
{

}
