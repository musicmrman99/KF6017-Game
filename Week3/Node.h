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

private:
    ValuePtr value;
    std::vector<NodePtr> children;

    Node(const ValuePtr& value) : value(value) {}

public:
    /* Create
    -------------------- */

    static NodePtr create(const ValuePtr& value) {
        return NodePtr(new Node<T>(value));
    }
    static NodePtr create(T* value) {
        return create(ValuePtr(value));
    }
    static NodePtr create() {
        return create(nullptr);
    }

    /* Get
    -------------------- */

    ValuePtr getValue() const {
        return value;
    }
    const std::vector<NodePtr>& getChildren() const {
        return children;
    }

    /* Set Value
    -------------------- */

    void setValue(const ValuePtr& value) {
        this->value = value;
    }
    void setValue(T* value) {
        setValue(ValuePtr(value));
    }
    void clearValue() {
        setValue(nullptr);
    }

    /* Add Child
    -------------------- */

    NodePtr addChild(const NodePtr& child) {
        children.push_back(child);
        return child; // Pass parameter back to caller for easier tree construction
    }
    NodePtr addChild(const ValuePtr& child) {
        return addChild(create(child));
    }
    NodePtr addChild(T* value) {
        return addChild(ValuePtr(value));
    }
    NodePtr addChild() {
        return addChild(nullptr);
    }

    /* Find
    -------------------- */

    // Find the node containing the given value.
    // Return an empty optional if no node in the tree contains the given value.
    template <class C = std::equal_to<T>>
    static std::optional<NodePtr> find(const NodePtr& root, const ValuePtr& value) {
        static C comparator;

        if (root) {
            // Base case - found
            if (
                !root->value && !value ||
                root->value && value && comparator(*root->value, *value)
                ) {
                return root; // Copy up to N (= depth of tree) times
            }

            // Recursive case
            for (const NodePtr& node : root->children) {
                const std::optional<NodePtr>& found = find(node, value);
                if (found) return found;
            }
        }

        // Base case - not found (in this branch of the tree)
        return std::nullopt;
    }

    // Overload for ease of directly passing `new`-ed objects.
    // Creates a smart pointer from value, so always deallocates given object.
    // See `std::optional<NodePtr> find(const ConstValuePtr)` for details;
    template <class C = std::equal_to<T>>
    static std::optional<NodePtr> find(const NodePtr& root, T* value) {
        return find(root, ValuePtr(value));
    }

    /* Find Parent
    -------------------- */

private:
    // Recusive findParent() implementation
    template <class C = std::equal_to<T>>
    static std::optional<NodePtr> findParent(const NodePtr& root, const ValuePtr& value, const NodePtr& parent) {
        static C comparator;

        if (root) {
            // Base case - found
            if (
                !root->value && !value ||
                root->value && value && comparator(*root->value, *value)
                ) {
                return parent;
            }

            // Recursive case
            for (const NodePtr& node : root->children) {
                const std::optional<NodePtr>& found = findParent(node, value, root);
                if (found) return found;
            }
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
    static std::optional<NodePtr> findParent(const NodePtr& root, const ValuePtr& value) {
        return findParent(root, value, nullptr);
    }

    // Overload for ease of directly passing `new`-ed objects.
    // Creates a smart pointer from value, so always deallocates given object.
    // See `std::optional<NodePtr> findParent(const ConstValuePtr)` for details.
    template <class C = std::equal_to<T>>
    static std::optional<NodePtr> findParent(const NodePtr& root, T* value) {
        return findParent(root, ValuePtr(value));
    }

    /* Find Path
    -------------------- */

private:
    // Find the node containing the given value.
    // Return an empty optional if no node in the tree contains the given value.
    template <class C = std::equal_to<T>>
    static void findPath(const NodePtr& root, const ValuePtr& value, std::optional<std::vector<NodePtr>>& path) {
        static C comparator;

        if (root) {
            // Base case - found
            if (
                !root->value && !value ||
                root->value && value && comparator(*root->value, *value)
            ) {
                path = { root };
                return;
            }

            // Recursive case
            for (const NodePtr& node : root->children) {
                findPath(node, value, path);
                if (path) {
                    path.value().push_back(root);
                    return;
                };
            }
        }

        // Base case - not found (in this branch of the tree)
        path = std::nullopt;
    }

public:
    template <class C = std::equal_to<T>>
    static std::optional<std::vector<NodePtr>> findPath(const NodePtr& root, const ValuePtr& value) {
        std::optional<std::vector<NodePtr>> path = std::nullopt;
        findPath(root, value, path);
        if (path) {
            std::reverse(path.value().begin(), path.value().end());
        }
        return path;
    }

    // Overload for ease of directly passing `new`-ed objects.
    // Creates a smart pointer from value, so always deallocates given object.
    // See `std::optional<NodePtr> find(const ConstValuePtr)` for details;
    template <class C = std::equal_to<T>>
    static std::optional<std::vector<NodePtr>> findPath(const NodePtr& root, T* value) {
        return findPath(root, ValuePtr(value));
    }
};
