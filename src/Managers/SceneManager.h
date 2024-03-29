//
// Created by Jacek on 24.03.2024.
//

#ifndef OPENGLGP_SCENEMANAGER_H
#define OPENGLGP_SCENEMANAGER_H

#define SCENEMANAGER SceneManager::GetInstance()

#include <../thirdparty/nlohmann/json.hpp>
#include "Core/Node.h"
#include "NodesManager.h"


class SceneManager {
public:

    static SceneManager& GetInstance();

    nlohmann::json SerializeRoot(std::shared_ptr<Node> root);

    void SaveToJsonFile(const nlohmann::json &jsonData, const string &filePath);

    std::shared_ptr<Node> LoadFromJsonFile(const string &filePath);

    SceneManager(const SceneManager&) = delete;
    SceneManager& operator=(const SceneManager&) = delete;

    ~SceneManager() = default;

private:
    SceneManager() = default;
};


#endif //OPENGLGP_SCENEMANAGER_H
