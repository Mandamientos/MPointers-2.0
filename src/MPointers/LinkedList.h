#ifndef LINKEDLIST_H
#define LINKEDLIST_H

#include "MPointers.h"
#include <iostream>

template<typename T>
struct Node {
    T data;
    MPointer<Node<T>> next;

    Node() : data(), next() {}
};

template<typename T>
class LinkedList {
private:
    MPointer<Node<T>> head_;

public:
    LinkedList() : head_() {}

    void push_back(const T& value) {
        MPointer<Node<T>> newNodePtr = MPointer<Node<T>>::New();
        Node<T> newNode;
        newNode.data = value;
        newNode.next = MPointer<Node<T>>();

        *newNodePtr = newNode;

        if (head_.isNull()) {
            head_ = newNodePtr;
            return;
        }

        MPointer<Node<T>> current = head_;
        while (true) {
            Node<T> currentNode = *current;
            if (currentNode.next.isNull()) {
                currentNode.next = newNodePtr;
                *current = currentNode;
                break;
            }
            current = currentNode.next;
        }
    }

    void print() const {
        MPointer<Node<T>> current = head_;
        while (!current.isNull()) {
            Node<T> currentNode = *current;
            std::cout << currentNode.data << " -> ";
            current = currentNode.next;
        }
        std::cout << "NULL" << std::endl;
    }

    bool isEmpty() const {
        return head_.isNull();
    }
};

#endif
