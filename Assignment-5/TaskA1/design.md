# Task A1 - Polymorphic Bill Hierarchy

## A1.1 Common interface and data

Every bill must provide:

- `double total() const` - pure virtual, computes the amount to be paid.
- `std::string type() const` - virtual, returns the bill category label.
- `void print() const` - non-virtual, uses `type()` and `total()`.
- `virtual ~Bill()` - safe deletion through a base pointer.

Common data: `patient`, `billId` and `date`. The implementation in Task B1 keeps
`patient` because the given skeleton fixes that member; `billId` and `date` are
shown here as part of the full design.

## A1.2 UML hierarchy

```
                         +--------------------------------+
                         | Bill (abstract)                |
                         +--------------------------------+
                         | # patient : string             |
                         | # billId : string              |
                         | # date : string                |
                         +--------------------------------+
                         | + total() : double = 0  [pure] |
                         | + type() : string       [virt] |
                         | + print() : void        [non-v]|
                         | + virtual ~Bill()              |
                         +--------------------------------+
                                    ^
         +--------------------------+---------------------------+
         |              |                    |                  |
+----------------+ +----------------+ +----------------+ +----------------+
| OutPatientBill | | InPatientBill  | | DayCareBill    | | EmergencyBill  |
+----------------+ +----------------+ +----------------+ +----------------+
| consult, tests | | days, perDay,  | | packagePrice   | | base, hours,   |
|                | | surgery, meds  | |                | | hourlyCharge   |
+----------------+ +----------------+ +----------------+ +----------------+
| total() [over] | | total() [over] | | total() [over] | | total() [over] |
| type()  [over] | | type()  [over] | | type()  [over] | | type()  [over] |
+----------------+ +----------------+ +----------------+ +----------------+
```

## A1.3 Pseudocode for the day total

```
SET dayTotal TO 0
FOR EACH bill IN bills:
    SET dayTotal TO dayTotal + bill.total()
END FOR
PRINT dayTotal
```

## Alternative: one structure with a type field and switch

```
ENUM BillType { OP, IP, DAYCARE, EMERGENCY }
STRUCTURE Bill:
    type, patient
    consult, tests, days, perDay, surgery, medicines
    packagePrice, base, hours, hourlyCharge

FUNCTION total(b):
    SWITCH b.type:
        CASE OP: RETURN b.consult + b.tests
        CASE IP: RETURN b.days * b.perDay + b.surgery + b.medicines
        CASE DAYCARE: RETURN b.packagePrice
        CASE EMERGENCY: RETURN b.base + b.hours * b.hourlyCharge
```

Both versions are O(n) for n bills: one switch or one virtual call per bill. The
single structure stores all fields for every bill, so most fields are unused. The
hierarchy stores only the fields of the actual bill type plus a vptr. A new type
requires editing the enum, every switch, the constructor/factory and the field
list in the single-structure design. In the hierarchy it requires one new derived class.
