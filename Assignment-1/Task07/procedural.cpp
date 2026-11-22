#include <iostream>
constexpr long long MIN_BALANCE_PAISE = 5000;
long long cardBalance = 10000;
bool canTravel(long long fare) { return cardBalance - fare >= MIN_BALANCE_PAISE; }
void deduct(long long fare)
{
    if (canTravel(fare)) cardBalance -= fare;
}
int main()
{
    deduct(1000);
    deduct(3600);
    deduct(2000);
    std::cout << "procedural balance = " << cardBalance << " paise\n";
    cardBalance = -500;
    std::cout << "after careless assignment = " << cardBalance << " paise\n";
}