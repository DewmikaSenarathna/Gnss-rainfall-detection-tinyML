# Rainfall Detection and Short-Term Prediction using GNSS and TinyML

![Project](https://img.shields.io/badge/Project-EC6020%20Mini%20Project-0A66C2?style=for-the-badge&logo=google-scholar&logoColor=white)
![Program](https://img.shields.io/badge/Program-Electronics%20%26%20Computer%20Engineering-1D4ED8?style=for-the-badge&logo=bookstack&logoColor=white)
![TinyML](https://img.shields.io/badge/TinyML-TensorFlow%20Lite-F57C00?style=for-the-badge&logo=tensorflow&logoColor=white)
![Embedded](https://img.shields.io/badge/Embedded-STM32-0B132A?style=for-the-badge&logo=stmicroelectronics&logoColor=white)
![App](https://img.shields.io/badge/App-Flutter-02569B?style=for-the-badge&logo=flutter&logoColor=white)
![Status](https://img.shields.io/badge/Status-Work%20in%20Progress-CA8A04?style=for-the-badge&logo=clockify&logoColor=white)

## 🎓 EC6020 - Mini Project
**Degree Program:** Electronics and Computer Engineering
**Project Type:** Hardware-Software Integrated Embedded System

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
- Implement the complete system on an **STM32 microcontroller**
- Provide a user interface to visualize system outputs
- Comply fully with **EC6020 Mini Project guidelines**

---
<br>


## 🧠 Key Technical Concepts

- GNSS Signal-to-Noise Ratio (C/N0) analysis
- Embedded systems design using STM32
- Sensor interfacing using UART and I2C
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
- Feature extraction (mean SNR, SNR variation, rainfall intensity, humidity level)
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
|
|-- README.md
|-- docs/
|   |-- imgs/
|   |-- pdfs/
|   `-- presentations/
|
|-- hardware/
|   |-- Gerber_Embeded-PCB_PCB_Embeded-PCB/
|   `-- pcb/
|
|-- firmware/
|   |-- Arduino/
|   |-- stm32_code/
|   `-- Tests/
|
|-- ml/
|   |-- dataset/
|   |-- training_scripts/
|   |-- model/
|   `-- testing/
|
`-- app/
    `-- NimbusAI/
```

---
<br>


## 🛠 Hardware Components

- STM32 microcontroller
- GNSS receiver module
- Humidity sensor
- Rainfall intensity sensor
- Power regulation and support circuitry

---
<br>


## 🔌 Circuit Design

Complete circuit artifacts are available in the hardware folder:

- Schematic: [hardware/Gerber_Embeded-PCB_PCB_Embeded-PCB/Schematic_Embeded-PCB_2026-03-23.pdf](hardware/Gerber_Embeded-PCB_PCB_Embeded-PCB/Schematic_Embeded-PCB_2026-03-23.pdf)
- PCB layout: [hardware/Gerber_Embeded-PCB_PCB_Embeded-PCB/PCB_PCB_Embeded-PCB_2026-03-23.pdf](hardware/Gerber_Embeded-PCB_PCB_Embeded-PCB/PCB_PCB_Embeded-PCB_2026-03-23.pdf)

---
<br>


## 💻 Software and Tools

- **Embedded Development:** STM32CubeIDE, Embedded C/C++
- **Machine Learning:** Python, NumPy, Pandas, Scikit-learn
- **TinyML:** TensorFlow Lite for Microcontrollers
- **Mobile Application:** Flutter (NimbusAI)
- **Version Control:** Git and GitHub
- **Testing and Debugging:** Serial monitors, logic analyzer, multimeter

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


## 📚 References

1. ITU-R P.838-3: Specific attenuation model for rain for use in prediction methods
2. ITU-R P.618: Propagation data and prediction methods required for Earth-space systems
3. TensorFlow Lite for Microcontrollers documentation: https://www.tensorflow.org/lite/microcontrollers
4. TinyML Foundation resources: https://www.tinyml.org
5. NumPy documentation: https://numpy.org/doc/
6. pandas documentation: https://pandas.pydata.org/docs/
7. scikit-learn documentation: https://scikit-learn.org/stable/

---
<br>


## 👥 Team and Mentors

This project is developed by a **5-member team** as part of the **EC6020 Mini Project**.
Team roles (hardware design, firmware development, ML modeling, UI development, documentation) are clearly defined in the project documentation.

| Name | Role |
|---|---|
| Member 1 | Team Lead |
| Member 2 | Firmware Engineer |
| Member 3 | ML Engineer |
| Member 4 | Hardware Engineer |
| Member 5 | App and Documentation Engineer |

| Name | Role |
|---|---|
| Mentor 1 | Academic Supervisor |
| Mentor 2 | Technical Mentor (if applicable) |

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

Work in progress - ongoing development and testing.

---

