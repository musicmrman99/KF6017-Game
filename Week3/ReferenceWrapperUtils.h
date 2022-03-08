#pragma once

#include <memory>

// A comparator for just the upgrade of a purchasable upgrade, ignoring whether it's purchased.
template <class T>
struct ReferenceWrapperLess final {
    bool operator() (const std::reference_wrapper<T>& a, const std::reference_wrapper<T>& b) const {
        return a.get() < b.get();
    }
};
