#include "UpgradeTreeUI.h"

UpgradeTreeUI::UPtr UpgradeTreeUI::create() {
    return UPtr(new UpgradeTreeUI());
}

void UpgradeTreeUI::draw() {
    MyDrawEngine* graphics = MyDrawEngine::GetInstance();
    MyDrawEngine::GetInstance()->WriteText(
        graphics->GetViewport().GetTopLeft(), 
        formatTree(upgradeTree()->getRootUpgrade()).c_str(),
        MyDrawEngine::CYAN
    );
}

std::wstring UpgradeTreeUI::formatTree(UpgradeTree::NodePtr node, int indent) const {
    // Open
    const std::wstring& upgradeName = node->getValue()->upgrade.getName();
    std::wstring purchased = node->getValue()->purchased ? L"Purchased" : L"Not Purchased";
    std::wstring ret = std::wstring(indent, ' ') + upgradeName + L" (" + purchased + L")\n";

    // Children
    auto children = node->getChildren();
    for (const UpgradeTree::NodePtr& subNode : children) {
        ret += formatTree(subNode, indent + 4);
    }

    // Return
    return ret;
}
