//
// Created by Jacek on 24.03.2024.
//

#ifndef OPENGLGP_RESOURCEMANAGER_H
#define OPENGLGP_RESOURCEMANAGER_H

#define RESOURCEMANAGER ResourceManager::GetInstance()

#include <map>
#include <string>
#include "shader_s.h"
#include "ComputeShader.h"
#include "Model.h"
#include "Sound.h"
#include "../../thirdparty/nlohmann/json.hpp"

class ResourceManager {
public:
    static ResourceManager &GetInstance();

    nlohmann::json Serialize();

    void Deserialize(nlohmann::json data);

    ~ResourceManager() = default;

    shared_ptr<Shader> CreateShader(const char *vertexPath, const char *fragmentPath, string name);

    shared_ptr<Shader> GetShaderByName(const string &name);

    shared_ptr<ComputeShader> CreateComputeShader(const char *path, string name);

    shared_ptr<ComputeShader> GetComputeShaderByName(const string &name);

    shared_ptr<Model> CreateModel(string path, string name);

    shared_ptr<Model> GetModelByName(const string &name);

    shared_ptr<Sound> CreateSound(string name, string path, int id, int soundType);

    shared_ptr<Sound> GetSoundByName(const string &name);

    shared_ptr<Sound> GetSoundByID(int id);


private:
    ResourceManager() = default;

    ResourceManager(const ResourceManager &) = delete;

    ResourceManager &operator=(const ResourceManager &) = delete;

    std::vector<std::shared_ptr<Shader>> Shaders;
    std::vector<std::shared_ptr<ComputeShader>> cShaders;
    std::vector<std::shared_ptr<Model>> Models;
    std::vector<std::shared_ptr<Sound>> Sounds;

};


#endif //OPENGLGP_RESOURCEMANAGER_H
