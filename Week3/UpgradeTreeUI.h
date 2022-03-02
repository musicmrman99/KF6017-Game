#pragma once

#include "Node.h"

#include "MyDrawEngine.h"
#include "GraphicsModel.h"

#include "UpgradeTree.h"

class UpgradeTreeUI : public GraphicsModel {
private:
    const UpgradeTree& tree;

public:
    UpgradeTreeUI(const UpgradeTree& tree) : tree(tree) {}

    virtual void draw() override {
        MyDrawEngine* graphics = MyDrawEngine::GetInstance();
        MyDrawEngine::GetInstance()->WriteText(Vector2D(-1000, 700), formatTree(tree.getRootUpgrade()).c_str(), MyDrawEngine::CYAN);
    }

    std::wstring formatTree(UpgradeTree::NodePtr node, int indent = 0) const {
        // Open
        const std::wstring& upgradeName = node->getValue()->upgrade.getName();
        std::wstring purchased = node->getValue()->purchased ? L"Purchased" : L"Not Purchased";
        std::wstring ret = upgradeName + L" (" + purchased + L") {";

        // Children
        auto children = node->getChildren();
        if (!children.empty()) ret += L"\n";
        for (const UpgradeTree::NodePtr& subNode : children) {
            ret += formatTree(subNode);
        }
        if (!children.empty()) ret += L"\n";

        // Close
        ret += L"},\n";

        // Return
        return ret;
    }
};
