#include <iostream>
#include <string>
#include "ArrayQueue.h"
#include "LinkedList.h"
#include "utils.h"

int main() {
    std::cout << "============================================\n";
    std::cout << "          LAB WORK 1.1\n";
    std::cout << "    Linear Data Structures Research\n";
    std::cout << "============================================\n";

    // =========================================================
    // LEVEL 1 - Queue on array, element type: int
    // =========================================================
    std::cout << "\n============================================\n";
    std::cout << "  LEVEL 1: Array Queue (int elements)\n";
    std::cout << "============================================\n";

    ArrayQueue queue1;

    // insert a few numbers
    std::cout << "\n--- inserting elements ---\n";
    int toAdd[] = { 15, 27, 43, 8, 61, 34, 99 };
    for (int v : toAdd) {
        bool ok = queue1.enqueue(v);
        std::cout << "  enqueue(" << v << ") -> " << (ok ? "OK" : "FAIL") << "\n";
    }

    std::cout << "\n  state after inserts:\n";
    queue1.print();

    // remove 3 elements from the front
    std::cout << "\n--- removing 3 elements ---\n";
    try {
        for (int i = 0; i < 3; ++i) {
            int val = queue1.dequeue();
            std::cout << "  dequeue() -> " << val << "\n";
        }
    }
    catch (const std::exception& e) {
        std::cerr << "  error: " << e.what() << "\n";
    }

    std::cout << "\n  state after removals:\n";
    queue1.print();

    // =========================================================
    // LEVEL 2 - Singly linked list, element type: string (binary)
    // =========================================================
    std::cout << "\n============================================\n";
    std::cout << "  LEVEL 2: Linked List (binary strings)\n";
    std::cout << "============================================\n";

    LinkedList list2;

    // insert binary numbers as strings
    std::cout << "\n--- inserting elements ---\n";
    std::string binsL2[] = { "1010", "11001", "110", "10001", "1111", "1000", "10101" };
    //              dec:       10      25       6      17      15       8       21
    for (const std::string& s : binsL2) {
        list2.insertBack(s);
        std::cout << "  insertBack(\"" << s << "\")  // "
            << binaryToDecimal(s) << " in decimal\n";
    }

    std::cout << "\n  state after inserts:\n";
    list2.printWithDecimal();

    // remove one from front and one from back
    std::cout << "\n--- removing 2 elements ---\n";
    try {
        std::string v = list2.removeFront();
        std::cout << "  removeFront() -> \"" << v
            << "\"  (" << binaryToDecimal(v) << " dec)\n";

        v = list2.removeBack();
        std::cout << "  removeBack()  -> \"" << v
            << "\"  (" << binaryToDecimal(v) << " dec)\n";
    }
    catch (const std::exception& e) {
        std::cerr << "  error: " << e.what() << "\n";
    }

    std::cout << "\n  state after removals:\n";
    list2.printWithDecimal();

    // =========================================================
    // LEVEL 3 - combine both structures
    // task: remove even elements from the list,
    //       copy the rest into a queue (as decimal values)
    // =========================================================
    std::cout << "\n============================================\n";
    std::cout << "  LEVEL 3: List + Queue combined task\n";
    std::cout << "============================================\n";

    LinkedList list3;   // first structure  (linked list)
    ArrayQueue queue3;  // second structure (array queue)

    // fill the list with binary numbers 10-19
    std::cout << "\n--- building the list (1st structure) ---\n";
    std::string binsL3[] = {
        "1010",  "1011",  "1100",  "1101",
        "1110",  "1111",  "10000", "10001",
        "10010", "10011"
    };
    // decimal: 10  11   12   13   14   15   16   17   18   19

    for (const std::string& s : binsL3) {
        list3.insertBack(s);
        std::cout << "  insertBack(\"" << s << "\")  // = "
            << binaryToDecimal(s) << "\n";
    }

    std::cout << "\n  initial list state:\n";
    list3.printWithDecimal();

    // remove all nodes with even decimal values
    std::cout << "\n--- removing even elements from the list ---\n";
    list3.removeEvenElements();

    std::cout << "\n  list after removing even elements:\n";
    list3.printWithDecimal();

    // copy what's left into the queue, converting to decimal first
    std::cout << "\n--- copying odd elements to the queue (as decimal) ---\n";
    ListNode* curr = list3.getHead();
    while (curr != nullptr) {
        int dec = binaryToDecimal(curr->data);
        bool ok = queue3.enqueue(dec);
        std::cout << "  enqueue(\"" << curr->data << "\" -> "
            << dec << ")  -> " << (ok ? "OK" : "FAIL") << "\n";
        curr = curr->next;
    }

    // print final state of both structures
    std::cout << "\n--- final state of both structures ---\n";

    std::cout << "\n  1st structure (linked list, binary):\n";
    list3.print();

    std::cout << "\n  2nd structure (array queue, decimal):\n";
    queue3.print();

    return 0;
}