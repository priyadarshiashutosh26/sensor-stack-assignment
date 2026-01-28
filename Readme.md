## I2C Sensor Error Handling & Stack Implementation

## Overview

This project demonstrates a simple, embedded-style design using two components:

* **Python** simulates an I2C sensor, generates common I2C errors, and logs numeric error codes.
* **C** implements a linked-list stack that stores these error codes with a maximum size of 32 entries and circular overwrite behavior.

The solution runs fully on **Windows**, without Raspberry Pi hardware or real I2C.

## Design Decisions

* **Python** is used to mock I2C behavior and error handling in a clear, hardware-independent way.
* **C** is used for the stack to demonstrate low-level memory management.
* Logic is clearly separated:

  * Python handles sensor simulation and error generation.
  * C handles data storage and stack operations.
* Error codes are exchanged via a simple text file to mimic real system boundaries.

## How to Build and Run

### 1. Run I2C Simulation

```bash
python problem1_i2c_simulator.py
```

This generates `error_codes.txt`.

### 2. Compile Stack Program

```bash
gcc problem2_error_stack.c -o error_stack
```

### 3. Run Stack Program

```bash
error_stack
```


## Assumptions

* Error codes are integers
* Maximum stack size is 32 entries
* Oldest stack entry is overwritten on overflow
* System runs in a single-threaded environment
* Randomized errors are acceptable for simulation

## Notes

The Python I2C logic can be replaced with real Raspberry Pi I2C libraries with minimal changes, and the C stack is suitable for embedded systems with fixed memory constraints.
