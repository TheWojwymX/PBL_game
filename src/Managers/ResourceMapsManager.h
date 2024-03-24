//
// Created by Jacek on 24.03.2024.
//

#ifndef OPENGLGP_RESOURCEMAPSMANAGER_H
#define OPENGLGP_RESOURCEMAPSMANAGER_H

#include <map>
#include <string>
#include "shader_s.h"
#include "Model.h"

class ResourceMapsManager {
public:
    static ResourceMapsManager& GetInstance();

    void RegisterModel(const std::string& name, Model* model);
    void RegisterShader(const std::string& name, Shader* shader);

    Model* GetModelByName(const std::string& name) const;
    Shader* GetShaderByName(const std::string& name) const;

    std::string FindModelName(Model* model) const;
    std::string FindShaderName(Shader* shader) const;

    ~ResourceMapsManager() = default;

private:
    ResourceMapsManager() = default;

    std::map<std::string, Model*> modelMap;
    std::map<std::string, Shader*> shaderMap;

    // Usunięcie możliwości kopiowania
    ResourceMapsManager(const ResourceMapsManager&) = delete;
    ResourceMapsManager& operator=(const ResourceMapsManager&) = delete;
};


#endif //OPENGLGP_RESOURCEMAPSMANAGER_H
