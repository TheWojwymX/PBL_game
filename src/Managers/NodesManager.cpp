#include "NodesManager.h"

#include <utility>

NodesManager& NodesManager::getInstance() {
    static NodesManager instance;
    return instance;
}

std::shared_ptr<Node> NodesManager::getNodeByID(size_t id) {
    if (id < nodes.size()) {
        return nodes[id];
    }
    return nullptr;
}

std::shared_ptr<Node> NodesManager::getNodeByName(const std::string& name) {
    for (const auto& node : nodes) {
        if (node->_name == name) {
            return node;
        }
    }
    return nullptr;
}

std::shared_ptr<Node> NodesManager::createNode(const std::shared_ptr<Node>& parentNode, string nameOfNode) {
    auto node = std::make_shared<Node>();
    node->_id = _nextNodeID;
    addNodeAt(node, _nextNodeID);
    _nextNodeID++;
    parentNode->AddChild(node);
    node->_name = nameOfNode;
    return node;
}

void NodesManager::addNode(const std::shared_ptr<Node>& node) {
    nodes.push_back(node);
}

void NodesManager::addNodeAt(std::shared_ptr<Node> node, size_t position) {
    if (position >= nodes.size()) {
        nodes.resize(position + 1, nullptr);
    }
    nodes[position] = node;
}
