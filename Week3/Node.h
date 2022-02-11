#pragma once

#include <vector>
#include <functional>

template <class T>
class Node {
private:
    T* value;
    std::vector<Node<T>*> children;

public:
    Node() : value(nullptr) {}
    Node(T* value) : value(value) {}
    ~Node() {
        if (value) {
            delete value;
            value = nullptr;
        }
        for (Node<T>*& child : children) {
            if (child) {
                delete child;
                child = nullptr;
            }
        }
    }

    T* getValue() const {
        return value;
    }
    void setValue(T* value) {
        // Delete before loosing track if needed
        if (this->value && this->value != value) delete this->value;
        this->value = value;
    }

    std::vector<Node<T>*> getChildren() const {
        return children;
    }

    template <class C = std::equal_to<T>>
    Node<T>* find(const T* value) {
        static C comparator;
        if (comparator(*this->value, *value)) return this;
        Node<T>* found;
        for (Node<T>* node : children) {
            found = node->find(value);
            if (found) return found;
        }
        return nullptr;
    }

    template <class C = std::equal_to<T>>
    Node<T>* findParent(const T* value) {
        return findParent(value, nullptr);
    }

    template <class C = std::equal_to<T>>
    Node<T>* findParent(const T* value, Node<T>* parent) {
        static C comparator;
        if (comparator(*this->value, *value)) return parent;
        Node<T>* found;
        for (Node<T>* node : children) {
            found = node->findParent(value, this);
            if (found) return found;
        }
        return nullptr;
    }

    Node<T>* addChild(T* child) {
        Node<T>* node = new Node<T>(child);
        children.push_back(node);
        return node;
    }
};

template <class T>
using Tree = Node<T>;
