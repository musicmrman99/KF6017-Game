#pragma once

#include <memory>

template <class T>
class Referencing {
private:
    std::weak_ptr<T> _ref;

public:
    virtual void setRef(std::weak_ptr<T> ref) {
        _ref = ref;
    }

    virtual std::weak_ptr<T> ref() {
        return _ref;
    }
};
