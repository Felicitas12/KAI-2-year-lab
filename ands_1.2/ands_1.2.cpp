#include <iostream>
#include <cmath>
#include <cstdlib>
#include <ctime>
#include <cstdio>
#include <stdexcept>
#include <string>

using namespace std;

class Square {
private:
    double vx[4], vy[4];
    double side;

    void initVertices(double x0, double y0) {
        vx[0] = x0;        vy[0] = y0;
        vx[1] = x0 + side; vy[1] = y0;
        vx[2] = x0 + side; vy[2] = y0 + side;
        vx[3] = x0;        vy[3] = y0 + side;
    }

public:
    Square() : side(0.0) {
        for (int i = 0; i < 4; i++) vx[i] = vy[i] = 0.0;
    }

    Square(double x0, double y0, double a) {
        if (a <= 0.0) throw invalid_argument("Side length must be positive");
        side = a;
        initVertices(x0, y0);
    }

    double area()      const { return side * side; }
    double perimeter() const { return 4.0 * side; }
    double getSide()   const { return side; }
    double key()       const { return area(); }

    void print() const {
        printf("side=%5.2f  area=%7.2f  perim=%6.2f"
            "  | V:(%.1f,%.1f)(%.1f,%.1f)(%.1f,%.1f)(%.1f,%.1f)",
            side, area(), perimeter(),
            vx[0], vy[0], vx[1], vy[1],
            vx[2], vy[2], vx[3], vy[3]);
    }
};

Square randomSquare(int maxSide = 10) {
    double x = static_cast<double>(rand() % 21) - 10.0;
    double y = static_cast<double>(rand() % 21) - 10.0;
    double side = 1.0 + static_cast<double>(rand() % maxSide);
    return Square(x, y, side);
}

enum SlotState { EMPTY, OCCUPIED, DELETED };

struct Slot {
    Square    elem;
    double    key;
    SlotState state;
    Slot() : key(0.0), state(EMPTY) {}
};

class HashTable {
private:
    Slot* table;
    int   size;

    int hashMul(double k) const {
        static const double A = 0.6180339887498948;
        long long ki = static_cast<long long>(fabs(k));
        double    fr = static_cast<double>(ki) * A;
        fr -= floor(fr);
        return static_cast<int>(size * fr) % size;
    }

public:
    explicit HashTable(int sz) : size(sz) {
        if (sz <= 0) throw invalid_argument("Size must be positive");
        table = new Slot[size];
    }

    ~HashTable() { delete[] table; }

    void clear() {
        for (int i = 0; i < size; i++) table[i].state = EMPTY;
    }

    bool insertSimple(const Square& sq) {
        double k = sq.key();
        int    h0 = hashMul(k);

        if (table[h0].state == OCCUPIED) {
            printf("  [COLLISION]  pos=%2d, key=%.0f  -> NOT inserted\n", h0, k);
            return false;
        }
        table[h0].elem = sq;
        table[h0].key = k;
        table[h0].state = OCCUPIED;
        printf("  [+] pos=%2d, key=%.0f  -> inserted\n", h0, k);
        return true;
    }

    bool insertQuadratic(const Square& sq) {
        double k = sq.key();
        int    h0 = hashMul(k);

        for (int i = 0; i < size; i++) {
            int pos = (h0 + i * i) % size;
            if (table[pos].state != OCCUPIED) {
                table[pos].elem = sq;
                table[pos].key = k;
                table[pos].state = OCCUPIED;
                if (i == 0)
                    printf("  [+] pos=%2d, key=%.0f\n", pos, k);
                else
                    printf("  [PROBE i=%d] pos=%2d, key=%.0f  -> collision resolved\n", i, pos, k);
                return true;
            }
        }
        printf("  [FAIL] key=%.0f -- table full / probing cycle\n", k);
        return false;
    }

    int deleteByPerimeter(double threshold) {
        int count = 0;
        for (int i = 0; i < size; i++) {
            if (table[i].state == OCCUPIED &&
                table[i].elem.perimeter() > threshold) {
                printf("  [-] pos=%2d  perim=%.2f > %.2f  -> deleted\n",
                    i, table[i].elem.perimeter(), threshold);
                table[i].state = DELETED;
                count++;
            }
        }
        return count;
    }

    void print() const {
        const string sep(90, '-');
        printf("\n%s\n", sep.c_str());
        printf("%-4s  %-8s  %s\n", "Pos", "Key", "Element");
        printf("%s\n", sep.c_str());
        for (int i = 0; i < size; i++) {
            printf("[%2d]  ", i);
            switch (table[i].state) {
            case EMPTY:    printf("(empty)\n");   break;
            case DELETED:  printf("(deleted)\n"); break;
            case OCCUPIED:
                printf("%-8.0f  ", table[i].key);
                table[i].elem.print();
                printf("\n");
                break;
            }
        }
        printf("%s\n", sep.c_str());
    }
};

int main() {
    srand(static_cast<unsigned>(time(nullptr)));

    int sz;
    printf("Enter hash table size (recommended prime, e.g. 11 or 13): ");
    scanf_s("%d", &sz);
    if (sz <= 0) { printf("Invalid size.\n"); return 1; }

    printf("\n========== LEVEL 1: Insert WITHOUT collision resolution ==========\n");
    printf("Hash function: multiplication method  h(k) = floor(m * frac(k * A))\n");
    printf("Key: square area (side * side)\n");
    printf("Inserting squares with side = 1, 2, ..., %d\n\n", sz);
    {
        HashTable ht(sz);
        for (int s = 1; s <= sz; s++) {
            Square sq(0.0, 0.0, static_cast<double>(s));
            ht.insertSimple(sq);
        }
        printf("\nHash table contents:\n");
        ht.print();
    }

    printf("\n========== LEVEL 2: Quadratic probing  h(k,i) = (h0 + i^2) %% m ==========\n");
    printf("Inserting %d random squares:\n\n", sz);

    HashTable ht2(sz);
    {
        int inserted = 0, attempts = 0;
        while (inserted < sz && attempts < sz * 5) {
            Square sq = randomSquare();
            if (ht2.insertQuadratic(sq)) inserted++;
            attempts++;
        }
        printf("\nHash table contents:\n");
        ht2.print();
    }

    printf("\n========== LEVEL 3: Delete elements where perimeter > threshold ==========\n");
    {
        double threshold;
        printf("Enter perimeter threshold: ");
        scanf_s("%lf", &threshold);

        printf("\nDeleting elements with perimeter > %.2f:\n\n", threshold);
        int deleted = ht2.deleteByPerimeter(threshold);
        printf("\nTotal deleted: %d\n", deleted);

        printf("\nHash table after deletion:\n");
        ht2.print();
    }

    return 0;
}