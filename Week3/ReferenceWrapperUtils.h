#pragma once

#include <memory>

// A comparator for reference wrappers
template <class T>
struct ReferenceWrapperLess final {
    bool operator() (const std::reference_wrapper<T>& a, const std::reference_wrapper<T>& b) const {
        return a.get() < b.get();
    }
};

// A predicate for reference wrappers
template <class T>
class ReferenceWrapperEqPredicate final {
private:
	std::reference_wrapper<T> obj;

public:
	ReferenceWrapperEqPredicate(T& obj) : obj(obj) {};

	bool operator() (const std::reference_wrapper<T>& other) const {
		return other.get() == static_cast<const std::reference_wrapper<T>>(obj).get();
	}
};
