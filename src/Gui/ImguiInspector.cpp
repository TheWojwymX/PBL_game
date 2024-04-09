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

    ImGui::Spacing(); ImGui::Spacing();
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
    std::shared_ptr<Component> component;

    switch (componentType)
    {
        case BLOCK:
            component = std::make_shared<Block>(Block());
            selectedObject->AddComponent(component);
            break;
        case BLOCK_MANAGER:
            component = std::make_shared<BlockManager>(BlockManager());
            selectedObject->AddComponent(component);
            break;
        case CAMERA:
            component = std::make_shared<Camera>(Camera());
            selectedObject->AddComponent(component);
            break;
        case INSTANCE_RENDERER:
//            component = std::make_shared<InstanceRenderer>(InstanceRenderer());
//            selectedObject->AddComponent(component);
            break;
        case MESH_RENDERER:
//            component = std::make_shared<MeshRenderer>(MeshRenderer());
//            selectedObject->AddComponent(component);
            break;
        case PLAYER_CONTROLLER:
//            component = std::make_shared<PlayerController>(PlayerController());
//            selectedObject->AddComponent(component);
            break;
        case STATIC_BLOCK_CONTROLLER:
//            component = std::make_shared<StaticBlockController>(StaticBlockController());
//            selectedObject->AddComponent(component);
            break;
        default:
            break;
    }
}
