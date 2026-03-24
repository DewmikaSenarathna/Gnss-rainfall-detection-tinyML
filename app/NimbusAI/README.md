# NimbusAI

NimbusAI is a Flutter-based rainfall monitoring dashboard that visualizes realtime weather station data and rainfall prediction status from Firebase Realtime Database.

## Overview

NimbusAI is designed for environmental monitoring workflows where fast visibility of station readings is required.

Main dashboard outputs:
- Temperature
- Humidity
- SNR Max
- Rain Value (sensor value)
- Rain Intensity (prediction level: No Rain, Low Rain, High Rain)

## Tech Stack

- Flutter (Dart)
- Firebase Core
- Firebase Realtime Database
- fl_chart (trend visualization)

## Project Structure

- lib/main.dart: app entrypoint
- lib/screens/dashboard_screen.dart: realtime dashboard UI
- lib/models/sensor_data.dart: payload model and parsing
- lib/services/api_service.dart: sensor API service
- lib/config/app_config.dart: environment-based runtime configuration

## Run Locally

1. Install dependencies:

flutter pub get

2. Add local Firebase file:

3. Run app:

flutter run

## Build APK

flutter build apk
