#include "UpgradeTreeObserver.h"

UpgradeTree& UpgradeTreeObserver::upgradeTree() const { return *_upgradeTree; }

void UpgradeTreeObserver::setUpgradeTree(UpgradeTree& upgradeTree) {
    _upgradeTree = &upgradeTree;
}
