# NimbusAI App

<p align="center">
  <img src="assets/images/app_icon.png" alt="NimbusAI App Icon" width="400" />
</p>

<p align="center">
  Flutter Dashboard for Real-Time Rainfall Monitoring and Prediction
</p>

<p align="center">
  <img src="https://img.shields.io/badge/Framework-Flutter-0B5ED7" alt="Flutter" />
  <img src="https://img.shields.io/badge/Backend-Firebase%20Realtime%20Database-FF6F00" alt="Firebase" />
  <img src="https://img.shields.io/badge/Charts-fl__chart-198754" alt="fl_chart" />
  <img src="https://img.shields.io/badge/Project-NimbusAI-6C757D" alt="NimbusAI" />
</p>

---

## Document Navigation

- [1. Overview](#1-overview)
- [2. Dashboard Outputs](#2-dashboard-outputs)
- [3. Tech Stack](#3-tech-stack)
- [4. Application Structure](#4-application-structure)
- [5. Configuration and Environment](#5-configuration-and-environment)
- [6. Local Development Setup](#6-local-development-setup)
- [7. Build and Release](#7-build-and-release)
- [8. Security and Publishing Checklist](#8-security-and-publishing-checklist)
- [9. Troubleshooting](#9-troubleshooting)

---

## 1. Overview

NimbusAI is a Flutter-based rainfall monitoring dashboard that visualizes real-time weather station data and rainfall prediction status from Firebase Realtime Database.

The app is designed for environmental monitoring workflows where operators need immediate visibility into station-level readings and predicted rain intensity.

---

## 2. Dashboard Outputs

The dashboard displays:

- Temperature
- Humidity
- SNR Max
- Rain Value (sensor value)
- Rain Intensity (prediction level: No Rain, Low Rain, High Rain)

---

## 3. Tech Stack

- Flutter (Dart)
- Firebase Core
- Firebase Realtime Database
- fl_chart (trend visualization)

---

## 4. Application Structure

Core files and responsibilities:

- `lib/main.dart`: Application entry point.
- `lib/screens/dashboard_screen.dart`: Real-time dashboard UI.
- `lib/models/sensor_data.dart`: Sensor payload model and parsing logic.
- `lib/services/api_service.dart`: Sensor API service layer.
- `lib/config/app_config.dart`: Runtime configuration from environment values.

---

## 5. Configuration and Environment

This project is prepared for public publishing.

Sensitive or environment-specific values are externalized through runtime defines and local-only files.

Reference and local files:

- `.env.example`: Environment template.
- `.env`: Local-only environment file (ignored by git).
- `android/app/google-services.json`: Local-only Firebase file (ignored by git).
- `android/app/google-services.json.example`: Firebase onboarding template.

### Required Keys

- `FIREBASE_DATABASE_URL`
- `SENSOR_API_ENDPOINTS`

### Optional Secret Catalog Keys

Useful in CI/CD secret stores:

- `FIREBASE_PROJECT_ID`
- `FIREBASE_API_KEY`
- `FIREBASE_APP_ID`
- `FIREBASE_MESSAGING_SENDER_ID`

### Example Values

```env
FIREBASE_DATABASE_URL=https://your-project-id-default-rtdb.region.firebasedatabase.app
SENSOR_API_ENDPOINTS=http://192.168.1.100/data,http://192.168.4.1/data
```

---

## 6. Local Development Setup

### Step 1: Install Dependencies

```bash
flutter pub get
```

### Step 2: Add Local Firebase File

- Copy `android/app/google-services.json.example` to `android/app/google-services.json`.
- Replace placeholder values with your Firebase project values.

### Step 3: Run the App

```bash
flutter run
```

### Optional Runtime Override (dart-define)

```bash
flutter run --dart-define=FIREBASE_DATABASE_URL=https://your-db-url --dart-define=SENSOR_API_ENDPOINTS=http://192.168.1.100/data,http://192.168.4.1/data
```

---

## 7. Build and Release

Build Android APK:

```bash
flutter build apk
```

For production distribution, also validate app signing and package identity before release.

---

## 8. Security and Publishing Checklist

- Keep `.env` out of version control (already ignored).
- Keep `android/app/google-services.json` out of version control (already ignored).
- Verify Firebase rules for production access.
- Confirm package name and signing configuration.
- Replace sample endpoints with deployment endpoint values.
- Keep README and screenshots current.

---

## 9. Troubleshooting

If the dashboard shows no live values:

- Confirm the Firebase URL is correct.
- Verify sensor API endpoints are reachable from the test network.
- Check that runtime defines were passed correctly.
- Validate Firebase configuration files are present locally.

---
