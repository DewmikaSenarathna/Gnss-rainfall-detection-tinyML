# Rainfall Detection and Short-Term Prediction using GNSS and TinyML

## EC6020 – Mini Project  
**Degree Program:** Electronics and Computer Engineering  
**Project Type:** Hardware–Software Integrated Embedded System  

---
<br>


## 📌 Project Overview

This project focuses on the design and development of an **intelligent embedded system** for **rainfall detection and short-term rainfall prediction** using **GNSS signal attenuation**, **environmental sensors**, and **Tiny Machine Learning (TinyML)**.

Unlike traditional rainfall monitoring systems that rely on expensive radar or large-scale meteorological infrastructure, this system leverages **Signal-to-Noise Ratio (SNR) variations of GNSS signals** as an indirect environmental sensing mechanism. These GNSS-derived features are combined with **humidity** and **rainfall intensity sensor data**, and processed locally on an **STM32 microcontroller** using a lightweight TinyML model.

The system performs **real-time inference at the edge**, without requiring cloud connectivity, making it suitable for **low-cost, low-power, and scalable deployment**.

---
<br>


## 🎯 Project Objectives

- Detect rainfall events in real time using GNSS signal attenuation  
- Predict short-term rainfall conditions using TinyML  
- Integrate multiple sensors for improved reliability (sensor fusion)  
- Implement the complete system on a **STM32 microcontroller**  
- Provide a user interface to visualize system outputs  
- Comply fully with **EC6020 Mini Project guidelines**

---
<br>


## 🧠 Key Technical Concepts

- GNSS Signal-to-Noise Ratio (C/N₀) analysis  
- Embedded systems design using STM32  
- Sensor interfacing using UART and I²C  
- Feature extraction from time-series GNSS data  
- TinyML deployment on resource-constrained hardware  
- Edge-based machine learning inference  

---
<br>


## 🧩 System Architecture (High-Level)

**Inputs:**
- GNSS SNR values from GNSS receiver
- Humidity sensor readings
- Rainfall intensity sensor output

**Processing:**
- Feature extraction (mean SNR, SNR variation, Rainfall intensity, Humidity level)
- TinyML inference on STM32

**Outputs:**
- Rainfall detection status
- Short-term rainfall prediction
- Humidity level
- Data visualization through UI

---
<br>


## 📂 Repository Structure

The repository is organized to clearly separate **documentation**, **hardware design**, **firmware**, **machine learning**, and **user interface** components.

```text
Gnss-rainfall-detection-tinyML/
│
├── README.md
├── docs/
│   ├── proposal.pdf
│   ├── presentations/
│
├── hardware/
│   ├── circuit_diagram.pdf
│   ├── pcb/
│
├── firmware/
│   ├── stm32_code/
│
├── ml/
│   ├── dataset/
│   ├── training_scripts/
│   ├── model/
│
└── ui/
    ├── dashboard/

```
---
<br>


---

## 🛠 Hardware Components

- STM32 microcontroller 
- GNSS receiver module
- Humidity sensor
- Rainfall intensity sensor
- Power regulation and support circuitry

---
<br>


## 💻 Software and Tools

- **Embedded Development:** STM32CubeIDE, Embedded C/C++  
- **Machine Learning:** Python, NumPy, Pandas, Scikit-learn  
- **TinyML:** TensorFlow Lite for Microcontrollers  
- **Version Control:** Git & GitHub  
- **Testing & Debugging:** Serial monitors, logic analyzer, multimeter  

---
<br>


## 🔬 Machine Learning Approach

- GNSS and sensor data are collected and logged
- Features are extracted from time-series data
- A lightweight ML model is trained offline
- The trained model is converted to TinyML format
- The model is deployed on STM32 for real-time inference

---
<br>


## 🧪 Testing and Validation

- System tested under dry and rainy conditions
- Model predictions compared with actual rainfall observations
- Performance evaluated based on accuracy and response time

---
<br>


## 📦 Deliverables

- Fully functional embedded prototype
- TinyML model deployed on STM32
- GitHub repository with complete documentation
- MID and END evaluation presentations

---
<br>


## 👥 Team Information

This project is developed by a **5-member team** as part of the **EC6020 Mini Project**.  
Team roles (hardware design, firmware development, ML modeling, UI development, documentation) are clearly defined in the project documentation.

---
<br>


## ⚠️ Notes

- This repository is intended for **academic and educational purposes only**
- All work complies with EC6020 project rules and originality requirements

---
<br>


## 📄 License

This project is developed as part of an academic course and is not intended for commercial use without permission.

---
<br>


## ✅ Status

🚧 **Work in Progress** – Ongoing development and testing

---

"# Gnss-rainfall-detection-tinyML" 
"# Gnss-rainfall-detection-tinyML" 
