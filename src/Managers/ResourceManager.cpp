//
// Created by Jacek on 24.03.2024.
//

#include "ResourceManager.h"

ResourceManager& ResourceManager::GetInstance() {
    static ResourceManager instance;
    return instance;
}

void ResourceManager::RegisterModel(const std::string& name, Model* model) {
    modelMap[name] = model;
}

void ResourceManager::RegisterShader(const std::string& name, Shader* shader) {
    shaderMap[name] = shader;
}

Model* ResourceManager::GetModelByName(const std::string& name) const {
    auto it = modelMap.find(name);
    return it != modelMap.end() ? it->second : nullptr;
}

std::string ResourceManager::FindModelName(Model* model) const {
    for (const auto& pair : modelMap) {
        if (pair.second == model) return pair.first;
    }
    return "";
}

std::string ResourceManager::FindShaderName(Shader* shader) const {
    for (const auto& pair : shaderMap) {
        if (pair.second == shader) return pair.first;
    }
    return "";
}

std::shared_ptr<Shader> ResourceManager::getShaderByName(const std::string& name) {
    for (const auto& node : Shaders) {
        if (node->_name == name) {
            return node;
        }
    }
    return nullptr;
}

shared_ptr<Shader> ResourceManager::createShader(const char* vertexPath , const char* fragmentPath, string name) {
    auto shader = std::make_shared<Shader>(vertexPath, fragmentPath, name);
    Shaders.push_back(shader);
    return shader;
}

nlohmann::json ResourceManager::Serialize() {
    nlohmann::json shadersJson = nlohmann::json::array();
    for (auto& shader : Shaders) {
        shadersJson.push_back(shader->Serialize());
    }
    return shadersJson;
}