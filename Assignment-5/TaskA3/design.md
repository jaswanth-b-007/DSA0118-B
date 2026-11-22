# Task A3 - Abstract Payer Design

## A3.1 Payer interface

```
class Payer {
public:
    virtual ~Payer() = default;
    virtual double amountCovered(const Bill& b) const = 0;
};

class SelfPay : public Payer {
    double amountCovered(const Bill& b) const override { return b.total(); }
};

class Insurance : public Payer {
    double cover;
    double coPayPercent;
    double amountCovered(const Bill& b) const override {
        return std::min(b.total(), cover) * (100 - coPayPercent) / 100;
    }
};

class GovernmentScheme : public Payer {
    std::map<std::string, double> packageRate;
    double amountCovered(const Bill& b) const override {
        auto it = packageRate.find(b.type());
        return it == packageRate.end() ? 0.0 : std::min(b.total(), it->second);
    }
};
```

## A3.2 Extension

A corporate tie-up needs only a new class; no existing class is edited:

```
class CorporateTieUp : public Payer {
    double contractLimit;
public:
    double amountCovered(const Bill& b) const override {
        return std::min(b.total(), contractLimit) * 0.85;
    }
};
```

Add one line where the payer is created and the existing billing loop works unchanged.

## Alternative: std::function policy

A `std::function<double(const Bill&)>` can hold a lambda or function object chosen at
run time. Both designs are O(1) per call. The abstract class stores one vptr plus the
payer's own data members. `std::function` stores a callable in a type-erased buffer;
larger captures may use the heap. Data such as the cover limit is a normal member in
the class design, but must be captured inside the callable in the function design,
which makes it less visible and harder to inspect.
