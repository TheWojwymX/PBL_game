//
// Created by TheWojwymX on 26.03.2024.
//

#include "ImguiMain.h"
#include "ImguiHierarchy.h"
#include "imgui.h"
#include "../Managers/SceneManager.h"
#include "../imgui_impl/imgui_impl_glfw.h"
#include "../imgui_impl/imgui_impl_opengl3.h"

ImguiHierarchy::ImguiHierarchy()
{
    _newObjectName = new char[100]{"Name"};
    _filePath = new char[100]{"..\\..\\scenes\\test3.json" };
}

void ImguiHierarchy::draw(std::shared_ptr<Node> root, int nextNodeId, std::shared_ptr<Node> selectedObject, ImguiMain *imguiMain)
{
    ImGui::Begin("Hierarchy");
    ImGui::InputText("scene Path", _filePath, 128);
    if (ImGui::Button("SAVE") == true)
    {
        nlohmann::json jsonData = SCENEMANAGER.SerializeRoot (root);
        SCENEMANAGER.SaveToJsonFile(jsonData, _filePath);
    }
    ImGui::SameLine();
    if (ImGui::Button("LOAD") == true)
    {
        SCENEMANAGER.LoadFromJsonFile(_filePath);
    }

    ImGui::InputText("Name:", _newObjectName, 128);
    ImGui::PushStyleColor(ImGuiCol_Button, ImVec4{ 0.1f, 0.8f, 0.1f, 1.0f });
    if (ImGui::Button("Add Node") == true)
    {
        addGameObject(imguiMain);
    }
    ImGui::PopStyleColor();

    ImGui::SameLine();
    ImGui::PushStyleColor(ImGuiCol_Button, ImVec4{ 0.7f, 0.1f, 0.1f, 1.0f });
    if (ImGui::Button("Remove Node") == true)
    {
        root->removeChild(selectedObject);
        selectedObject = root;
    }
    ImGui::PopStyleColor();

    ImGui::SameLine();
    ImGui::PushStyleColor(ImGuiCol_Button, ImVec4{ 0.9f, 0.5f, 0.0f, 1.0f });
    if (ImGui::Button("Move Node") == true)
    {
        // Moving object implementation goes here
    }
    ImGui::PopStyleColor();

    ImGui::Text("=================");
    if (ImGui::TreeNode("Scene Graph"))
    {
        DrawGameObjectHierarchy(root, imguiMain);
        ImGui::TreePop();
    }
    ImGui::End();

}

void ImguiHierarchy::addGameObject(ImguiMain *imguiMain)
{
    std::shared_ptr<Node> root = imguiMain->GetRoot();
    std::shared_ptr<Node> selectedObject = imguiMain->GetSelectedObject();
    auto newGameObject = std::make_shared<Node>();
    newGameObject->_name = _newObjectName;

    if (selectedObject)
    {
        selectedObject->AddChild(newGameObject);
    }
    else
    {
        root->AddChild(newGameObject);
    }


}

void ImguiHierarchy::DrawGameObjectHierarchy(std::shared_ptr<Node> root, ImguiMain* imguiMain) {
    bool isOpen = ImGui::TreeNodeEx(root->_name.c_str(), ImGuiTreeNodeFlags_DefaultOpen);

    // Use the pointer to the node as the identifier
    ImGui::PushID(root.get());
    if (ImGui::IsItemClicked() && !ImGui::IsItemToggledOpen()) {
        // Handle the click event and set the selected object
        imguiMain->SetSelectedObject(root);
    }
    ImGui::PopID();

    if (isOpen) {
        // Render all components of the current node
        for (auto& component : root->getComponents()) {
            component->addToHierarchy();
        }

        // Iterate through all children of the current node
        for (auto& child : root->getChildren()) {
            // Draw the current child node recursively
            DrawGameObjectHierarchy(child, imguiMain);
        }

        ImGui::TreePop(); // Close the parent node's tree node if it was opened
    }
}

