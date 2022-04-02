#pragma once

#include <memory>

template <class A, class B>
std::unique_ptr<A> static_unique_pointer_cast(std::unique_ptr<B> ptr) {
    return std::unique_ptr<A>(static_cast<A*>(ptr.release()));
}

template <class A, class B>
std::weak_ptr<A> static_weak_pointer_cast(std::weak_ptr<B> ptr) {
    return std::static_pointer_cast<A>(ptr.lock());
}
