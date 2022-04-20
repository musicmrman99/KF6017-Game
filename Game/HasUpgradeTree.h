#pragma once

#include "UpgradeTree.h"
#include "UpgradeTreeObserver.h"

// A trait that provides an upgrade tree.
//
// To use this trait, implement buildUpgradeTree(UpgradeTree&), then in
// your class's constructor, initialise the trait with the root upgrade
// in the initialiser list and run initUpgradeTree() in the body.
class HasUpgradeTree {
private:
	UpgradeTree::Ptr _upgradeTree;

public:
	HasUpgradeTree(const Upgrade& upgrade);
	virtual ~HasUpgradeTree();

	void initUpgradeTree();
	virtual void buildUpgradeTree(UpgradeTree& upgradeTree) = 0;

	// Make changing the EventEmitter reflected in the given EventEmitter-dependant object.
	void trackUpgradeTreeObserver(UpgradeTreeObserver::WPtr eventEmitterObserver);

	UpgradeTree::WPtr upgradeTreeWPtr();
	UpgradeTree::Ptr upgradeTreePtr();
	UpgradeTree& upgradeTree();
	// No setter - HasUpgradeTree provides a constant upgrade tree for the life of the object.
};
