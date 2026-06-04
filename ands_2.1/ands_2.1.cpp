#include <iostream>
#include <iomanip>
#include <cmath>
#include <stdexcept>

double f1(double x) {
    return 1.0 / std::sqrt(x * x + 9.0);
}

double f2(double x) {
    return x * x * x * x - x - 1.0;
}

double f2d(double x) {
    return 4.0 * x * x * x - 1.0;
}

double f3(double x, double y) {
    return std::exp(-x) - 2.0 * x;
}

double trapezoid(double a, double b, double h) {
    int n = static_cast<int>((b - a) / h);
    double sum = 0.5 * (f1(a) + f1(b));
    for (int i = 1; i < n; ++i)
        sum += f1(a + i * h);
    return sum * h;
}

double midpoint(double a, double b, double h) {
    int n = static_cast<int>((b - a) / h);
    double sum = 0.0;
    for (int i = 0; i < n; ++i)
        sum += f1(a + (i + 0.5) * h);
    return sum * h;
}

double simpson(double a, double b, double h) {
    int n = static_cast<int>((b - a) / h);
    if (n % 2 != 0) n++;
    double step = (b - a) / n;
    double sum = f1(a) + f1(b);
    for (int i = 1; i < n; ++i)
        sum += f1(a + i * step) * (i % 2 == 0 ? 2.0 : 4.0);
    return sum * step / 3.0;
}

double bisection(double a, double b, double eps) {
    if (f2(a) * f2(b) > 0)
        throw std::runtime_error("No sign change on interval.");
    double mid = a;
    while ((b - a) / 2.0 > eps) {
        mid = (a + b) / 2.0;
        if (f2(mid) == 0.0) break;
        else if (f2(a) * f2(mid) < 0) b = mid;
        else a = mid;
    }
    return (a + b) / 2.0;
}

double newton(double x0, double eps, int maxIter = 1000) {
    double x = x0;
    for (int i = 0; i < maxIter; ++i) {
        double fx = f2(x);
        double fpx = f2d(x);
        if (std::abs(fpx) < 1e-14)
            throw std::runtime_error("Derivative near zero.");
        double x1 = x - fx / fpx;
        if (std::abs(x1 - x) < eps) return x1;
        x = x1;
    }
    return x;
}

double secant(double a, double b, double eps, int maxIter = 1000) {
    if (f2(a) * f2(b) > 0)
        throw std::runtime_error("No sign change on interval.");
    for (int i = 0; i < maxIter; ++i) {
        double fa = f2(a), fb = f2(b);
        double c = b - fb * (b - a) / (fb - fa);
        if (std::abs(c - b) < eps) return c;
        if (f2(a) * f2(c) < 0) b = c;
        else a = c;
    }
    return (a + b) / 2.0;
}

void rungeKutta3(double x0, double y0, double xEnd, double h) {
    std::cout << std::fixed << std::setprecision(6);
    std::cout << "\n" << std::setw(12) << "x"
        << std::setw(16) << "y" << "\n";
    std::cout << std::string(28, '-') << "\n";

    double x = x0, y = y0;
    std::cout << std::setw(12) << x << std::setw(16) << y << "\n";

    while (x < xEnd - 1e-9) {
        double k1 = h * f3(x, y);
        double k2 = h * f3(x + h / 2.0, y + k1 / 2.0);
        double k3 = h * f3(x + h, y - k1 + 2.0 * k2);
        y += (k1 + 4.0 * k2 + k3) / 6.0;
        x += h;
        std::cout << std::setw(12) << x << std::setw(16) << y << "\n";
    }
}

int main() {
    std::cout << "============================================\n";
    std::cout << "       Lab 2.1 – Numerical Methods        \n";
    std::cout << "============================================\n";

    {
        std::cout << "\n=== TASK 1: Numerical Integration ===\n";
        std::cout << "f(x) = 1/sqrt(x^2+9),  [a,b] = [3, 8]\n\n";

        double a, b, h;
        std::cout << "Enter a (default 3): ";
        std::cin >> a;
        std::cout << "Enter b (default 8): ";
        std::cin >> b;
        std::cout << "Enter h (default 1.0): ";
        std::cin >> h;

        std::cout << std::fixed << std::setprecision(8);
        std::cout << "\nTrapezoid method : " << trapezoid(a, b, h) << "\n";
        std::cout << "Midpoint method  : " << midpoint(a, b, h) << "\n";
        std::cout << "Simpson method   : " << simpson(a, b, h) << "\n";
        std::cout << "Exact (reference): " << (std::log(b + std::sqrt(b * b + 9)) - std::log(a + std::sqrt(a * a + 9))) << "\n";
    }

    {
        std::cout << "\n=== TASK 2: Root Finding ===\n";
        std::cout << "f(x) = x^4 - x - 1\n\n";

        double a, b, eps;
        std::cout << "Enter interval a: ";
        std::cin >> a;
        std::cout << "Enter interval b: ";
        std::cin >> b;
        std::cout << "Enter tolerance eps: ";
        std::cin >> eps;

        std::cout << std::fixed << std::setprecision(10);

        try {
            double r = bisection(a, b, eps);
            std::cout << "Bisection method : x = " << r << "  f(x) = " << f2(r) << "\n";
        }
        catch (const std::exception& e) {
            std::cout << "Bisection: " << e.what() << "\n";
        }

        try {
            double x0 = (a + b) / 2.0;
            double r = newton(x0, eps);
            std::cout << "Newton method    : x = " << r << "  f(x) = " << f2(r) << "\n";
        }
        catch (const std::exception& e) {
            std::cout << "Newton: " << e.what() << "\n";
        }

        try {
            double r = secant(a, b, eps);
            std::cout << "Secant method    : x = " << r << "  f(x) = " << f2(r) << "\n";
        }
        catch (const std::exception& e) {
            std::cout << "Secant: " << e.what() << "\n";
        }
    }

    {
        std::cout << "\n=== TASK 3: ODE – Runge-Kutta 3rd Order ===\n";
        std::cout << "dy/dx = e^(-x) - 2x\n\n";

        double x0, y0, xEnd, h;
        std::cout << "Enter initial x0: ";
        std::cin >> x0;
        std::cout << "Enter initial y0: ";
        std::cin >> y0;
        std::cout << "Enter final x: ";
        std::cin >> xEnd;
        std::cout << "Enter step h: ";
        std::cin >> h;

        rungeKutta3(x0, y0, xEnd, h);
    }

    return 0;
}