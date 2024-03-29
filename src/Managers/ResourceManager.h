//
// Created by Jacek on 24.03.2024.
//

#ifndef OPENGLGP_RESOURCEMANAGER_H
#define OPENGLGP_RESOURCEMANAGER_H

#define RESOURCEMANAGER ResourceManager::GetInstance()

#include <map>
#include <string>
#include "shader_s.h"
#include "Model.h"
#include "../../thirdparty/nlohmann/json.hpp"

class ResourceManager {
public:
    static ResourceManager& GetInstance();

    void RegisterModel(const std::string& name, Model* model);
    void RegisterShader(const std::string& name, Shader* shader);

    Model* GetModelByName(const std::string& name) const;

    std::string FindModelName(Model* model) const;
    std::string FindShaderName(Shader* shader) const;

    nlohmann::json Serialize();

    ~ResourceManager() = default;

    shared_ptr<Shader> createShader(const char *vertexPath, const char *fragmentPath, string name);

    shared_ptr<Shader> getShaderByName(const string &name);
private:
    ResourceManager() = default;

    std::map<std::string, Model*> modelMap;
    std::map<std::string, Shader*> shaderMap;

    ResourceManager(const ResourceManager&) = delete;
    ResourceManager& operator=(const ResourceManager&) = delete;

    std::vector<std::shared_ptr<Shader>> Shaders;
    std::vector<std::shared_ptr<Model>> Models;
};


#endif //OPENGLGP_RESOURCEMANAGER_H
