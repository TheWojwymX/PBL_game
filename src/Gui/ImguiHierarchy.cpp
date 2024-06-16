//
// Created by TheWojwymX on 26.03.2024.
//

#include "ImguiMain.h"
#include "ImguiHierarchy.h"
#include "imgui.h"
#include "../Managers/SceneManager.h"
#include <algorithm>

ImguiHierarchy::ImguiHierarchy()
{
    _newObjectName = new char[100]{"Name"};
    _filePath = new char[100]{"..\\..\\scenes\\test3.json" };
}

void ImguiHierarchy::draw(std::shared_ptr<Node>& root, std::shared_ptr<Node> selectedObject, ImguiMain *imguiMain)
{
    ImGui::Begin("Hierarchy");
    ImGui::Text("Scene Path:");
    ImGui::PushItemWidth(-1);
    ImGui::InputText("##Path", _filePath, 128);
    ImGui::PopItemWidth();


    if (ImGui::Button("SAVE") == true)
    {
        nlohmann::json jsonData = SCENEMANAGER.SerializeRoot (root);
        SCENEMANAGER.SaveToJsonFile(jsonData, _filePath);
    }
    ImGui::SameLine();
    if (ImGui::Button("LOAD") == true)
    {
        GAMEMANAGER.root = SCENEMANAGER.LoadFromJsonFile(_filePath);
        GAMEMANAGER.root->Init();
    }

    ImGui::Dummy(ImVec2(0.0f, 10.0f));

    ImGui::Text("Name:");
    ImGui::PushItemWidth(-1);
    ImGui::InputText("##Name", _newObjectName, 128);
    ImGui::PopItemWidth();
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
        root->RemoveChild(selectedObject);
        selectedObject = root;
    }
    ImGui::PopStyleColor();

    ImGui::Dummy(ImVec2(0.0f, 10.0f));
    
    DrawGameObjectHierarchy(root, imguiMain);
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
    bool hasChildren = !root->getChildren().empty();

    bool isSelected = (imguiMain->GetSelectedObject() == root);

    ImVec4 textColor = isSelected ? ImVec4(1.0f, 1.0f, 0.0f, 1.0f) : ImVec4(1.0f, 1.0f, 1.0f, 1.0f);

    ImGui::PushStyleColor(ImGuiCol_Text, textColor);
    bool isExpandable = ImGui::TreeNodeEx(root->_name.c_str(), ImGuiTreeNodeFlags_DefaultOpen | (hasChildren ? 0 : ImGuiTreeNodeFlags_Leaf));
    ImGui::PopStyleColor();

    // Use the pointer to the node as the identifier
    ImGui::PushID(root.get());
    if (ImGui::IsItemClicked() && !ImGui::IsItemToggledOpen()) {
        imguiMain->SetSelectedObject(root);
    }
    ImGui::PopID();

    if (isExpandable) {
        if (hasChildren) {
            for (auto& child : root->getChildren()) {
                DrawGameObjectHierarchy(child, imguiMain);
            }
        }
        ImGui::TreePop();
    }
}
