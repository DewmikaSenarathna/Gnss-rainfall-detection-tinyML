# NimbusAI ML Module

<p align="center">
  <img src="/docs/imgs/Logo.png" alt="NimbusAI Logo" width="400" />
</p>

<p align="center">
  Machine Learning Pipeline for GNSS-Based Rainfall Detection and Short-Term Intensity Prediction
</p>

<p align="center">
  <img src="https://img.shields.io/badge/Scope-ML%20Pipeline-0B5ED7" alt="Scope" />
  <img src="https://img.shields.io/badge/Features-SNR%20%2B%20Weather-198754" alt="Features" />
  <img src="https://img.shields.io/badge/Deployment-TinyML%20on%20STM32-FF6F00" alt="Deployment" />
  <img src="https://img.shields.io/badge/Data-Firebase%20Ready-6C757D" alt="Data" />
</p>

---

## Document Navigation

- [1. Module Overview](#1-module-overview)
- [2. ML Objectives](#2-ml-objectives)
- [3. Feature Space and Constraints](#3-feature-space-and-constraints)
- [4. Theoretical Basis: Rain Intensity and SNR](#4-theoretical-basis-rain-intensity-and-snr)
- [5. Modeling Strategy and Evolution](#5-modeling-strategy-and-evolution)
- [6. Final Prediction Approach](#6-final-prediction-approach)
- [7. Data Pipeline](#7-data-pipeline)
- [8. Repository Layout (ML)](#8-repository-layout-ml)
- [9. Training and Experiment Workflow](#9-training-and-experiment-workflow)
- [10. Embedded TinyML Deployment Notes](#10-embedded-tinyml-deployment-notes)
- [11. Testing and Validation](#11-testing-and-validation)
- [12. Tools and Dependencies](#12-tools-and-dependencies)
- [13. Future ML Improvements](#13-future-ml-improvements)
- [14. References](#14-references)

---

## 1. Module Overview

This module contains the machine learning assets and workflows for NimbusAI. It supports end-to-end rainfall intelligence, from dataset preparation and model development to embedded deployment constraints.

The ML pipeline is designed around GNSS attenuation behavior and environmental sensing, with a practical focus on low-cost, edge-compatible inference.

---

## 2. ML Objectives

- Detect rainfall-related atmospheric behavior using GNSS signal attenuation patterns.
- Predict short-term rainfall intensity using compact ML methods suitable for embedded systems.
- Maintain compatibility with STM32 memory and computation limits.
- Enable dataset growth and model refinement through Firebase-connected data collection.

---

## 3. Feature Space and Constraints

### 3.1 Full Theoretical Feature Space

From the project literature and domain investigation, the following features are relevant to rainfall attenuation:

- Signal-to-Noise Ratio (SNR)
- Satellite elevation angle
- Satellite azimuth
- Temperature
- Relative humidity
- Atmospheric pressure
- Precipitable Water Vapor (PWV) / Zenith Wet Delay (ZWD)
- Time / seasonal variation
- Wind speed

### 3.2 Selected Features Used in Current Embedded ML

Due to sensor availability and embedded constraints, model training is based on:

- Temperature
- Humidity
- GNSS SNR values
- Rain sensor output (Rain Value)

This selection balances predictive utility with practical deployment feasibility.

---

## 4. Theoretical Basis: Rain Intensity and SNR

### 4.1 Core Relations

- Specific attenuation: `gamma = a * R^b`
- Total attenuation: `A = gamma * Lp`
- Linear SNR: `SNR = Pr / N`
- dB SNR: `SNR(dB) = Pr(dB) - N(dB)`
- Received power: `Pr = Pt - A`
- Combined rain-to-SNR model: `SNR = Pt - (a * R^b * Lp) - N`
- Extended model with environmental terms: `SNR = Pt - (a * R^b * Lp + alpha*T + beta*H) - N`

### 4.2 Variables

- `gamma`: specific attenuation (dB/km)
- `R`: rain intensity (mm/h)
- `a, b`: frequency and polarization dependent constants
- `A`: total attenuation (dB)
- `Lp`: propagation path length (km)
- `Pt`: transmitted power (dB)
- `Pr`: received power (dB)
- `N`: noise power (dB)
- `T`: temperature
- `H`: humidity
- `alpha, beta`: environmental coefficients

---

## 5. Modeling Strategy and Evolution

### 5.1 Initial Classification Trials

Early experimentation used classification-oriented models such as:

- Random Forest
- Decision Tree
- Multi-Layer Perceptron (MLP)
- Artificial Neural Networks

### 5.2 Observed Challenges

- High variability of environmental conditions
- Non-linear and dynamic attenuation behavior
- Limited dataset volume and diversity

These results indicated that rainfall attenuation behaves more like a continuous physical process than a static class boundary problem.

### 5.3 Calibration-Driven Refinement

Modeling was improved by:

- Aligning features with physical attenuation theory
- Improving data preprocessing to reduce inconsistencies and noise
- Transitioning toward regression-oriented modeling

---

## 6. Final Prediction Approach

The current approach emphasizes regression-oriented prediction and level interpretation:

- Regression for capturing continuous relationships among SNR and environmental features
- Multinomial Logistic Regression used for intensity-level categorization based on continuous behavior

Why this approach was retained:

- Better alignment with physical rainfall dynamics
- Smoother transitions across predicted intensity levels
- Improved practical behavior compared to early classification-only approaches

---

## 7. Data Pipeline

### 7.1 Data Sources

- GNSS-derived SNR signals
- Environmental sensors (temperature, humidity)
- Rain sensor values
- Firebase data logs for expansion and retraining

### 7.2 Pipeline Stages

- Data collection
- Cleaning and aggregation
- Feature extraction and scaling
- Model training and evaluation
- Export for TinyML/embedded usage

### 7.3 Adaptive Learning Direction

A semi-online retraining path is planned using continuous Firebase data growth to improve robustness over time.

---

## 8. Repository Layout (ML)

```text
ml/
├── README.md
├── dataset/
│   ├── Aggregrated_Data/
│   ├── Clean_Data/
│   ├── Firebase_Data/
│   └── Merged_Data/
│ 
├── model/
│   ├── Data_Preprocessing/
│   ├── ML_Model/
│   └── STM32/
│ 
├── testing/
│   ├── Test_01/
│   ├── Test_02/
│   └── Test_03/
│ 
└── training_scripts/
    ├── Gnss_rainfall_detection.ipynb
    └── Gnss_rainfall_detection_tinyML.ipynb
```

---

## 9. Training and Experiment Workflow

1. Prepare datasets from `dataset/` sources.
2. Execute preprocessing assets in `model/Data_Preprocessing/`.
3. Train and evaluate models in `model/ML_Model/` and notebooks under `training_scripts/`.
4. Validate performance against dry/rainy condition behavior.
5. Convert and package deployment-friendly artifacts in `model/STM32/`.

---

## 10. Embedded TinyML Deployment Notes

### 10.1 STM32 Constraints

- Limited RAM and Flash
- No full native TensorFlow Lite runtime support in baseline firmware
- Tight real-time compute budgets

### 10.2 Practical Deployment Method

- Use compact inference logic for runtime compatibility
- Embed model parameters (weights/biases) directly where needed
- Keep preprocessing and model dimensions lightweight for deterministic timing

This remains consistent with TinyML edge principles and project hardware constraints.

---

## 11. Testing and Validation

Validation strategy includes:

- Condition-based testing under dry and rainy periods
- Prediction comparison against observed rainfall behavior
- Response and consistency checks for edge deployment viability

Artifacts and experiment batches are maintained under `ml/testing/`.

---

## 12. Tools and Dependencies

- Python
- NumPy
- pandas
- scikit-learn
- Jupyter notebooks
- TensorFlow Lite for Microcontrollers (deployment direction)

---

## 13. Future ML Improvements

- Full online learning integration at edge/cloud boundaries
- Advanced regression or hybrid model design
- Additional atmospheric features (PWV, pressure) where feasible
- Improved TensorFlow Lite Micro compatibility path
- Broader and more diverse weather-condition datasets

---

## 14. References

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

For complete project context, see the root documentation in `README.md`.
