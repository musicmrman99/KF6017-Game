#pragma once

#include "Combinator.h"

template <class Collection, class Predicate>
class None final : public Combinator<Collection, bool> {
    virtual auto combine(const Collection& collection) const
        -> decltype(std::begin(collection), std::end(collection), true)
    {
        static Predicate predicate;

        for (const auto& item : collection) {
            if (predicate(item)) return false;
        }
        return true;
    }
};

template <class Collection, class Predicate>
class Any final : public Combinator<Collection, bool> {
    virtual auto combine(const Collection& collection) const
        -> decltype(std::begin(collection), std::end(collection), true)
    {
        static Predicate predicate;

        for (const auto& item : collection) {
            if (predicate(item)) return true;
        }
        return false;
    }
};

// Any && NotAll
template <class Collection, class Predicate>
class OnlySome final : public Combinator<Collection, bool> {
    virtual auto combine(const Collection& collection) const
        -> decltype(std::begin(collection), std::end(collection), true)
    {
        static Predicate predicate;

        bool some = false;
        bool all = true;
        for (const auto& item : collection) {
            if (predicate(item)) some = true;
            if (!predicate(item)) all = false;
            if (some && !all) break;
        }
        return some && !all;
    }
};

template <class Collection, class Predicate>
class NotAll final : public Combinator<Collection, bool> {
    virtual auto combine(const Collection& collection) const
        -> decltype(std::begin(collection), std::end(collection), true)
    {
        static Predicate predicate;

        for (const auto& item : collection) {
            if (!predicate(item)) return true;
        }
        return false;
    }
};

template <class Collection, class Predicate>
class All final : public Combinator<Collection, bool> {
    virtual auto combine(const Collection& collection) const
        -> decltype(std::begin(collection), std::end(collection), true)
    {
        static Predicate predicate;

        for (const auto& item : collection) {
            if (!predicate(item)) return false;
        }
        return true;
    }
};
