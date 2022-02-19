#pragma once

#include <vector>
#include <functional>
#include <optional>
#include <memory>

/*
Usage example:

#include <iostream>
#include "Node.h"

struct Example {
    int a;
    bool b;

    // If you need to use find<C>() or findParent<C>() without specifying
    // the comparator, C, then you'll need this.
    bool operator== (const Example& other) const {
        return other.a == a && other.b == b;
    }
};

int main() {
    // All new Examples will be deallocated automatically
    auto root = Node<Example>::create(new Example{ 1, true });
    root->addChild(new Example{ 2, false });
    auto child2 = root->addChild(new Example{ 3, false });
    child2->addChild(new Example{ 4, true });

    auto found = root->find(new Example{ 4, true });
    if (found) {
        const int a = (*found)->getValue()->a;
        const std::string b = (*found)->getValue()->b ? "yes" : "no";
        std::cout << "Found: " << a << ", (" << b << ")" << std::endl;
    } else {
        std::cout << "Not Found" << std::endl;
    }
}
*/

// Represents a node in a tree of nodes.
template <class T>
class Node {
public:
    using ValuePtr = std::shared_ptr<T>;
    using NodePtr = std::shared_ptr<Node<T>>;

    using ConstValuePtr = std::shared_ptr<const T>;
    using ConstNodePtr = std::shared_ptr<const Node<T>>;

private:
    ValuePtr value;
    const NodePtr self;
    std::vector<NodePtr> children;

    Node(ValuePtr value) : value(value), self(NodePtr(this)) {}

public:
    /* Pointer
    -------------------- */

    // Get NodePtr to Node.
    NodePtr ptr() {
        return self;
    }

    /* Create
    -------------------- */

    static NodePtr create(ValuePtr value) { return (new Node<T>(value))->ptr(); }
    static NodePtr create(T* value) { return create(ValuePtr(value)); }
    static NodePtr create() { return create(nullptr); }

    /* Get
    -------------------- */

    ValuePtr getValue() const { return value; }
    const std::vector<NodePtr>& getChildren() const {
        return children;
    }

    /* Set
    -------------------- */

    void setValue(ValuePtr value) { this->value = value; }
    void setValue(T* value) { setValue(ValuePtr(value)); }
    void clearValue() { setValue(nullptr); }

    NodePtr addChild(NodePtr child) {
        children.push_back(child);
        return child; // Pass parameter back to caller for easier tree construction
    }
    NodePtr addChild(ValuePtr child) { return addChild(create(child)); }
    NodePtr addChild(T* value) { return addChild(ValuePtr(value)); }
    NodePtr addChild() { return addChild(nullptr); }

    /* Search - find() and findParent()
    -------------------- */

    // Find the node containing the given value.
    // Return an empty optional if no node in the tree contains the given value.
    template <class C = std::equal_to<T>>
    std::optional<NodePtr> find(const ConstValuePtr value) {
        static C comparator;

        // Base case - found
        if (
            !this->value && !value ||
            this->value && value && comparator(*this->value, *value)
            ) {
            return this->ptr(); // Copy up to N (= depth of tree) times
        }

        // Recursive case
        for (NodePtr node : children) {
            const std::optional<NodePtr>& found = node->find(value);
            if (found) return found;
        }

        // Base case - not found (in this branch of the tree)
        return std::nullopt;
    }

    // Overload for ease of directly passing `new`-ed objects.
    // Creates a smart pointer from value, so always deallocates given object.
    // See `std::optional<NodePtr> find(const ConstValuePtr)` for details;
    template <class C = std::equal_to<T>>
    std::optional<NodePtr> find(const T* value) { return find(ConstValuePtr(value)); }

private:
    // Recusive findParent() implementation
    template <class C = std::equal_to<T>>
    std::optional<NodePtr> findParent(const ConstValuePtr value, NodePtr parent) {
        static C comparator;

        // Base case - found
        if (
            !this->value && !value ||
            this->value && value && comparator(*this->value, *value)
            ) {
            return parent;
        }

        // Recursive case
        for (NodePtr node : children) {
            const std::optional<NodePtr>& found = node->findParent(value, this->ptr());
            if (found) return found;
        }

        // Base case - not found (in this branch of the tree)
        return std::nullopt;
    }

public:
    // Find the parent node (in this tree) of the node containing the given value.
    // Return an empty optional if no node in the tree contains the given value.
    // Return an optional containing a NodePtr of nullptr if the root node (ie. this node) contains
    // the given value (ie. the value was found, but the root node has no parent).
    template <class C = std::equal_to<T>>
    std::optional<NodePtr> findParent(const ConstValuePtr value) {
        return findParent(value, nullptr);
    }

    // Overload for ease of directly passing `new`-ed objects.
    // Creates a smart pointer from value, so always deallocates given object.
    // See `std::optional<NodePtr> findParent(const ConstValuePtr)` for details.
    template <class C = std::equal_to<T>>
    std::optional<NodePtr> findParent(const T* value) {
        return findParent(ConstValuePtr(value));
    }
};
