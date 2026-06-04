#include <iostream>
#include <vector>
#include <algorithm>
#include <random>
#include <chrono>
#include <iomanip>
#include <string>

struct Node {
    int value;
    Node* next;
    Node(int v) : value(v), next(nullptr) {}
};

class SortedLinkedList {
public:
    Node* head = nullptr;

    void insert(int val) {
        Node* newNode = new Node(val);
        if (!head || head->value >= val) {
            newNode->next = head;
            head = newNode;
            return;
        }
        Node* curr = head;
        while (curr->next && curr->next->value < val)
            curr = curr->next;
        newNode->next = curr->next;
        curr->next = newNode;
    }

    bool linearSearch(int target) const {
        Node* curr = head;
        while (curr) {
            if (curr->value == target) return true;
            curr = curr->next;
        }
        return false;
    }

    void clear() {
        while (head) {
            Node* tmp = head;
            head = head->next;
            delete tmp;
        }
    }

    ~SortedLinkedList() { clear(); }
};

std::vector<int> randomData(int n, int maxVal = 1000000, unsigned seed = 42) {
    std::mt19937 rng(seed);
    std::uniform_int_distribution<int> dist(1, maxVal);
    std::vector<int> v(n);
    for (auto& x : v) x = dist(rng);
    return v;
}

long long now_ns() {
    return std::chrono::duration_cast<std::chrono::nanoseconds>(
        std::chrono::steady_clock::now().time_since_epoch()).count();
}

template<typename Func>
double measure_avg(Func fn, int reps = 5) {
    long long total = 0;
    for (int i = 0; i < reps; ++i) {
        long long t0 = now_ns();
        fn();
        long long t1 = now_ns();
        total += (t1 - t0);
    }
    return static_cast<double>(total) / reps;
}

void printRow(const std::string& label, int n, double ns) {
    std::cout << std::left << std::setw(40) << label
        << std::right << std::setw(8) << n
        << std::setw(18) << std::fixed << std::setprecision(0) << ns
        << " ns\n";
}

void task1() {
    std::cout << "\n=== TASK 1: Insertion into a Sorted Linked List ===\n";
    std::cout << std::left << std::setw(40) << "Operation"
        << std::right << std::setw(8) << "N"
        << std::setw(18) << "Average Time\n";
    std::cout << std::string(66, '-') << "\n";

    const int N = 100;
    for (int size : {N, N* N, N* N* N}) {
        auto data = randomData(size);
        double avg = measure_avg([&]() {
            SortedLinkedList lst;
            for (int x : data) lst.insert(x);
            });
        printRow("Insert (linked list)", size, avg);
    }

    std::cout << "\n-- CSV for Excel (Task 1) --\n";
    std::cout << "N,Time (ns)\n";
    for (int size : {N, N* N, N* N* N}) {
        auto data = randomData(size);
        double avg = measure_avg([&]() {
            SortedLinkedList lst;
            for (int x : data) lst.insert(x);
            });
        std::cout << size << "," << std::fixed << std::setprecision(0) << avg << "\n";
    }
}

void task2() {
    std::cout << "\n=== TASK 2: Linear Search – Linked List vs Array ===\n";
    std::cout << std::left << std::setw(40) << "Structure"
        << std::right << std::setw(8) << "N"
        << std::setw(18) << "Average Time\n";
    std::cout << std::string(66, '-') << "\n";

    const int N = 100;
    std::cout << "\n-- CSV for Excel (Task 2) --\n";
    std::cout << "N,Linked List (ns),Array (ns)\n";

    for (int size : {N, N* N, N* N* N}) {
        auto data = randomData(size, size);
        auto targets = randomData(size / 2, size * 2);

        SortedLinkedList lst;
        for (int x : data) lst.insert(x);

        double avgList = measure_avg([&]() {
            volatile bool found = false;
            for (int t : targets) found = lst.linearSearch(t);
            (void)found;
            });

        std::vector<int> arr = data;
        double avgArr = measure_avg([&]() {
            volatile bool found = false;
            for (int t : targets) {
                for (int x : arr) {
                    if (x == t) { found = true; break; }
                }
            }
            (void)found;
            });

        printRow("Linear search (linked list)", size, avgList);
        printRow("Linear search (array)      ", size, avgArr);
        std::cout << "\n";

        std::cout << size << ","
            << std::fixed << std::setprecision(0) << avgList << ","
            << avgArr << "\n";
    }
}

void task3() {
    std::cout << "\n=== TASK 3: Best / Average / Worst Case (N=10000) ===\n";

    const int N = 10000;
    auto data = randomData(N, N);

    SortedLinkedList lst;
    for (int x : data) lst.insert(x);

    std::vector<int> arr = data;

    int minVal = *std::min_element(arr.begin(), arr.end());
    int absent = N * 2;
    auto midTargets = randomData(N, N * 2);

    auto linearSearchArr = [&](int target) -> bool {
        for (int x : arr) if (x == target) return true;
        return false;
        };

    std::cout << std::left << std::setw(30) << "Case"
        << std::right << std::setw(20) << "Linked List (ns)"
        << std::setw(20) << "Array (ns)\n";
    std::cout << std::string(70, '-') << "\n";

    std::cout << std::left << std::setw(30) << "Best"
        << std::right << std::setw(20) << "search for head/first element"
        << std::setw(20) << "(not measured)\n";
    std::cout << std::left << std::setw(30) << "Average"
        << std::right << std::setw(20) << "random targets mix"
        << std::setw(20) << "(not measured)\n";
    std::cout << std::left << std::setw(30) << "Worst"
        << std::right << std::setw(20) << "absent element, full scan"
        << std::setw(20) << "(not measured)\n";
}

int main() {
    std::cout << "============================================\n";
    std::cout << "      Lab 1.6 – Algorithm Analysis        \n";
    std::cout << "============================================\n";

    task1();
    task2();
    task3();

    std::cout << "\nDone. Copy the CSV blocks into Excel to build charts.\n";
    return 0;
}