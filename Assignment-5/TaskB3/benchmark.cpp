#include <chrono>
#include <iostream>
#include <memory>
#include <stdexcept>
#include <variant>
#include <vector>

volatile double sink = 0.0;
volatile long long acceptedSink = 0;

// ---------------- virtual version ----------------
class Bill {
public:
    virtual ~Bill() = default;
    virtual double total() const = 0;
};

struct OP : Bill {
    double consult = 0.0;
    double tests = 0.0;
    OP() = default;
    OP(double c, double t) : consult(c), tests(t) {}
    double total() const override { return consult + tests; }
};

struct IP : Bill {
    int days = 0;
    double rate = 0.0;
    double surgery = 0.0;
    IP() = default;
    IP(int d, double r, double s) : days(d), rate(r), surgery(s) {}
    double total() const override { return days * rate + surgery; }
};

// ---------------- switch version ----------------
struct SwitchBill {
    int type = 0;      // 0 = OP, 1 = IP
    double a = 0.0;
    double b = 0.0;
    int c = 0;
};

double switchTotal(const SwitchBill& s)
{
    switch (s.type) {
        case 0: return s.a + s.b;
        case 1: return s.c * s.a + s.b;
        default: return 0.0;
    }
}

// ---------------- variant version ----------------
double variantTotal(const std::variant<OP, IP>& v)
{
    return std::visit([](const auto& b) { return b.total(); }, v);
}

// ---------------- filling helpers ----------------
SwitchBill makeSwitch(std::size_t i)
{
    SwitchBill s;
    if (i % 2 == 0) {
        std::size_t k = i / 2;
        s.type = 0;
        s.a = 100.0 + k * 10;
        s.b = 200.0 + k * 20;
    } else {
        std::size_t k = i / 2;
        s.type = 1;
        s.a = 100.0;
        s.b = 500.0 + k * 50;
        s.c = 2;
    }
    return s;
}

// ---------------- timer ----------------
template <class F>
double ms(F fn)
{
    auto start = std::chrono::steady_clock::now();
    fn();
    auto end = std::chrono::steady_clock::now();
    return std::chrono::duration<double, std::milli>(end - start).count();
}

void correctnessCheck()
{
    const std::size_t n = 10;
    std::vector<OP> ops(n / 2);
    std::vector<IP> ips(n / 2);
    std::vector<Bill*> bills;
    std::vector<SwitchBill> sw(n);
    std::vector<std::variant<OP, IP>> var(n);

    std::size_t oi = 0, ii = 0;
    for (std::size_t i = 0; i < n; ++i) {
        sw[i] = makeSwitch(i);
        if (i % 2 == 0) {
            ops[oi] = OP(sw[i].a, sw[i].b);
            var[i] = ops[oi];
            bills.push_back(&ops[oi++]);
        } else {
            ips[ii] = IP(sw[i].c, sw[i].a, sw[i].b);
            var[i] = ips[ii];
            bills.push_back(&ips[ii++]);
        }
    }

    double virtualSum = 0, switchSum = 0, variantSum = 0;
    for (Bill* b : bills) virtualSum += b->total();
    for (const auto& s : sw) switchSum += switchTotal(s);
    for (const auto& v : var) variantSum += variantTotal(v);

    std::cout << "correctness n=10: virtual " << virtualSum
              << " switch " << switchSum
              << " variant " << variantSum << '\n';
}

void billBenchmark(std::size_t n)
{
    std::vector<OP> ops(n / 2);
    std::vector<IP> ips(n - n / 2);
    std::vector<Bill*> bills;
    bills.reserve(n);
    std::vector<SwitchBill> sw(n);
    std::vector<std::variant<OP, IP>> var(n);

    std::size_t oi = 0, ii = 0;
    for (std::size_t i = 0; i < n; ++i) {
        sw[i] = makeSwitch(i);
        if (i % 2 == 0) {
            ops[oi] = OP(sw[i].a, sw[i].b);
            var[i] = ops[oi];
            bills.push_back(&ops[oi++]);
        } else {
            ips[ii] = IP(sw[i].c, sw[i].a, sw[i].b);
            var[i] = ips[ii];
            bills.push_back(&ips[ii++]);
        }
    }

    double vt = ms([&] {
        for (Bill* b : bills) sink += b->total();
    });
    double st = ms([&] {
        for (const auto& s : sw) sink += switchTotal(s);
    });
    double vat = ms([&] {
        for (const auto& v : var) sink += variantTotal(v);
    });

    std::cout << "bills n=" << n
              << " virtual_ms=" << vt
              << " switch_ms=" << st
              << " variant_ms=" << vat << '\n';
}

enum class Status { Ok, Negative, TooLarge };

Status validateCode(double amount, double& out)
{
    if (amount < 0) return Status::Negative;
    if (amount > 1e6) return Status::TooLarge;
    out = amount;
    return Status::Ok;
}

void validateException(double amount)
{
    if (amount < 0) throw std::invalid_argument("negative");
    if (amount > 1e6) throw std::out_of_range("too large");
}

void exceptionBenchmark(int invalidPercent)
{
    const std::size_t n = 1000000;
    std::size_t accepted = 0;

    double codeMs = ms([&] {
        for (std::size_t i = 0; i < n; ++i) {
            bool invalid = (invalidPercent == 1 && i % 100 == 0)
                        || (invalidPercent == 50 && i % 2 == 0);
            double amount = invalid ? -1.0 : 100.0;
            double out = 0;
            if (validateCode(amount, out) == Status::Ok) {
                ++accepted;
                sink += out;
            }
        }
    });

    accepted = 0;
    double excMs = ms([&] {
        for (std::size_t i = 0; i < n; ++i) {
            bool invalid = (invalidPercent == 1 && i % 100 == 0)
                        || (invalidPercent == 50 && i % 2 == 0);
            double amount = invalid ? -1.0 : 100.0;
            try {
                validateException(amount);
                ++accepted;
                sink += amount;
            } catch (const std::exception&) {
                // expected for invalid cases
            }
        }
    });

    acceptedSink = accepted;
    std::cout << "exceptions invalid=" << invalidPercent << "%"
              << " return_code_ms=" << codeMs
              << " exception_ms=" << excMs
              << " accepted=" << acceptedSink << '\n';
}

int main()
{
    correctnessCheck();
    billBenchmark(1000);
    billBenchmark(1000000);
    billBenchmark(10000000);
    exceptionBenchmark(0);
    exceptionBenchmark(1);
    exceptionBenchmark(50);
}
