#include <iomanip>
#include <iostream>
class Money {
    long long paise;
public:
    Money(long long rupees = 0, int p = 0) : paise(rupees * 100 + p) {}
    Money operator+(const Money& o) const { return Money(0, paise + o.paise); }
    bool operator==(const Money& o) const { return paise == o.paise; }
    friend std::ostream& operator<<(std::ostream& os, const Money& m)
    {
        long long r = m.paise / 100, p = m.paise % 100;
        if (p < 0) { r -= 1; p = -p; }
        os << "Rs " << r << '.' << std::setw(2) << std::setfill('0') << p << std::setfill(' ');
        return os;
    }
};
int main()
{
    Money a(120, 75), b(79, 25);
    Money c = a + b;
    std::cout << c << '\n' << std::boolalpha << (c == Money(200)) << '\n';
}