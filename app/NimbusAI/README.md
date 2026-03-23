# NimbusAI

NimbusAI is a Flutter-based rainfall monitoring dashboard that visualizes realtime weather station data and rainfall prediction status from Firebase Realtime Database.

<<<<<<< Updated upstream
=======
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

## Configuration

This repository is prepared for public publishing.

Sensitive or environment-specific values are externalized through runtime defines.

Reference file:
- .env.example

Local-only file (ignored by git):
- .env

Firebase local-only file (ignored by git):
- android/app/google-services.json

Template for onboarding:
- android/app/google-services.json.example

### Required Keys

- FIREBASE_DATABASE_URL
- SENSOR_API_ENDPOINTS

Optional secret catalog keys (useful in CI/CD secret stores):
- FIREBASE_PROJECT_ID
- FIREBASE_API_KEY
- FIREBASE_APP_ID
- FIREBASE_MESSAGING_SENDER_ID

Example:

FIREBASE_DATABASE_URL=https://your-project-id-default-rtdb.region.firebasedatabase.app
SENSOR_API_ENDPOINTS=http://192.168.1.100/data,http://192.168.4.1/data

## Run Locally

1. Install dependencies:

flutter pub get

2. Add local Firebase file:

- Copy android/app/google-services.json.example to android/app/google-services.json
- Replace placeholder values using your Firebase project values

3. Run app:

flutter run

Optional runtime override using dart defines:

flutter run --dart-define=FIREBASE_DATABASE_URL=https://your-db-url --dart-define=SENSOR_API_ENDPOINTS=http://192.168.1.100/data,http://192.168.4.1/data

## Build APK

flutter build apk

## Publish Checklist

- Keep .env out of version control (already ignored)
- Keep android/app/google-services.json out of version control (already ignored)
- Verify Firebase rules for production access
- Confirm package name and app signing configs
- Replace sample endpoints with your deployment endpoint values
- Confirm README and screenshots are up to date

## License

Add your preferred license before publishing.
>>>>>>> Stashed changes
