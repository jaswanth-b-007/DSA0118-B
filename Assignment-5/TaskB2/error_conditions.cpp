#include <cstddef>
#include <iostream>
#include <stdexcept>
#include <string>
#include <utility>

class BillingError : public std::runtime_error {
public:
    using std::runtime_error::runtime_error;
};

class ClaimError : public std::runtime_error {
public:
    using std::runtime_error::runtime_error;
};

class RecordError : public std::runtime_error {
public:
    using std::runtime_error::runtime_error;
};

struct Trial {
    std::string label;
    explicit Trial(std::string s) : label(std::move(s))
    {
        std::cout << "start " << label << ' ';
    }
    ~Trial() { std::cout << "~" << label << ' '; }
};

void validateCharge(double amount)
{
    if (amount < 0) throw BillingError("negative charge");
}

void approve(double billed, double cover)
{
    if (billed > cover) throw ClaimError("exceeds cover");
}

double packageRate(const std::string& code)
{
    if (code != "OP" && code != "IP") throw BillingError("unknown package");
    return code == "OP" ? 1000.0 : 20000.0;
}

double parseAmount(const std::string& text)
{
    return std::stod(text);   // throws std::invalid_argument for "abc"
}

void forceAllocationFailure()
{
    std::size_t n = static_cast<std::size_t>(-1);
    double* block = new double[n];
    (void)block;
}

void runOne(const std::string& label, void (*fn)())
{
    try {
        Trial t(label);
        fn();
        std::cout << "ok ";
    } catch (const std::exception& e) {
        std::cout << "caught " << e.what() << ' ';
    }
}

void negativeCharge() { validateCharge(-10); }
void highClaim() { approve(60000, 50000); }
void unknownPackage() { (void)packageRate("XYZ"); }
void badNumber() { (void)parseAmount("abc"); }
void allocationFailure() { forceAllocationFailure(); }

int main()
{
    try {
        runOne("negative", negativeCharge);
        runOne("claim", highClaim);
        runOne("package", unknownPackage);
        runOne("number", badNumber);
        runOne("memory", allocationFailure);
        std::cout << '\n';
    } catch (...) {
        std::cout << "top-level catch(...) kept the program running\n";
    }
}
