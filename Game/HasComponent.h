#pragma once

#include <memory>

template <class Component>
class HasComponent {
private:
	using ComponentPtr = std::shared_ptr<Component>;

	ComponentPtr _component;

public:
	using Ptr = std::shared_ptr<HasComponent<Component>>;

	HasComponent(ComponentPtr component);
	ComponentPtr component();
};

template <class Component>
inline HasComponent<Component>::HasComponent(ComponentPtr component) : _component(component) {}

template <class Component>
inline typename HasComponent<Component>::ComponentPtr HasComponent<Component>::component() {
	return _component;
}
