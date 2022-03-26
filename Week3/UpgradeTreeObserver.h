#pragma once

#include "UpgradeTree.h"

class UpgradeTreeObserver {
private:
	// Has to be a pointer, or it would have to be initialised on construction.
	UpgradeTree* _upgradeTree;

public:
	using WPtr = std::weak_ptr<UpgradeTreeObserver>;

	virtual ~UpgradeTreeObserver() {}

	UpgradeTree& upgradeTree() const;
	void setUpgradeTree(UpgradeTree& upgradeTree); // This will only be called once.
};
