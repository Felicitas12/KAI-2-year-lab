#include <iostream>
#include <string>
#include <iomanip>
#include <vector>

using namespace std;

class Student {
public:
    string lastName;
    string firstName;
    int group;
    unsigned int studentID;

    Student() : group(0), studentID(0) {}

    Student(const string& ln, const string& fn, int g, unsigned int id)
        : lastName(ln), firstName(fn), group(g), studentID(id) {
    }

    void print() const {
        cout << left
            << setw(6) << group
            << setw(12) << studentID
            << setw(15) << lastName
            << firstName << "\n";
    }

    static void printHeader() {
        cout << left
            << setw(6) << "Group"
            << setw(12) << "ID"
            << setw(15) << "Last Name"
            << "First Name" << "\n";
        cout << string(50, '-') << "\n";
    }
};

void printArray(const vector<Student>& arr) {
    Student::printHeader();
    for (const auto& s : arr) s.print();
    cout << "\n";
}

// ── LEVEL 1: Shell sort — Knuth gap sequence ──────────────────────────────
// gaps: 1, 4, 13, 40, 121, ...  (h = 3*h + 1)
// sort key: group ascending
void shellSortKnuth(vector<Student>& arr) {
    int n = arr.size();
    int h = 1;
    while (h < n / 3) h = 3 * h + 1;

    while (h >= 1) {
        for (int i = h; i < n; i++) {
            Student tmp = arr[i];
            int j = i;
            while (j >= h && arr[j - h].group > tmp.group) {
                arr[j] = arr[j - h];
                j -= h;
            }
            arr[j] = tmp;
        }
        h /= 3;
    }
}

// ── LEVEL 2: Shell sort — Shell gap sequence ──────────────────────────────
// gaps: n/2, n/4, ..., 1
// sort key: group ascending, then studentID ascending within group
void shellSortShell(vector<Student>& arr) {
    int n = arr.size();
    for (int gap = n / 2; gap > 0; gap /= 2) {
        for (int i = gap; i < n; i++) {
            Student tmp = arr[i];
            int j = i;
            while (j >= gap) {
                const Student& prev = arr[j - gap];
                bool shouldSwap = (prev.group > tmp.group) ||
                    (prev.group == tmp.group && prev.studentID > tmp.studentID);
                if (!shouldSwap) break;
                arr[j] = arr[j - gap];
                j -= gap;
            }
            arr[j] = tmp;
        }
    }
}

// ── LEVEL 3: Bottom-up merge sort ────────────────────────────────────────
// sort key: group ascending
void merge(vector<Student>& arr, int left, int mid, int right) {
    vector<Student> tmp(arr.begin() + left, arr.begin() + right + 1);
    int i = 0, j = mid - left + 1, k = left;
    int leftSize = mid - left + 1, rightSize = right - mid;

    while (i < leftSize && j < leftSize + rightSize) {
        if (tmp[i].group <= tmp[j].group)
            arr[k++] = tmp[i++];
        else
            arr[k++] = tmp[j++];
    }
    while (i < leftSize)            arr[k++] = tmp[i++];
    while (j < leftSize + rightSize) arr[k++] = tmp[j++];
}

void mergeSortBottomUp(vector<Student>& arr) {
    int n = arr.size();
    for (int width = 1; width < n; width *= 2) {
        for (int left = 0; left < n - width; left += 2 * width) {
            int mid = left + width - 1;
            int right = min(left + 2 * width - 1, n - 1);
            merge(arr, left, mid, right);
        }
    }
}

int main() {
    vector<Student> base = {
        {"Kovalenko",  "Ivan",    102, 5003},
        {"Boyko",      "Olena",   101, 5011},
        {"Melnyk",     "Serhiy",  103, 5007},
        {"Ivanchenko", "Petro",   101, 5001},
        {"Tymchenko",  "Andriy",  102, 5015},
        {"Romanenko",  "Dmytro",  103, 5009},
        {"Ponomarenko","Vasyl",   101, 5020},
        {"Kravchenko", "Natalia", 102, 5002},
        {"Savchenko",  "Oksana",  103, 5018},
        {"Bondarenko", "Mykola",  101, 5006},
    };

    cout << "=== LEVEL 1: Shell sort (Knuth gaps) — by group ASC ===\n\n";
    {
        vector<Student> arr = base;
        cout << "Before:\n"; printArray(arr);
        shellSortKnuth(arr);
        cout << "After:\n";  printArray(arr);
    }

    cout << "=== LEVEL 2: Shell sort (Shell gaps) — by group ASC, then by ID ASC ===\n\n";
    {
        vector<Student> arr = base;
        cout << "Before:\n"; printArray(arr);
        shellSortShell(arr);
        cout << "After:\n";  printArray(arr);
    }

    cout << "=== LEVEL 3: Bottom-up merge sort — by group ASC ===\n\n";
    {
        vector<Student> arr = base;
        cout << "Before:\n"; printArray(arr);
        mergeSortBottomUp(arr);
        cout << "After:\n";  printArray(arr);
    }

    return 0;
}