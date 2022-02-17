#pragma once

#include <type_traits>

template <class T, class Super>
using Derived = typename std::enable_if<std::is_base_of<Super, T>::value, T>::type;
