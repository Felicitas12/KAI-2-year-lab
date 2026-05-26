#include <iostream>
#include <string>
#include <iomanip>
#include <vector>
#include <queue>

using namespace std;

class Student {
public:
    string       lastName;
    string       firstName;
    int          group;
    string       gender;
    unsigned int recordID;
    int          cityCode;

    Student() : group(0), recordID(0), cityCode(0) {}

    Student(const string& ln, const string& fn, int g,
        const string& gen, unsigned int rid, int cc = 0)
        : lastName(ln), firstName(fn), group(g),
        gender(gen), recordID(rid), cityCode(cc) {
    }

    void print(bool showCity = false) const {
        cout << left
            << setw(8) << gender
            << setw(10) << recordID
            << setw(7) << group
            << setw(15) << lastName
            << firstName;
        if (showCity) cout << "  [city:" << cityCode << "]";
        cout << "\n";
    }

    static void printHeader(bool showCity = false) {
        cout << left
            << setw(8) << "Gender"
            << setw(10) << "RecordID"
            << setw(7) << "Group"
            << setw(15) << "Last Name"
            << "First Name";
        if (showCity) cout << "  City";
        cout << "\n" << string(showCity ? 62 : 52, '-') << "\n";
    }
};

void printArray(const vector<Student>& arr, bool showCity = false) {
    Student::printHeader(showCity);
    for (const auto& s : arr) s.print(showCity);
    cout << "\n";
}

// ─── LEVEL 1 ─────────────────────────────────────────────────────────────────
// Ordered by gender (Female first), within gender by recordID ascending.
// New element inserted at the correct position using sequential search.

void insertOrdered(vector<Student>& arr, const Student& s) {
    int pos = (int)arr.size();
    for (int i = 0; i < (int)arr.size(); i++) {
        bool before = (s.gender < arr[i].gender) ||
            (s.gender == arr[i].gender && s.recordID < arr[i].recordID);
        if (before) { pos = i; break; }
    }
    arr.insert(arr.begin() + pos, s);
}

// Interpolation search for recordID within the male-students sub-range.
// Returns index in arr if found, -1 otherwise.
int interpolationSearch(const vector<Student>& arr, unsigned int key) {
    int left = -1, right = -1;
    for (int i = 0; i < (int)arr.size(); i++) {
        if (arr[i].gender == "Male") {
            if (left == -1) left = i;
            right = i;
        }
    }
    if (left == -1) return -1;

    while (left <= right &&
        key >= arr[left].recordID &&
        key <= arr[right].recordID) {
        unsigned int lo = arr[left].recordID;
        unsigned int hi = arr[right].recordID;
        if (lo == hi) return (lo == key) ? left : -1;

        int pos = left + (int)((double)(key - lo) * (right - left) / (hi - lo));
        if (pos < left || pos > right) return -1;
        if (arr[pos].gender != "Male")  return -1;

        if (arr[pos].recordID == key) return pos;
        if (arr[pos].recordID < key)  left = pos + 1;
        else                          right = pos - 1;
    }
    return -1;
}

// ─── BST Node (shared by Levels 2 & 3) ───────────────────────────────────────

class BSTNode {
public:
    Student  data;
    BSTNode* left;
    BSTNode* right;
    BSTNode(const Student& s) : data(s), left(nullptr), right(nullptr) {}
};

static void bfsPrint(BSTNode* root) {
    if (!root) { cout << "  (empty)\n\n"; return; }
    Student::printHeader(true);
    queue<BSTNode*> q;
    q.push(root);
    while (!q.empty()) {
        BSTNode* cur = q.front(); q.pop();
        cur->data.print(true);
        if (cur->left)  q.push(cur->left);
        if (cur->right) q.push(cur->right);
    }
    cout << "\n";
}

static void clearTree(BSTNode* n) {
    if (!n) return;
    clearTree(n->left);
    clearTree(n->right);
    delete n;
}

