# Task A4 - Exception Strategy

## A4.1 Errors and hierarchy

Errors to handle:

- negative charge or negative cover
- claim above cover
- unknown package code
- missing patient record
- text where a number is expected (`std::stod` -> `std::invalid_argument`)
- memory allocation failure (`new` -> `std::bad_alloc`)

```
std::exception
 +-- std::runtime_error
 |    +-- BillingError  (invalid charges, unknown package)
 |    +-- ClaimError    (claim above cover)
 |    +-- RecordError   (missing patient record)
 +-- std::invalid_argument  (bad numeric input)
 +-- std::bad_alloc         (allocation failure)
```

## A4.2 Exception specifications

- May throw: `total()`, `amountCovered()`, `approve()`, `settle()`, `parseAmount()`.
- Must not throw: destructors, getters, `type()`. Mark them `noexcept`.
- Destructors must not throw because stack unwinding during exception handling may
  call them; a throwing destructor during unwinding calls `std::terminate`.

## Alternative: return codes

Return codes avoid unwinding cost on the error path, but every function must return a
`Status` and fill an error string through an output parameter. The normal path has an
extra branch check. The error path is cheaper than exception unwinding, but the caller
must remember to check every result. Space grows by a status value and an error message
at each call level, and code size grows with the checks. Exceptions keep the normal
path clean and cannot be silently ignored.
