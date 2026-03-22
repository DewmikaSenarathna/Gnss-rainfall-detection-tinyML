class SensorData {
  final double temperature;
  final double humidity;
  final double snr;
  final double rainIntensity;
  final int rainLevel;
  final String prediction;

  SensorData({
    required this.temperature,
    required this.humidity,
    required this.snr,
    required this.rainIntensity,
    required this.rainLevel,
    required this.prediction,
  });

  factory SensorData.fromJson(Map<String, dynamic> json) {
    final rainLevel = _readInt(json['rainLevel'] ?? json['rain_level']);
    final prediction = _readPrediction(
      json['prediction'],
      rainLevel,
    );

    return SensorData(
      temperature: _readDouble(json['temperature'] ?? json['temperatur']),
      humidity: _readDouble(json['humidity']),
      snr: _readDouble(json['snr'] ?? json['maxSNR'] ?? json['max_snr']),
      rainIntensity: _readDouble(
        json['rain_intensity'] ?? json['rainValue'] ?? json['rain_sensor'],
      ),
      rainLevel: rainLevel,
      prediction: prediction,
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

  static int _readInt(dynamic value) {
    if (value is int) {
      return value;
    }
    if (value is num) {
      return value.round();
    }
    if (value is String) {
      return int.tryParse(value) ?? 0;
    }
    return 0;
  }

  static String _readPrediction(dynamic prediction, int rainLevel) {
    if (prediction != null && prediction.toString().trim().isNotEmpty) {
      return prediction.toString();
    }

    switch (rainLevel) {
      case 0:
        return 'No Rain';
      case 1:
        return 'Low Rain';
      case 2:
        return 'High Rain';
      default:
        return 'Unknown';
    }
  }
}