static BSTNode* rotateRight(BSTNode* y) {
    BSTNode* x = y->left;
    y->left = x->right;
    x->right = y;
    return x;
}

static BSTNode* rotateLeft(BSTNode* x) {
    BSTNode* y = x->right;
    x->right = y->left;
    y->left = x;
    return y;
}

// ─── LEVEL 2: BST — every new node inserted at root via rotations ─────────────

class RootInsertBST {
private:
    BSTNode* root;

    BSTNode* insertAtRoot(BSTNode* node, const Student& s) {
        if (!node) return new BSTNode(s);
        if (s.cityCode < node->data.cityCode) {
            node->left = insertAtRoot(node->left, s);
            return rotateRight(node);
        }
        else if (s.cityCode > node->data.cityCode) {
            node->right = insertAtRoot(node->right, s);
            return rotateLeft(node);
        }
        return node;
    }

    BSTNode* find(BSTNode* node, int key) const {
        if (!node || node->data.cityCode == key) return node;
        return (key < node->data.cityCode) ? find(node->left, key)
            : find(node->right, key);
    }

public:
    RootInsertBST() : root(nullptr) {}
    ~RootInsertBST() { clearTree(root); }

    void insert(const Student& s) {
        root = insertAtRoot(root, s);
        cout << "  Inserted city=" << s.cityCode << " (" << s.lastName << "):\n";
        bfsPrint(root);
    }

    void search(int key) const {
        BSTNode* r = find(root, key);
        cout << "Search city=" << key << ": ";
        if (r) { cout << "Found\n"; Student::printHeader(true); r->data.print(true); }
        else     cout << "Not found\n";
        cout << "\n";
    }
};

// ─── LEVEL 3: Splay Tree (amortized balancing) ────────────────────────────────
// After each insert the newly added node is splayed to the root.

class SplayTree {
private:
    BSTNode* root;

    BSTNode* splay(BSTNode* t, int key) {
        if (!t || t->data.cityCode == key) return t;

        if (key < t->data.cityCode) {
            if (!t->left) return t;
            if (key < t->left->data.cityCode) {          // zig-zig left
                t->left->left = splay(t->left->left, key);
                t = rotateRight(t);
            }
            else if (key > t->left->data.cityCode) {   // zig-zag
                t->left->right = splay(t->left->right, key);
                if (t->left->right) t->left = rotateLeft(t->left);
            }
            return t->left ? rotateRight(t) : t;
        }
        else {
            if (!t->right) return t;
            if (key > t->right->data.cityCode) {         // zig-zig right
                t->right->right = splay(t->right->right, key);
                t = rotateLeft(t);
            }
            else if (key < t->right->data.cityCode) {  // zig-zag
                t->right->left = splay(t->right->left, key);
                if (t->right->left) t->right = rotateRight(t->right);
            }
            return t->right ? rotateLeft(t) : t;
        }
    }

    BSTNode* insertSplay(BSTNode* t, const Student& s) {
        if (!t) return new BSTNode(s);
        t = splay(t, s.cityCode);
        if (t->data.cityCode == s.cityCode) return t;

        BSTNode* n = new BSTNode(s);
        if (s.cityCode < t->data.cityCode) {
            n->right = t;
            n->left = t->left;
            t->left = nullptr;
        }
        else {
            n->left = t;
            n->right = t->right;
            t->right = nullptr;
        }
        return n;
    }

public:
    SplayTree() : root(nullptr) {}
    ~SplayTree() { clearTree(root); }

    void insert(const Student& s) {
        root = insertSplay(root, s);
        cout << "  Inserted city=" << s.cityCode << " (" << s.lastName << "):\n";
        bfsPrint(root);
    }

    void search(int key) {
        root = splay(root, key);
        cout << "Search city=" << key << ": ";
        if (root && root->data.cityCode == key) {
            cout << "Found (splayed to root)\n";
            Student::printHeader(true);
            root->data.print(true);
        }
        else {
            cout << "Not found\n";
        }
        cout << "\n";
    }
};

