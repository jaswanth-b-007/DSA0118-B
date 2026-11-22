# Task A2 - Runtime Binding

## A2.1 Early and late binding

- Early binding: the compiler fixes the called function from the static type. A call
  such as `b->print()` calls `Bill::print()` directly because `print()` is not virtual.
- Late binding: a call to a virtual function through a pointer is resolved at run time
  through the object's vptr. Inside `Bill::print()`, the call `total()` is virtual, so
  an `OPBill` object reaches `OPBill::total()`.

## A2.2 Vtables

```
OPBill object                 vtable for OPBill
+----------------+           +--------------------------------+
| vptr ----------+---------->| destructor -> ~OPBill          |
| (base fields)  |           | total     -> OPBill::total    |
+----------------+           +--------------------------------+

InPatientBill object          vtable for InPatientBill
+----------------+           +--------------------------------+
| vptr ----------+---------->| destructor -> ~InPatientBill   |
| (base fields)  |           | total     -> InPatientBill::total
| (own fields)   |           +--------------------------------+
+----------------+

Bill vtable (abstract)
+--------------------------------+
| destructor -> ~Bill            |
| total     -> 0 (pure)          |
+--------------------------------+
```

`b->show()` calls the non-virtual `Bill::show()` directly. The `total()` call inside
`show()` reads `b`'s vptr and calls the slot for `total`, so an `OPBill` object returns 500.

## A2.3 Virtual destructor

Deleting through `Bill*` must call the destructor of the complete object. Without a
virtual destructor only `~Bill()` runs; any resource owned by the derived class is
not released.

## Prediction

Program output (hand-traced):

```
500 ~OPBill ~Bill 
```

If `virtual` is removed from `~Bill`, only `~Bill` runs:
`500 ~Bill `. AddressSanitizer may not report a leak for this exact class because
`OPBill` owns no separate heap block. Add a dynamically allocated member to the
derived class to make the missing destructor visible as a leak. See
`TaskB1/virtual_dtor_demo.cpp`.

## Alternative: dynamic_cast chain

Without virtual `total()`, each bill would be tested with `dynamic_cast` until the
right type is found. One virtual call is O(1); up to k casts for k bill types is O(k).
The class must still be polymorphic to use `dynamic_cast`, so the vptr space cost remains.
