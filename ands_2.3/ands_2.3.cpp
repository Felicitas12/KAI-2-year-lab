#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <cmath>

// --- Level 1: Arrangements with repetition (n=5 sections, k=18 students) ---
// Each student independently chooses one of 5 sections => 5^18

long long power(long long base, int exp) {
    long long result = 1;
    for (int i = 0; i < exp; i++) result *= base;
    return result;
}

void level1() {
    std::cout << "\n=== LEVEL 1: Arrangements with repetition (no repeat constraint) ===\n";
    std::cout << "Problem: 18 students, 5 sport sections\n";
    std::cout << "Type: Arrangements with repetition\n";
    std::cout << "Formula: n^k, where n = sections, k = students\n\n";

    int n, k;
    std::cout << "Enter n (sections): ";
    std::cin >> n;
    std::cout << "Enter k (students): ";
    std::cin >> k;

    long long result = power(n, k);
    std::cout << "Result: " << n << "^" << k << " = " << result << " ways\n";
}

// --- Level 2: Arrangements with repetition (n=16 hex digits, k=6 positions) ---
// 6-digit hex numbers with repetition => 16^6

void level2() {
    std::cout << "\n=== LEVEL 2: Arrangements with repetition ===\n";
    std::cout << "Problem: 6-digit hexadecimal numbers with repeated digits\n";
    std::cout << "Type: Arrangements with repetition\n";
    std::cout << "Formula: n^k, where n = hex digits (16), k = positions (6)\n\n";

    int n, k;
    std::cout << "Enter n (number of distinct digits, e.g. 16): ";
    std::cin >> n;
    std::cout << "Enter k (number of positions, e.g. 6): ";
    std::cin >> k;

    long long result = power(n, k);
    std::cout << "Result: " << n << "^" << k << " = " << result << " hex numbers\n";
}

// --- Level 3: Generate all arrangements with repetition, write to file ---
// For demonstration: n sections, k students (use small values to avoid huge files)

void generateArrangements(int n, int k, std::ofstream& out) {
    std::vector<int> current(k, 0);
    long long count = 0;
    while (true) {
        for (int i = 0; i < k; i++) {
            out << (current[i] + 1);
            if (i < k - 1) out << " ";
        }
        out << "\n";
        count++;

        int pos = k - 1;
        while (pos >= 0 && current[pos] == n - 1) {
            current[pos] = 0;
            pos--;
        }
        if (pos < 0) break;
        current[pos]++;
    }
    std::cout << "Total arrangements written: " << count << "\n";
}

void level3() {
    std::cout << "\n=== LEVEL 3: Full list of arrangements to file ===\n";
    std::cout << "Note: Use small values (e.g. n=3, k=2) to avoid huge output\n\n";

    int n, k;
    std::cout << "Enter n (sections): ";
    std::cin >> n;
    std::cout << "Enter k (students): ";
    std::cin >> k;

    long long total = power(n, k);
    if (total > 1000000) {
        std::cout << "Too many combinations (" << total << "). Use n<=5, k<=8.\n";
        return;
    }

    std::ofstream out("arrangements.txt");
    if (!out.is_open()) { std::cerr << "Cannot open arrangements.txt\n"; return; }

    out << "Arrangements with repetition: n=" << n << " sections, k=" << k << " students\n";
    out << "Format: section assignment for each student\n\n";

    generateArrangements(n, k, out);
    out.close();
    std::cout << "Results saved to arrangements.txt\n";
}

int main() {
    level1();
    level2();
    level3();
    return 0;
}