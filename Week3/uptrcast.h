#pragma once

#include <memory>

template <class A, class B>
std::unique_ptr<A> static_unique_pointer_cast(std::unique_ptr<B> ptr) {
    return std::unique_ptr<A>(static_cast<A*>(ptr.release()));
}
