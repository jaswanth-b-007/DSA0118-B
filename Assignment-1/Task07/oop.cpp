#include <iostream>
constexpr long long MIN_BALANCE_PAISE = 5000;
class SmartCard {
public:
    explicit SmartCard(long long openingPaise) : balance_{openingPaise < 0 ? 0 : openingPaise} {}
    bool canTravel(long long fare) const { return fare >= 0 && balance_ - fare >= MIN_BALANCE_PAISE; }
    bool deduct(long long fare)
    {
        if (!canTravel(fare)) return false;
        balance_ -= fare;
        return true;
    }
    bool recharge(long long paise)
    {
        if (paise <= 0) return false;
        balance_ += paise;
        return true;
    }
    long long balance() const { return balance_; }
private:
    long long balance_;
};
int main()
{
    SmartCard card{10000};
    std::cout << std::boolalpha;
    std::cout << "deduct 1000 -> " << card.deduct(1000) << ", balance " << card.balance() << '\n';
    std::cout << "deduct 3600 -> " << card.deduct(3600) << ", balance " << card.balance() << '\n';
    std::cout << "deduct 2000 -> " << card.deduct(2000) << ", balance " << card.balance() << '\n';
    std::cout << "recharge -500 -> " << card.recharge(-500) << ", balance " << card.balance() << '\n';
    std::cout << "recharge 2000 -> " << card.recharge(2000) << ", balance " << card.balance() << '\n';
}