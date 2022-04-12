#include "Symbol.h"

bool Symbol::operator== (const Symbol& other) const {
	return &other == this;
}
bool Symbol::operator< (const Symbol& other) const {
	return &other < this;
}
