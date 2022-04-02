#include "UpgradeTreeUI.h"

void UpgradeTreeUI::draw() {
    MyDrawEngine* graphics = MyDrawEngine::GetInstance();
    MyDrawEngine::GetInstance()->WriteText(
        graphics->GetViewport().GetTopLeft(), 
        formatTree(upgradeTree().getRootUpgrade()).c_str(),
        MyDrawEngine::CYAN
    );
}

std::wstring UpgradeTreeUI::formatTree(UpgradeTree::NodePtr node, int indent) const {
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
