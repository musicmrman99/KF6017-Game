#include "HasUpgradeTree.h"

HasUpgradeTree::HasUpgradeTree(const Upgrade& root) :
    _upgradeTree(UpgradeTree::Ptr(new UpgradeTree(root))) {
}

HasUpgradeTree::~HasUpgradeTree() {}

void HasUpgradeTree::initUpgradeTree() {
    buildUpgradeTree(*_upgradeTree);
}

void HasUpgradeTree::trackUpgradeTreeObserver(UpgradeTreeObserver::WPtr upgradeTreeObserver) {
    if (auto uto = upgradeTreeObserver.lock()) uto->setUpgradeTree(_upgradeTree);
}

UpgradeTree::WPtr HasUpgradeTree::upgradeTreeWPtr() { return _upgradeTree; }
UpgradeTree::Ptr HasUpgradeTree::upgradeTreePtr() { return _upgradeTree; }
UpgradeTree& HasUpgradeTree::upgradeTree() { return *_upgradeTree; }
