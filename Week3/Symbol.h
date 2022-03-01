#pragma once

// Symbol
class Symbol {
public:
	Symbol() {}
	bool operator== (const Symbol& other) const {
		return &other == this;
	}
	bool operator< (const Symbol& other) const {
		return &other < this;
	}
};