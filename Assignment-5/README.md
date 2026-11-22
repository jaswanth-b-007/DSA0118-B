# DSA01-192472251-Assignment5

**Name:** B. Jaswanth  
**Register Number:** 192472251  
**Assignment:** Assignment 5 - CO5 (Reliable Software with Runtime Polymorphism and Exceptions)  
**Course:** Object Oriented Programming with C++ (DSA01)

## Repository layout

- `TaskA1` to `TaskA5` - Part A design notes, pseudocode, UML/vtable diagrams and exception strategy.
- `TaskB1` - polymorphic billing implementation, completed TODO program and binding/destructor demos.
- `TaskB2` - error-condition tests, prediction program and completed claim-settlement program.
- `TaskB3` - virtual / switch / variant and exception / return-code benchmarks.
- `TaskB4` - practical extension and production decision.
- `screenshots` - expected-output images and clearly marked placeholders for real run screenshots.

## Compiler and commands

Use g++ with C++17 and warnings enabled:

```bash
g++ -std=c++17 -Wall -Wextra TaskB1/billing_system.cpp -o TaskB1/billing_system
g++ -std=c++17 -Wall -Wextra TaskB1/complete_todo.cpp -o TaskB1/complete_todo
g++ -std=c++17 -Wall -Wextra TaskB1/runtime_binding_demo.cpp -o TaskB1/runtime_binding_demo
g++ -std=c++17 -Wall -Wextra TaskB1/virtual_dtor_demo.cpp -o TaskB1/virtual_dtor_demo
g++ -std=c++17 -Wall -Wextra TaskB2/error_conditions.cpp -o TaskB2/error_conditions
g++ -std=c++17 -Wall -Wextra TaskB2/predict_validate.cpp -o TaskB2/predict_validate
g++ -std=c++17 -Wall -Wextra TaskB2/complete_todo.cpp -o TaskB2/complete_todo
g++ -std=c++17 -Wall -Wextra -O2 TaskB3/benchmark.cpp -o TaskB3/benchmark
g++ -std=c++17 -Wall -Wextra TaskB4/extension.cpp -o TaskB4/extension
```

For sanitizer runs:

```bash
g++ -std=c++17 -Wall -Wextra -fsanitize=address -g TaskB1/virtual_dtor_demo.cpp -o TaskB1/virtual_dtor_asan
g++ -std=c++17 -Wall -Wextra -fsanitize=address -g TaskB2/error_conditions.cpp -o TaskB2/error_conditions_asan
g++ -std=c++17 -Wall -Wextra -fsanitize=address -g TaskB4/extension.cpp -o TaskB4/extension_asan
./TaskB1/virtual_dtor_asan
./TaskB2/error_conditions_asan
./TaskB4/extension_asan
```

## How to run

Run each executable from the repository root and compare its output with the matching
`expected_output.txt` file in the same task folder. The PNG files in `screenshots`
are hand-traced expected outputs and are clearly labelled; replace them with real
terminal screenshots after compiling and running on your machine.

## Status of measured evidence

No C++ compiler was available while this repository was prepared. Compiler output,
sanitizer reports, timings and actual screenshots are therefore marked
**Pending execution** rather than invented. Fill them in after running the commands above.
