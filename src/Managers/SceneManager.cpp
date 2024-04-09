//
// Created by Jacek on 24.03.2024.
//

#include "SceneManager.h"

SceneManager &SceneManager::GetInstance() {
    static SceneManager instance;
    return instance;
}

nlohmann::json SceneManager::SerializeRoot(std::shared_ptr<Node> root) {

    nlohmann::json data;

    data["nextNodeId"] = NODESMANAGER._nextNodeID;
    data["nextComponentId"] = COMPONENTSMANAGER._nextComponentID;
    data["root"] = root->Serialize();
    data["resources"] = RESOURCEMANAGER.Serialize();
    return data;
}

void SceneManager::SaveToJsonFile(const nlohmann::json &jsonData, const std::string &filePath) {
    std::ofstream outputFile(filePath);
    if (outputFile.is_open()) {
        outputFile << jsonData.dump(4);
        outputFile.close();
    } else {
        std::cerr << "The file could not be opened for writing: " << filePath << std::endl;
    }
}


std::shared_ptr<Node> SceneManager::LoadFromJsonFile(const string &filePath) {

    std::ifstream inputFile(filePath);
    if (inputFile.is_open()) {

        nlohmann::json jsonFile;
        inputFile >> jsonFile;
        inputFile.close();

        auto rootNodeJson = jsonFile["root"];
        auto rootNode = std::make_shared<Node>();

        auto resourcesNodeJson = jsonFile["resources"];
        RESOURCEMANAGER.Deserialize(resourcesNodeJson);
        rootNode->Deserialize(rootNodeJson);

        COMPONENTSMANAGER._nextComponentID = jsonFile["nextComponentId"].get<int>();
        NODESMANAGER._nextNodeID = jsonFile["nextNodeId"].get<int>();

        COMPONENTSMANAGER.Initiate();

        return rootNode;

    } else {
        std::cerr << "Can't open JSON " << filePath << std::endl;
    }

}
