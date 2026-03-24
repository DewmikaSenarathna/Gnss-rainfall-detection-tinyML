class AppConfig {
  AppConfig._();

  static const String firebaseDatabaseUrl = String.fromEnvironment(
    'FIREBASE_DATABASE_URL',
    defaultValue: '',
  );

  static const String sensorApiEndpoints = String.fromEnvironment(
    'SENSOR_API_ENDPOINTS',
    defaultValue: '',
  );
}
