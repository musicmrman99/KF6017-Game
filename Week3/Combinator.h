#pragma once

template <class Collection, class Result>
class Combinator {
public:
    virtual ~Combinator() {};
    virtual Result combine(const Collection& collection) const = 0;
};
