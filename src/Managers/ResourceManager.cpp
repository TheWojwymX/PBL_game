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
    std::cout << "No shader named: "<< name << std::endl;
    return nullptr;
}

shared_ptr<ComputeShader> ResourceManager::CreateComputeShader(const char *path, string name) {
    auto shader = std::make_shared<ComputeShader>(path, name);
    cShaders.push_back(shader);
    return shader;
}

std::shared_ptr<ComputeShader> ResourceManager::GetComputeShaderByName(const string &name) {
    for (const auto &shader: cShaders) {
        if (shader->_name == name) {
            return shader;
        }
    }
    std::cout << "No such compute shader!" << std::endl;
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
    std::cout << "No such model: " << name << std::endl;
    return nullptr;
}

shared_ptr<Sound> ResourceManager::CreateSound(string name, string path, int id, int soundType, float constVolume) {
    auto sound = std::make_shared<Sound>(name, path, id, static_cast<SoundType>(soundType), constVolume);
    Sounds.push_back(sound);
    return sound;
}

std::shared_ptr<Sound> ResourceManager::GetSoundByName(const string &name) {
    for (const auto &sound: Sounds) {
        if (sound->_name == name) {
            return sound;
        }
    }
    std::cout << "No such sound: " << name << std::endl;
    return nullptr;
}

std::shared_ptr<Sound> ResourceManager::GetSoundByID(int id) {
    for (const auto &sound: Sounds) {
        if (sound->_id == id) {
            return sound;
        }
    }
    std::cout << "No such sound of id: " << id << std::endl;
    return nullptr;
}

nlohmann::json ResourceManager::Serialize() {
    nlohmann::json data;
    nlohmann::json shadersJson = nlohmann::json::array();
    nlohmann::json cShadersJson = nlohmann::json::array();
    nlohmann::json modelsJson = nlohmann::json::array();
    nlohmann::json soundsJson = nlohmann::json::array();

    for (auto &shader: Shaders) {
        shadersJson.push_back(shader->Serialize());
    }

    for (auto &shader: cShaders) {
        cShadersJson.push_back(shader->Serialize());
    }

    for (auto &model: Models) {
        modelsJson.push_back(model->Serialize());
    }

    for (auto &sound: Sounds) {
        soundsJson.push_back(sound->Serialize());
    }

    data["Shaders"] = shadersJson;
    data["cShaders"] = cShadersJson;
    data["Models"] = modelsJson;
    data["Sounds"] = soundsJson;

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

    if (data.contains("cShaders")) {
        for (auto &shader: data["cShaders"]) {
            string path = shader["path"].get<string>();
            CreateComputeShader(path.c_str(), shader["ShaderName"].get<string>());
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
        for (auto &sound: data["Sounds"]) {
            string name = sound["SoundName"].get<string>();
            string path = sound["Path"].get<string>();
            int id = sound["SoundID"].get<int>();
            int soundType = sound["Type"].get<int>();
            float soundVolume = sound["Volume"].get<float>();
            CreateSound(name, path, id, soundType, soundVolume);
        }
    }


}