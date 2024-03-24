//
// Created by Jacek on 24.03.2024.
//

#include "SceneManager.h"

nlohmann::json SceneManager::SerializeRoot(std::shared_ptr<Node> root, int nextNodeId){

    nlohmann::json data;

    data["nextNodeId"] = nextNodeId;
    data["root"] = root->Serialize();

    return data;
}

void SceneManager::SaveToJsonFile(const nlohmann::json& jsonData, const std::string& filePath) {
    std::ofstream outputFile(filePath);
    if (outputFile.is_open()) {
        outputFile << jsonData.dump(4);
        outputFile.close();
    } else {
        std::cerr << "The file could not be opened for writing: " << filePath << std::endl;
    }
}