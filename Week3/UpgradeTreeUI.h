#pragma once

#include "Node.h"

#include "MyDrawEngine.h"
#include "Shapes.h"
#include "GraphicsModel.h"

#include "UpgradeTree.h"

class UpgradeTreeUI : public GraphicsModel {
private:
    const UpgradeTree& tree;

public:
    UpgradeTreeUI(const UpgradeTree& tree) : tree(tree) {}

    virtual void draw() override;
    std::wstring formatTree(UpgradeTree::NodePtr node, int indent = 0) const;
};
