#pragma once
#include <string>
enum class Zone { Inner, Middle, Outer };
constexpr long long BASE_PAISE[] = {1000, 2000, 3000};
constexpr int PEAK_SURCHARGE_PERCENT = 20;
constexpr int CONCESSION_PERCENT = 10;
constexpr long long MIN_BALANCE_PAISE = 5000;
inline bool isPeak(int hour) { return (hour >= 8 && hour < 10) || (hour >= 17 && hour < 20); }
inline bool toZone(int code, Zone& z)
{
    if (code < 0 || code > 2) return false;
    z = static_cast<Zone>(code);
    return true;
}
inline long long fare(Zone z, int hour, bool concession)
{
    long long f = BASE_PAISE[static_cast<int>(z)];
    if (isPeak(hour)) f += f * PEAK_SURCHARGE_PERCENT / 100;
    if (concession) f -= f * CONCESSION_PERCENT / 100;
    return f;
}
class SmartCard {
public:
    explicit SmartCard(long long paise) : balance_{paise} {}
    bool canTravel(long long f) const { return f >= 0 && balance_ - f >= MIN_BALANCE_PAISE; }
    bool deduct(long long f) { if (!canTravel(f)) return false; balance_ -= f; return true; }
    bool recharge(long long p) { if (p <= 0) return false; balance_ += p; return true; }
    long long balance() const { return balance_; }
private:
    long long balance_;
};