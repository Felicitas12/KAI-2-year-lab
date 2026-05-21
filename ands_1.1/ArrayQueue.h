#pragma once
#include <iostream>
#include <stdexcept>

// Level 1 - queue using a plain array (static allocation)
// elements are integers, max size is fixed at 10
class ArrayQueue {
private:
    static const int CAPACITY = 10;
    int data[CAPACITY];
    int count; // how many elements are currently in the queue

public:
    ArrayQueue();

    // returns true if count reached the array size
    bool isFull() const;

    // returns true if there's nothing in the queue
    bool isEmpty() const;

    // add to the tail, returns false if queue is full so we don't crash
    bool enqueue(int value);

    // remove from the head and shift everything left
    // throws if queue is empty
    int dequeue();

    int size() const;

    void print() const;
};