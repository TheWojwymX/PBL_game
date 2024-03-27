//
// Created by TheWojwymX on 27.03.2024.
//

#include <iostream>
#include "ImguiInspector.h"
#include "imgui.h"
#include "Core/Component.h"
#include "Component/Block.h"
#include "Core/Node.h"

ImguiInspector::ImguiInspector()
{
    newComponent = 0;
}

void ImguiInspector::draw(std::shared_ptr<Node> selectedObject, ImguiMain* imguiMain)
{
    ImGui::Begin("Inspector");

    static const char* items[]{ " ","Block", "BlockData", "BlockManager",
                                "Camera", "InstanceRenderer", "MeshRenderer",
                                "PlayerController", "StaticBlockController"};

    // Selected Game object will go here:
    ImGui::Text(" Selected Game object ");
    selectedObject->addToInspector(imguiMain);
    //

    ImGui::Text(" ");
    ImGui::Text("--------------------");
    ImGui::Text("New Component");
    ImGui::ListBox(".", &newComponent, items, IM_ARRAYSIZE(items));

    if (newComponent != 0)
    {
        // skip one for transform
        addComponent(selectedObject, (component_type)(newComponent + 1));
        newComponent = 0;
    }
    ImGui::End();
}

void ImguiInspector::addComponent(std::shared_ptr<Node> selectedObject, component_type componentType)
{
    std::shared_ptr<Component> component;

    switch (componentType)
    {
        case BLOCK:
            component = std::make_shared<Block>();
            break;
        default:
            break;
    }

    selectedObject->AddComponent(component);
    // then setup object
}
