#include "ArrayQueue.h"

ArrayQueue::ArrayQueue() : count(0) {}

bool ArrayQueue::isFull() const {
    return count == CAPACITY;
}

bool ArrayQueue::isEmpty() const {
    return count == 0;
}

bool ArrayQueue::enqueue(int value) {
    if (isFull()) {
        std::cout << "  [!] queue is full, can't add: " << value << std::endl;
        return false;
    }
    // just put the new element at the end
    data[count++] = value;
    return true;
}

int ArrayQueue::dequeue() {
    if (isEmpty()) {
        throw std::underflow_error("queue is empty, nothing to remove");
    }

    int value = data[0]; // save the front element

    // shift everything one position to the left
    // since we're removing from the head (not the tail) we have to do this
    for (int i = 0; i < count - 1; ++i) {
        data[i] = data[i + 1];
    }
    --count;

    return value;
}

int ArrayQueue::size() const {
    return count;
}

void ArrayQueue::print() const {
    if (isEmpty()) {
        std::cout << "  [ queue is empty ]" << std::endl;
        return;
    }

    std::cout << "  Queue (head -> tail): ";
    for (int i = 0; i < count; ++i) {
        std::cout << data[i];
        if (i < count - 1) std::cout << " -> ";
    }
    std::cout << std::endl;
}