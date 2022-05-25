#pragma once

#include "Node.h"

#include "MyDrawEngine.h"
#include "Shapes.h"
#include "GraphicsModel.h"

#include "UpgradeTreeObserver.h"
#include "EventHandlerObserverOf.h"

class UpgradeTreeUI final : public GraphicsModel, public UpgradeTreeObserver {
public:
    using Ptr = std::shared_ptr<UpgradeTreeUI>;
    using UPtr = std::unique_ptr<UpgradeTreeUI>;
    using WPtr = std::weak_ptr<UpgradeTreeUI>;

    static UPtr create();

    virtual void draw() override;
    std::wstring formatTree(UpgradeTree::NodePtr node, int indent = 0) const;
};
