#include "QueueUtils.h"

template <class T>
void shiftInto(std::queue<T>& a, std::queue<T>& b) {
	while (!a.empty()) {
		b.push(a.front());
		a.pop();
	}
}
