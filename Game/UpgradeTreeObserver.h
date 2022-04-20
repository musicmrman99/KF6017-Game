#pragma once

#include "UpgradeTree.h"

class UpgradeTreeObserver {
private:
	UpgradeTree::WPtr _upgradeTree;

public:
	using WPtr = std::weak_ptr<UpgradeTreeObserver>;

	virtual ~UpgradeTreeObserver() {}

	UpgradeTree::Ptr upgradeTree() const;
	void setUpgradeTree(UpgradeTree::WPtr upgradeTree); // This will only be called once.
};
