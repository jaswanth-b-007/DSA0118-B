#include <iostream>
#include "fare_core.hpp"
int passed = 0, failed = 0;
void check(const char* name, long long expected, long long actual)
{
    bool ok = expected == actual;
    (ok ? passed : failed)++;
    std::cout << (ok ? "PASS  " : "FAIL  ") << name << "  expected " << expected << "  actual " << actual << '\n';
}
int main()
{
    std::cout << "--- normal journeys ---\n";
    check("Inner  12:00 off-peak", 1000, fare(Zone::Inner, 12, false));
    check("Middle 12:00 off-peak", 2000, fare(Zone::Middle, 12, false));
    check("Outer  12:00 off-peak", 3000, fare(Zone::Outer, 12, false));
    check("Inner  09:00 peak    ", 1200, fare(Zone::Inner, 9, false));
    check("Middle 18:00 peak    ", 2400, fare(Zone::Middle, 18, false));
    check("Outer  08:00 peak    ", 3600, fare(Zone::Outer, 8, false));
    std::cout << "--- boundary hours (Inner zone) ---\n";
    check("07:00 off-peak", 1000, fare(Zone::Inner, 7, false));
    check("08:00 peak    ", 1200, fare(Zone::Inner, 8, false));
    check("10:00 off-peak", 1000, fare(Zone::Inner, 10, false));
    check("17:00 peak    ", 1200, fare(Zone::Inner, 17, false));
    check("19:00 peak    ", 1200, fare(Zone::Inner, 19, false));
    check("20:00 off-peak", 1000, fare(Zone::Inner, 20, false));
    std::cout << "--- minimum balance boundary ---\n";
    check("balance 6000, fare 1000 (leaves exactly 5000) allowed", 1, SmartCard{6000}.canTravel(1000));
    check("balance 5999, fare 1000 (leaves 4999) denied        ", 0, SmartCard{5999}.canTravel(1000));
    SmartCard c{6000};
    c.deduct(1000);
    check("balance after exact-limit journey", 5000, c.balance());
    check("next journey refused, balance unchanged", 0, c.deduct(1000));
    check("balance still", 5000, c.balance());
    std::cout << "--- invalid input ---\n";
    Zone z = Zone::Inner;
    check("unknown zone 7 rejected ", 0, toZone(7, z));
    check("unknown zone -1 rejected", 0, toZone(-1, z));
    SmartCard r{10000};
    check("negative recharge rejected", 0, r.recharge(-500));
    check("balance unchanged after bad recharge", 10000, r.balance());
    int h = 0;
    check("hour "ab" rejected ", 0, parseHour("ab", h));
    check("hour "9x" rejected ", 0, parseHour("9x", h));
    check("hour "24" rejected ", 0, parseHour("24", h));
    check("hour "-1" rejected ", 0, parseHour("-1", h));
    check("hour "17" accepted ", 1, parseHour("17", h));
    std::cout << "--- five journeys, total calculated by hand = 10640 paise ---\n";
    struct J { Zone z; int hour; bool student; long long byHand; };
    const J js[] = {{Zone::Inner, 7, false, 1000}, {Zone::Middle, 9, false, 2400}, {Zone::Outer, 12, false, 3000},
                    {Zone::Outer, 17, true, 3240}, {Zone::Inner, 20, false, 1000}};
    SmartCard card{50000};
    long long total = 0;
    for (const J& j : js) {
        long long f = fare(j.z, j.hour, j.student);
        check("journey fare", j.byHand, f);
        if (card.deduct(f)) total += f;
    }
    check("total of five journeys", 10640, total);
    check("final balance", 39360, card.balance());
    std::cout << "\nPassed " << passed << ", failed " << failed << '\n';
    return failed == 0 ? 0 : 1;
}
inline bool parseHour(const std::string& s, int& hour)
{
    try {
        std::size_t used = 0;
        int v = std::stoi(s, &used);
        if (used != s.size() || v < 0 || v > 23) return false;
        hour = v;
        return true;
    } catch (...) {
        return false;
    }
}