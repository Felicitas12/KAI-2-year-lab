#include <iostream>
#include <string>
#include <iomanip>
#include <queue>
#include <vector>

using namespace std;

class Student {
public:
    string lastName;
    string firstName;
    int course;
    unsigned int studentID;
    string gender;
    string residence;

    Student() : course(0), studentID(0) {}

    Student(const string& ln, const string& fn,
        int c, unsigned int id,
        const string& g, const string& r)
        : lastName(ln), firstName(fn),
        course(c), studentID(id),
        gender(g), residence(r) {
    }

    void print() const {
        cout << left
            << setw(12) << studentID
            << setw(15) << lastName
            << setw(12) << firstName
            << setw(7) << course
            << setw(10) << gender
            << residence << "\n";
    }

    static void printHeader() {
        cout << left
            << setw(12) << "ID"
            << setw(15) << "Last Name"
            << setw(12) << "First Name"
            << setw(7) << "Year"
            << setw(10) << "Gender"
            << "Residence" << "\n";
        cout << string(72, '-') << "\n";
    }
};

class TreeNode {
public:
    Student data;
    TreeNode* left;
    TreeNode* right;

    TreeNode(const Student& s) : data(s), left(nullptr), right(nullptr) {}
};

class BinaryTree {
private:
    TreeNode* root;

    TreeNode* insert(TreeNode* node, const Student& s) {
        if (!node) return new TreeNode(s);
        if (s.studentID < node->data.studentID)
            node->left = insert(node->left, s);
        else if (s.studentID > node->data.studentID)
            node->right = insert(node->right, s);
        else
            cout << "Student with ID " << s.studentID << " already exists. Skipped.\n";
        return node;
    }

    bool matchesCriteria(const Student& s) const {
        return s.gender == "Male" && s.course == 3 && s.residence == "Dormitory";
    }

    TreeNode* findMin(TreeNode* node) const {
        while (node->left) node = node->left;
        return node;
    }

    TreeNode* deleteNode(TreeNode* node, unsigned int id) {
        if (!node) return nullptr;
        if (id < node->data.studentID)
            node->left = deleteNode(node->left, id);
        else if (id > node->data.studentID)
            node->right = deleteNode(node->right, id);
        else {
            if (!node->left && !node->right) {
                delete node;
                return nullptr;
            }
            else if (!node->left) {
                TreeNode* tmp = node->right;
                delete node;
                return tmp;
            }
            else if (!node->right) {
                TreeNode* tmp = node->left;
                delete node;
                return tmp;
            }
            else {
                TreeNode* successor = findMin(node->right);
                node->data = successor->data;
                node->right = deleteNode(node->right, successor->data.studentID);
            }
        }
        return node;
    }

    void collectMatching(TreeNode* node, vector<unsigned int>& ids) const {
        if (!node) return;
        if (matchesCriteria(node->data))
            ids.push_back(node->data.studentID);
        collectMatching(node->left, ids);
        collectMatching(node->right, ids);
    }

    void clear(TreeNode* node) {
        if (!node) return;
        clear(node->left);
        clear(node->right);
        delete node;
    }

public:
    BinaryTree() : root(nullptr) {}
    ~BinaryTree() { clear(root); }

    void insert(const Student& s) {
        root = insert(root, s);
    }

    void printBFS() const {
        if (!root) { cout << "Tree is empty.\n"; return; }
        Student::printHeader();
        queue<TreeNode*> q;
        q.push(root);
        while (!q.empty()) {
            TreeNode* cur = q.front(); q.pop();
            cur->data.print();
            if (cur->left)  q.push(cur->left);
            if (cur->right) q.push(cur->right);
        }
        cout << "\n";
    }

    void searchByCriteria() const {
        cout << "\n--- Search: Male, Year 3, Dormitory ---\n";
        Student::printHeader();
        if (!root) { cout << "Tree is empty.\n"; return; }
        bool found = false;
        queue<TreeNode*> q;
        q.push(root);
        while (!q.empty()) {
            TreeNode* cur = q.front(); q.pop();
            if (matchesCriteria(cur->data)) { cur->data.print(); found = true; }
            if (cur->left)  q.push(cur->left);
            if (cur->right) q.push(cur->right);
        }
        if (!found) cout << "No matching records found.\n";
        cout << "\n";
    }

    void deleteByCriteria() {
        vector<unsigned int> ids;
        collectMatching(root, ids);
        if (ids.empty()) { cout << "No nodes match the criteria.\n"; return; }
        cout << "Deleting nodes with IDs: ";
        for (unsigned int id : ids) cout << id << " ";
        cout << "\n";
        for (unsigned int id : ids)
            root = deleteNode(root, id);
    }
};

int main() {
    vector<Student> students = {
        {"Kovalenko", "Ivan",    3, 5000, "Male",   "Dormitory"},
        {"Boyko",     "Olena",   2, 3000, "Female", "Dormitory"},
        {"Melnyk",    "Serhiy",  4, 2000, "Male",   "Apartment"},
        {"Ivanchenko","Petro",   3, 4000, "Male",   "Apartment"},
        {"Tymchenko", "Andriy",  3, 7000, "Male",   "Dormitory"},
        {"Romanenko", "Dmytro",  3, 8000, "Male",   "Dormitory"},
        {"Ponomarenko","Vasyl",  3, 4500, "Male",   "Dormitory"},
    };

    BinaryTree tree;

    cout << "=== LEVEL 1: Build tree & BFS traversal ===\n\n";
    for (const auto& s : students) tree.insert(s);
    cout << "Tree contents (BFS):\n";
    tree.printBFS();

    cout << "=== LEVEL 2: Search by criteria ===\n";
    tree.searchByCriteria();

    cout << "=== LEVEL 3: Delete by criteria ===\n\n";
    cout << "Before deletion (BFS):\n";
    tree.printBFS();
    tree.deleteByCriteria();
    cout << "\nAfter deletion (BFS):\n";
    tree.printBFS();

    return 0;
}