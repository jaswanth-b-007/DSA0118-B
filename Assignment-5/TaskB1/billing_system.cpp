#include <algorithm>
#include <iostream>
#include <map>
#include <memory>
#include <string>
#include <utility>
#include <vector>

// ---------------- Bill hierarchy (Part A1) ----------------
class Bill {
protected:
    std::string patient;
    std::string id;
public:
    Bill(std::string p, std::string i) : patient(std::move(p)), id(std::move(i)) {}
    virtual ~Bill() = default;
    virtual double total() const = 0;
    virtual std::string type() const = 0;
    void print() const
    {
        std::cout << type() << ' ' << patient << ": " << total() << '\n';
    }
};

class OutPatientBill : public Bill {
    double consultation;
    double tests;
public:
    OutPatientBill(std::string p, std::string i, double c, double t)
        : Bill(std::move(p), std::move(i)), consultation(c), tests(t) {}
    double total() const override { return consultation + tests; }
    std::string type() const override { return "OP"; }
};

class InPatientBill : public Bill {
    int days;
    double perDay;
    double surgery;
    double medicines;
public:
    InPatientBill(std::string p, std::string i, int d, double rate,
                  double s, double med = 0.0)
        : Bill(std::move(p), std::move(i)), days(d), perDay(rate),
          surgery(s), medicines(med) {}
    double total() const override
    {
        return days * perDay + surgery + medicines;
    }
    std::string type() const override { return "IP"; }
};

class DayCareBill : public Bill {
    double packagePrice;
public:
    DayCareBill(std::string p, std::string i, double price)
        : Bill(std::move(p), std::move(i)), packagePrice(price) {}
    double total() const override { return packagePrice; }
    std::string type() const override { return "DAYCARE"; }
};

class EmergencyBill : public Bill {
    double base;
    double hours;
    double hourlyCharge;
public:
    EmergencyBill(std::string p, std::string i, double b, double h, double rate)
        : Bill(std::move(p), std::move(i)), base(b), hours(h), hourlyCharge(rate) {}
    double total() const override { return base + hours * hourlyCharge; }
    std::string type() const override { return "EMERGENCY"; }
};

// ---------------- Payer hierarchy (Part A3) ----------------
class Payer {
public:
    virtual ~Payer() = default;
    virtual double amountCovered(const Bill& b) const = 0;
    virtual std::string name() const { return "Payer"; }
};

class SelfPay : public Payer {
public:
    double amountCovered(const Bill& b) const override { return b.total(); }
    std::string name() const override { return "SelfPay"; }
};

class Insurance : public Payer {
    double cover;
    double coPayPercent;
public:
    Insurance(double c, double cp) : cover(c), coPayPercent(cp) {}
    double amountCovered(const Bill& b) const override
    {
        return std::min(b.total(), cover) * (100.0 - coPayPercent) / 100.0;
    }
    std::string name() const override { return "Insurance"; }
};

class GovernmentScheme : public Payer {
    std::map<std::string, double> packageRate;
public:
    explicit GovernmentScheme(const std::map<std::string, double>& rates)
        : packageRate(rates) {}
    double amountCovered(const Bill& b) const override
    {
        auto it = packageRate.find(b.type());
        return it == packageRate.end() ? 0.0 : std::min(b.total(), it->second);
    }
    std::string name() const override { return "GovernmentScheme"; }
};

int main()
{
    std::vector<std::unique_ptr<Bill>> bills;
    bills.push_back(std::make_unique<OutPatientBill>("Ravi", "OP001", 500, 1200));
    bills.push_back(std::make_unique<InPatientBill>("Meena", "IP001", 3, 2500, 40000));
    bills.push_back(std::make_unique<DayCareBill>("Kiran", "DC001", 25000));
    bills.push_back(std::make_unique<EmergencyBill>("Latha", "EM001", 2000, 4, 1500));

    std::map<std::string, double> rates = {
        {"OP", 1000}, {"IP", 20000}, {"DAYCARE", 18000}, {"EMERGENCY", 5000}
    };
    std::vector<std::unique_ptr<Payer>> payers;
    payers.push_back(std::make_unique<SelfPay>());
    payers.push_back(std::make_unique<Insurance>(30000, 10));
    payers.push_back(std::make_unique<GovernmentScheme>(rates));

    double dayTotal = 0;
    for (const auto& bill : bills) {
        bill->print();
        dayTotal += bill->total();
        for (const auto& payer : payers) {
            std::cout << "  " << payer->name() << ": "
                      << payer->amountCovered(*bill) << '\n';
        }
    }
    std::cout << "Day total " << dayTotal << '\n';
}
