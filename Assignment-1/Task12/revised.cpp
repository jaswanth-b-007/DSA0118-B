#include <array>
#include <iostream>
enum class Zone { Inner, Middle, Outer, Airport, Count };
constexpr std::array<long long, static_cast<int>(Zone::Count)> BASE_PAISE = {1000, 2000, 3000, 6000};
constexpr int PEAK_SURCHARGE_PERCENT = 20;
constexpr int WEEKEND_DISCOUNT_PERCENT = 10;
constexpr int CONCESSION_PERCENT = 10;
constexpr long long MIN_BALANCE_PAISE = 5000;
bool isPeak(int hour) { return (hour >= 8 && hour < 10) || (hour >= 17 && hour < 20); }
struct Journey { Zone zone; int hour; bool weekend; bool concession; };
long long fare(const Journey& j)
{
    long long f = BASE_PAISE[static_cast<std::size_t>(j.zone)];
    if (j.weekend) f -= f * WEEKEND_DISCOUNT_PERCENT / 100;
    else if (isPeak(j.hour)) f += f * PEAK_SURCHARGE_PERCENT / 100;
    if (j.concession) f -= f * CONCESSION_PERCENT / 100;
    return f;
}
class Pass {
public:
    Pass() = default;
    Pass(Zone a, Zone b) : a_{a}, b_{b}, active_{true} {}
    bool covers(Zone z) const { return active_ && (z == a_ || z == b_); }
private:
    Zone a_{Zone::Inner}, b_{Zone::Inner};
    bool active_ = false;
};
class SmartCard {
public:
    explicit SmartCard(long long paise) : balance_{paise} {}
    void attach(const Pass& p) { pass_ = p; }
    bool canTravel(long long f) const { return f >= 0 && balance_ - f >= MIN_BALANCE_PAISE; }
    long long travel(const Journey& j)
    {
        if (pass_.covers(j.zone)) return 0;
        long long f = fare(j);
        if (!canTravel(f)) return -1;
        balance_ -= f;
        return f;
    }
    long long balance() const { return balance_; }
private:
    long long balance_;
    Pass pass_;
};
int main()
{
    std::cout << "Airport weekday 12:00 off-peak:        " << fare({Zone::Airport, 12, false, false}) << '\n';
    std::cout << "Airport weekday 09:00 peak:            " << fare({Zone::Airport, 9, false, false}) << '\n';
    std::cout << "Outer Saturday 09:00 discount:         " << fare({Zone::Outer, 9, true, false}) << '\n';
    std::cout << "Middle Sunday 12:00 discount:          " << fare({Zone::Middle, 12, true, false}) << '\n';
    std::cout << "Inner weekday 07:00 unchanged:         " << fare({Zone::Inner, 7, false, false}) << '\n';
}