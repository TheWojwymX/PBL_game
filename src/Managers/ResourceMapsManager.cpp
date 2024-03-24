//
// Created by Jacek on 24.03.2024.
//

#include "ResourceMapsManager.h"

ResourceMapsManager& ResourceMapsManager::GetInstance() {
    static ResourceMapsManager instance;
    return instance;
}

void ResourceMapsManager::RegisterModel(const std::string& name, Model* model) {
    modelMap[name] = model;
}

void ResourceMapsManager::RegisterShader(const std::string& name, Shader* shader) {
    shaderMap[name] = shader;
}

Model* ResourceMapsManager::GetModelByName(const std::string& name) const {
    auto it = modelMap.find(name);
    return it != modelMap.end() ? it->second : nullptr;
}

Shader* ResourceMapsManager::GetShaderByName(const std::string& name) const {
    auto it = shaderMap.find(name);
    return it != shaderMap.end() ? it->second : nullptr;
}

std::string ResourceMapsManager::FindModelName(Model* model) const {
    for (const auto& pair : modelMap) {
        if (pair.second == model) return pair.first;
    }
    return "";
}

std::string ResourceMapsManager::FindShaderName(Shader* shader) const {
    for (const auto& pair : shaderMap) {
        if (pair.second == shader) return pair.first;
    }
    return "";
}
