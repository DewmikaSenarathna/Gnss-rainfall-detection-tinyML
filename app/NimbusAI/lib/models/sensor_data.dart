class SensorData {
  final double temperature;
  final double humidity;
  final double snr;
  final double rainIntensity;
  final String prediction;

  SensorData({
    required this.temperature,
    required this.humidity,
    required this.snr,
    required this.rainIntensity,
    required this.prediction,
  });

  factory SensorData.fromJson(Map<String, dynamic> json) {
    return SensorData(
      temperature: _readDouble(json['temperature']),
      humidity: _readDouble(json['humidity']),
      snr: _readDouble(json['snr']),
      rainIntensity: _readDouble(json['rain_intensity']),
      prediction: (json['prediction'] ?? 'Unknown').toString(),
    );
  }

  static double _readDouble(dynamic value) {
    if (value is num) {
      return value.toDouble();
    }
    if (value is String) {
      return double.tryParse(value) ?? 0;
    }
    return 0;
  }
}