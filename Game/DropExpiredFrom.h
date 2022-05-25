#pragma once

template <class T>
void dropExiredFrom(std::list<T>& objects) {
    objects.remove_if([](const T& object) { return object.obj.expired(); });
}
