#include <iostream>
#include <iomanip>

double mySqrt(int A) {
    double x = A / 2;

    if (A <= 0)
        return 0;
    if (A == 1)
        return 1;
    while (std::abs(A - x * x) > 0.00001) {
        x = (x + A / x) / 2;
    }
    return x;
}

int main()  {
    int A = 0;

    std::cout << std::fixed << std::setprecision(2);
    // std::cout.unsetf(std::ios_base::fixed);
    // std::cout << std::setprecision(-1);

    std::cout << "Enter A:" << std::endl;
    std::cin >> A;
    std::cout << "Sqrt(" << A << ") = " << mySqrt(A) << std::endl;
}

