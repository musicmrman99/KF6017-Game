#pragma once

// Symbol
class Symbol {
public:
	Symbol() {}
	virtual ~Symbol() {}

	bool operator== (const Symbol& other) const;
	bool operator< (const Symbol& other) const;
};
