#pragma once
#include <iostream>
#include <stdexcept>
#include "ListNode.h"

// Level 2 - singly linked list
// stores positive integers in binary string form like "1101"
// we only keep a pointer to the head, nothing else (no size counter etc.)
class LinkedList {
private:
    ListNode* head;

public:
    LinkedList();
    ~LinkedList(); // need destructor to free all nodes

    // check if head is nullptr - that means the list is empty
    bool isEmpty() const;

    // insert at the end of the list
    void insertBack(const std::string& value);

    // insert at the beginning
    void insertFront(const std::string& value);

    // remove the first element, throws if list is empty
    std::string removeFront();

    // remove the last element, throws if list is empty
    std::string removeBack();

    // used in level 3 - removes all nodes whose decimal value is even
    void removeEvenElements();

    // just returns head so we can iterate from outside (needed in level 3)
    ListNode* getHead() const;

    // print only binary strings
    void print() const;

    // print with decimal values next to each element
    void printWithDecimal() const;
};