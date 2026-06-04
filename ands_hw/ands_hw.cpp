// ============================================================
//  Combined Tasks: LUP Decomposition | Graph DFS | Runge-Kutta
// ============================================================

#include <iostream>
#include <vector>
#include <list>
#include <stack>
#include <algorithm>
#include <iomanip>
#include <cmath>
#include <functional>

using namespace std;

// ============================================================
//  TASK 1 — LUP Decomposition (solving Ax = b)
// ============================================================

namespace Task1 {

    typedef vector<vector<double>> Matrix;
    typedef vector<double> Vec;

    void printMatrix(const Matrix& M, const string& name) {
        int n = M.size();
        cout << name << ":\n";
        for (int i = 0; i < n; i++) {
            cout << "[ ";
            for (int j = 0; j < n; j++)
                cout << setw(10) << fixed << setprecision(4) << M[i][j] << " ";
            cout << "]\n";
        }
        cout << "\n";
    }

    void printVector(const Vec& v, const string& name) {
        cout << name << ": [ ";
        for (double x : v) cout << setw(10) << fixed << setprecision(4) << x << " ";
        cout << "]\n\n";
    }

    void printSystem(const Matrix& A, const Vec& b) {
        int n = A.size();
        cout << "System of linear equations:\n";
        for (int i = 0; i < n; i++) {
            for (int j = 0; j < n; j++) {
                if (j > 0 && A[i][j] >= 0) cout << " + ";
                else if (j > 0 && A[i][j] < 0) cout << " ";
                cout << fixed << setprecision(1) << A[i][j] << "*x" << (j + 1);
            }
            cout << " = " << fixed << setprecision(1) << b[i] << "\n";
        }
        cout << "\n";
    }

    void lupDecompose(Matrix A, Matrix& L, Matrix& U, Matrix& P) {
        int n = A.size();
        L = Matrix(n, Vec(n, 0.0));
        U = Matrix(n, Vec(n, 0.0));
        P = Matrix(n, Vec(n, 0.0));

        vector<int> perm(n);
        for (int i = 0; i < n; i++) perm[i] = i;
        for (int i = 0; i < n; i++) L[i][i] = 1.0;

        for (int k = 0; k < n; k++) {
            double maxVal = 0.0;
            int maxRow = k;
            for (int i = k; i < n; i++) {
                if (fabs(A[i][k]) > maxVal) { maxVal = fabs(A[i][k]); maxRow = i; }
            }
            if (maxRow != k) {
                swap(A[k], A[maxRow]);
                swap(perm[k], perm[maxRow]);
                for (int j = 0; j < k; j++) swap(L[k][j], L[maxRow][j]);
            }
            for (int i = k + 1; i < n; i++) {
                L[i][k] = A[i][k] / A[k][k];
                for (int j = k; j < n; j++) A[i][j] -= L[i][k] * A[k][j];
            }
            for (int j = k; j < n; j++) U[k][j] = A[k][j];
        }
        for (int i = 0; i < n; i++) P[i][perm[i]] = 1.0;
    }

    Vec forwardSubstitution(const Matrix& L, const Vec& b) {
        int n = L.size();
        Vec y(n, 0.0);
        for (int i = 0; i < n; i++) {
            y[i] = b[i];
            for (int j = 0; j < i; j++) y[i] -= L[i][j] * y[j];
        }
        return y;
    }

    Vec backwardSubstitution(const Matrix& U, const Vec& y) {
        int n = U.size();
        Vec x(n, 0.0);
        for (int i = n - 1; i >= 0; i--) {
            x[i] = y[i];
            for (int j = i + 1; j < n; j++) x[i] -= U[i][j] * x[j];
            x[i] /= U[i][i];
        }
        return x;
    }

    Vec multiplyMatVec(const Matrix& M, const Vec& v) {
        int n = M.size();
        Vec result(n, 0.0);
        for (int i = 0; i < n; i++)
            for (int j = 0; j < n; j++)
                result[i] += M[i][j] * v[j];
        return result;
    }