// ─── main ─────────────────────────────────────────────────────────────────────

int main() {
    vector<Student> raw = {
        {"Kovalenko",    "Ivan",       102, "Male",   10023},
        {"Boyko",        "Olena",      101, "Female", 20011},
        {"Melnyk",       "Serhiy",     103, "Male",   10007},
        {"Ivanchenko",   "Petro",      101, "Male",   10001},
        {"Tymchenko",    "Andriy",     102, "Male",   10015},
        {"Romanenko",    "Dmytro",     103, "Male",   10009},
        {"Ponomarenko",  "Vasyl",      101, "Male",   10020},
        {"Kravchenko",   "Natalia",    102, "Female", 20002},
        {"Savchenko",    "Oksana",     103, "Female", 20018},
        {"Bondarenko",   "Mykola",     101, "Male",   10006},
        {"Lysenko",      "Anna",       102, "Female", 20005},
        {"Marchenko",    "Oleksiy",    103, "Male",   10030},
        {"Petrenko",     "Yulia",      101, "Female", 20014},
        {"Shevchenko",   "Vitaliy",    102, "Male",   10012},
        {"Karpenko",     "Iryna",      103, "Female", 20008},
        {"Moroz",        "Bohdan",     101, "Male",   10025},
        {"Tkachenko",    "Larysa",     102, "Female", 20021},
        {"Rudenko",      "Oleksandr",  103, "Male",   10033},
        {"Yushchenko",   "Sofiia",     101, "Female", 20016},
        {"Kharchenko",   "Denys",      102, "Male",   10017},
        {"Zinchenko",    "Kateryna",   103, "Female", 20027},
        {"Levchenko",    "Hryhorii",   101, "Male",   10040},
    };

    // ── Level 1 ──────────────────────────────────────────────────────────────
    cout << "=== LEVEL 1: Interpolation search among male students ===\n\n";

    vector<Student> arr;
    for (const auto& s : raw) insertOrdered(arr, s);

    cout << "Ordered array (" << arr.size() << " entries):\n";
    printArray(arr);

    for (unsigned int key : {10015u, 10033u, 99999u, 20011u}) {
        int idx = interpolationSearch(arr, key);
        cout << "RecordID=" << key << " -> ";
        if (idx >= 0) { cout << "Found (index " << idx << "): " << arr[idx].lastName; }
        else            cout << "Not found among male students";
        cout << "\n";
    }
    cout << "\n";

    // ── Levels 2 & 3 shared data ─────────────────────────────────────────────
    vector<Student> bstData = {
        {"Kovalenko",   "Ivan",    102, "Male",   10023, 44},
        {"Boyko",       "Olena",   101, "Female", 20011, 32},
        {"Melnyk",      "Serhiy",  103, "Male",   10007, 57},
        {"Ivanchenko",  "Petro",   101, "Male",   10001, 48},
        {"Tymchenko",   "Andriy",  102, "Male",   10015, 22},
        {"Romanenko",   "Dmytro",  103, "Male",   10009, 63},
        {"Ponomarenko", "Vasyl",   101, "Male",   10020, 38},
        {"Kravchenko",  "Natalia", 102, "Female", 20002, 51},
        {"Savchenko",   "Oksana",  103, "Female", 20018, 29},
        {"Bondarenko",  "Mykola",  101, "Male",   10006, 41},
    };

    cout << "=== LEVEL 2: BST with root insertion (key: city code) ===\n\n";
    {
        RootInsertBST bst;
        for (const auto& s : bstData) bst.insert(s);
        bst.search(48);
        bst.search(99);
    }

    cout << "=== LEVEL 3: Splay Tree — amortized balancing (key: city code) ===\n\n";
    {
        SplayTree st;
        for (const auto& s : bstData) st.insert(s);
        st.search(48);
        st.search(99);
    }

    return 0;
}