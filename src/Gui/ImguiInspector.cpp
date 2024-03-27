//
// Created by TheWojwymX on 27.03.2024.
//

#include <iostream>
#include "ImguiInspector.h"
#include "imgui.h"

ImguiInspector::ImguiInspector()
{
    newComponent = 0;
}

void ImguiInspector::draw()
{
    ImGui::Begin("Inspector");

    static const char* items[]{ " ","Block", "BlockData", "BlockManager",
                                "Camera", "InstanceRenderer", "MeshRenderer",
                                "PlayerController", "StaticBlockController"};

    // Selected Game object will go here:
    ImGui::Text(" Selected Game object ");
    //

    ImGui::Text(" ");
    ImGui::Text("--------------------");
    ImGui::Text("New Component");
    ImGui::ListBox(".", &newComponent, items, IM_ARRAYSIZE(items));

    if (newComponent != 0)
    {
        addComponent();
        newComponent = 0;
    }
    ImGui::End();
}

void ImguiInspector::addComponent()
{

}