    void run() {
        cout << "\n=== TASK 1: LUP Decomposition ===\n\n";
        int n;
        cout << "Enter the size of the system: ";
        cin >> n;

        Matrix A(n, Vec(n));
        Vec b(n);

        cout << "Enter matrix A row by row:\n";
        for (int i = 0; i < n; i++) {
            cout << "Row " << (i + 1) << ": ";
            for (int j = 0; j < n; j++) cin >> A[i][j];
        }
        cout << "Enter right-hand side vector b:\n";
        for (int i = 0; i < n; i++) { cout << "b[" << (i + 1) << "]: "; cin >> b[i]; }

        printSystem(A, b);
        Matrix L, U, P;
        lupDecompose(A, L, U, P);
        printMatrix(L, "L (lower triangular)");
        printMatrix(U, "U (upper triangular)");
        printMatrix(P, "P (permutation matrix)");

        Vec Pb = multiplyMatVec(P, b);
        Vec y = forwardSubstitution(L, Pb);
        Vec x = backwardSubstitution(U, y);

        cout << "Solution:\n";
        for (int i = 0; i < n; i++)
            cout << "x" << (i + 1) << " = " << fixed << setprecision(6) << x[i] << "\n";
    }

} // namespace Task1


// ============================================================
//  TASK 2 — Student Email Graph (Circular-K Graph + DFS)
// ============================================================

namespace Task2 {

    class Graph {
    private:
        int numVertices;
        vector<list<int>> adjList;

        void addEdgeOnce(int u, int v) {
            for (int x : adjList[u]) if (x == v) return;
            adjList[u].push_back(v);
            adjList[v].push_back(u);
        }

    public:
        Graph(int n) : numVertices(n), adjList(n) {}

        void buildCircularKGraph(int k) {
            for (int i = 0; i < numVertices; i++)
                for (int d = 1; d <= k; d++)
                    addEdgeOnce(i, (i + d) % numVertices);
        }

        void printAdjList() const {
            cout << "Adjacency list representation:\n";
            for (int i = 0; i < numVertices; i++) {
                cout << "Student " << setw(2) << i << ": ";
                for (int nb : adjList[i]) cout << nb << " ";
                cout << "\n";
            }
            cout << "\n";
        }

        void dfs(int startVertex) {
            vector<bool> visited(numVertices, false);
            stack<int> st;

            cout << "DFS traversal starting from student " << startVertex << ":\n";
            st.push(startVertex);

            while (!st.empty()) {
                int v = st.top(); st.pop();
                if (visited[v]) continue;
                visited[v] = true;
                cout << "  -> Student " << v << "\n";

                vector<int> neighbors(adjList[v].begin(), adjList[v].end());
                sort(neighbors.rbegin(), neighbors.rend());
                for (int nb : neighbors)
                    if (!visited[nb]) st.push(nb);
            }
            cout << "\n";
        }

        bool verifyMinDegree(int minDeg) const {
            for (int i = 0; i < numVertices; i++)
                if ((int)adjList[i].size() < minDeg) return false;
            return true;
        }

        int degree(int v) const { return (int)adjList[v].size(); }
    };

    void run() {
        cout << "\n=== TASK 2: NAU Student Email Graph ===\n";
        cout << "24 students went on a trip to Italy.\n";
        cout << "After returning, some pairs exchanged email addresses.\n";
        cout << "Each student has at least 11 email contacts.\n\n";

        Graph g(24);
        g.buildCircularKGraph(11);

        cout << "Each student has exactly 22 contacts (11 forward + 11 backward).\n";
        cout << "Min degree >= 11 check: "
            << (g.verifyMinDegree(11) ? "PASSED" : "FAILED") << "\n\n";

        g.printAdjList();

        int startVertex;
        cout << "Enter starting vertex for DFS traversal (0 to 23): ";
        cin >> startVertex;

        if (startVertex < 0 || startVertex >= 24) {
            cout << "Invalid input. Defaulting to vertex 0.\n";
            startVertex = 0;
        }

        g.dfs(startVertex);
        cout << "Direct email contacts of student " << startVertex
            << ": " << g.degree(startVertex) << "\n";
    }

} // namespace Task2


// ============================================================
//  TASK 3 — Runge-Kutta 3rd Order Method
// ============================================================

namespace Task3 {

    typedef function<double(double, double)> OdeFunc;

    struct TableRow {
        double t, y, k1, k2, k3;
    };

    vector<TableRow> rungeKutta3(OdeFunc f, double t0, double y0, double h, double tEnd) {
        vector<TableRow> table;
        double t = t0, y = y0;

        while (t <= tEnd + 1e-10) {
            double k1 = h * f(t, y);
            double k2 = h * f(t + h / 2.0, y + k1 / 2.0);
            double k3 = h * f(t + h, y - k1 + 2.0 * k2);

            table.push_back({ t, y, k1, k2, k3 });

            y = y + (k1 + 4.0 * k2 + k3) / 6.0;
            t += h;
        }
        return table;
    }

