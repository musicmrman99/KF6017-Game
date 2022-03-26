#pragma once

#include <list>
#include <memory>

template <class T>
class Tracker {
private:
    std::list<std::weak_ptr<T>> objects;

public:
    virtual ~Tracker() {}

    void track(std::weak_ptr<T> object) {
        objects.push_back(object);
    }

    void dropExpired() {
        objects.remove_if([](const std::weak_ptr<T>& object) { return object.expired(); });
    }

    std::list<std::weak_ptr<T>>& getTracked() {
        return objects;
    }
};
