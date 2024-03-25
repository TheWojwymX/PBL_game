//
// Created by Jacek on 25.03.2024.
//

#include "Core/Node.h"
#include <memory>
#include <vector>

#ifndef OPENGLGP_NODESMANAGER_H
#define OPENGLGP_NODESMANAGER_H


class NodesManager {
public:
    static NodesManager &getInstance();

    std::shared_ptr<Node> getNodeByID(size_t id) {
        if (id < nodes.size()) {
            return nodes[id];
        }
        return nullptr;
    }

    std::shared_ptr<Node> getNodeByName(const std::string& name) {
        for (const auto& node : nodes) {
            if (node->name == name) {
                return node;
            }
        }
        return nullptr;
    }

    std::shared_ptr<Node> createNode() {
        auto node = std::make_shared<Node>();
        node->id = nodes.size();
        nodes.push_back(node);
        return node;
    }

    void addNode(std::shared_ptr<Node> node) {
        nodes.push_back(node);
    }

    void addNodeAt(std::shared_ptr<Node> node, size_t position) {
        if (position >= nodes.size()) {
            nodes.resize(position + 1, nullptr);
        }
        nodes[position] = node;
    }

    ~NodesManager() = default;

//private:
    NodesManager() = default;

    std::vector<std::shared_ptr<Node>> nodes;

    NodesManager(const NodesManager&) = delete;
    NodesManager& operator=(const NodesManager&) = delete;
};


#endif //OPENGLGP_NODESMANAGER_H
