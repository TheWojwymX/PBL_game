//
// Created by Jacek on 24.03.2024.
//

#ifndef OPENGLGP_SCENEMANAGER_H
#define OPENGLGP_SCENEMANAGER_H

#include <../thirdparty/nlohmann/json.hpp>
#include "Core/Node.h"


class SceneManager {
public:
    static nlohmann::json SerializeRoot(std::shared_ptr<Node> root, int nextNodeId);

    static void SaveToJsonFile(const nlohmann::json &jsonData, const string &filePath);

    static Node LoadFromJsonFile(const string &filePath);
};


#endif //OPENGLGP_SCENEMANAGER_H
