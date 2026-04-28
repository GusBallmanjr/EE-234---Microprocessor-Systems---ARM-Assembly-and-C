# Slide Switch & XADC Interface Project  
### RealDigital BlackBoard — ARM Cortex‑A9 - C Implementation
## Gus Ballman

## Overview
This project demonstrates digital input, analog sampling, and LED output on the **RealDigital BlackBoard**, which uses the **Xilinx Zynq‑7007S SoC** (dual‑core ARM Cortex‑A9 + Artix‑7 FPGA).  
All logic is implemented in **bare‑metal C** running on the ARM Cortex‑A9.

The system allows the user to:

+ Enter a **12‑bit binary number** using the slide switches  
+ Capture that number using **pushbutton 0**  
+ Read the **XADC** value from the on‑board potentiometer using **pushbutton 1**  
+ Display both values on the 12 LEDs  
+ Compare the two values and indicate the result using the RGB LED

---

## Features

### 1. Read 12‑bit Value from Slide Switches
+ The 12 slide switches represent a **12‑bit unsigned integer**.
+ When **pushbutton 0** is pressed:
  - The switch value is latched into a software register.
  - The value is displayed on the **12 on‑board LEDs**.

### 2. Read ADC Voltage from the On‑Board Potentiometer
+ The RealDigital BlackBoard includes a potentiometer wired to the **XADC**.
+ When **pushbutton 1** is pressed:
  - The ADC is sampled.
  - The 12‑bit ADC result is displayed on the **12 LEDs**.

### 3. Compare ADC Value to Switch Value
After both values have been captured:

| Condition | RGB LED Output |
|----------|----------------|
| **ADC < Switch Value** | Red |
| **ADC > Switch Value** | Blue |
| **ADC == Switch Value** | Green |

Only one color is active at a time.

---

## Hardware Platform
+ **RealDigital BlackBoard**
  - Dual‑core ARM Cortex‑A9  
  - Artix‑7 FPGA fabric  
+ 12 slide switches  
+ 12 LEDs  
+ Pushbuttons 0 and 1  
+ On‑board potentiometer connected to XADC  
+ RGB LED device  

---

## Software Environment
+ Bare‑metal C (no OS)  
+ Compiled and deployed using:
  - Xilinx Vitis  
  - Custom board support for the RealDigital BlackBoard  
+ Direct register‑level access for:
  - GPIO  
  - XADC  
  - RGB LED control  
  - Pushbutton input  

---

## System Behavior Summary
1. **Set switches** → press **PB0** → LEDs show switch value  
2. **Adjust potentiometer** → press **PB1** → LEDs show ADC value  
3. **System compares values** → RGB LED shows:  
   - **Red** if ADC < Switch  
   - **Blue** if ADC > Switch  
   - **Green** if ADC == Switch  

---

## License
This project is provided for educational and embedded‑systems development use.
