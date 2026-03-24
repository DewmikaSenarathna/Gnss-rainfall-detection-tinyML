# Rainfall Detection and Short-Term Prediction using GNSS and TinyML

<p align="center">
  <img src="docs/imgs/Logo.png" alt="NimbusAI Logo" width="40" />
</p>

<h2 align="center">NimbusAI</h2>

<p align="center">
  Embedded Intelligence for Localized Rainfall Detection and Short-Term Prediction
</p>

<p align="center">
  <img src="https://img.shields.io/badge/Domain-GNSS%20%2B%20TinyML-0B5ED7" alt="Domain Badge" />
  <img src="https://img.shields.io/badge/Platform-STM32-198754" alt="Platform Badge" />
  <img src="https://img.shields.io/badge/Cloud-Firebase-FF6F00" alt="Cloud Badge" />
  <img src="https://img.shields.io/badge/App-NimbusAI-6C757D" alt="App Badge" />
</p>

---
<br>

## 🧭 Document Navigation

- [EC6020 Mini Project](#ec6020-mini-project)
- [📌 Project Overview](#project-overview)
- [🎯 Project Objectives](#project-objectives)
- [🧠 Key Technical Concepts](#key-technical-concepts)
- [🎓 Theoretical Background](#theoretical-background)
- [🧩 System Architecture (High-Level)](#system-architecture)
- [📂 Repository Structure](#repository-structure)
- [🛠 Hardware Components](#hardware-components)
- [🔬 Machine Learning Approach](#machine-learning-approach)
- [🔌 Circuit Design](#circuit-design)
- [💻 Software and Tools](#software-and-tools)
- [🧪 Testing and Validation](#testing-and-validation)
- [🧠 Embedded Implementation Challenges](#embedded-implementation-challenges)
- [🎯 System Integration and Output](#system-integration-and-output)
- [🎓 Future Improvements](#future-improvements)
- [🧩 Conclusion](#conclusion)
- [👥 Team and Mentors](#team-and-mentors)
- [🖼 Project Gallery (Group Members)](#project-gallery-group-members)
- [📚 References](#references)
- [⚠️ Notes](#notes)
- [📄 License](#license)
- [✅ Status](#status)

---
<br>

<a id="ec6020-mini-project"></a>

## EC6020 Mini Project

**Degree Program:** Electronics and Computer Engineering  
**Project Type:** Hardware-Software Integrated Embedded System

---
<br>

<a id="project-overview"></a>

## 📌 Project Overview

This project focuses on the design and development of an **intelligent embedded system** for **rainfall detection and short-term rainfall prediction** using **GNSS signal attenuation**, **environmental sensors** and **Tiny Machine Learning (TinyML)**.

Unlike traditional rainfall monitoring systems that rely on expensive radar or large-scale meteorological infrastructure, this system leverages **Signal-to-Noise Ratio (SNR) variations of GNSS signals** as an indirect environmental sensing mechanism. These GNSS-derived features are combined with **humidity** and **rainfall intensity sensor data** and processed locally on an **STM32 microcontroller** using a lightweight TinyML model.

While many existing studies utilize satellite image processing and cloud-based models, there is limited research focused on direct GNSS signal behavior combined with embedded intelligence. Therefore, this project begins with a thorough theoretical investigation of GNSS signal attenuation characteristics and their relationship with atmospheric conditions.

The system performs **real-time inference at the edge**, without requiring cloud connectivity, making it suitable for **low-cost, low-power and scalable deployment**.

---
<br>

<a id="project-objectives"></a>

## 🎯 Project Objectives

- Detect rainfall events in real time using GNSS signal attenuation.
- Predict short-term rainfall conditions using TinyML.
- Integrate multiple sensors for improved reliability (sensor fusion).
- Implement the complete system on an **STM32 microcontroller**.
- Provide a user interface to visualize system outputs.

---
<br>

<a id="key-technical-concepts"></a>

## 🧠 Key Technical Concepts

- GNSS Signal-to-Noise Ratio (C/N₀) analysis.
- Embedded systems design using STM32.
- Sensor interfacing using UART and I²C.
- Feature extraction from time-series GNSS data.
- TinyML deployment on resource-constrained hardware.
- Edge-based machine learning inference.

---
<br>

<a id="theoretical-background"></a>

## 🎓 Theoretical Background

Based on literature review and theoretical understanding, several environmental and signal-related parameters were identified as influential in rainfall attenuation:

- Signal-to-Noise Ratio (SNR)
- Satellite Elevation Angle
- Satellite Azimuth
- Temperature
- Relative Humidity
- Atmospheric Pressure
- Precipitable Water Vapor (PWV) / Zenith Wet Delay (ZWD)
- Time / Seasonal Variations
- Wind Speed

These parameters collectively describe the physical interaction between GNSS signals and atmospheric conditions, particularly during rainfall events.

<br>
### 1. Specific Attenuation

`gamma = a * R^b`

<br>
### 2. Total Attenuation

`A = gamma * Lp`

<br>
### 3. SNR (Linear Form)

`SNR = Pr / N`

<br>
### 4. SNR (dB Form)

`SNR(dB) = Pr(dB) - N(dB)`

<br>
### 5. Received Power

`Pr = Pt - A`

<br>
### 6. Combined Model (Rain -> SNR)

`SNR = Pt - (a * R^b * Lp) - N`

<br>
### 7. Extended Model (Temperature and Humidity)

`SNR = Pt - (a * R^b * Lp + alpha*T + beta*H) - N`

<br>
### 📌 Variables

- `gamma` = specific attenuation (dB/km)
- `R` = rain intensity (mm/h)
- `a`, `b` = constants (frequency and polarization dependent)
- `A` = total attenuation (dB)
- `Lp` = path length (km)
- `Pt` = transmitted power (dB)
- `Pr` = received power (dB)
- `N` = noise power (dB)
- `T` = temperature
- `H` = humidity
- `alpha`, `beta` = environmental coefficients

---
<br>

<a id="system-architecture"></a>

## 🧩 System Architecture (High-Level)

**Inputs:**
- GNSS SNR values from GNSS receiver
- Humidity sensor readings
- Rain sensor output
- Temperature readings

**Processing:**
- Feature extraction (Mean SNR, Temperature, Humidity level, Rain value)
- TinyML inference on STM32

**Outputs:**
- Rainfall detection status
- Short-term rainfall prediction
- Data visualization through UI

---
<br>

<a id="repository-structure"></a>

## 📂 Repository Structure

The repository is organized to clearly separate **documentation**, **hardware design**, **firmware**, **machine learning**, and **app** components.

```text
Gnss-rainfall-detection-tinyML/
│
├── docs/
│   ├── pdfs/
│   ├── imgs/
│   └── presentations/
│
├── hardware/
│   └──Gerber_Embeded-PCB_PCB_Embeded-PCB/
│
├── firmware/
│   ├── Arduino/
│   ├── Tests/
│   └── stm32_code/
│
├── ml/
│   ├── dataset/
│   ├── training_scripts/
│   ├── testing/
│   └── model/
│ 
├── app/
│   └── NimbusAI/
│ 
├── README.md
└── .gitignore
```

---
<br>

<a id="hardware-components"></a>

## 🛠 Hardware Components

- STM32 microcontroller
- GNSS receiver module
- Humidity sensor
- Rainfall intensity sensor
- Power regulation and support circuitry

---
<br>

<a id="machine-learning-approach"></a>

## 🔬 Machine Learning Approach

- GNSS and sensor data are collected and logged
- Features are extracted from time-series data
- A lightweight ML model is trained offline
- The trained model is converted to TinyML format
- The model is deployed on STM32 for real-time inference

<br>

### Feature Selection Under System Constraints

Due to hardware limitations and sensor availability in the embedded system, a subset of features was selected for model training:

- Temperature
- Humidity
- GNSS SNR values
- Rain sensor output (Rain Value)

This selection ensures a balance between model performance and embedded system feasibility, as also reflected in the system architecture described in the proposal documentation.

<br>

### Initial Classification Approach

Initially, several classification-based machine learning models were implemented:

- Random Forest
- Decision Tree
- Multi-Layer Perceptron (MLP)
- Artificial Neural Networks

However, experimental results indicated low prediction accuracy. This limitation arises due to:

- High variability in environmental conditions
- Non-linear and dynamic nature of rainfall attenuation
- Insufficient dataset size and diversity

<br>

### Insight

Rainfall attenuation is not a static classification problem. Instead, it behaves as a continuous physical phenomenon influenced by multiple dynamic variables. Therefore, classification models failed to generalize effectively.

<br>

### Calibration and Model Improvement

To address this limitation, the project transitioned toward a theoretical calibration approach:

- Research papers and domain knowledge were used to understand signal attenuation physics
- Feature relationships were refined using analytical reasoning
- Data preprocessing was improved to reduce noise and inconsistencies

Following this, regression-based modeling techniques were explored.

<br>

### Regression-Based Prediction Model

The project adopted a regression-oriented approach, specifically:

- Multinomial Logistic Regression (for intensity level categorization based on continuous prediction)

This approach demonstrated improved performance because:

- It captures continuous relationships between variables
- It aligns better with physical rainfall behavior
- It allows smoother prediction transitions between intensity levels

<br>

### Adaptive Learning and Cloud Integration

To further improve model performance, a retraining mechanism was proposed:

- Real-time data collection via sensors
- Cloud storage using Firebase Realtime Database
- Continuous dataset expansion for model refinement

This introduces a semi-online learning capability, enabling the system to adapt to changing environmental conditions over time.

---
<br>

<a id="circuit-design"></a>

## 🔌 Circuit Design

Complete circuit artifacts are available in the hardware folder:

### Schematic Design

<p align="center">
  <img src="docs/imgs/Schematic_Embeded-PCB.png" width="400" alt="Schematic diagram" />
</p>

<br>

### PCB Design

<p align="center">
  <img src="docs/imgs/PCB_PCB_Embeded-PCB.png" width="400" alt="PCB layout" />
</p>

<br>

### Final Design

<p align="center">
  <img src="docs/imgs/Final PCB Design.jpg" width="400" alt="NimbusAI Final PCB Design" />
</p>

---
<br>

<a id="software-and-tools"></a>

## 💻 Software and Tools

- **Embedded Development:** STM32CubeIDE, STM32CubeProgrammer, Embedded C/C++
- **Machine Learning:** Python, NumPy, Pandas, Scikit-learn
- **TinyML:** TensorFlow Lite for Microcontrollers
- **Mobile Application:** Flutter (NimbusAI)
- **Version Control:** Git and GitHub
- **Testing and Debugging:** Serial monitors, logic analyzer, multimeter

---
<br>

<a id="testing-and-validation"></a>

## 🧪 Testing and Validation

- System tested under dry and rainy conditions
- Model predictions compared with actual rainfall observations
- Performance evaluated based on accuracy and response time

---
<br>

<a id="embedded-implementation-challenges"></a>

## 🧠 Embedded Implementation Challenges

### STM32 Limitations

- Limited RAM and Flash memory
- No native support for TensorFlow Lite
- Restricted computational capability

<br>

### Solution Strategy

Instead of deploying full ML frameworks:

- Model parameters (weights and biases) were manually embedded into firmware
- Inference was implemented using lightweight mathematical computations

This aligns with TinyML principles and ensures real-time execution on STM32, consistent with the project system design.

---
<br>

<a id="system-integration-and-output"></a>

## 🎯 System Integration and Output

The final system includes:

- GNSS module for SNR acquisition
- Environmental sensors (temperature, humidity, rain sensor)
- STM32 microcontroller for processing
- OLED display for real-time output

Additionally:

- ESP8266 WiFi module enables cloud connectivity
- Firebase is used for data logging and remote access
- A mobile application (NimbusAI App) is introduced for user interaction

---
<br>

<a id="future-improvements"></a>

## 🎓 Future Improvements

- Full online learning integration on edge devices
- Advanced regression or hybrid ML models
- Inclusion of additional features (for example: PWV, pressure)
- Optimization for TensorFlow Lite Micro compatibility
- Improved dataset collection across diverse weather conditions

---
<br>

<a id="conclusion"></a>

## 🧩 Conclusion

NimbusAI demonstrates a novel and practical approach to rainfall prediction by combining:

- GNSS signal attenuation
- Environmental sensing
- TinyML-based embedded intelligence

The project successfully highlights that accurate, real-time rainfall prediction is feasible using low-cost embedded systems, without relying on expensive infrastructure. Furthermore, it establishes a strong foundation for future research in edge-based environmental monitoring systems.

---
<br>

<a id="team-and-mentors"></a>

## 👥 Team and Mentors

This project is developed by a **5-member team** as part of the **EC6020 Mini Project**.  
Team roles (hardware design, firmware development, ML modeling, App development, documentation) are clearly defined in the project documentation.

| Member | Role | Name |
| --- | --- | --- |
| Member 1 | Team Lead | Kumari H.A.R. |
| Member 2 | Firmware Engineer | Senarath S.M.D.P. |
| Member 3 | ML Engineer | Senarathna S.A.D.H.D. |
| Member 4 | Hardware Engineer | Galahitiyawa G.R.M.M. |
| Member 5 | App and Documentation Engineer | Patabadige M.P.H.R. |

| Name | Role |
| --- | --- |
| Dr.Thanuja Uruththirakodeeswaran | Academic Supervisor, Department of Computer Engineering |
| Eng.Pirunthapan Yogathasan | Supervisor, Department of Computer Engineering |

---
<br>

<a id="project-gallery-group-members"></a>

## 🖼 Project Gallery (Group Members)

<p align="center">
  <img src="docs/imgs/Group%20members.jpg" width="400" alt="NimbusAI Group Members" />
</p>

---
<br>

<a id="references"></a>

## 📚 References

1. ITU-R P.838-3: Specific attenuation model for rain for use in prediction methods.
2. ITU-R P.618: Propagation data and prediction methods required for Earth-space systems.
3. TensorFlow Lite for Microcontrollers documentation: https://www.tensorflow.org/lite/microcontrollers
4. TinyML Foundation resources: https://www.tinyml.org
5. NumPy documentation: https://numpy.org/doc/
6. pandas documentation: https://pandas.pydata.org/docs/
7. scikit-learn documentation: https://scikit-learn.org/stable/
8. Alozie et al., "Rain Signal Attenuation Modeling," Sustainability, 2022. https://doi.org/10.3390/su141811744
9. ITU-R P.838-3, Rain Attenuation Model. https://www.itu.int/rec/R-REC-P.838
10. ITU-R P.530-17, Propagation Prediction Methods. https://www.itu.int/rec/R-REC-P.530
11. Appuhamilage et al., Impact of temperature and humidity on SINR in LTE networks, 2025: https://doi.org/10.59324/ejaset.2025.3(2).09
12. Hao and Lu, Variation of relative humidity and air temperature, 2022: https://doi.org/10.3390/atmos13081171
13. Romps, Analytical model for tropical relative humidity, 2014: https://doi.org/10.1175/JCLI-D-14-00255.1
14. Agbo and Edet, Meteorological analysis of climatic parameters, 2021: https://doi.org/10.1007/s00704-022-04226-x

---
<br>

<a id="notes"></a>

## ⚠️ Notes

- This repository is intended for **academic and educational purposes only**

---
<br>

<a id="license"></a>

## 📄 License

This project is developed as part of an academic course and is not intended for commercial use without permission.

---
<br>

<a id="status"></a>

## ✅ Status

🚧 **Work in Progress** - Ongoing development and testing

---
