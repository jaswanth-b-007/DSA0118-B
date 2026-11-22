#include <climits>
#include <cfloat>
#include <iomanip>
#include <iostream>
int main()
{
    std::cout << "--- sizeof and range on this system ---
";
    std::cout << "bool          " << sizeof(bool) << " byte\n";
    std::cout << "char          " << sizeof(char) << " byte   [" << CHAR_MIN << ", " << CHAR_MAX << "]\n";
    std::cout << "int           " << sizeof(int) << " bytes  [" << INT_MIN << ", " << INT_MAX << "]\n";
    std::cout << "unsigned int  " << sizeof(unsigned) << " bytes  [0, " << UINT_MAX << "]\n";
    std::cout << "long long     " << sizeof(long long) << " bytes  [" << LLONG_MIN << ", " << LLONG_MAX << "]\n";
    std::cout << "float         " << sizeof(float) << " bytes  (about " << FLT_DIG << " digits)\n";
    std::cout << "double        " << sizeof(double) << " bytes  (about " << DBL_DIG << " digits)\n";
    std::cout << "\n--- type compatibility: int + double + bool ---\n";
    int i = 3; double d = 2.5; bool b = true;
    std::cout << "i + d + b = " << i + d + b << ", sizeof(i + d + b) = " << sizeof(i + d + b)
              << " (everything is promoted to double)\n";
    std::cout << "\n--- is int enough for a day's collection? ---\n";
    long long journeys = 3000000, avgFare = 2000;
    std::cout << "3,000,000 x 2000 paise = " << journeys * avgFare << " paise; INT_MAX = " << INT_MAX << '\n';
    std::cout << "\n--- precision problem: 0.1 is not exact in binary ---\n";
    std::cout << std::setprecision(17) << "0.1 + 0.2 = " << 0.1 + 0.2 << ", (0.1 + 0.2 == 0.3) = "
              << std::boolalpha << (0.1 + 0.2 == 0.3) << '\n';
}