#pragma once

#include <memory>

template <class T>
class SelfReferencing {
private:
    std::weak_ptr<T> _self;

public:
    virtual void setSelf(std::weak_ptr<T> self) {
        _self = self;
    }

    virtual std::weak_ptr<T> self() {
        return _self;
    }
};
