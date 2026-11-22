#include <algorithm>
#include <iostream>
#include <memory>
#include <string>
#include <utility>
#include <vector>

// Existing classes (unchanged) -------------------------------
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

class OutPatientBill : public Bill {
    double consultation;
    double tests;
public:
    OutPatientBill(std::string p, std::string i, double c, double t)
        : Bill(std::move(p), std::move(i)), consultation(c), tests(t) {}
    double total() const override { return consultation + tests; }
    std::string type() const override { return "OP"; }
};

// NEW bill type ----------------------------------------------
class PharmacyBill : public Bill {
    double itemCost;
    double discountPercent;
public:
    PharmacyBill(std::string p, std::string i, double cost, double discount)
        : Bill(std::move(p), std::move(i)), itemCost(cost),
          discountPercent(discount) {}
    double total() const override
    {
        return itemCost * (100.0 - discountPercent) / 100.0;
    }
    std::string type() const override { return "PHARMACY"; }
};

// NEW payer --------------------------------------------------
class CorporatePayer : public Payer {
    double contractLimit;
public:
    explicit CorporatePayer(double limit) : contractLimit(limit) {}
    double amountCovered(const Bill& b) const override
    {
        return std::min(b.total(), contractLimit) * 0.9;
    }
    std::string name() const override { return "CorporateTieUp"; }
};

int main()
{
    std::vector<std::unique_ptr<Bill>> bills;
    bills.push_back(std::make_unique<OutPatientBill>("Ravi", "OP001", 500, 1200));
    bills.push_back(std::make_unique<PharmacyBill>("Asha", "PH001", 1000, 10));

    std::vector<std::unique_ptr<Payer>> payers;
    payers.push_back(std::make_unique<SelfPay>());
    payers.push_back(std::make_unique<Insurance>(30000, 10));
    payers.push_back(std::make_unique<CorporatePayer>(20000));

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
