# Arbitrary Precision Calculator (APC)

A robust C-based calculator for performing arithmetic operations on integers of arbitrary size. 
This project uses doubly linked lists to handle numbers that exceed the capacity of standard primitive types like `long long`.

## Architectural Approach

### Data Representation
Numbers are stored using a `Number` structure containing:
- A doubly linked list (`LL`) where each node stores a single digit.
- A `sign` integer (1 for positive, -1 for negative).
- Pointers to both `head` and `tail` for efficient bidirectional traversal (needed for operations like addition and multiplication).

### Memory Management & Safety
To ensure clean memory management, the project follows these core principles:
- **Status Returns:** All major operations return a `Status` enum (`SUCCESS`, `FAILURE`, `DIVISION_BY_ZERO`). This allows the program to halt and clean up gracefully if an allocation fails.
- **Node Detachment:** During complex operations (like multiplication), intermediate results are swapped. The nodes are "detached" from temporary structures before the structures are freed to avoid double-free errors.
- **Centralized Cleanup:** Every `Number` allocated is tracked and freed at the end of the `main` lifecycle, even on failure paths.

## Core Functions

### Arithmetic Operations
- `addition / subtraction`: These functions act as wrappers. They compare the signs and magnitudes of the inputs to determine whether to perform `add_magnitudes` or `sub_magnitudes`.
- `multiplication`: Implements the standard long multiplication algorithm. It multiplies the first number by each digit of the second, shifts the results using a `digits_pos` counter, and accumulates them into a final sum.
- `division`: Implements long division. It builds a "current dividend" digit-by-digit and uses `find_cofactor` to determine the largest multiple of the divisor that fits, calculating the quotient and remainder iteratively.

### Helper Utilities
- `prepend_digit / append_digit`: Handles the dynamic allocation of new nodes at the start or end of the linked list.
- `trim_zeroes`: Removes leading zeroes from the linked list after operations to keep the representation minimal and correct.
- `compare_magnitudes`: Compares the absolute values of two numbers by first checking their lengths and then performing a digit-by-digit comparison from the head.
- `free_num / free_number_nodes`: Recursively traverses the list to free all allocated memory, ensuring no leaks occur.

## Compilation and Usage

### Build
```bash
make
```

### Run
```bash
./build/bin/apc <number1> <operator> <number2>
```
*Operators supported: `+`, `-`, `x`, `/`*

Example:
```bash
./build/bin/apc 12345678901234567890 + 98765432109876543210
```
