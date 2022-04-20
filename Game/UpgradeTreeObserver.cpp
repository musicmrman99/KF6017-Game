#include "UpgradeTreeObserver.h"

UpgradeTree::Ptr UpgradeTreeObserver::upgradeTree() const { return _upgradeTree.lock(); }
void UpgradeTreeObserver::setUpgradeTree(UpgradeTree::WPtr upgradeTree) { _upgradeTree = upgradeTree; }
