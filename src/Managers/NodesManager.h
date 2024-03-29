#ifndef OPENGLGP_NODESMANAGER_H
#define OPENGLGP_NODESMANAGER_H

#define NODESMANAGER NodesManager::getInstance()

#include "Core/Node.h"
#include <memory>
#include <vector>
#include <string>

class NodesManager {
public:
    static NodesManager &getInstance();

    std::shared_ptr<Node> getNodeByID(size_t id);
    std::shared_ptr<Node> getNodeByName(const std::string& name);
    std::shared_ptr<Node> createNode(const std::shared_ptr<Node>& parentNode, string nameOfNode);

    int _nextNodeID;

    friend class Node;

private:

    void addNode(const std::shared_ptr<Node>& node);
    void addNodeAt(std::shared_ptr<Node> node, size_t position);

    NodesManager() = default;
    ~NodesManager() = default;
    NodesManager(const NodesManager&) = delete;
    NodesManager& operator=(const NodesManager&) = delete;

    std::vector<std::shared_ptr<Node>> nodes;
};

#endif //OPENGLGP_NODESMANAGER_H
