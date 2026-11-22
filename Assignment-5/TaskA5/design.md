# Task A5 - Recovery and Rethrowing

## A5.1 Where to catch

1. Low-level validation throws a specific exception, for example `ClaimError`.
2. A service function catches only the exception it can log, writes the log message
   and rethrows the same object with `throw;`.
3. `main()` catches `std::exception` (and finally `catch (...)`) to report the failure
   to the user and keep the program running for the next case.

## A5.2 Stack unwinding and RAII

When an exception is thrown, C++ destroys every fully constructed automatic object
from the throw point back to the matching handler. This is stack unwinding. Objects
such as `std::unique_ptr`, `std::vector`, `std::string` and file streams have
destructors that release their resources, so memory and files are freed without code
in every catch block.

## Prediction

Output (hand-traced):

```
~f ~main caught 42 
```

`~f` runs first because `g` is destroyed while leaving `f()`. `~main` runs next when
the `try` block is unwound. The statement `std::cout << "not printed "` is skipped
because the throw transfers control out of `f()` immediately.

## Alternative: delete in every catch block

Manual cleanup has the same basic time cost but repeats code at every catch site.
Each new error path adds another place where a `delete` can be forgotten. RAII keeps
cleanup in one place, next to the resource, so the number of leak points does not grow
with the number of handlers.
