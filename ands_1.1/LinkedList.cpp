#include "LinkedList.h"
#include "utils.h"

LinkedList::LinkedList() : head(nullptr) {}

// walk the list and delete every node one by one
LinkedList::~LinkedList() {
    while (!isEmpty()) {
        removeFront();
    }
}

bool LinkedList::isEmpty() const {
    return head == nullptr;
}

void LinkedList::insertBack(const std::string& value) {
    ListNode* newNode = new ListNode(value);

    if (isEmpty()) {
        head = newNode;
        return;
    }

    // walk to the last node
    ListNode* curr = head;
    while (curr->next != nullptr) {
        curr = curr->next;
    }
    curr->next = newNode;
}

void LinkedList::insertFront(const std::string& value) {
    ListNode* newNode = new ListNode(value);
    newNode->next = head;
    head = newNode;
}

std::string LinkedList::removeFront() {
    if (isEmpty()) {
        throw std::underflow_error("list is empty, can't remove from front");
    }

    ListNode* temp = head;
    std::string value = temp->data;
    head = head->next;
    delete temp;

    return value;
}

std::string LinkedList::removeBack() {
    if (isEmpty()) {
        throw std::underflow_error("list is empty, can't remove from back");
    }

    // if there's only one element, reuse removeFront
    if (head->next == nullptr) {
        return removeFront();
    }

    // walk until we're at the second-to-last node
    ListNode* curr = head;
    while (curr->next->next != nullptr) {
        curr = curr->next;
    }

    std::string value = curr->next->data;
    delete curr->next;
    curr->next = nullptr;

    return value;
}

void LinkedList::removeEvenElements() {
    ListNode* current = head;
    ListNode* prev = nullptr;

    while (current != nullptr) {
        int decVal = binaryToDecimal(current->data);

        if (decVal % 2 == 0) {
            // this node needs to go - unlink it and free memory
            ListNode* toDelete = current;

            if (prev == nullptr) {
                // removing the head
                head = current->next;
            }
            else {
                prev->next = current->next;
            }

            current = current->next;
            delete toDelete;
        }
        else {
            // odd - keep it, move on
            prev = current;
            current = current->next;
        }
    }
}

ListNode* LinkedList::getHead() const {
    return head;
}

void LinkedList::print() const {
    if (isEmpty()) {
        std::cout << "  [ list is empty ]" << std::endl;
        return;
    }

    std::cout << "  List: ";
    ListNode* curr = head;
    while (curr != nullptr) {
        std::cout << curr->data;
        if (curr->next != nullptr) std::cout << " -> ";
        curr = curr->next;
    }
    std::cout << std::endl;
}

void LinkedList::printWithDecimal() const {
    if (isEmpty()) {
        std::cout << "  [ list is empty ]" << std::endl;
        return;
    }

    ListNode* curr = head;
    int       idx = 1;
    while (curr != nullptr) {
        std::cout << "  [" << idx++ << "] "
            << curr->data
            << "  (= " << binaryToDecimal(curr->data) << " dec)"
            << std::endl;
        curr = curr->next;
    }
}