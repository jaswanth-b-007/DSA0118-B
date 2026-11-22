#include <iostream>

// Compile once as shown to demonstrate the missing derived destructor.
// Then remove the word virtual from ~Bill and compile with -fsanitize=address.

struct Bill {
    virtual ~Bill() { std::cout << "~Bill "; }
    virtual double total() const = 0;
};

struct OPBill : Bill {
    double* extra;
    OPBill() : extra(new double[100]) {}
    ~OPBill() override
    {
        std::cout << "~OPBill ";
        delete[] extra;
    }
    double total() const override { return 500; }
};

int main()
{
    Bill* b = new OPBill;
    std::cout << b->total() << ' ';
    delete b;
    std::cout << '\n';
}
