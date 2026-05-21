#pragma once
#include <string>

// a single node in the linked list
// stores a binary number as a string, e.g. "1011"
// and a pointer to the next node (nullptr if it's the last one)
struct ListNode {
    std::string data;
    ListNode* next;

    explicit ListNode(const std::string& val) : data(val), next(nullptr) {}
};
