#### EE 234 Project 3 - Zynq‑7000 Assembly Stopwatch Project
# Gus Ballman

This project implements a complete stopwatch system on the Zynq‑7000 SoC using ARMv7 assembly.  
It progresses through four major stages:

1. Seven‑segment display and GPIO interfacing  
2. A four‑digit decimal (BCD) counter  
3. Stopwatch control flow (Start/Stop/Increment/Clear)  
4. Hardware‑accurate timing using the Zynq Triple Timer Counter (TTC) module  

All code is written in bare‑metal ARM assembly and interacts directly with memory‑mapped I/O.

---

## 1. Seven‑Segment Display & GPIO Interface

The first stage focuses on learning the Zynq memory‑mapped I/O system and writing reusable functions for the seven‑segment display, slide switches, and pushbuttons.

### Implemented Features

#### Display a value on a single digit

A function accepts:

+ digit number (1–4)  
+ value (0–15 for hex/BCD)

The function safely handles out‑of‑range digit numbers (e.g., digit > 4) by ignoring invalid inputs or mapping them to a safe default behavior.

#### Read 12‑bit slide switches

A function reads the slide switch register and returns the lower 12 bits as a value.

#### Read 4‑bit pushbuttons

A function reads the pushbutton register and returns the lower 4 bits as a value.

### Demonstration Program

The program:

+ Reads a 12‑bit value from the switches  
+ Reads a 4‑bit value from the pushbuttons  
+ Displays:
  - The switch value across digits 1–3 (each group of four switches is treated as one hexadecimal digit)
  - The pushbutton value on digit 4

This verifies correct GPIO and seven‑segment operation.

---

## 2. Four‑Digit Decimal (BCD) Counter

The second stage implements a 0000–9999 decimal counter using Binary‑Coded Decimal (BCD).

### Counter Storage

+ Four BCD digits are stored on the stack as local variables.
+ A frame pointer is set up in `main`, and each digit is accessed via stack offsets.
+ To modify a digit:
  - Load it from the stack into a register
  - Modify it
  - Store it back to the stack

In the main program header comments, the locations of the counter variables on the stack are documented.

### Behavior

+ The counter starts at 0000 and counts up to 9999, then rolls over to 0000.
+ Each digit only counts 0–9 before rolling over and incrementing the next digit.
+ Proper carry behavior is implemented:
  - 0099 → 0100  
  - 0999 → 1000  
  - 9999 → 0000  

### Controls

+ Two pushbuttons are used:
  - Clear: resets all four BCD digits to zero and updates the display.
  - Increment: increments the selected digit and updates the display.
+ Slide switches select which of the four digits to increment, so you don’t have to count all the way up from 0000 to reach higher digits.

### Edge Detection

To ensure the counter increments only once per button press:

+ The code detects a positive edge on the increment button.
+ This is done by storing the previous button state and comparing it to the current state in the polling loop.

---

## 3. Stopwatch Control Flow

The third stage adds full stopwatch behavior on top of the BCD counter.

### Controls

+ **Start**: when pressed, the counter begins incrementing continuously.
+ **Stop**: when pressed, the counter stops incrementing.
+ **Clear**: when the counter is stopped, resets the count to 0000.
+ **Increment**: when the counter is stopped, adds 1 to the count each time Increment is pressed.

### Software Delay Timebase

Without a delay, the counter would run too fast to be useful.  
A software delay loop (dummy loop) is used to slow the counter:

+ A simple loop counts down from a constant value.
+ The constant is tuned so that the least significant digit increments approximately once per millisecond.
+ The delay constant is defined near the top of the code so it can be easily adjusted.

### Control Flow Structure

The stopwatch control flow can be implemented in multiple ways, such as:

+ Multiple loops (e.g., one for idle, one for running), or  
+ A single loop with branches based on a “running” flag.

The implementation explores these options and uses conditional branches to manage Start/Stop/Increment/Clear behavior.

---

## 4. Hardware Timer (TTC) Timebase

The final stage replaces the software delay with a more accurate hardware timebase using the Zynq Triple Timer Counter (TTC).

A separate assembly file contains all TTC‑related functions, each exported with `.global` so they can be called from the main program.

### TTC Functions

+ `ttc_init`  
+ `ttc_SetInterval`  
+ `ttc_SetPrescale`  
+ `ttc_Enable`  
+ `ttc_Disable`  
+ `ttc_Poll`  

### TTC Initialization

The TTC initialization subroutine:

1. Disables the counter  
2. Configures the timer clock  
3. Sets the prescale value  
4. Sets the interval value  
5. Enables the interval interrupt flag  
6. Enables the timer in interval mode  

### Timer Parameter Calculation

When operating in interval mode:

+ The prescale value divides the 111 MHz peripheral clock by `2^(prescale + 1)`.
  - Prescale = 0 → divide by 2  
  - Prescale = 1 → divide by 4  
  - Prescale = 2 → divide by 8  
  - etc.
+ The interval register defines how many of these divided clock pulses occur per interval.

The interval time can be approximated as:

```text
T ≈ interval / (111 MHz / 2^(prescale + 1))
