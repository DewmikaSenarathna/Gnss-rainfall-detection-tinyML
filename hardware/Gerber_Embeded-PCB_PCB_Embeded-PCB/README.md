# 📌 STM32F103C8T6 Embedded PCB Design  

This repository contains the **schematic** and **PCB layout design** of a custom embedded controller board based on the **STM32F103C8T6 ARM Cortex-M3 microcontroller**.  

The design implements a complete minimum system including clock circuitry, power stabilization network, boot configuration headers, sensor interface connectors, and indication peripherals.

---

## 🔧 Hardware Features  

- **Microcontroller:** STM32F103C8T6  
- **External Clock:** 16 MHz crystal oscillator  
- **Reset Circuit:**  
  - Dedicated **reset push button** for manual microcontroller reset  

- **Boot Configuration:**  
  - **Male header pin configuration** available for setting  
    - **BOOT0 = 1 / 0**  
    - **BOOT1 = 1 / 0**  
  - Allows selection between Flash memory boot, System bootloader mode, and SRAM boot mode  

---

## ⚡ Power Supply & Filtering  

To ensure stable operation of the MCU and connected peripherals:  

- **Three (3 × 100 nF) decoupling capacitors** are placed along the power supply lines near the microcontroller  
- This helps to  
  - Reduce voltage ripple  
  - Suppress switching noise  
  - Improve system reliability  

- Proper **VCC and GND routing strategy** implemented in PCB layout  

---

## 🔌 Sensor & Module Interfaces  

The PCB includes **female header connectors (2.54 mm pitch)** for direct connection of external modules such as:  

- GPS module  
- Rain level sensor  
- DHT11 temperature & humidity sensor  
- I²C OLED display  

These connectors allow easy plug-and-play hardware expansion.

---

## 📐 PCB Routing Specifications  

- **0.25 mm track width** used for STM32 fine-pitch signal routing  
- **0.50 mm track width** used for general signal and power routing  
- Mixed **SMD and Through-hole component placement**  

---

## 🔊 Additional On-Board Features  

- Buzzer interface for audio indication  
- Multiple GPIO expansion headers  
- Programming / debugging interface header  

---

## 📂 Repository Contents  

- `Schematic_Embedded-PCB.pdf` → Complete circuit schematic  
- `PCB_Embedded-PCB.pdf` → PCB layout design  
- `Gerber/` → PCB manufacturing files  

---

## 🎯 Applications  

- Embedded system prototyping  
- IoT environmental monitoring systems  
- Academic engineering projects  
- Sensor data acquisition platforms  

---