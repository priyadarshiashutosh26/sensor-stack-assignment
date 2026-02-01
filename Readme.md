# README

## What this is

This project shows how I’d handle sensor errors in a simple embedded-style system.

* **Python** simulates an I2C sensor and generates common I2C-related errors.
* Those errors are written as numeric codes to a file.
* **C** reads those error codes and stores them in a stack implemented using a linked list.
* The stack has a fixed size (32). If it overflows, the oldest error is overwritten.

Everything runs on a **Windows machine**, no Raspberry Pi or real I2C hardware needed.


## Why it’s designed this way

* Python is easy to use for mocking hardware behavior and error conditions.
* C is used for the stack to show proper memory handling and embedded-style constraints.
* The logic is clearly split:

  * Python → sensor behavior and error generation
  * C → data storage and stack operations
* A text file is used to pass error codes, which keeps the two parts independent and realistic.


## How to run it

1. Run the Python script:

   ```bash
   python sensor.py
   ```

2. Compile the C program:

   ```bash
   gcc stack.c -o stack
   ```

3. Run the stack program:

   ```bash
   stack
   ```

## Assumptions

* Error codes are simple integers
* Stack size is limited to 32 entries
* Old errors are overwritten when the stack is full
* Single-threaded execution
* Random errors are acceptable for simulation

