//
// Created by Jacek on 24.03.2024.
//

#include "ResourceManager.h"

ResourceManager &ResourceManager::GetInstance() {
    static ResourceManager instance;
    return instance;
}

shared_ptr<Shader> ResourceManager::CreateShader(const char *vertexPath, const char *fragmentPath, string name) {
    auto shader = std::make_shared<Shader>(vertexPath, fragmentPath, name);
    Shaders.push_back(shader);
    return shader;
}

std::shared_ptr<Shader> ResourceManager::GetShaderByName(const string &name) {
    for (const auto &shader: Shaders) {
        if (shader->_name == name) {
            return shader;
        }
    }
    return nullptr;
}

shared_ptr<Model> ResourceManager::CreateModel(string path, string name) {
    auto model = std::make_shared<Model>(path, name);
    Models.push_back(model);
    return model;
}

std::shared_ptr<Model> ResourceManager::GetModelByName(const string &name) {
    for (const auto &model: Models) {
        if (model->_name == name) {
            return model;
        }
    }
    return nullptr;
}

shared_ptr<Sound> ResourceManager::CreateSound(string path, string name) {
    auto sound = std::make_shared<Sound>(path, name);
    Sounds.push_back(sound);
    return sound;
}

/*std::shared_ptr<Sound> ResourceManager::GetSoundByName(const string &name) {
    for (const auto &sound: Models) {
        if (model->_name == name) {
            return model;
        }
    }
    return nullptr;
}*/



nlohmann::json ResourceManager::Serialize() {
    nlohmann::json data;
    nlohmann::json shadersJson = nlohmann::json::array();
    nlohmann::json modelsJson = nlohmann::json::array();

    for (auto &shader: Shaders) {
        shadersJson.push_back(shader->Serialize());
    }

    for (auto &model: Models) {
        modelsJson.push_back(model->Serialize());
    }
    data["Shaders"] = shadersJson;
    data["Models"] = modelsJson;

    return data;
}

void ResourceManager::Deserialize(nlohmann::json data) {
    if (data.contains("Shaders")) {
        for (auto &shader: data["Shaders"]) {
            string vertexPath = shader["vertexPath"].get<string>();
            string fragmentPath = shader["fragmentPath"].get<string>();
            CreateShader(vertexPath.c_str(), fragmentPath.c_str(), shader["ShaderName"].get<string>());
        }
    }

    if (data.contains("Models")) {
        for (auto &model: data["Models"]) {
            string name = model["ModelName"].get<string>();
            string path = model["Path"].get<string>();
            CreateModel(path, name);
        }
    }

    if (data.contains("Sounds")) {
        for (auto &model: data["Sounds"]) {
            string name = model["ModelName"].get<string>();
            string path = model["Path"].get<string>();
            CreateSound(path, name);
        }
    }


}