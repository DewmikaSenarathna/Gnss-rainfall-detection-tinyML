class AppConfig {
  AppConfig._();

  static const String firebaseDatabaseUrl = String.fromEnvironment(
    'FIREBASE_DATABASE_URL',
    defaultValue:
        'https://rainfallprediction-945cd-default-rtdb.asia-southeast1.firebasedatabase.app',
  );

  static const String sensorApiEndpoints = String.fromEnvironment(
    'SENSOR_API_ENDPOINTS',
    defaultValue: 'http://10.19.40.192/data,http://192.168.4.1/data',
  );
}