    void printTable(const vector<TableRow>& table) {
        cout << setw(10) << "t"
            << setw(14) << "y(t)"
            << setw(14) << "k1"
            << setw(14) << "k2"
            << setw(14) << "k3" << "\n";
        cout << string(66, '-') << "\n";
        for (const auto& row : table) {
            cout << setw(10) << fixed << setprecision(4) << row.t
                << setw(14) << fixed << setprecision(6) << row.y
                << setw(14) << fixed << setprecision(6) << row.k1
                << setw(14) << fixed << setprecision(6) << row.k2
                << setw(14) << fixed << setprecision(6) << row.k3 << "\n";
        }
        cout << "\n";
    }

    void printAsciiPlot(const vector<TableRow>& table) {
        int width = 60, height = 20;
        double tMin = table.front().t, tMax = table.back().t;
        double yMin = table[0].y, yMax = table[0].y;
        for (const auto& row : table) {
            if (row.y < yMin) yMin = row.y;
            if (row.y > yMax) yMax = row.y;
        }
        double yRange = yMax - yMin;
        if (fabs(yRange) < 1e-12) yRange = 1.0;

        vector<string> grid(height, string(width, ' '));
        for (const auto& row : table) {
            int col = (int)((row.t - tMin) / (tMax - tMin) * (width - 1));
            int r = (int)((yMax - row.y) / yRange * (height - 1));
            if (col >= 0 && col < width && r >= 0 && r < height)
                grid[r][col] = '*';
        }

        cout << "y(t) plot (ASCII):\n";
        cout << fixed << setprecision(4);
        cout << setw(10) << yMax << " |\n";
        for (int r = 0; r < height; r++)
            cout << "           |" << grid[r] << "\n";
        cout << "           +" << string(width, '-') << "\n";
        cout << setw(12) << tMin << setw(width - 4) << tMax << "\n";
        cout << "            t\n\n";
    }

    void run() {
        cout << "\n=== TASK 3: Runge-Kutta 3rd Order Method ===\n\n";
        cout << "Solving ODE: dy/dt = f(t, y)\n";
        cout << "Default equation: dy/dt = -2*t*y (Gaussian decay)\n\n";

        double t0, y0, h, tEnd;
        cout << "Enter initial time t0: ";       cin >> t0;
        cout << "Enter initial value y(t0): ";   cin >> y0;
        cout << "Enter step size h: ";           cin >> h;
        cout << "Enter end time: ";              cin >> tEnd;

        OdeFunc f = [](double t, double y) { return -2.0 * t * y; };
        auto exactSolution = [](double t, double y0e, double t0e) {
            return y0e * exp(-(t * t - t0e * t0e));
            };

        cout << "\nComputing with Runge-Kutta 3rd order...\n\n";
        vector<TableRow> result = rungeKutta3(f, t0, y0, h, tEnd);

        cout << "Results table:\n";
        printTable(result);

        cout << "Comparison with exact solution y(t) = y0 * exp(-(t^2 - t0^2)):\n";
        cout << setw(10) << "t"
            << setw(14) << "y_rk3"
            << setw(14) << "y_exact"
            << setw(14) << "error" << "\n";
        cout << string(52, '-') << "\n";
        for (const auto& row : result) {
            double exact = exactSolution(row.t, y0, t0);
            double err = fabs(row.y - exact);
            cout << setw(10) << fixed << setprecision(4) << row.t
                << setw(14) << fixed << setprecision(6) << row.y
                << setw(14) << fixed << setprecision(6) << exact
                << setw(14) << scientific << setprecision(2) << err << "\n";
        }
        cout << "\n";

        printAsciiPlot(result);
    }

} // namespace Task3


// ============================================================
//  MAIN — Task Selection Menu
// ============================================================

int main() {
    cout << "========================================\n";
    cout << "  Numerical Methods & Graph Algorithms  \n";
    cout << "========================================\n";
    cout << "Select a task to run:\n";
    cout << "  1 — LUP Decomposition (solve Ax = b)\n";
    cout << "  2 — Student Email Graph (DFS)\n";
    cout << "  3 — Runge-Kutta 3rd Order Method\n";
    cout << "  0 — Exit\n";
    cout << "----------------------------------------\n";

    int choice;
    do {
        cout << "\nYour choice: ";
        cin >> choice;
        switch (choice) {
        case 1: Task1::run(); break;
        case 2: Task2::run(); break;
        case 3: Task3::run(); break;
        case 0: cout << "Goodbye!\n"; break;
        default: cout << "Invalid choice. Try again.\n";
        }
    } while (choice != 0);

    return 0;
}