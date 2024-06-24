//
// Created by TheWojwymX on 27.03.2024.
//
#pragma once

#include <iostream>
#include "ImguiInspector.h"
#include "imgui.h"
#include "Core/Component.h"

#include "Component/Block.h"
#include "Component/MeshRenderer.h"
#include "Component/StaticBlockController.h"
#include "Core/Node.h"

ImguiInspector::ImguiInspector()
{
    newComponent = 0;
}

void ImguiInspector::draw(std::shared_ptr<Node> selectedObject, ImguiMain* imguiMain)
{
    ImGui::Begin("Inspector");

    static const char* items[]{ " ","Block", "BlockManager",
                                "Camera", "InstanceRenderer", "MeshRenderer",
                                "PlayerController", "StaticBlockController"};

    selectedObject->addToInspector(imguiMain);

    ImGui::Dummy(ImVec2(0.0f, 10.0f));

    ImGui::Text("New Component:");
    ImGui::PushItemWidth(-1);
    ImGui::ListBox("##", &newComponent, items, IM_ARRAYSIZE(items));
    ImGui::PopItemWidth();

    if (newComponent != 0)
    {
        addComponent(selectedObject, (component_type)(newComponent - 1));
        newComponent = 0;
    }
    ImGui::End();
}

void ImguiInspector::addComponent(std::shared_ptr<Node> selectedObject, component_type componentType)
{
}
