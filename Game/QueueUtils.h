#pragma once

#include <queue>

template <class From, class To = From>
void shiftInto(std::queue<From>& from, std::queue<To>& to) {
	while (!from.empty()) {
		to.push(from.front());
		from.pop();
	}
}
