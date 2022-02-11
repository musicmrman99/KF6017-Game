#pragma once

#include <vector>

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

    T* getValue() {
        return value;
    }
    std::vector<Node<T>*> getChildren() {
        return children;
    }

    Node<T>* addChild(T* child) {
        Node<T>* node = new Node<T>(child);
        children.push_back(node);
        return node;
    }
};

template <class T>
using Tree = Node<T>;
