#include <iostream>
#include <iomanip>
int main()
{
    long long paise = 0;
    double rupees = 0.0;
    float flo = 0.0f;
    int ovf = 0;
    long long vals[] = {1000, 1200, 2000, 2400, 3000, 3600, 1080, 2160, 3240};
    int n = sizeof(vals) / sizeof(vals[0]);
    for (int round = 0; round < 3000000 / n; ++round) {
        for (int i = 0; i < n; ++i) {
            paise += vals[i];
            rupees += vals[i] / 100.0;
            flo += vals[i] / 100.0f;
            ovf += vals[i];
        }
    }
    std::cout << "paise total   = " << paise << "\n";
    std::cout << std::fixed << std::setprecision(4);
    std::cout << "double rupees = " << rupees << "\n";
    std::cout << "float  rupees = " << flo << "\n";
    std::cout << "int   total   = " << ovf << "\n";
}